// glutEx1.cpp : 定义控制台应用程序的入口点。
//

#if defined(WIN32)
#include <windows.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#pragma comment(lib, "glew32.lib")
#include <glew.h>
#include "glut.h"

#define BITMAP_ID 0x4D42
#define iSize 64

float fTranslate;
float fRotate;
float fScale = 1.0f; // set inital scale value to 1.0f

float mat_ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};
float mat_diffuse[] = {0.6f, 0.6f, 0.6f, 1.0f};
float mat_specular[] = {0.6f, 0.6f, 0.6f, 1.0f};

GLfloat lightPos[] = {5.0f, 5.0f, 5.0f, 1.0f};
GLfloat lightColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat lightSpotPos[] = {0.0f, 10.0f, 0.0f, 1.0f};
GLfloat lightSpotCutOff = 2.0f;
GLfloat lightSpotDirection[] = {0.0f, -1.0f, 0.0f};
GLfloat lightSpotExponent = 2.0f;

bool teapot = false;
bool bPersp = false;
bool bAnim = false;
bool bWire = false;
bool overlay = false;

int wHeight = 0;
int wWidth = 0;

static GLubyte checkImage[iSize][iSize][3];
GLuint *texture = new GLuint[4];

unsigned char *LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader)
{
	FILE *filePtr;					   // 文件指针
	BITMAPFILEHEADER bitmapFileHeader; // bitmap文件头
	unsigned char *bitmapImage;		   // bitmap图像数据
	unsigned int imageIdx = 0;		   // 图像位置索引
	unsigned char tempRGB;			   // 交换变量

	// 以“二进制+读”模式打开文件filename
	filePtr = fopen(filename, "rb");
	if (filePtr == NULL)
		return NULL;
	// 读入bitmap文件图
	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	// 验证是否为bitmap文件
	if (bitmapFileHeader.bfType != BITMAP_ID)
	{
		fprintf(stderr, "Error in LoadBitmapFile: the file is not a bitmap file\n");
		return NULL;
	}

	// 读入bitmap信息头
	fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	// 将文件指针移至bitmap数据
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);
	// 为装载图像数据创建足够的内存
	bitmapImage = new unsigned char[bitmapInfoHeader->biSizeImage];
	// 验证内存是否创建成功
	if (!bitmapImage)
	{
		fprintf(stderr, "Error in LoadBitmapFile: memory error\n");
		return NULL;
	}

	// 读入bitmap图像数据
	fread(bitmapImage, 1, bitmapInfoHeader->biSizeImage, filePtr);
	// 确认读入成功
	if (bitmapImage == NULL)
	{
		fprintf(stderr, "Error in LoadBitmapFile: memory error\n");
		return NULL;
	}

	//由于bitmap中保存的格式是BGR，下面交换R和B的值，得到RGB格式
	for (imageIdx = 0;
		 imageIdx < bitmapInfoHeader->biSizeImage; imageIdx += 3)
	{
		tempRGB = bitmapImage[imageIdx];
		bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
		bitmapImage[imageIdx + 2] = tempRGB;
	}

	// 关闭bitmap图像文件
	fclose(filePtr);
	return bitmapImage;
}

void texload(int i, char *filename)
{
	BITMAPINFOHEADER bitmapInfoHeader; // bitmap信息头
	unsigned char *bitmapData;		   // 纹理数据

	bitmapData = LoadBitmapFile(filename, &bitmapInfoHeader);
	glBindTexture(GL_TEXTURE_2D, texture[i]);
	// 指定当前纹理的放大/缩小过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D,
				 0,							//mipmap层次(通常为，表示最上层)
				 GL_RGB,					//我们希望该纹理有红、绿、蓝数据
				 bitmapInfoHeader.biWidth,  //纹理宽带，必须是n，若有边框+2
				 bitmapInfoHeader.biHeight, //纹理高度，必须是n，若有边框+2
				 0,							//边框(0=无边框, 1=有边框)
				 GL_RGB,					//bitmap数据的格式
				 GL_UNSIGNED_BYTE,			//每个颜色数据的类型
				 bitmapData);				//bitmap数据指针
}

void MakeImage()
{
	int i, j, c;

	for (i = 0; i < iSize; i++)
	{
		for (j = 0; j < iSize; j++)
		{
			c = ((((i & 0x8) == 0) ^ ((j & 0x8)) == 0)) * 255;
			checkImage[i][j][0] = (GLubyte)c;
			checkImage[i][j][1] = (GLubyte)0;
			checkImage[i][j][2] = (GLubyte)0;
		}
	}
}

