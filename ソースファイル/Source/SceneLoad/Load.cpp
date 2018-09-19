#include	"Load.h"
#include	"SYSTEM\Framework.h"


//------------------------------------------------------------------
//
//		スタティックメンバ変数
//
//------------------------------------------------------------------
bool SCENE_LOAD::loadEndflg;
Scene* SCENE_LOAD::nextScene = nullptr;
int SCENE_LOAD::percentage;
int SCENE_LOAD::time;

//------------------------------------------------------------------
//
//		初期化(コンストラクタ)
//
//------------------------------------------------------------------
SCENE_LOAD::SCENE_LOAD(char* filename, Scene* nextScene)
{
	if (loadFlg) return;
	loadEndflg = false;
	percentage = 0;

	obj = std::make_unique<iex2DObj>((char*)"DATA\\IMAGE\\title_kari.png");
	tips.resize(3);
	std::unique_ptr < iex2DObj>tip;

	tip = std::make_unique<iex2DObj>((char*)"DATA\\IMAGE\\UI\\Tips0.png");
	tips[0] = std::move(tip);

	tip = std::make_unique<iex2DObj>((char*)"DATA\\IMAGE\\UI\\Tips1.png");
	tips[2] = std::move(tip);

	tip = std::make_unique<iex2DObj>((char*)"DATA\\IMAGE\\UI\\Tips2.png");
	tips[1] = std::move(tip);

	number = std::make_unique<iex2DObj>((char*)"DATA\\IMAGE\\UI\\judge.png");

	push = std::make_unique<iex2DObj>((char*)"DATA\\IMAGE\\UI\\push.png");


	cursor = std::make_unique<iex2DObj>((char*)"DATA\\IMAGE\\Cursor.png");
	alpha = 0x00FFFFFF;
	alphaFlag = true;

	nowTips = 0;
	nowPercentage = 0.0f;

	pushAlpha = 0xFFFFFFFF;
	pushPlus = -1;
	SCENE_LOAD::nextScene = nextScene;
	time = 0;
	loadFlg = true;
	_beginthread(Thread, 0, (void*)this->nextScene);
}


//-------------------------------------------------------------------
//
//		解放
//
//-------------------------------------------------------------------
SCENE_LOAD::~SCENE_LOAD()
{


	time = 0;

}

//---------------------------------------------------------------------
//
//	スレッド
//
//---------------------------------------------------------------------
void SCENE_LOAD::Thread(void* arg)
{
	nextScene->Initialize();
	loadEndflg = true;
	_endthread();
}


//----------------------------------------------------------------------
//
//		更新
//
//-----------------------------------------------------------------------
void SCENE_LOAD::Update()
{
	if (alphaFlag)  alpha += 0x02000000;
	if (!alphaFlag) alpha -= 0x02000000;

	if (alpha == 0xFEFFFFFF && alphaFlag)
		alphaFlag = false;
	if (alpha == 0x00FFFFFF && !alphaFlag)
		alphaFlag = true;


	if (KEY_Get(KEY_LEFT, 0) == 3 || KEY_Get(KEY_LEFT, 1) == 3)
	{
		nowTips--;
		if (nowTips < 0)nowTips = 0;
	}

	if (KEY_Get(KEY_RIGHT, 0) == 3 || KEY_Get(KEY_RIGHT, 1) == 3)
	{
		nowTips++;
		if (nowTips >= tips.size())nowTips = tips.size() - 1;
	}

	if (loadEndflg)
	{
		pushAlpha = pushAlpha+0x03000000*pushPlus;
		if ((pushAlpha>>24) ==0xFF || (pushAlpha>>24)==0x00)
			pushPlus *= -1;
	}
	if (loadEndflg&& (KEY_Get(KEY_SELECT, 0) == 3 || KEY_Get(KEY_SELECT, 1) == 3))
		MainFrame->ChangeScene(nextScene);
}

//-----------------------------------------------------------------------
//
//		描画
//
//-----------------------------------------------------------------------
void SCENE_LOAD::Render()
{
	//画面クリア
	iexView view;
	view.Clear();

	tips[nowTips]->Render(0, 0, 1920, 1080, 0, 0, 1920, 1080);

	int hundred = nowPercentage / 100;
	int ten = (int)nowPercentage%100 / 10;
	int one = (int)nowPercentage % 10;

	/*
	number->Render(0, 1080 - 256, 128, 128, (hundred % 7) * 128, (hundred / 7) * 128,128,128);
	number->Render(128, 1080 - 256, 128, 128, (ten % 7) * 128, (ten / 7) * 128, 128, 128);
	number->Render(256, 1080 - 256, 128, 128, (one % 7) * 128, (one / 7) * 128, 128, 128);
	*/

	if (nowTips == 0)
		cursor->Render(1920-200, 300, 256, 256, 256, 0, 256, 256);
	else if(nowTips ==2)
		cursor->Render(-10, 300, 256, 256, 0, 0, 256, 256);
	else
	{
		cursor->Render(1920 - 200, 300, 256, 256, 256, 0, 256, 256);
		cursor->Render(-10, 300, 256, 256, 0, 0, 256, 256);
	}

	if (loadEndflg)
		push->Render(630, 630, 512, 512, 0, 0, 512, 512,RS_COPY,pushAlpha);

	obj->Render(1400, 780, 500, 250, 0, 0, 1000, 500, RS_COPY, alpha);
}