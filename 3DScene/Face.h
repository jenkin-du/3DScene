#pragma once

#include <vector>
#include "Vector3.h"
#include "Texture.h"

#define TRANGLE 3
#define QUADRANGLE 4
#define POLYGON 5

using namespace std;

class Face
{
public:
	Face();
	~Face();

	//����
	vector<int> vertexIndexArr;

	//������
	int normalIndex;

	//��������
	vector<int> textureIndexArr;

	//������� type:3 ������ 4 �ı���
	int type;

	//ʹ�õĲ���
	Texture texture;
private:

};

