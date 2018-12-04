#include "ObjLoader.h"

ObjLoader::ObjLoader()
{

}

ObjLoader::~ObjLoader()
{
}

//材质索引类
class MTLIndex
{
public:
	MTLIndex();
	~MTLIndex();

	int meshIndex;
	int faceIndex;

	string mtlName;

private:

};

MTLIndex::MTLIndex()
{
}

MTLIndex::~MTLIndex()
{
}

static GLfloat groundSize = 5000;

//读取文件
void ObjLoader::load(const char * path)
{
	//打开文件
	fstream objFile(path);
	string line;

	//材质索引
	vector<MTLIndex> indexArr;

	//材质库文件
	string mtlFileName;
	//当前使用的材质
	string currentMtl;

	//第几个网格体
	int meshIndex = -1;

	//读obj文件
	while (getline(objFile, line))
	{

		//空的字符串或者注释
		if (line.empty() || line[0] == '#') {
			continue;
		}

		//将一行按空格分割为字符数组
		vector<string> arr = split(line, " ");

		//材质库的文件名
		if (arr[0] == ("mtllib")) {
			mtlFileName = arr[1];
		}

		//当前使用的材质库
		if (arr[0] == ("usemtl")) {
			currentMtl = arr[1];
		}

		//一个mesh
		if (arr[0] == ("g")) {

			Mesh mesh;
			mesh.name = arr[1];

			//加入到mesh数组中
			meshIndex++;
			meshArr.push_back(mesh);
		}

		//顶点
		if (arr[0] == ("v"))
		{
			Vector3 vextex;
			vextex.x = (GLfloat)atof(arr[1].c_str());
			vextex.y = (GLfloat)atof(arr[2].c_str());
			vextex.z = (GLfloat)atof(arr[3].c_str());

			vextexArr.push_back(vextex);
		}

		//法向量
		if (arr[0] == ("vn")) {

			Vector3 normal;
			normal.x = (GLfloat)atof(arr[1].c_str());
			normal.y = (GLfloat)atof(arr[2].c_str());
			normal.z = (GLfloat)atof(arr[3].c_str());

			normalArr.push_back(normal);
		}

		//纹理坐标
		if (arr[0] == ("vt")) {

			Vector3 texture;
			texture.x = (GLfloat)atof(arr[1].c_str());
			texture.y = (GLfloat)atof(arr[2].c_str());

			textureArr.push_back(texture);
		}

		//面
		if (arr[0] == ("f")) {

			Face face;
			int size = arr.size() - 1;
			if (size == 3) {
				face.type = TRANGLE;
			}
			else if (size == 4) {
				face.type = QUADRANGLE;
			}
			else if (size > 4) {
				face.type = POLYGON;
			}

			//获取面的数据
			for (int i = 1; i < arr.size(); i++) {

				//分割字符串
				vector<string> indexs = split(arr[i], "/");

				int indexV = atoi(indexs[0].c_str());
				int indexVT = atoi(indexs[1].c_str());
				int indexVN = atoi(indexs[2].c_str());

				face.vertexIndexArr.push_back(indexV - 1);
				face.textureIndexArr.push_back(indexVT - 1);
				face.normalIndex = indexVN - 1;
			}

			//添加材质索引
			MTLIndex index;
			index.meshIndex = meshIndex;
			index.faceIndex = meshArr[meshIndex].faceArr.size();
			index.mtlName = currentMtl;
			indexArr.push_back(index);

			//将面加入到网格体中
			meshArr[meshIndex].faceArr.push_back(face);
		}
	}


	//读材质文件

	//拼接材质路径 obj文件要和mtl文件放在同一路径下
	string basePath = "";
	vector<string> paths = split(path, "/");
	for (int i = 0; i < paths.size() - 1; i++) {
		basePath += paths[i];
		basePath += "/";
	}
	string mtlFilePath = basePath + mtlFileName;

	fstream mtlFile(mtlFilePath);
	while (getline(mtlFile, line)) {
		//空的字符串或者注释
		if (line.empty() || line[0] == '#') {
			continue;
		}

		//将一行按空格分割为字符数组
		vector<string> arr = split(line, " ");
		//材质
		if (arr[0] == ("newmtl")) {

			//读取材质信息

			Vector3 ka;
			Vector3 kd;
			Vector3 ks;

			string path;

			while (getline(mtlFile, line)) {
				//退出条件
				if (line.empty()) {
					break;
				}

				//将一行按空格分割为字符数组
				vector<string> mtlArr = split(line, " ");

				//ka
				if (mtlArr[0] == ("Ka")) {

					ka.x = (GLfloat)atof(mtlArr[1].c_str());
					ka.y = (GLfloat)atof(mtlArr[2].c_str());
					ka.z = (GLfloat)atof(mtlArr[3].c_str());
				}

				//kd
				if (mtlArr[0] == ("Kd")) {

					kd.x = (GLfloat)atof(mtlArr[1].c_str());
					kd.y = (GLfloat)atof(mtlArr[2].c_str());
					kd.z = (GLfloat)atof(mtlArr[3].c_str());
				}

				//ks
				if (mtlArr[0] == ("Ks")) {

					ks.x = (GLfloat)atof(mtlArr[1].c_str());
					ks.y = (GLfloat)atof(mtlArr[2].c_str());
					ks.z = (GLfloat)atof(mtlArr[3].c_str());
				}

				//map_kd
				if (mtlArr[0] == ("map_Kd")) {

					path = mtlArr[1];
					path = basePath + path;
				}
			}


			//找到使用的面
			string name = arr[1];
			for (int i = 0; i < indexArr.size(); i++) {
				if (indexArr[i].mtlName == name) {

					MTLIndex index = indexArr[i];

					meshArr[index.meshIndex].faceArr[index.faceIndex].texture.ka = ka;
					meshArr[index.meshIndex].faceArr[index.faceIndex].texture.kd = kd;
					meshArr[index.meshIndex].faceArr[index.faceIndex].texture.ks = ks;

					meshArr[index.meshIndex].faceArr[index.faceIndex].texture.name = name;
					meshArr[index.meshIndex].faceArr[index.faceIndex].texture.path = path;
				}
			}
		}
	}
}

