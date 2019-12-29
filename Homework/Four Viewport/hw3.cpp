#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"

#include <glut/glut.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

bool bPersp = false;
bool bAnim = false;
bool bWire = false;

GLfloat eye[] = {0, 0, 8}; // eye and center are used to move the object
GLfloat center[] = {0, 0, 0};
//GLfloat up[] = {0.1949,5.201,0.9801};
GLfloat up[] = {0,1,0};
GLfloat cx = 1.6227;
GLfloat cy = 1.5913;
GLfloat Angle = 0;

void Draw_Leg()
{
    glScalef(1, 1, 3);
    glutSolidCube(1.0);
}

void Draw_Table()
{
    glPushMatrix();
    glTranslatef(0, 0, 3.5);
    glScalef(5, 4, 1);
    glutSolidCube(1.0);
    glPopMatrix();

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

void Draw_Scene()
{
    glPushMatrix();
    glTranslatef(0.0f, -0.5f, 0.0f);
    glRotatef(Angle, 0, 1.0f, 0);
    glRotatef(-90, 1, 0, 0);
    glScalef(0.3, 0.3, 0.3);
    Draw_Table();
    glPopMatrix();
}

void idle()
{
    glutPostRedisplay();
}

void key(unsigned char k, int x, int y)
{
    switch (k)
    {
    case 27:
    case 'q':
        exit(0);
        break;
    case 'p':
        bPersp = !bPersp;
        break;
    case ' ':
        bAnim = !bAnim;
        break;
    case 'o':
        bWire = !bWire;
        break;
    case 'z':
        eye[2] -= 0.1f;
        center[2] -= 0.1f;
        break;
    case 'c':
        eye[2] += 0.1f;
        center[2] += 0.1f;
        break;
    case 'w':
        eye[1] -= 0.1f;
        center[1] -= 0.1f;
        break;
    case 's':
        eye[1] += 0.1f;
        center[1] += 0.1f;
        break;
    case 'a':
        eye[0] += 0.1f;
        center[0] += 0.1f;
        break;
    case 'd':
        eye[0] -= 0.1f;
        center[0] -= 0.1f;
        break;
    }
}

void getFPS()
{
    static int frame = 0, time, timebase = 0;
    static char buffer[256];

    char mode[64];

    strcpy(mode, "naive");

    frame++;
    time = glutGet(GLUT_ELAPSED_TIME);
    if (time - timebase > 1000)
    {
        sprintf(buffer, "FPS:%4.2f %s", frame * 1000.0
                / (time - timebase), mode);
        timebase = time;
        frame = 0;
    }

    char *c;
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);    // 选择投影矩阵
    glPushMatrix();                 // 保存原矩阵
    glLoadIdentity();               // 装入单位矩阵
    glOrtho(0, 480, 0, 480, -1, 1); // 位置正投影
    glMatrixMode(GL_MODELVIEW);     // 选择Modelview矩阵
    glPushMatrix();                 // 保存原矩阵
    glLoadIdentity();               // 装入单位矩阵
    glRasterPos2f(10, 10);
    for (c = buffer; *c != '\0'; c++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
    glMatrixMode(GL_PROJECTION); // 选择投影矩阵
    glPopMatrix();               // 重置为原保存矩阵
    glMatrixMode(GL_MODELVIEW);  // 选择Modelview矩阵
    glPopMatrix();               // 重置为原保存矩阵
    glEnable(GL_DEPTH_TEST);
}

void Draw_Lines()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-3, 3, -3, 3, -100, 100);
    glMatrixMode(GL_MODELVIEW);
    glDisable(GL_LIGHTING);
    glColor3f(1.0, 0.0, 0.0);
    glViewport(0, 0, 600, 600);
    glBegin(GL_LINES);
    glVertex2f(-5.0, 0);
    glVertex2f(5.0, 0);
    glVertex2f(0.0, -5.0);
    glVertex2f(0.0, 5.0);
    glEnd();
}

void Display()
{
    glMatrixMode(GL_PROJECTION);//1
    glLoadIdentity();
    if (bPersp)
    {
        gluPerspective(22.5f, 1, 0.1f, 100.0f);
    }
    else
    {
        glOrtho(-3, 0, 0, 3, -100, 100);
    }
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 300, 300, 300);
    if (bPersp)
    {
        glLoadIdentity();
        gluLookAt(eye[0], eye[1], eye[2],
                  center[0] - cx, center[1] + cy, center[2],
                  up[0], up[1], up[2]);
    }
    Draw_Scene();

    glMatrixMode(GL_PROJECTION);//2
    glLoadIdentity();
    if (bPersp)
    {
        gluPerspective(22.5f, 1, 0.1f, 100.0f);
    }
    else
    {
        glOrtho(0, 3, 0, 3, -100, 100);
    }
    glMatrixMode(GL_MODELVIEW);
    glViewport(300, 300, 300, 300);
    if (bPersp)
    {
        glLoadIdentity();
        gluLookAt(eye[0], eye[1], eye[2],
                  center[0] + cx, center[1] + cy, center[2],
                  -up[0], up[1], up[2]);
    }
    Draw_Scene();

    glMatrixMode(GL_PROJECTION);//3
    glLoadIdentity();
    if (bPersp)
    {
        gluPerspective(22.5f, 1, 0.1f, 100.0f);
    }
    else
    {
        glOrtho(-3, 0, -3, 0, -100, 100);
    }
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, 300, 300);
    if (bPersp)
    {
        glLoadIdentity();
        gluLookAt(eye[0], eye[1], eye[2],
                  center[0] - cx, center[1] - cy, center[2],
                  -up[0], up[1], -up[2]);
    }
    Draw_Scene();
    getFPS();

    glMatrixMode(GL_PROJECTION);//4
    glLoadIdentity();
    if (bPersp)
    {
        gluPerspective(22.5f, 1, 0.1f, 100.0f);
    }
    else
    {
        glOrtho(0, 3, -3, 0, -100, 100);
    }
    glMatrixMode(GL_MODELVIEW);
    glViewport(300, 0, 300, 300);
    if (bPersp)
    {
        glLoadIdentity();
        gluLookAt(eye[0], eye[1], eye[2],
                  center[0] + cx, center[1] - cy, center[2],
                  up[0], up[1], -up[2]);
    }
    Draw_Scene();
    
    if (bAnim){
        Angle += 0.5f;
    }
}

void redraw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity(); // Reset The Current Modelview Matrix

    Draw_Lines();

    if (bWire)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    GLfloat white[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat light_pos[] = {5, 5, 5, 1};

    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, white);
    glEnable(GL_LIGHT0);

    gluLookAt(eye[0], eye[1], eye[2],
              center[0], center[1], center[2],
              0, 1, 0);
    
    Display();

    glutSwapBuffers();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Four Viewport");
    glutDisplayFunc(redraw);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}

#pragma clang diagnostic pop
