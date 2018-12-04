#pragma once
#include <string>
#include "Vector3.h"
using namespace std;

class Texture
{
public:
	Texture();
	~Texture();
	//材质名称
	string name;
	//材质路径
	string path;
	//ka
	Vector3 ka;
	//kd
	Vector3 kd;
	//ks
	Vector3 ks;
	//纹理ID
	int id = 0;

private:

};

