#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )//Hide the dos window

//Header files
#include "glut.h"
#include <math.h>

void Init(void)
{
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);//Set the color of the background(White)
	glColor3f(0.0, 0.0, 1.0);//Set the color of the circle(Blue)

	glMatrixMode(GL_PROJECTION);// Select the projection matrix
	glLoadIdentity();// Reset the projection matrix
	gluOrtho2D(0.0, 500.0, 0.0, 500.0);//Set the range of model coordinates corresponding to the screen area
	glMatrixMode(GL_MODELVIEW);// Select the modelview matrix
}

//Symmetry transform
void plotCirclePixel(int cx, int cy, int x, int y)
{
	//Every 1/8 circle
	glBegin(GL_POINTS);
	glVertex2i(cx + x, cy + y);
	glVertex2i(cx - x, cy + y);
	glVertex2i(cx + x, cy - y);
	glVertex2i(cx - x, cy - y);
	glVertex2i(cx + y, cy + x);
	glVertex2i(cx - y, cy + x);
	glVertex2i(cx + y, cy - x);
	glVertex2i(cx - y, cy - x);

	glEnd();
}

void BresenhamCircle(int cx, int cy, int r)
{
	int x = 0, y = r; //Initial position
	int d = 1 - r;//Initial d

	glBegin(GL_POINTS);
	glVertex2i(cx, cy);//Draw a point at the center of the circle
	glEnd();

	while (x <= y) {
		plotCirclePixel(cx, cy, x, y);
		if (d <= 0) {//Inside
			d += 2 * x + 3;
		}
		else {//Outside
			d += (x - y) * 2 + 5;
			y--;
		}
		x++;
	}
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	BresenhamCircle(250, 250, 150);
	glFlush();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowPosition(400, 100);//The position of the graphics window
	glutInitWindowSize(500, 500);//The size of the graphics window
	glutCreateWindow("Bresenham Circle");//The name of the graphics window

	Init();
	glutDisplayFunc(display);
	glutMainLoop();

	return 0;
}