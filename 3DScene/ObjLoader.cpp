#include "ObjLoader.h"

ObjLoader::ObjLoader()
{

}

ObjLoader::~ObjLoader()
{
}

//����������
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

//��ȡ�ļ�
void ObjLoader::load(const char * path)
{
	//���ļ�
	fstream objFile(path);
	string line;

	//��������
	vector<MTLIndex> indexArr;

	//���ʿ��ļ�
	string mtlFileName;
	//��ǰʹ�õĲ���
	string currentMtl;

	//�ڼ���������
	int meshIndex = -1;

	//��obj�ļ�
	while (getline(objFile, line))
	{

		//�յ��ַ�������ע��
		if (line.empty() || line[0] == '#') {
			continue;
		}

		//��һ�а��ո�ָ�Ϊ�ַ�����
		vector<string> arr = split(line, " ");

		//���ʿ���ļ���
		if (arr[0] == ("mtllib")) {
			mtlFileName = arr[1];
		}

		//��ǰʹ�õĲ��ʿ�
		if (arr[0] == ("usemtl")) {
			currentMtl = arr[1];
		}

		//һ��mesh
		if (arr[0] == ("g")) {

			Mesh mesh;
			mesh.name = arr[1];

			//���뵽mesh������
			meshIndex++;
			meshArr.push_back(mesh);
		}

		//����
		if (arr[0] == ("v"))
		{
			Vector3 vextex;
			vextex.x = (GLfloat)atof(arr[1].c_str());
			vextex.y = (GLfloat)atof(arr[2].c_str());
			vextex.z = (GLfloat)atof(arr[3].c_str());

			vextexArr.push_back(vextex);
		}

		//������
		if (arr[0] == ("vn")) {

			Vector3 normal;
			normal.x = (GLfloat)atof(arr[1].c_str());
			normal.y = (GLfloat)atof(arr[2].c_str());
			normal.z = (GLfloat)atof(arr[3].c_str());

			normalArr.push_back(normal);
		}

		//��������
		if (arr[0] == ("vt")) {

			Vector3 texture;
			texture.x = (GLfloat)atof(arr[1].c_str());
			texture.y = (GLfloat)atof(arr[2].c_str());

			textureArr.push_back(texture);
		}

		//��
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

			//��ȡ�������
			for (int i = 1; i < arr.size(); i++) {

				//�ָ��ַ���
				vector<string> indexs = split(arr[i], "/");

				int indexV = atoi(indexs[0].c_str());
				int indexVT = atoi(indexs[1].c_str());
				int indexVN = atoi(indexs[2].c_str());

				face.vertexIndexArr.push_back(indexV - 1);
				face.textureIndexArr.push_back(indexVT - 1);
				face.normalIndex = indexVN - 1;
			}

			//��Ӳ�������
			MTLIndex index;
			index.meshIndex = meshIndex;
			index.faceIndex = meshArr[meshIndex].faceArr.size();
			index.mtlName = currentMtl;
			indexArr.push_back(index);

			//������뵽��������
			meshArr[meshIndex].faceArr.push_back(face);
		}
	}


	//�������ļ�

	//ƴ�Ӳ���·�� obj�ļ�Ҫ��mtl�ļ�����ͬһ·����
	string basePath = "";
	vector<string> paths = split(path, "/");
	for (int i = 0; i < paths.size() - 1; i++) {
		basePath += paths[i];
		basePath += "/";
	}
	string mtlFilePath = basePath + mtlFileName;

	fstream mtlFile(mtlFilePath);
	while (getline(mtlFile, line)) {
		//�յ��ַ�������ע��
		if (line.empty() || line[0] == '#') {
			continue;
		}

		//��һ�а��ո�ָ�Ϊ�ַ�����
		vector<string> arr = split(line, " ");
		//����
		if (arr[0] == ("newmtl")) {

			//��ȡ������Ϣ

			Vector3 ka;
			Vector3 kd;
			Vector3 ks;

			string path;

			while (getline(mtlFile, line)) {
				//�˳�����
				if (line.empty()) {
					break;
				}

				//��һ�а��ո�ָ�Ϊ�ַ�����
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


			//�ҵ�ʹ�õ���
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

//��Ⱦ��ά����
void ObjLoader::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//��������ϵ
	glLineWidth(1);

	glBegin(GL_LINES);
	{
		//x��
		glColor3f(255, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(1000000, 0, 0);

		//y��
		glColor3f(0, 255, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 1000000, 0);

		//z��
		glColor3f(0, 0, 255);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, 1000000);
	}
	glEnd();


	////������
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

	//�������3d����
	int meshSize = meshArr.size();
	for (int i = 0; i < meshSize; i++) {

		//������
		int faceSize = meshArr[i].faceArr.size();
		for (int j = 0; j < faceSize; j++) {

			Face face = meshArr[i].faceArr[j];
			//����ͼӳ��
			glBindTexture(GL_TEXTURE_2D, face.texture.id);
			//���ò�������
			GLfloat ambient[4] = { face.texture.ka.x, face.texture.ka.y, face.texture.ka.z, 1.0 };
			GLfloat diffuse[4] = { face.texture.kd.x, face.texture.kd.y, face.texture.kd.z,1.0 };
			GLfloat specular[4] = { face.texture.ks.x, face.texture.ks.y, face.texture.ks.z, 1.0 };

			glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
			glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
			glMaterialf(GL_FRONT, GL_SHININESS, 25);


			int type = face.type;
			//��������
			if (type == TRANGLE) {

				glBegin(GL_TRIANGLES);
				{
					int vnIndex = meshArr[i].faceArr[j].normalIndex;
					//���Ʒ�����
					glNormal3f(normalArr[vnIndex].x, normalArr[vnIndex].y, normalArr[vnIndex].z);

					//���ƶ���
					for (int k = 0; k < meshArr[i].faceArr[j].vertexIndexArr.size(); k++) {

						//��ͼ����
						int vtIndex = meshArr[i].faceArr[j].textureIndexArr[k];
						//��ͼ
						glTexCoord2f(textureArr[vtIndex].x, textureArr[vtIndex].y);

						int vIndex = meshArr[i].faceArr[j].vertexIndexArr[k];
						//����
						glVertex3f(vextexArr[vIndex].x, vextexArr[vIndex].y, vextexArr[vIndex].z);
					}
				}
				glEnd();
			}

			//�ı���
			if (type == QUADRANGLE) {

				glBegin(GL_QUADS);
				{

					int vnIndex = meshArr[i].faceArr[j].normalIndex;
					//���Ʒ�����
					glNormal3f(normalArr[vnIndex].x, normalArr[vnIndex].y, normalArr[vnIndex].z);

					//���ƶ���
					for (int k = 0; k < meshArr[i].faceArr[j].vertexIndexArr.size(); k++) {

						//��ͼ����
						int vtIndex = meshArr[i].faceArr[j].textureIndexArr[k];
						//��ͼ
						glTexCoord2f(textureArr[vtIndex].x, textureArr[vtIndex].y);

						int vIndex = meshArr[i].faceArr[j].vertexIndexArr[k];
						//����
						glVertex3f(vextexArr[vIndex].x, vextexArr[vIndex].y, vextexArr[vIndex].z);
					}
				}
				glEnd();
			}

			//�����
			if (type == POLYGON) {

				glBegin(GL_POLYGON);
				{
					int vnIndex = meshArr[i].faceArr[j].normalIndex;
					//���Ʒ�����
					glNormal3f(normalArr[vnIndex].x, normalArr[vnIndex].y, normalArr[vnIndex].z);

					//���ƶ���
					for (int k = 0; k < meshArr[i].faceArr[j].vertexIndexArr.size(); k++) {

						//��ͼ����
						int vtIndex = meshArr[i].faceArr[j].textureIndexArr[k];
						//��ͼ
						glTexCoord2f(textureArr[vtIndex].x, textureArr[vtIndex].y);

						int vIndex = meshArr[i].faceArr[j].vertexIndexArr[k];
						//����
						glVertex3f(vextexArr[vIndex].x, vextexArr[vIndex].y, vextexArr[vIndex].z);
					}
				}
				glEnd();
			}
		}
	}

	glFlush();	// �뵥�����Ӧ
				// glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

//��ʼ������
void ObjLoader::initTexture()
{
	string mtlName = "";//��ǰ��ʼ���Ĳ��ʣ������ظ�
	int textureID = 0;
	for (int i = 0; i < meshArr.size(); i++) {
		for (int j = 0; j < meshArr[i].faceArr.size(); j++) {

			if (meshArr[i].faceArr[j].texture.name != mtlName) {
				//��ʼ��
				//����ͼƬ
				textureID = SOIL_load_OGL_texture(meshArr[i].faceArr[j].texture.path.c_str(),
					SOIL_LOAD_AUTO,
					SOIL_CREATE_NEW_ID,
					SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);

				// ���ֽڶ���
				glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
				//�󶨲���
				glBindTexture(GL_TEXTURE_2D, textureID);
				//������ͼģʽ
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

//�ָ��ַ���
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


