//顶点着色器
//膨胀
var VSHADER_SOURCE = "" +
    "attribute vec4 a_Position;\n" +
    "attribute vec4 a_Color;\n" +
    //"varying vec4 a_normal;\n" +
    "uniform mat4 u_ModelViewMatrix;\n" +
    "varying vec4 v_Color;\n" +
    "void main(){" +
    "   gl_Position = u_ModelViewMatrix * (a_Position+ vec4(0,0,0,1) + 0.08*a_Position);\n" +
    "   v_Color = a_Color + vec4(0.5,0.5,0.5,1);\n" +
    "}\n";
//不膨胀
var VSHADER_SOURCE1 = "" +
    "attribute vec4 a_Position;\n" +
    "attribute vec4 a_Color;\n" +
    //"varying vec4 a_normal;\n" +
    "uniform mat4 u_ModelViewMatrix;\n" +
    "varying vec4 v_Color;\n" +
    "void main(){" +
    "   gl_Position = u_ModelViewMatrix * (a_Position + vec4(0,0,0,1));\n" +
    "   v_Color = a_Color;\n" +
    "}\n";

//片元着色器
var FSHADER_SOURCE = "" +
    "#ifdef GL_ES\n" +
    "precision mediump float;\n" +
    "#endif\n" +
    "varying vec4 v_Color;\n" +
    "void main(){" +
    "   gl_FragColor = v_Color;\n" +
    "}\n";

//声明js需要的相关变量

function main() {
    var canvas = document.getElementById("canvas");
    var gl = getWebGLContext(canvas);
    if (!gl) {
        console.log("你的浏览器不支持WebGL");
        return;
    }

    //初始化着色器
    // if (!initShaders(gl, VSHADER_SOURCE, FSHADER_SOURCE)) {
    //     console.log("无法初始化着色器");
    //     return;
    // }

    var vertShader = gl.createShader(gl.VERTEX_SHADER);
    gl.shaderSource(vertShader, VSHADER_SOURCE);
    gl.compileShader(vertShader);

    var my_vertShader = gl.createShader(gl.VERTEX_SHADER);
    gl.shaderSource(my_vertShader, VSHADER_SOURCE1);
    gl.compileShader(my_vertShader);

    var fragShader = gl.createShader(gl.FRAGMENT_SHADER);
    gl.shaderSource(fragShader, FSHADER_SOURCE);
    gl.compileShader(fragShader);
    //合并程序
    var program1 = gl.createProgram();
    gl.attachShader(program1, vertShader);
    gl.attachShader(program1, fragShader);

    gl.linkProgram(program1);
    gl.useProgram(program1);
    gl.program = program1;

    var n = initVertexBuffers(gl, program1);
    if (n < 0) {
        console.log("无法创建缓冲区");
        return;
    }

    //设置视角矩阵的相关信息
    program1.u_ModelViewMatrix = gl.getUniformLocation(program1, "u_ModelViewMatrix");
    if (program1.u_ModelViewMatrix < 0) {
        console.log("无法获取矩阵变量的存储位置");
        return;
    }

    //设置底色
    gl.clearColor(0.0, 0.0, 0.0, 1.0);

    //进入场景初始化
    draw(gl, n, program1.u_ModelViewMatrix);
    
    var program2 = gl.createProgram();
    gl.attachShader(program2, my_vertShader);
    gl.attachShader(program2, fragShader);

    gl.linkProgram(program2);
    gl.useProgram(program2);
    gl.program = program2;

    var n = initVertexBuffers(gl, program2);
    program2.u_ModelViewMatrix = gl.getUniformLocation(program2, "u_ModelViewMatrix");
    front_draw(gl, n, program2.u_ModelViewMatrix);   // my function

}

function draw(gl, n, u_ModelViewMatrix) {
    
    
    //设置视角矩阵的相关信息（视点，视线，上方向）
    var viewMatrix = new Matrix4();
    viewMatrix.setLookAt(3,3,7,0,0,0,0,1,0);

    //设置模型矩阵的相关信息
    var modelMatrix = new Matrix4();
    modelMatrix.setRotate(0, 0, 0, 1);

    //设置透视投影矩阵
    var projMatrix = new Matrix4();
    projMatrix.setPerspective(30,canvas.width/canvas.height,1,100);

    //计算出模型视图矩阵 viewMatrix.multiply(modelMatrix)相当于在着色器里面u_ViewMatrix * u_ModelMatrix
    var modeViewMatrix = projMatrix.multiply(viewMatrix.multiply(modelMatrix));

    //将试图矩阵传给u_ViewMatrix变量
    gl.uniformMatrix4fv(u_ModelViewMatrix, false, modeViewMatrix.elements);

    //开启隐藏面清除
    gl.enable(gl.DEPTH_TEST);

    //清空颜色和深度缓冲区
    gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

    gl.enable(gl.CULL_FACE);
    gl.cullFace(gl.FRONT); 
    //绘制图形
    gl.drawElements(gl.TRIANGLES,n,gl.UNSIGNED_BYTE,0);
    gl.disable(gl.CULL_FACE);

}

