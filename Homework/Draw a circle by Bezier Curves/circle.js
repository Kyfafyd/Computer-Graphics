(function () {
  'use strict';

  /** 中心 X */
  const CX = 300;
  /** 中心 Y */
  const CY = 300;
  const KAPPA = (-1 + Math.sqrt(2)) / 3 * 4;

  class ArcStarategy {
    drawCircle(param) {
      const rx = param.rx;
      const ry = rx;
      return [
        ['M', CX - rx, CY],
        ['A', rx, ry, 0, 1, 0, CX + rx, CY],
        ['A', rx, ry, 0, 1, 0, CX - rx, CY],
      ];
    }

    drawControlPoints(param) {
      return this.drawCircle(param)
        .filter(op => op[0] === 'A')
        .map(op => [op[6], op[7]]);
    }
  }

  class CubicBezierStarategy {
    drawCircle(param) {
      const rx = param.rx;
      const ry = rx;
      return [
        ['M', CX - rx, CY],
        ['C', CX - rx, CY - KAPPA * ry, CX - KAPPA * rx, CY - ry, CX, CY - ry],
        ['C', CX + KAPPA * rx, CY - ry, CX + rx, CY - KAPPA * ry, CX + rx, CY],
        ['C', CX + rx, CY + KAPPA * ry, CX + KAPPA * rx, CY + ry, CX, CY + ry],
        ['C', CX - KAPPA * rx, CY + ry, CX - rx, CY + KAPPA * ry, CX - rx, CY],
      ];
    }

    drawControlPoints(param) {
      return this.drawCircle(param)
        .filter(op => op[0] === 'C')
        .reduce(
          (list, op) => list.concat([
            [op[1], op[2]],
            [op[3], op[4]],
            [op[5], op[6]]
          ]),
          []
        );
    }

  }

  const STRATEGIES = {
    arc: new ArcStarategy,
    cubic_bezier: new CubicBezierStarategy,
  };

  new Vue({
    el: '#app',
    data: {
      width: '600px',
      height: '600px',
      figure: 'circle',
      rx: 140,
      ry: 140,
      strategy: 'arc',
      hidePoints: false,
    },
    watch: {
      strategy(val) {
        if (!this.hasSupportedFigure(val, this.figure)) {
          const figures = this.getSupportedFigures(val);
          this.figure = figures[0];
        }
      }
    },
    computed: {
      drawCircle() {
        const ops = this.getCirclePaths();
        return ops.map(op => op.join(' ')).join(' ');
      },
      pathLines() {
        return this.getCirclePaths()
          .map(op => [op[0], ...op.slice(1).map(Math.round)].join(' '));
      },
      drawControlPoints() {
        if (STRATEGIES[this.strategy]) {
          const param = this.makeParameter();
          const points = STRATEGIES[this.strategy].drawControlPoints(param);
          return points.map(p => ({
            x: p[0],
            y: p[1],
            r: '5px',
            text: this.getCoordText(p)
          }));
        }
        return [];
      },
    },
    methods: {
      getCirclePaths() {
        if (STRATEGIES[this.strategy]) {
          const param = this.makeParameter();
          return STRATEGIES[this.strategy].drawCircle(param)
        }
        return [];
      },
      getCoordText(p) {
        const x = Math.round(p[0]);
        const y = Math.round(p[1]);
        return `(${x}, ${y})`;
      },
      makeParameter() {
        return {
          r: this.rx,
          rx: this.rx,
          ry: this.ry,
          figure: this.figure,
        };
      },
    }
  });
})();