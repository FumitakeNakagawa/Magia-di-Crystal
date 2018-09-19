#pragma once
#include	"iextreme.h"
#include	"SceneMain\CAMERA\CAMERA.h"

#include <memory>
using namespace std;

#define   TIME_MAX      120      //  ŠÔ


class UI
{

};



//*****************************************************************************************************************************
//
//	ƒ^ƒCƒ€
//
//*****************************************************************************************************************************
class TIME
{
private:
	iex2DObj *timer;  //  ‰æ‘œ
	std::unique_ptr<iex2DObj> start_finish;
	std::unique_ptr<iex2DObj> black;
	int cnt;          //  c‚èŠÔ
	int second;       //  ‚P•b

	bool flg;
	int  x;

	int sizeX, sizeY;


public:
	TIME();
	~TIME();
	//	‰Šú‰»
	bool Initialize(char* filename);
	//	XVE•`‰æ
	void Update();	//	XV
	void Render();	//	•`‰æ
	void Render(int x, int y, int dx, int dy, int sx, int sy, int w, int h);
	int Getcnt() { return cnt; }
	void Setcnt(int c) { cnt = c; }

};




//*****************************************************************************************************************************
//
//	‚g‚oƒQ[ƒW
//
//*****************************************************************************************************************************
class LIFEGAUGE
{
private:
	std::unique_ptr<iex2DObj>frame;   //  ˜g‰æ‘œ
	std::unique_ptr<iex2DObj>gauge;   //  ‚g‚o‰æ‘œ
	float hp[2];
	float maxhp[2];
	int width[2];
	int height[2];

	std::unique_ptr<iexView> view;

public:
	LIFEGAUGE();  //(˜g,‚g‚o)
	~LIFEGAUGE();
	//	‰Šú‰»
	bool Initialize(float h, int index);
	//	XVE•`‰æ
	void Update(float h,int index);	//	XV
	void Render(int no);	//	•`‰æ
	void Render(CAMERA* camera,const Vector3 &pos);

	void Sethp(float l);
};

//*****************************************************************************************************************************
//
//	ƒNƒŠƒXƒ^ƒ‹è—Ì
//
//*****************************************************************************************************************************

class CRYSTAL_UI
{
private:
	std::unique_ptr<iex2DObj> image;
	DWORD color;

public:
	CRYSTAL_UI() {}

	void Initialize(char* filename);
	void Setcolor(DWORD h) { color = h; }
	void Render(CAMERA* camera, const Vector3 &pos);
	void Render(int x, int y);
};

class ICON
{
private: 
	std::unique_ptr<iex2DObj> image;

public:
	void Initialize();
	void Render(CAMERA* camera, const Vector3 &pos,int no);
};