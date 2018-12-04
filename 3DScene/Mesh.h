#pragma once

#include<vector>
#include "Face.h"
using namespace std;

//网格物体，用于存储导入openGL中的obj文件
class Mesh
{
public:
	Mesh();
	~Mesh();

	string name;

	//面
	vector<Face> faceArr;

private:

};

