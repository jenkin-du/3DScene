#pragma once

#include<vector>
#include "Face.h"
using namespace std;

//�������壬���ڴ洢����openGL�е�obj�ļ�
class Mesh
{
public:
	Mesh();
	~Mesh();

	string name;

	//��
	vector<Face> faceArr;

private:

};

