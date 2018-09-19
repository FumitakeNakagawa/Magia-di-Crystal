#pragma once

#include	"iextreme.h"
#include	"textLoader.h"
#include	<iostream>
#include	<vector>
#include	<string>

class SCRIPT :protected textLoader
{
private:
	std::string string;
	char strB[64];
public:


	SCRIPT(char* filename) :textLoader(filename) {}
	bool Initialize(char* filename) { Open(filename); }
	//	char* Load(char* filename);
	void EndBlock();
	void BeginBlock(char* blockName);

	void* LoadObj() {}//ŽŽŒ±‰^—p

	iexShader* LoadShader();

	iexMesh* LoadMesh();
	iex3DObj* Load3DObj();

	iex2DObj* Load2DObj();

	Vector3 LoadVector3();

	std::vector<iex2DObj*>* LoadSerialData();


	char* LoadStringB() {  LoadString(strB); return strB; }

	float LoadFloat2(char* name);

	int LoadInt2(char* name);

	bool Search(char* str) { return textLoader::Search(str); }


	bool EndLoop();

};