void LoadMyTexture(int i)
{
	glBindTexture(GL_TEXTURE_2D, texture[i]);
	// 指定当前纹理的放大/缩小过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D,
				 0,				   //mipmap层次(通常为，表示最上层)
				 GL_RGB,		   //我们希望该纹理有红、绿、蓝数据
				 iSize,			   //纹理宽带，必须是n，若有边框+2
				 iSize,			   //纹理高度，必须是n，若有边框+2
				 0,				   //边框(0=无边框, 1=有边框)
				 GL_RGB,		   //bitmap数据的格式
				 GL_UNSIGNED_BYTE, //每个颜色数据的类型
				 checkImage);	  //bitmap数据指针
}

void Init()
{
	MakeImage();

	glGenTextures(4, texture); // 第一参数是需要生成标示符的个数, 第二参数是返回标示符的数组
	char *file1 = new char[20]{"ID.bmp"};
	char *file2 = new char[20]{"Crack.bmp"};
	char *file3 = new char[20]{"Spot.bmp"};
	texload(0, file1); // 加载纹理一
	texload(1, file2); // 加载纹理二
	texload(2, file3); // 加载纹理三
	LoadMyTexture(3);  // 加载纹理四

	glDisable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glDisable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE2);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glDisable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
}

void Draw_Leg();
void Draw_Leg2();
void Draw_Cube(GLfloat size);
void Draw_Cube2(GLfloat size);

void Draw_Scene()
{
	glEnable(GL_TEXTURE_2D);
	if (teapot)
		glBindTexture(GL_TEXTURE_2D, texture[0]);
	else
		glBindTexture(GL_TEXTURE_2D, texture[3]);
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMateriali(GL_FRONT, GL_SHININESS, 32);
	glTranslatef(0, 0, 4.7f);
	glRotatef(90, 1, 0, 0);
	glutSolidTeapot(1);
	glPopMatrix();

	if (overlay)
	{
		glDisable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE1);
		glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE2);
		glEnable(GL_TEXTURE_2D);
		glPushMatrix();
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
		glMateriali(GL_FRONT, GL_SHININESS, 32);
		glTranslatef(0, 0, 3.5);
		glScalef(5, 4, 1);
		Draw_Cube2(1.0f);
		glPopMatrix();
		glActiveTexture(GL_TEXTURE1);
		glDisable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE2);
		glDisable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);

		glDisable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE1);
		glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE2);
		glEnable(GL_TEXTURE_2D);
		glPushMatrix();
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
		glMateriali(GL_FRONT, GL_SHININESS, 32);
		glTranslatef(1.5, 1, 1.5);
		Draw_Leg2();
		glPopMatrix();
		glActiveTexture(GL_TEXTURE1);
		glDisable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE2);
		glDisable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);

		glDisable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE1);
		glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE2);
		glEnable(GL_TEXTURE_2D);
		glPushMatrix();
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
		glMateriali(GL_FRONT, GL_SHININESS, 32);
		glTranslatef(-1.5, 1, 1.5);
		Draw_Leg2();
		glPopMatrix();
		glActiveTexture(GL_TEXTURE1);
		glDisable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE2);
		glDisable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);

		glDisable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE1);
		glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE2);
		glEnable(GL_TEXTURE_2D);
		glPushMatrix();
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
		glMateriali(GL_FRONT, GL_SHININESS, 32);
		glTranslatef(1.5, -1, 1.5);
		Draw_Leg2();
		glPopMatrix();
		glActiveTexture(GL_TEXTURE1);
		glDisable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE2);
		glDisable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);

		glDisable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE1);
		glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE2);
		glEnable(GL_TEXTURE_2D);
		glPushMatrix();
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
		glMateriali(GL_FRONT, GL_SHININESS, 32);
		glTranslatef(-1.5, -1, 1.5);
		Draw_Leg2();
		glPopMatrix();
		glActiveTexture(GL_TEXTURE1);
		glDisable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE2);
		glDisable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glPushMatrix();
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
		glMateriali(GL_FRONT, GL_SHININESS, 32);
		glTranslatef(0, 0, 3.5);
		glScalef(5, 4, 1);
		Draw_Cube(1.0f);
		glPopMatrix();

		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glPushMatrix();
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
		glMateriali(GL_FRONT, GL_SHININESS, 32);
		glTranslatef(1.5, 1, 1.5);
		Draw_Leg();
		glPopMatrix();

		glPushMatrix();
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
		glMateriali(GL_FRONT, GL_SHININESS, 32);
		glTranslatef(-1.5, 1, 1.5);
		Draw_Leg();
		glPopMatrix();

		glPushMatrix();
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
		glMateriali(GL_FRONT, GL_SHININESS, 32);
		glTranslatef(1.5, -1, 1.5);
		Draw_Leg();
		glPopMatrix();

		glPushMatrix();
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
		glMateriali(GL_FRONT, GL_SHININESS, 32);
		glTranslatef(-1.5, -1, 1.5);
		Draw_Leg();
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);
	}
}

