#include <stdlib.h>
#include "glut.h"

float fRotate = 0.0f; //��ת���ӣ���������ӣ�
float fScale = 1.0f; //��������

float tRotate = 0.0f; //��ת���ӣ������

bool bPersp = false; //�Ƿ�Ϊ͸��ͶӰ ��vs ��ͶӰ��
bool bAnim = false; // ����������Ƿ���ת
bool bWire = false; // ����ģʽ�Ƿ�Ϊ���� ��vs ��䣩
bool isRotate = false; //����Ƿ���ת

int wHeight = 0;
int wWidth = 0;

int min(int x, int y)
{
	return x < y ? x : y;
}

//�����Ȳ�
void Draw_Leg()
{
	glScalef(1, 1, 3);
	glutSolidCube(1.0);
}

void Draw_Scene(float place[])
{
	//�����
	glPushMatrix();
	glTranslatef(place[0], place[1], place[2]);//����ƽ��
	glRotatef(90, 1, 0, 0);
	glRotatef(tRotate, 0, 1, 0);//������ת
	glutSolidTeapot(1);
	glPopMatrix();

	//������
	glPushMatrix();
	glTranslatef(0, 0, 3.5);
	glScalef(5, 4, 1);
	glutSolidCube(1.0);
	glPopMatrix();

	//��������
	glPushMatrix();
	glTranslatef(1.5, 1, 1.5);
	Draw_Leg();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, 1, 1.5);
	Draw_Leg();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.5, -1, 1.5);
	Draw_Leg();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, -1, 1.5);
	Draw_Leg();
	glPopMatrix();

}

void updateView(int width, int height)
{
	glViewport(0, 0, width, height);//�����Ӵ���С

	glMatrixMode(GL_PROJECTION);//���þ���ģʽΪͶӰ
	glLoadIdentity();	 //��ʼ������Ϊ��λ����  

	float whRatio = (GLfloat)width / (GLfloat)height;//������ʾ����

	if (bPersp) {
		gluPerspective(45, whRatio, 1, 100); //͸��ͶӰ
		//������

		//GLdouble fovy �ӽ�
		//GLdouble aspect ��߱�
		//GLdouble zNear ����
		//GLdouble zFar Զ��
	}
	else
		glOrtho(-3, 3, -3, 3, -100, 100); //��ͶӰ
		//����

		//GLdouble left,
		//GLdouble right,
		//GLdouble bottom,
		//GLdouble top,
		//GLdouble near,
		//GLdouble far

	glMatrixMode(GL_MODELVIEW);	//���þ���ģʽΪģ��
}

void reshape(int width, int height)
{
	if (height == 0)	//����߶�Ϊ0
	{
		height = 1;	//�ø߶�Ϊ1��������ַ�ĸΪ0������
	}

	height = width = min(height, width);
	wHeight = height;
	wWidth = width;

	updateView(wHeight, wWidth); //�����ӽ�
}

void idle()
{
	glutPostRedisplay();//���õ�ǰ���ƺ��� 
}

float eye[] = { 0, 0, 8 };
float center[] = { 0, 0, 0 };
float place[] = { 0, 0, 5 };

//�����ص�����
void key(unsigned char k, int x, int y)
{
	switch (k)
	{
	case 'q': {exit(0); break; } //�˳�
	case 'p': {bPersp = !bPersp; updateView(wHeight, wWidth); break; } //�л���ͶӰ��͸��ͶӰ

	case ' ': {bAnim = !bAnim; break; } //��תģʽ���л�
	case 'o': {bWire = !bWire; break; } //��Ⱦ��ʽ���л�

	//�������

	case 'a': { //�����ƶ�
		center[0] += 0.1f;
		eye[0] += 0.1f;
		break;
	}
	case 'd': { //�����ƶ�
		center[0] -= 0.1f;
		eye[0] -= 0.1f;
		break;
	}
	case 'w': { //�����ƶ�
		center[1] -= 0.1f;
		eye[1] -= 0.1f;
		break;
	}
	case 's': { //�����ƶ�
		center[1] += 0.1f;
		eye[1] += 0.1f;
		break;
	}
	case 'z': { //��ǰ�ƶ�
		center[2] -= 0.1f;
		eye[2] -= 0.1f;
		break;
	}
	case 'c': { //����ƶ�
		center[2] += 0.1f;
		eye[2] += 0.1f;
		break;
	}

			  //�����ز���

	case 'l': { //���Ʋ��
		place[0] += 0.1f;
		if (place[0] > 1.5f)place[0] = 1.5f; //���������淶Χ
		break;
	}
	case 'j': { //���Ʋ��
		place[0] -= 0.1f;
		if (place[0] < -1.5f)place[0] = -1.5f;
		break;
	}
	case 'i': { //���Ʋ��
		place[1] += 0.1f;
		if (place[1] > 1.5f)place[1] = 1.5f;
		break;
	}
	case 'k': { //ǰ�Ʋ��
		place[1] -= 0.1f;
		if (place[1] < -1.5f)place[1] = -1.5f;
		break;
	}
	case 'e': { //��ת���
		isRotate = !isRotate;
		break;
	}
	}
}


void redraw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //�����ɫ�������Ȼ���
	glLoadIdentity(); //��ʼ������Ϊ��λ����

	gluLookAt(eye[0], eye[1], eye[2],
		center[0], center[1], center[2],
		0, 1, 0);

	//  �۲�λ��(eye[0],eye[1],eye[2]
	//	����λ��(center[0],center[1],center[2]
	//	�۲췽��(x, y, z) 

	if (bWire) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //���ö���λ���ģʽ�������棬����  
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //���ö���λ���ģʽ�������棬���  
	}

	glEnable(GL_DEPTH_TEST);  //������Ȳ���
	glEnable(GL_LIGHTING); //��������ģʽ
	GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 }; // ������ɫ
	GLfloat light_pos[] = { 5,5,5,1 };  //�����Դλ��

	glLightfv(GL_LIGHT0, GL_POSITION, light_pos); //���õ�0�Ź�Դ�Ĺ���λ��
	glLightfv(GL_LIGHT0, GL_AMBIENT, white); //���õ�0�Ź�Դ��η����Ĺ�����ɫ����������ɫ��
	glEnable(GL_LIGHT0); //������0�Ź�Դ

	glRotatef(fRotate, 0, 1.0f, 0);	//��ת
	glRotatef(-90, 1, 0, 0); //��ת
	glScalef(0.2, 0.2, 0.2); //����
	Draw_Scene(place); //��������

	if (bAnim) fRotate += 0.5f; //�������
	if (isRotate) tRotate += 0.5f; //�����ת

	glutSwapBuffers(); //����������  
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);//��glut�ĳ�ʼ�� 
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);//��ʼ����ʾģʽ:RGB��ɫģ�ͣ���Ȳ��ԣ�˫����   
	glutInitWindowSize(480, 480);//���ô��ڴ�С  
	int windowHandle = glutCreateWindow("Ex 3");//���ô��ڱ���   

	glutDisplayFunc(redraw);//ע����ƻص�����  
	glutReshapeFunc(reshape);//ע���ػ�ص����� 
	glutKeyboardFunc(key);//ע�ᰴ���ص�����
	glutIdleFunc(idle);//ע��ȫ�ֻص�����������ʱ����  

	glutMainLoop();// glut�¼�����ѭ��
	return 0;
}

