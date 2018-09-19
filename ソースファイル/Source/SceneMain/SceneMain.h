//*****************************************************************************************************************************
//
//		メインシーン
//
//*****************************************************************************************************************************
#include	<memory>
#include	<vector>
#include	"..\\Source\SYSTEM\Scene.h"

#include	"CAMERA\\CAMERA.h"

#include	"CHARACTOR\PLAYER\PLAYER.h"
#include	"CHARACTOR\PARTNER\Partner.h"

#include	"STAGE\BackGround\BackGround.h"
#include	"STAGE\Stage.h"
#include	"CRYSTAL\Crystal.h"

#include	"EFFECT\Effect.h"

#include	"SceneMain\UI\\UI.h"
#include	"OTHER\Vector2.h"


class	SCENE_MAIN : public Scene
{
private:
	string stageName;
	std::vector<string> bgName;
	

	std::unique_ptr<PLAYER>	pl1;
	std::unique_ptr<PLAYER>	pl2;

	std::unique_ptr<BASE_PARTNER> plPartner1[2];
	std::unique_ptr<BASE_PARTNER> plPartner2[2];

	std::list<BACK_BASE> bg;
	std::vector<STAGE>	stage;
	std::vector<CRYSTAL>crystal;//クリスタル

	std::unique_ptr<CAMERA>	camera;

	std::unique_ptr<TIME> timer;
	std::unique_ptr<LIFEGAUGE> lifegauge;


	std::vector<CRYSTAL_UI> crystal_ui;
	std::unique_ptr<ICON> icon;

	std::unique_ptr<iex2DObj> result_bg;
	std::unique_ptr<iex2DObj> start_finish;
	std::unique_ptr<iex2DObj> menu_image;
	std::unique_ptr<iex2DObj> pause[2];
	std::unique_ptr<iex2DObj> cursor;
	int cursorY;
	std::vector<std::unique_ptr<iex2DObj>>resultPng;

	//std::unique_ptr<EFFECT> effect;

	int Player_no1, Player_no2;
	int partner_no1, partner_no2;
	int Char_no1, Char_no2;

	int step;

	unsigned int num;      //  リザルト時のクリスタル移動に使用
	Vector3 vec;  //  リザルト時のクリスタル移動に使用
	float t, tmax;  //  リザルト時のクリスタル移動に使用
	int n0, n1, n2;   //  リザルト時のクリスタル移動に使用
	bool check;   //  リザルト時のクリスタル移動に使用
	bool check2;   //  リザルト時の勝ち・負けの移動に使用
	int ti;        //  リザルト時の勝ち・負けの移動に使用
	int width_Y;   //  リザルト時の勝ち・負けの移動に使用
	int moveY;	   //  リザルト時の勝ち・負けの移動に使用
	int result_time = 0;
	Vector2 size;


	//  追加--------------------
	bool crystalCheckFlag[3];
	//--------------------------

	unsigned int menu_no;
	int Before_step;
	int resultIndex;

	int winX, winY;
	int loseX, loseY;


	enum GAME_STEP
	{
		Preparation,
		Main,
		Result,
		Menu
	};



	void StageInit();
	void CharaInit();
	static Scene* nextScene;

	SCENE_MAIN(Scene* scene) { *this = scene; }
public:
	SCENE_MAIN() {}
	SCENE_MAIN(int plno1, int plno2,int chno1 ,int chno2,int ptno1, int ptno2, char* stageName);
	~SCENE_MAIN( );
	//	初期化
	bool Initialize( );
	//	更新・描画
	void Update( );	//	更新
	void Render( );	//	描画
	void StageUpdate();
	void CrystalUpdate();

					//プレイヤー死亡処理
	void DeadPlayer();

	Scene* operator=(Scene* scene) { return scene; }
};

