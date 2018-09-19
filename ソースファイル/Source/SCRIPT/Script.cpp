#include "Script.h"



//	�u���b�N�ɓ���
void SCRIPT::BeginBlock(char* blockName)
{
	char str[64];
	LoadString(str);
	string = str;
	if (string == blockName)
		In();
}

//	�u���b�N�𔲂���
void SCRIPT::EndBlock()
{
	Out();
}


//	int�^�ǂݍ���
int SCRIPT::LoadInt2(char* name)
{
	char str[64];
	LoadString(str);
	string = str;
	if (string == name)
		return textLoader::LoadInt();
	else return -1;
}

float SCRIPT::LoadFloat2(char* name)
{
	char str[64];
	LoadString(str);
	string = str;
	if (string == name)
		return textLoader::LoadFloat();
	else return -1.0f;
}

//	�x�N�g������
Vector3 SCRIPT::LoadVector3()
{
	Vector3 v;
	v.x = LoadFloat();
	v.y = LoadFloat();
	v.z = LoadFloat();

	return v;
}

//	�V�F�[�_�[�ǂݍ���
iexShader* SCRIPT::LoadShader()
{

	char filename[128];
	LoadString(filename);
	return new iexShader(filename);

}


//	2D�摜�ǂݍ���
iex2DObj* SCRIPT::Load2DObj()
{

	char filename[128];
	LoadString(filename);
	return new iex2DObj(filename);;

}


//	3D���f���ǂݍ���
iex3DObj* SCRIPT::Load3DObj()
{
	char filename[128];
	LoadString(filename);
	return new iex3DObj(filename);
}


//	���b�V���ǂݍ���
iexMesh* SCRIPT::LoadMesh()
{
	char filename[128];
	LoadString(filename);
	return  new iexMesh(filename);;
}


std::vector<iex2DObj*>* SCRIPT::LoadSerialData()
{
	std::vector<iex2DObj*> data;
	char filepass[256];
	LoadString(filepass);

	int size = LoadInt2("SIZE");

	for (int i = 0; i < size; i++)
	{
		std::string filename = filepass;

		if (filename.back() != '\\')
			filename += "\\" + std::to_string(i) + ".png";
		else
			filename += std::to_string(i) + ".png";

		data[i] = new iex2DObj((char*)filename.c_str());

	}

	return &data;
}


bool SCRIPT::EndLoop()
{
	char com[128];
	LoadString(com);
	string = com;

	if (string == "END")
		return true;

	return false;
}