#include<iostream>
#include<fstream>
#include<string>
#include <vector>
#include "ObjLoader.h"

using namespace std;

void init();
void setLight();
void display();
void onReshaped(int width, int height);
void onMouseMoved(int button, int state, int x, int y);
void onMotionNotified(int x, int y);
void idle();

static float c = 3.1415926 / 180.0f;
static float r = 300.0f;
static int degree = 60;
static int Y = -1;
static int X = -1;

//�۾�����λ��
static GLfloat eyeX = 6000;
static GLfloat eyeY = 6000;
static GLfloat eyeZ = 6000;

//�����Ǹ���
static GLfloat lookX = 0;
static GLfloat lookY = 0;
static GLfloat lookZ = 0;

//�Ƶ�λ��
static GLfloat lightX = 8000;
static GLfloat lightY = 8000;
static GLfloat lightZ = 8000;

//���������
static GLfloat zNear = 1.0f;
static GLfloat zFar = 20000.0f;

#define PATH "F:/ѧϰ/���/������߼�ͼ��ѧ/model/pavilion.obj"

//ת�����
static int TransformType = 0;
#define TRANSLATION 1
#define ROTATE 2
#define SCALE 3



ObjLoader loader;

int main(int argv, char *arg[])
{\
	glutInit(&argv, arg);
	//��ʼ��
	init();
	//���ø��ֻص�����
	//��Ⱦ
	glutDisplayFunc(display);
	//�ı���״
	glutReshapeFunc(onReshaped);
	//����ƶ�
	glutMouseFunc(onMouseMoved);
	//motion
	glutMotionFunc(onMotionNotified);
	//idle
	glutIdleFunc(idle);
	//��ѭ��
	glutMainLoop();

	return  0;
}


//��ʼ������
void init()
{
	//������άģ��
	loader.load(PATH);
	//����չʾģʽ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	//���ô���
	glutInitWindowSize(800, 600);
	glutCreateWindow("��ά����չʾ");
	//������Ȳ���
	glEnable(GL_DEPTH_TEST);
	//������ɫģʽ
	glShadeModel(GL_SMOOTH);
	//���ù�Դ
	setLight();
	//������Ȳ���
	glEnable(GL_DEPTH_TEST);
	//������ͼģʽ
	glEnable(GL_TEXTURE_2D);
	//������ɫģʽ
	glEnable(GL_COLOR_MATERIAL);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	//��ȡ����
	loader.initTexture();
}


//���ù�Դ
void setLight()
{

	GLfloat lightPosition[] = { lightX, lightY, lightZ, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glEnable(GL_LIGHTING); //���ù�Դ
	glEnable(GL_LIGHT0);   //ʹ��ָ���ƹ�

}

void display()
{
	//���ñ�����ɫ
	glClearColor(0.69f, 0.76f, 0.87f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	//�����ӵ�
	gluLookAt(eyeX*cos(c*degree), eyeY, eyeZ*sin(c*degree), lookX, lookY, lookZ,0.0f, 1.0f, 0.0f);
	//����ģ��
	loader.render();

	glPopMatrix();
	glutSwapBuffers();
}



//�ı���״ʱ
void onReshaped(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, (GLdouble)width / (GLdouble)height, zNear, zFar);
	glMatrixMode(GL_MODELVIEW);
}

//�ƶ����ʱ
void onMouseMoved(int button, int state, int x, int y)
{
	X = x; Y = y;
	//��ת
	if (state == GLUT_DOWN) {

		switch (button)
		{
			//��ת
		case GLUT_MIDDLE_BUTTON:
			TransformType = ROTATE;
			break;

			//ƽ��
		case GLUT_LEFT:
			TransformType = TRANSLATION;
			break;

		//	//����
		//case GLUT_WHEEL_UP:
		//	TransformType = TRANSLATION;
		//	break;
		default:
			TransformType = 0;
			break;
		}
		
	}
}


void onMotionNotified(int x, int y)
{
	int dx = x - X;
	int dy = y - Y;

	//��ת
	if (TransformType == ROTATE) {
		degree += dx;
	}

	//ƽ��
	if (TransformType == TRANSLATION) {
		lookX -= dx ;
		lookZ -= dy ;
	}

	X = x;
	Y = y;
	
}

//���躯��
void idle()
{
	glutPostRedisplay();
}