function front_draw(gl, n, u_ModelViewMatrix)   // TODO
{
    var viewMatrix = new Matrix4();
    viewMatrix.setLookAt(3,3,7,0,0,0,0,1,0);

    //设置模型矩阵的相关信息
    var modelMatrix = new Matrix4();
    modelMatrix.setRotate(0, 0, 0, 1);

    //设置透视投影矩阵
    var projMatrix = new Matrix4();
    projMatrix.setPerspective(30,canvas.width/canvas.height,1,100);

    //计算出模型视图矩阵 viewMatrix.multiply(modelMatrix)相当于在着色器里面u_ViewMatrix * u_ModelMatrix
    var modeViewMatrix = projMatrix.multiply(viewMatrix.multiply(modelMatrix));

    //将试图矩阵传给u_ViewMatrix变量
    gl.uniformMatrix4fv(u_ModelViewMatrix, false, modeViewMatrix.elements);

    //开启隐藏面清除
    gl.enable(gl.DEPTH_TEST);

    //清空颜色和深度缓冲区
    // gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

    gl.drawElements(gl.TRIANGLES,n,gl.UNSIGNED_BYTE,0);
}

function initVertexBuffers(gl, program) {
    // 创建一个立方体
    //    v6----- v5
    //   /|      /|
    //  v1------v0|
    //  | |     | |
    //  | |v7---|-|v4
    //  |/      |/
    //  v2------v3
    var verticesColors = new Float32Array([
        // 设置顶点和颜色（偷的顶点代码位置）
        1.0,  1.0,  1.0,     1.0,  1.0,  1.0,  // v0 White
        -1.0,  1.0,  1.0,     1.0,  0.0,  1.0,  // v1 Magenta
        -1.0, -1.0,  1.0,     1.0,  0.0,  0.0,  // v2 Red
        1.0, -1.0,  1.0,     1.0,  1.0,  0.0,  // v3 Yellow
        1.0, -1.0, -1.0,     0.0,  1.0,  0.0,  // v4 Green
        1.0,  1.0, -1.0,     0.0,  1.0,  1.0,  // v5 Cyan
        -1.0,  1.0, -1.0,     0.0,  0.0,  1.0,  // v6 Blue
        -1.0, -1.0, -1.0,     0.0,  0.0,  0.0   // v7 Black
    ]);

    //顶点索引
    var indices = new Uint8Array([
        0, 1, 2,   0, 2, 3,    // 前
        0, 3, 4,   0, 4, 5,    // 右
        0, 5, 6,   0, 6, 1,    // 上
        1, 6, 7,   1, 7, 2,    // 左
        7, 4, 3,   7, 3, 2,    // 下
        4, 7, 6,   4, 6, 5     // 后
    ]);

    //创建缓冲区对象
    var vertexColorBuffer = gl.createBuffer();
    var indexBuffer = gl.createBuffer();
    if (!vertexColorBuffer || !indexBuffer) {
        console.log("无法创建缓冲区对象");
        return -1;
    }

    //绑定缓冲区对象并写入数据
    gl.bindBuffer(gl.ARRAY_BUFFER, vertexColorBuffer);
    gl.bufferData(gl.ARRAY_BUFFER, verticesColors, gl.STATIC_DRAW);

    //获取数组中一个元素所占的字节数
    var fsize = verticesColors.BYTES_PER_ELEMENT;

    //获取顶点位置变量位置
    var a_Position = gl.getAttribLocation(program, "a_Position");
    if (a_Position < 0) {
        console.log("无法获取顶点位置的存储变量");
        return -1;
    }

    //对位置的顶点数据进行分配，并开启
    gl.vertexAttribPointer(a_Position, 3, gl.FLOAT, false, fsize * 6, 0);
    gl.enableVertexAttribArray(a_Position);
    //获取顶点颜色的变量
    var a_Color = gl.getAttribLocation(program, "a_Color");
    if (a_Color < 0) {
        console.log("无法获取顶点位置的存储变量");
        return -1;
    }

    //对位置的顶点数据进行分配，并开启
    gl.vertexAttribPointer(a_Color, 3, gl.FLOAT, false, fsize * 6, fsize * 3);
    gl.enableVertexAttribArray(a_Color);

    //将顶点索引数据写入缓冲区对象
    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER,indexBuffer);
    gl.bufferData(gl.ELEMENT_ARRAY_BUFFER,indices,gl.STATIC_DRAW);

    return indices.length;
}