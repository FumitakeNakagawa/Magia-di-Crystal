#pragma once

#include "iextreme.h"
#include <memory>
#include <list>
#include	<random>

class BACK_BASE
{
public :
	
protected:
	static std::random_device rnd;
	static std::mt19937 mt;
	static std::uniform_int_distribution<> randint;
	static std::uniform_real_distribution<>randfloat;
	
	std::shared_ptr<iex2DObj> obj;
	Vector3 pos, move;
	int dx, dy;
	int sx, sy;
	int width, height;
	DWORD color;

public:
	BACK_BASE() {  }
	virtual void Initialize(char* filename);
	virtual void Initialize(iex2DObj* obj);
	virtual void Update(){}
	virtual void Render(int x,int y,int dx,int dy,int sx,int sy,int w,int h,ULONG dwFlg,DWORD color,float z);
	virtual void Render();

	static void Random();
};

class BACK_MOVE : public BACK_BASE
{
protected:
	LVERTEX vertex[4];
	std::unique_ptr<iexShader> shader;

	DWORD u, v;
	int aliveTimer;
	int frame;
	int timer;

public:
	virtual void Initialize(char* filename);
	virtual void Initialize(iex2DObj* obj);
	virtual void Update() override=0;
	virtual void Render()override =0;
};
class BACK_CLOUD :public BACK_MOVE
{
public:
	virtual void Update();
	virtual void Render();
};

class BACK_STAR :public BACK_MOVE
{
	virtual void Update();
	virtual void Render();

};