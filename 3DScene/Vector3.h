#pragma once
#define GLUT_DISABLE_ATEXIT_HACK

#include <GL\GLUT.H>

//µã
class Vector3
{
public:
	Vector3();
	~Vector3();

	Vector3(GLfloat x, GLfloat y,GLfloat z);

	GLfloat x;
	GLfloat y;
	GLfloat z;

private:
};


