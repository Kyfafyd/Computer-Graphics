#include <stdio.h>
#include <windows.h>		

#include "include/glew.h"
#include "include/glut.h"
#include "include/CxBitmap.h"

#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
// Light and material Data
GLfloat fLightPos[4] = { -100.0f, 100.0f, 50.0f, 1.0f };  // Point source
GLfloat fNoLight[] = { 0.0f, 0.0f, 0.0f, 0.0f };
GLfloat fLowLight[] = { 0.25f, 0.25f, 0.25f, 1.0f };
GLfloat fBrightLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };

enum
{
	SUN = 10
};

GLfloat c_radius[11] = { 57.9f, 108.2f, 149.6f, 227.9f, 778.4f, 1426.7f,270.9f, 448.25f, 596.38f, 6.0f, 0.0f };
GLfloat s_radius[11] = { 5.0f, 6.0f, 6.3f, 4.9f, 35.0f, 30.0f,25.5f, 24.7f, 11.0f, 1.734f, 69.0f };
GLfloat c_angle[11] = { 0.0f,0.0f,0.0f, 0.0f,0.0f, 0.0f, 0.0f,0.0f, 0.0f, 0.0f,0.0f };
GLfloat c_period[11] = { 0.87f, 2.24f, 3.65f, 6.86f, 43.25f, 107.45f, 36.6f, 60.5f, 90.9f,0.48f, 0.7f };

char filename[][20] = {
	"data/mercurial.bmp","data/venus.bmp","data/earth.bmp","data/mars.bmp","data/jupiter.bmp","data/saturn.bmp",
	"data/uranus.bmp","data/neptune.bmp","data/pluto.bmp","data/moon.bmp","data/sun.bmp"
};

GLuint	texture[11];// 存放11个纹理

typedef struct
{
	GLfloat self_radius;//行星半径
	GLfloat cir_radius;//公转半径
	GLfloat angle;//观察角度
	GLfloat step;
}Planet;

Planet planet[11];

bool g_fullscreen = false;// 全屏标志缺省，缺省设定成全屏模式
int g_window_width = 1000;
int g_window_height = 750;
int g_zdepth = 0;
int g_hMove = 0;
int nRotateLeft = 0;
int nRotateRight = 0;
int nPercent = 10;
const int DEPTH_CHANGE = 5;
GLUquadric* quad;

int LoadGLTextures()// 载入位图并转换成纹理
{
	int Status = FALSE;// 状态指示器
	CxBitmap bitmap;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(11, texture);// 创建一个纹理

	for (int i = 0; i < 11; i++)// 载入位图，查错，如果未找到位图文件则退出
	{
		if (bitmap.LoadBitmap(filename[i]))
		{
			if (i == 10) Status = TRUE;// 将Status设为TRUE
			glBindTexture(GL_TEXTURE_2D, texture[i]);
			if (bitmap.GetBytesPerPixel() == 3)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmap.GetWidth(), bitmap.GetHeight(), 0,
					GL_BGR_EXT, GL_UNSIGNED_BYTE, bitmap.GetBuffer());
			}
			else if (bitmap.GetBytesPerPixel() == 4)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bitmap.GetWidth(), bitmap.GetHeight(), 0,
					GL_BGRA_EXT, GL_UNSIGNED_BYTE, bitmap.GetBuffer());
			}
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);//映射方式
		}
	}

	return Status;// 返回 Status的值
}

void Reshape(GLsizei width, GLsizei height)		// 重置OpenGL窗口大小
{
	if (height == 0)										// 防止被零除
	{
		height = 1;										// 将Height设为1
	}
	glViewport(0, 0, width, height);						// 重置当前的视口
	glMatrixMode(GL_PROJECTION);						// 选择投影矩阵
	glLoadIdentity();									// 重置投影矩阵
	gluPerspective(60.0f, (GLfloat)width / (GLfloat)height, 0.1f, 2000.0f);// 设置视口的大小
	glMatrixMode(GL_MODELVIEW);							// 选择模型观察矩阵
	glLoadIdentity();									// 重置模型观察矩阵
	gluLookAt(0, 0, 1500, 0, 0, 0, 0, 1, 0);
}

int Init()										// 此处开始对OpenGL进行所有设置
{
	if (!LoadGLTextures())								// 调用纹理载入子例程
	{
		return FALSE;									// 如果未能载入，返回FALSE
	}

	for (int i = 0; i < 11; i++)
	{
		planet[i].angle = 100.0;
		planet[i].self_radius = s_radius[i];
		planet[i].cir_radius = c_radius[i];
		planet[i].step = 2.0 / c_period[i];
	}

	// Cull backs of polygons
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_MULTISAMPLE_ARB);

	// Setup light parameters
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, fNoLight);
	glLightfv(GL_LIGHT0, GL_AMBIENT, fLowLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, fBrightLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, fBrightLight);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);

	// Mostly use material tracking
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glMaterialfv(GL_FRONT, GL_SPECULAR, fBrightLight);
	glMateriali(GL_FRONT, GL_SHININESS, 128);

	glShadeModel(GL_SMOOTH);							// 启用阴影平滑
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);				// 黑色背景
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// 告诉系统对透视进行修正

	quad = gluNewQuadric();
	gluQuadricNormals(quad, GL_SMOOTH);						// 使用平滑法线
	gluQuadricTexture(quad, GL_TRUE);						// 使用纹理
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

	return TRUE;
}

void Display()									// 从这里开始进行所有的绘制
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// 清除屏幕和深度缓存

	glTranslatef(0.0, 0.0, g_zdepth);
	glTranslatef(g_hMove, 0.0, 0.0);
	glBindTexture(GL_TEXTURE_2D, texture[SUN]);
	gluSphere(quad, planet[SUN].self_radius, 50, 67);
	for (int i = 0; i < 10; ++i)
	{
		glPushMatrix();
		glRotatef(planet[i].angle, 0.0, 1.0, 0.0);
		glTranslatef(0.0, 0.0, -(planet[i].cir_radius + planet[SUN].self_radius));
		glBindTexture(GL_TEXTURE_2D, texture[i]);
		gluSphere(quad, planet[i].self_radius, 100, 100);
		glPopMatrix();
	}

	glutSwapBuffers();
}

void TimerFunction(int value)
{
	for (int i = 0; i < 10; i++)
	{
		planet[i].angle += planet[i].step;
		if (planet[i].angle >= 360.0)
		{
			planet[i].angle -= 360.0;
		}
	}
	g_zdepth = 0;
	g_hMove = 0;
	if (KEY_DOWN(VK_UP)) g_zdepth = DEPTH_CHANGE;
	if (KEY_DOWN(VK_DOWN)) g_zdepth = -DEPTH_CHANGE;
	if (KEY_DOWN(VK_LEFT)) g_hMove = -DEPTH_CHANGE;
	if (KEY_DOWN(VK_RIGHT)) g_hMove = DEPTH_CHANGE;

	glutPostRedisplay();
	glutTimerFunc(33, TimerFunction, 1);
}

void SpecialKeys(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_F1:
		g_fullscreen = !g_fullscreen;
		if (g_fullscreen)
		{
			g_window_width = glutGet(GLUT_WINDOW_WIDTH);
			g_window_height = glutGet(GLUT_WINDOW_HEIGHT);
			glutFullScreen();
		}
		else glutReshapeWindow(g_window_width, g_window_height);
		break;
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1000, 750);
	glutCreateWindow("Solar Syatem");
	Init();
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutTimerFunc(33, TimerFunction, 1);
	glutSpecialFunc(SpecialKeys);
	glutMainLoop();

	return 0;
}