void Draw_Cube2(GLfloat size)
{
	float cx = size / 2.0f;
	float cy = size / 2.0f;
	float cz = size / 2.0f;

	glBegin(GL_QUADS);
	// 左面
	glMultiTexCoord2f(GL_TEXTURE1, 0.0f, 0.0f);
	glMultiTexCoord2f(GL_TEXTURE2, 0.0f, 0.0f);
	glVertex3f(-cx, -cy, -cz);
	glMultiTexCoord2f(GL_TEXTURE1, 0.0f, 1.0f);
	glMultiTexCoord2f(GL_TEXTURE2, 0.0f, 1.0f);
	glVertex3f(-cx, cy, -cz);
	glMultiTexCoord2f(GL_TEXTURE1, 1.0f, 1.0f);
	glMultiTexCoord2f(GL_TEXTURE2, 1.0f, 1.0f);
	glVertex3f(-cx, cy, cz);
	glMultiTexCoord2f(GL_TEXTURE1, 1.0f, 0.0f);
	glMultiTexCoord2f(GL_TEXTURE2, 1.0f, 0.0f);
	glVertex3f(-cx, -cy, cz);
	// 右面
	glMultiTexCoord2f(GL_TEXTURE1, 0.0f, 0.0f);
	glMultiTexCoord2f(GL_TEXTURE2, 0.0f, 0.0f);
	glVertex3f(cx, -cy, -cz);
	glMultiTexCoord2f(GL_TEXTURE1, 0.0f, 1.0f);
	glMultiTexCoord2f(GL_TEXTURE2, 0.0f, 1.0f);
	glVertex3f(cx, cy, -cz);
	glMultiTexCoord2f(GL_TEXTURE1, 1.0f, 1.0f);
	glMultiTexCoord2f(GL_TEXTURE2, 1.0f, 1.0f);
	glVertex3f(cx, cy, cz);
	glMultiTexCoord2f(GL_TEXTURE1, 1.0f, 0.0f);
	glMultiTexCoord2f(GL_TEXTURE2, 1.0f, 0.0f);
	glVertex3f(cx, -cy, cz);
	// 上面
	glMultiTexCoord2f(GL_TEXTURE1, 0.0f, 0.0f);
	glMultiTexCoord2f(GL_TEXTURE2, 0.0f, 0.0f);
	glVertex3f(-cx, cy, -cz);
	glMultiTexCoord2f(GL_TEXTURE1, 0.0f, 1.0f);
	glMultiTexCoord2f(GL_TEXTURE2, 0.0f, 1.0f);
	glVertex3f(cx, cy, -cz);
	glMultiTexCoord2f(GL_TEXTURE1, 1.0f, 1.0f);
	glMultiTexCoord2f(GL_TEXTURE2, 1.0f, 1.0f);
	glVertex3f(cx, cy, cz);
	glMultiTexCoord2f(GL_TEXTURE1, 1.0f, 0.0f);
	glMultiTexCoord2f(GL_TEXTURE2, 1.0f, 0.0f);
	glVertex3f(-cx, cy, cz);
	// 下面
	glMultiTexCoord2f(GL_TEXTURE1, 0.0f, 0.0f);
	glMultiTexCoord2f(GL_TEXTURE2, 0.0f, 0.0f);
	glVertex3f(-cx, -cy, -cz);
	glMultiTexCoord2f(GL_TEXTURE1, 0.0f, 1.0f);
	glMultiTexCoord2f(GL_TEXTURE2, 0.0f, 1.0f);
	glVertex3f(cx, -cy, -cz);
	glMultiTexCoord2f(GL_TEXTURE1, 1.0f, 1.0f);
	glMultiTexCoord2f(GL_TEXTURE2, 1.0f, 1.0f);
	glVertex3f(cx, -cy, cz);
	glMultiTexCoord2f(GL_TEXTURE1, 1.0f, 0.0f);
	glMultiTexCoord2f(GL_TEXTURE2, 1.0f, 0.0f);
	glVertex3f(-cx, -cy, cz);
	// 前面
	glMultiTexCoord2f(GL_TEXTURE1, 0.0f, 0.0f);
	glMultiTexCoord2f(GL_TEXTURE2, 0.0f, 0.0f);
	glVertex3f(-cx, -cy, -cz);
	glMultiTexCoord2f(GL_TEXTURE1, 0.0f, 1.0f);
	glMultiTexCoord2f(GL_TEXTURE2, 0.0f, 1.0f);
	glVertex3f(cx, -cy, -cz);
	glMultiTexCoord2f(GL_TEXTURE1, 1.0f, 1.0f);
	glMultiTexCoord2f(GL_TEXTURE2, 1.0f, 1.0f);
	glVertex3f(cx, cy, -cz);
	glMultiTexCoord2f(GL_TEXTURE1, 1.0f, 0.0f);
	glMultiTexCoord2f(GL_TEXTURE2, 1.0f, 0.0f);
	glVertex3f(-cx, cy, -cz);
	// 后面
	glMultiTexCoord2f(GL_TEXTURE1, 0.0f, 0.0f);
	glMultiTexCoord2f(GL_TEXTURE2, 0.0f, 0.0f);
	glVertex3f(-cx, -cy, cz);
	glMultiTexCoord2f(GL_TEXTURE1, 0.0f, 1.0f);
	glMultiTexCoord2f(GL_TEXTURE2, 0.0f, 1.0f);
	glVertex3f(cx, -cy, cz);
	glMultiTexCoord2f(GL_TEXTURE1, 1.0f, 1.0f);
	glMultiTexCoord2f(GL_TEXTURE2, 1.0f, 1.0f);
	glVertex3f(cx, cy, cz);
	glMultiTexCoord2f(GL_TEXTURE1, 1.0f, 0.0f);
	glMultiTexCoord2f(GL_TEXTURE2, 1.0f, 0.0f);
	glVertex3f(-cx, cy, cz);
	glEnd();
}