//渲染三维物体
void ObjLoader::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//绘制坐标系
	glLineWidth(1);

	glBegin(GL_LINES);
	{
		//x轴
		glColor3f(255, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(1000000, 0, 0);

		//y轴
		glColor3f(0, 255, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 1000000, 0);

		//z轴
		glColor3f(0, 0, 255);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, 1000000);
	}
	glEnd();


	////画地面
	glColor3f(105.0f, 105.0f, 105.0f);
	glBegin(GL_QUADS);
	{
		glNormal3f(0, -1, 0);
		glVertex3f(groundSize, 0, groundSize);
		glVertex3f(groundSize, 0, -groundSize);
		glVertex3f(-groundSize, 0, -groundSize);
		glVertex3f(-groundSize, 0, groundSize);
	}
	glEnd();

	//逐个绘制3d网格
	int meshSize = meshArr.size();
	for (int i = 0; i < meshSize; i++) {

		//绘制面
		int faceSize = meshArr[i].faceArr.size();
		for (int j = 0; j < faceSize; j++) {

			Face face = meshArr[i].faceArr[j];
			//绑定贴图映射
			glBindTexture(GL_TEXTURE_2D, face.texture.id);
			//设置材质属性
			GLfloat ambient[4] = { face.texture.ka.x, face.texture.ka.y, face.texture.ka.z, 1.0 };
			GLfloat diffuse[4] = { face.texture.kd.x, face.texture.kd.y, face.texture.kd.z,1.0 };
			GLfloat specular[4] = { face.texture.ks.x, face.texture.ks.y, face.texture.ks.z, 1.0 };

			glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
			glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
			glMaterialf(GL_FRONT, GL_SHININESS, 25);


			int type = face.type;
			//三角网格
			if (type == TRANGLE) {

				glBegin(GL_TRIANGLES);
				{
					int vnIndex = meshArr[i].faceArr[j].normalIndex;
					//绘制法向量
					glNormal3f(normalArr[vnIndex].x, normalArr[vnIndex].y, normalArr[vnIndex].z);

					//绘制顶点
					for (int k = 0; k < meshArr[i].faceArr[j].vertexIndexArr.size(); k++) {

						//贴图坐标
						int vtIndex = meshArr[i].faceArr[j].textureIndexArr[k];
						//贴图
						glTexCoord2f(textureArr[vtIndex].x, textureArr[vtIndex].y);

						int vIndex = meshArr[i].faceArr[j].vertexIndexArr[k];
						//顶点
						glVertex3f(vextexArr[vIndex].x, vextexArr[vIndex].y, vextexArr[vIndex].z);
					}
				}
				glEnd();
			}

			//四边形
			if (type == QUADRANGLE) {

				glBegin(GL_QUADS);
				{

					int vnIndex = meshArr[i].faceArr[j].normalIndex;
					//绘制法向量
					glNormal3f(normalArr[vnIndex].x, normalArr[vnIndex].y, normalArr[vnIndex].z);

					//绘制顶点
					for (int k = 0; k < meshArr[i].faceArr[j].vertexIndexArr.size(); k++) {

						//贴图坐标
						int vtIndex = meshArr[i].faceArr[j].textureIndexArr[k];
						//贴图
						glTexCoord2f(textureArr[vtIndex].x, textureArr[vtIndex].y);

						int vIndex = meshArr[i].faceArr[j].vertexIndexArr[k];
						//顶点
						glVertex3f(vextexArr[vIndex].x, vextexArr[vIndex].y, vextexArr[vIndex].z);
					}
				}
				glEnd();
			}

			//多边形
			if (type == POLYGON) {

				glBegin(GL_POLYGON);
				{
					int vnIndex = meshArr[i].faceArr[j].normalIndex;
					//绘制法向量
					glNormal3f(normalArr[vnIndex].x, normalArr[vnIndex].y, normalArr[vnIndex].z);

					//绘制顶点
					for (int k = 0; k < meshArr[i].faceArr[j].vertexIndexArr.size(); k++) {

						//贴图坐标
						int vtIndex = meshArr[i].faceArr[j].textureIndexArr[k];
						//贴图
						glTexCoord2f(textureArr[vtIndex].x, textureArr[vtIndex].y);

						int vIndex = meshArr[i].faceArr[j].vertexIndexArr[k];
						//顶点
						glVertex3f(vextexArr[vIndex].x, vextexArr[vIndex].y, vextexArr[vIndex].z);
					}
				}
				glEnd();
			}
		}
	}

	glFlush();	// 与单缓冲对应
				// glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

