#pragma once
#include <string>
#include "Vector3.h"
using namespace std;

class Texture
{
public:
	Texture();
	~Texture();
	//��������
	string name;
	//����·��
	string path;
	//ka
	Vector3 ka;
	//kd
	Vector3 kd;
	//ks
	Vector3 ks;
	//����ID
	int id = 0;

private:

};

