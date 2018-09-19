#pragma once

#include	"iextreme.h"
#include	"SYSTEM\Scene.h"
#include	"OTHER\Vector2.h"



class SCENE_SELECT_CHAR :public Scene
{
private:
	std::unique_ptr<iex2DObj> bg;
	std::unique_ptr<iex3DObj> Char[2];
	//std::unique_ptr<iex3DObj> char_2;
	std::unique_ptr<iex2DObj> choseIcon;
	std::unique_ptr<iex2DObj> decide;



	int iconX[2], iconY;


	std::unique_ptr<iexView> view;


	int frame;
	int x1, x2;
	bool de1, de2;
public:
	~SCENE_SELECT_CHAR();

	bool Initialize();
	//	更新・描画
	void Update();	//	更新
	void Render();	//	描画
};

#define X 200
#define Y 900
#define TEX_WIDTH 512
#define TEX_HEIGHT 512

class SCENE_SELECT_PARTNER :public Scene
{
private:
	std::unique_ptr<iex2DObj> bg;
	std::unique_ptr<iex2DObj> decide;
	std::unique_ptr<iex2DObj> next;
	std::unique_ptr<iex3DObj> partner[2][2];

	std::unique_ptr<iexView> view;


	int frame;
	int pl1, pl2;
	bool right_flg[2] = { false };
	bool left_flg[2] = { false };



	Vector3 pos[2][2];
	Vector3 angle[2];
	Vector3 scale[2][2];

	int color[2][2];

	void move(int pl_no);

	void SetMotion(int pl_no, int no, int mo_no) {
		if (partner[pl_no][no]->GetMotion() != mo_no)
			partner[pl_no][no]->SetMotion(mo_no);
	}


public:
	SCENE_SELECT_PARTNER(int n1, int n2);
	//SCENE_SELECT_PARTNER()
	~SCENE_SELECT_PARTNER();

	bool Initialize();
	//	更新・描画
	void Update();	//	更新
	void Render();	//	描画
};

class SCENE_SELECT_STAGE :public Scene
{
private:
	enum class STAGE
	{
		FOREST,
		RUIN,

		STAGE_MAX
	};
	
	enum class STEP
	{
		SELECT,
		DECIDE,
	};

	std::unique_ptr<iex2DObj> bg;
	std::unique_ptr<iex2DObj> image1, image2;//image1:ステージ画像 2：矢印・看板
	std::unique_ptr<iex2DObj> decide;
	std::unique_ptr<iex2DObj> nextIcon;
	std::unique_ptr<iexView> view;

	STAGE number;

	struct
	{
	Vector2 pos;
	Vector2 point;
	Vector2 move;
	int frame;
	}stage,board;
	int nextFrame;
	bool loop;
	Vector2 CursorPos[2];
	STEP step;

	bool canMove;
	bool canDecide;
public:

	void Select();
	bool Initialize();
	void Update();
	void Render();
};