#pragma once

#include	"iextreme.h"
#include	"SYSTEM\Scene.h"
#include	<process.h>
#include	<vector>
#include	<memory>
class SCENE_LOAD :public Scene
{
private:
	std::unique_ptr<iex2DObj> push;
	std::vector<std::unique_ptr<iex2DObj>> tips;				//ロード画面(変わるかも)
	std::unique_ptr<iex2DObj> obj;
	std::unique_ptr<iex2DObj>number;
	std::unique_ptr<iex2DObj> cursor;
	static Scene* nextScene;//ロード画面の次のシーン
	static bool loadEndflg;	//ロードが完了したかどうか
	static int percentage;	//ロード完了率
	static int time;
	int alpha;
	bool alphaFlag;

	int nowTips;
	float nowPercentage;

	DWORD pushAlpha;
	int pushPlus;
private:

	//スレッド(並行処理させる関数)
	static void Thread(void* arg);

public:
	//初期化・解放
	SCENE_LOAD() :obj(nullptr) {}
	SCENE_LOAD(char* filename, Scene* nextScene);
	~SCENE_LOAD();

	//更新
	void Update();
	//描画
	void Render();


	//セット
	static void SetPercentage(int num) { percentage = num; }
	static int GetPercentage() { return percentage; }
};