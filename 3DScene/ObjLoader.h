#include <vector>
#include "Mesh.h"
#include<iostream>
#include<fstream>
#include<string>
#include<stdlib.h>
#include <GL\SOIL.h>

//场景管理器
class ObjLoader
{
public:

	//mesh
	vector<Mesh> meshArr;

	//顶点
	vector<Vector3> vextexArr;
	//法向量
	vector<Vector3> normalArr;
	//纹理坐标
	vector<Vector3> textureArr;

	ObjLoader();

	~ObjLoader();

	//读obj文件
	void load(const char * path);

	//绘制3d模型
	void render();

	//初始化材质
	void initTexture();
private:

	//分割字符串
	vector<string> split(const string &str, const string &pattern);
};
