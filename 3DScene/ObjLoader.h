#include <vector>
#include "Mesh.h"
#include<iostream>
#include<fstream>
#include<string>
#include<stdlib.h>
#include <GL\SOIL.h>

//����������
class ObjLoader
{
public:

	//mesh
	vector<Mesh> meshArr;

	//����
	vector<Vector3> vextexArr;
	//������
	vector<Vector3> normalArr;
	//��������
	vector<Vector3> textureArr;

	ObjLoader();

	~ObjLoader();

	//��obj�ļ�
	void load(const char * path);

	//����3dģ��
	void render();

	//��ʼ������
	void initTexture();
private:

	//�ָ��ַ���
	vector<string> split(const string &str, const string &pattern);
};