//初始化材质
void ObjLoader::initTexture()
{
	string mtlName = "";//当前初始化的材质，以免重复
	int textureID = 0;
	for (int i = 0; i < meshArr.size(); i++) {
		for (int j = 0; j < meshArr[i].faceArr.size(); j++) {

			if (meshArr[i].faceArr[j].texture.name != mtlName) {
				//初始化
				//加载图片
				textureID = SOIL_load_OGL_texture(meshArr[i].faceArr[j].texture.path.c_str(),
					SOIL_LOAD_AUTO,
					SOIL_CREATE_NEW_ID,
					SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);

				// 按字节对齐
				glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
				//绑定材质
				glBindTexture(GL_TEXTURE_2D, textureID);
				//设置贴图模式
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

				mtlName = meshArr[i].faceArr[j].texture.name;
			}
			meshArr[i].faceArr[j].texture.id = textureID;
		}
	}


}

//分割字符串
vector<string> ObjLoader::split(const string & str, const string & pattern)
{
	char * strc = new char[strlen(str.c_str()) + 1];
	strcpy(strc, str.c_str());
	vector<string> resultVec;
	char* tmpStr = strtok(strc, pattern.c_str());
	while (tmpStr != NULL)
	{
		resultVec.push_back(string(tmpStr));
		tmpStr = strtok(NULL, pattern.c_str());
	}

	delete[] strc;

	return resultVec;
}


