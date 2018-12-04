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

	//顶点
	vector<int> vertexIndexArr;

	//法向量
	int normalIndex;

	//纹理坐标
	vector<int> textureIndexArr;

	//顶点个数 type:3 三角形 4 四边形
	int type;

	//使用的材质
	Texture texture;
private:

};