void Draw_Cube(GLfloat size)
{
	float cx = size / 2.0f;
	float cy = size / 2.0f;
	float cz = size / 2.0f;

	glBegin(GL_QUADS);
	// 左面
	glMultiTexCoord2f(GL_TEXTURE0, 0.0f, 0.0f);
	glVertex3f(-cx, -cy, -cz);
	glMultiTexCoord2f(GL_TEXTURE0, 0.0f, 1.0f);
	glVertex3f(-cx, cy, -cz);
	glMultiTexCoord2f(GL_TEXTURE0, 1.0f, 1.0f);
	glVertex3f(-cx, cy, cz);
	glMultiTexCoord2f(GL_TEXTURE0, 1.0f, 0.0f);
	glVertex3f(-cx, -cy, cz);
	// 右面
	glMultiTexCoord2f(GL_TEXTURE0, 0.0f, 0.0f);
	glVertex3f(cx, -cy, -cz);
	glMultiTexCoord2f(GL_TEXTURE0, 0.0f, 1.0f);
	glVertex3f(cx, cy, -cz);
	glMultiTexCoord2f(GL_TEXTURE0, 1.0f, 1.0f);
	glVertex3f(cx, cy, cz);
	glMultiTexCoord2f(GL_TEXTURE0, 1.0f, 0.0f);
	glVertex3f(cx, -cy, cz);
	// 上面
	glMultiTexCoord2f(GL_TEXTURE0, 0.0f, 0.0f);
	glVertex3f(-cx, cy, -cz);
	glMultiTexCoord2f(GL_TEXTURE0, 0.0f, 1.0f);
	glVertex3f(cx, cy, -cz);
	glMultiTexCoord2f(GL_TEXTURE0, 1.0f, 1.0f);
	glVertex3f(cx, cy, cz);
	glMultiTexCoord2f(GL_TEXTURE0, 1.0f, 0.0f);
	glVertex3f(-cx, cy, cz);
	// 下面
	glMultiTexCoord2f(GL_TEXTURE0, 0.0f, 0.0f);
	glVertex3f(-cx, -cy, -cz);
	glMultiTexCoord2f(GL_TEXTURE0, 0.0f, 1.0f);
	glVertex3f(cx, -cy, -cz);
	glMultiTexCoord2f(GL_TEXTURE0, 1.0f, 1.0f);
	glVertex3f(cx, -cy, cz);
	glMultiTexCoord2f(GL_TEXTURE0, 1.0f, 0.0f);
	glVertex3f(-cx, -cy, cz);
	// 前面
	glMultiTexCoord2f(GL_TEXTURE0, 0.0f, 0.0f);
	glVertex3f(-cx, -cy, -cz);
	glMultiTexCoord2f(GL_TEXTURE0, 0.0f, 1.0f);
	glVertex3f(cx, -cy, -cz);
	glMultiTexCoord2f(GL_TEXTURE0, 1.0f, 1.0f);
	glVertex3f(cx, cy, -cz);
	glMultiTexCoord2f(GL_TEXTURE0, 1.0f, 0.0f);
	glVertex3f(-cx, cy, -cz);
	// 后面
	glMultiTexCoord2f(GL_TEXTURE0, 0.0f, 0.0f);
	glVertex3f(-cx, -cy, cz);
	glMultiTexCoord2f(GL_TEXTURE0, 0.0f, 1.0f);
	glVertex3f(cx, -cy, cz);
	glMultiTexCoord2f(GL_TEXTURE0, 1.0f, 1.0f);
	glVertex3f(cx, cy, cz);
	glMultiTexCoord2f(GL_TEXTURE0, 1.0f, 0.0f);
	glVertex3f(-cx, cy, cz);
	glEnd();
}

