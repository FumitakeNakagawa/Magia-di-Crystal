#pragma once
#include	"iextreme.h"
#include	"SceneMain\CAMERA\CAMERA.h"

#include <memory>
using namespace std;

#define   TIME_MAX      120      //  ����


class UI
{

};



//*****************************************************************************************************************************
//
//	�^�C��
//
//*****************************************************************************************************************************
class TIME
{
private:
	iex2DObj *timer;  //  �摜
	std::unique_ptr<iex2DObj> start_finish;
	std::unique_ptr<iex2DObj> black;
	int cnt;          //  �c�莞��
	int second;       //  �P�b

	bool flg;
	int  x;

	int sizeX, sizeY;


public:
	TIME();
	~TIME();
	//	������
	bool Initialize(char* filename);
	//	�X�V�E�`��
	void Update();	//	�X�V
	void Render();	//	�`��
	void Render(int x, int y, int dx, int dy, int sx, int sy, int w, int h);
	int Getcnt() { return cnt; }
	void Setcnt(int c) { cnt = c; }

};




//*****************************************************************************************************************************
//
//	�g�o�Q�[�W
//
//*****************************************************************************************************************************
class LIFEGAUGE
{
private:
	std::unique_ptr<iex2DObj>frame;   //  �g�摜
	std::unique_ptr<iex2DObj>gauge;   //  �g�o�摜
	float hp[2];
	float maxhp[2];
	int width[2];
	int height[2];

	std::unique_ptr<iexView> view;

public:
	LIFEGAUGE();  //(�g,�g�o)
	~LIFEGAUGE();
	//	������
	bool Initialize(float h, int index);
	//	�X�V�E�`��
	void Update(float h,int index);	//	�X�V
	void Render(int no);	//	�`��
	void Render(CAMERA* camera,const Vector3 &pos);

	void Sethp(float l);
};

//*****************************************************************************************************************************
//
//	�N���X�^�����
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