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
static int degree = 60;
static int Y = -1;
static int X = -1;

//眼睛所在位置
static GLfloat eyeX = 6000;
static GLfloat eyeY = 6000;
static GLfloat eyeZ = 6000;

//看向那个点
static GLfloat lookX = 0;
static GLfloat lookY = 0;
static GLfloat lookZ = 0;

//灯的位置
static GLfloat lightX = 8000;
static GLfloat lightY = 8000;
static GLfloat lightZ = 8000;

//摄像机参数
static GLfloat zNear = 1.0f;
static GLfloat zFar = 20000.0f;

#define PATH "F:/学习/电科/计算机高级图形学/model/pavilion.obj"

//转换类别
static int TransformType = 0;
#define TRANSLATION 1
#define ROTATE 2
#define SCALE 3



ObjLoader loader;

int main(int argv, char *arg[])
{\
	glutInit(&argv, arg);
	//初始化
	init();
	//设置各种回调函数
	//渲染
	glutDisplayFunc(display);
	//改变形状
	glutReshapeFunc(onReshaped);
	//鼠标移动
	glutMouseFunc(onMouseMoved);
	//motion
	glutMotionFunc(onMotionNotified);
	//idle
	glutIdleFunc(idle);
	//主循环
	glutMainLoop();

	return  0;
}


//初始化函数
void init()
{
	//加载三维模型
	loader.load(PATH);
	//开启展示模式
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	//设置窗体
	glutInitWindowSize(800, 600);
	glutCreateWindow("三维场景展示");
	//开启深度测试
	glEnable(GL_DEPTH_TEST);
	//开启着色模式
	glShadeModel(GL_SMOOTH);
	//设置光源
	setLight();
	//开启深度测试
	glEnable(GL_DEPTH_TEST);
	//开启贴图模式
	glEnable(GL_TEXTURE_2D);
	//开启颜色模式
	glEnable(GL_COLOR_MATERIAL);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	//读取材质
	loader.initTexture();
}


//设置光源
void setLight()
{

	GLfloat lightPosition[] = { lightX, lightY, lightZ, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glEnable(GL_LIGHTING); //启用光源
	glEnable(GL_LIGHT0);   //使用指定灯光

}

void display()
{
	//设置背景颜色
	glClearColor(0.69f, 0.76f, 0.87f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	//设置视点
	gluLookAt(eyeX*cos(c*degree), eyeY, eyeZ*sin(c*degree), lookX, lookY, lookZ,0.0f, 1.0f, 0.0f);
	//绘制模型
	loader.render();

	glPopMatrix();
	glutSwapBuffers();
}



//改变形状时
void onReshaped(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, (GLdouble)width / (GLdouble)height, zNear, zFar);
	glMatrixMode(GL_MODELVIEW);
}

//移动鼠标时
void onMouseMoved(int button, int state, int x, int y)
{
	X = x; Y = y;
	//旋转
	if (state == GLUT_DOWN) {

		switch (button)
		{
			//旋转
		case GLUT_MIDDLE_BUTTON:
			TransformType = ROTATE;
			break;

			//平移
		case GLUT_LEFT:
			TransformType = TRANSLATION;
			break;

		//	//缩放
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

	//旋转
	if (TransformType == ROTATE) {
		degree += dx;
	}

	//平移
	if (TransformType == TRANSLATION) {
		lookX -= dx ;
		lookZ -= dy ;
	}

	X = x;
	Y = y;
	
}

//懒惰函数
void idle()
{
	glutPostRedisplay();
}