void Draw_Leg()
{
	glScalef(1, 1, 3);
	Draw_Cube(1.0);
}

void Draw_Leg2()
{
	glScalef(1, 1, 3);
	Draw_Cube2(1.0);
}

void updateView(int width, int height)
{
	glViewport(0, 0, width, height); // Reset The Current Viewport

	glMatrixMode(GL_PROJECTION); // Select The Projection Matrix
	glLoadIdentity();			 // Reset The Projection Matrix

	float whRatio = (GLfloat)width / (GLfloat)height;
	if (bPersp)
	{
		gluPerspective(45.0f, whRatio, 0.1f, 100.0f);
		//glFrustum(-3, 3, -3, 3, 3,100);
	}
	else
	{
		glOrtho(-3, 3, -3, 3, -100, 100);
	}

	glMatrixMode(GL_MODELVIEW); // Select The Modelview Matrix
}

void reshape(int width, int height)
{
	if (height == 0) // Prevent A Divide By Zero By
	{
		height = 1; // Making Height Equal One
	}

	wHeight = height;
	wWidth = width;

	updateView(wHeight, wWidth);
}

void idle()
{
	glutPostRedisplay();
}

float eye[] = {0, 0, 8};
float center[] = {0, 0, 0};

void key(unsigned char k, int x, int y)
{
	switch (k)
	{
	case 27:
	{
		exit(0);
		break;
	}
	case 'p':
	{
		bPersp = !bPersp;
		break;
	}
	case ' ':
	{
		bAnim = !bAnim;
		break;
	}
	case 'z':
	{
		bWire = !bWire;
		break;
	}
	case 'x':
	{
		teapot = !teapot;
		break;
	}
	case 'a':
	{
		eye[0] += 0.2f;
		center[0] += 0.2f;
		break;
	}
	case 'd':
	{
		eye[0] -= 0.2f;
		center[0] -= 0.2f;
		break;
	}
	case 's':
	{
		eye[1] += 0.2f;
		center[1] += 0.2f;
		break;
	}
	case 'w':
	{
		eye[1] -= 0.2f;
		center[1] -= 0.2f;
		break;
	}
	case 'q':
	{
		eye[2] -= 0.2f;
		center[2] -= 0.2f;
		break;
	}
	case 'e':
	{
		eye[2] += 0.2f;
		center[2] += 0.2f;
		break;
	}
	case ']':
	{
		overlay = !overlay;
		break;
	}
	}

	updateView(wHeight, wWidth);
}

void redraw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity(); // Reset The Current Modelview Matrix

	gluLookAt(eye[0], eye[1], eye[2],
			  center[0], center[1], center[2],
			  0, 1, 0); // 场景（0，0，0）的视点中心 (0,5,50)，Y轴向上

	if (bWire)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

	glLightfv(GL_LIGHT0, GL_AMBIENT, lightColor);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glEnable(GL_LIGHT0);

	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor);
	glLightfv(GL_LIGHT1, GL_SPECULAR, lightColor);
	glLightfv(GL_LIGHT1, GL_POSITION, lightSpotPos);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, lightSpotCutOff);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, lightSpotDirection);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, lightSpotExponent);
	glEnable(GL_LIGHT1);

	glTranslatef(0.0f, -1.5f, 0.0f); // Place the triangle at Center
	glRotatef(fRotate, 0, 1.0f, 0);  // Rotate around Y axis
	glRotatef(-90, 1, 0, 0);
	glScalef(0.5f, 0.5f, 0.5f);
	Draw_Scene(); // Draw triangle

	if (bAnim)
		fRotate += 0.5f;
	glutSwapBuffers();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(480, 480);
	int windowHandle = glutCreateWindow("Simple GLUT App");

	glewInit();
	Init();

	glutDisplayFunc(redraw);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(key);
	glutIdleFunc(idle);

	glutMainLoop();
	return 0;
}