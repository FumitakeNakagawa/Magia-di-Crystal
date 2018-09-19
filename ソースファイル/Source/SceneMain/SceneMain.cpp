#include	"iextreme.h"
#include	"system/system.h"


#include	"SceneLoad\Load.h"
#include	"SceneTitle\Title.h"
#include	"SceneMain.h"



#include	"COLISSION\COLLISION.h"
#include	"UI\UI.h"
#include	"OTHER\Sound.h"

#include "SCRIPT\Script.h"

#include	"SYSTEM\Framework.h"

//*****************************************************************************************************************************
//
//	グローバル変数
//
//*****************************************************************************************************************************
//	カメラ用

Scene* SCENE_MAIN::nextScene = nullptr;



SCENE_MAIN::SCENE_MAIN(int plno1, int plno2, int chno1, int chno2, int ptno1, int ptno2, char * stageName)
{
	this->stageName = stageName;
	Player_no1 = plno1;
	Player_no2 = plno2;

	Char_no1 = chno1;
	Char_no2 = chno2;

	partner_no1 = ptno1;
	partner_no2 = ptno2;
}

//	ステージ用
//*****************************************************************************************************************************
//
//	初期化
//
//*****************************************************************************************************************************
//ステージ初期化
void SCENE_MAIN::StageInit()
{
	//スクリプトファイル読み込み
	std::unique_ptr<SCRIPT> script;

	string scriptName = "DATA\\SCRIPT\\" + stageName + "\\LOAD.txt";
	script = std::make_unique<SCRIPT>((char*)scriptName.c_str());

	//----------------------------------------------------------------------------------------------------
	//
	//	ステージ関係
	//
	//----------------------------------------------------------------------------------------------------
	//読み込み

	//背景読み込み
	script->BeginBlock("BACKGROUND");

	while (1)
	{
		if (script->EndLoop())break;
		std::string str = script->LoadStringB();
		bgName.push_back(str);
	}
	script->EndBlock();


	for (unsigned int i = 0; i < bgName.size(); i++)
	{
		BACK_BASE tmp;
		tmp.Initialize((char*)bgName[i].c_str());
		bg.push_back(tmp);
	}

	//ステージモデル読み込み
	script->BeginBlock("RENDER");

	while (1)
	{
		if (script->EndLoop())break;
		STAGE tmp;
		iexMesh* mesh = script->LoadMesh();
		tmp.Load(mesh);
		stage.push_back(tmp);

	}
	script->EndBlock();

	//当たり判定用モデル読込み
	script->BeginBlock("COLLISION");
	unsigned int index = 0;

	while (1)
	{
		if (script->EndLoop())break;

		iexMesh* mesh = script->LoadMesh();
		COLLISION::Load(index, mesh);
		index++;
	}
	script->EndBlock();

	//クリスタル初期化
	script->BeginBlock("CRYSTAL");

	//クリスタルのモデル読み込み
	script->EndLoop();
	CRYSTAL::SetObject(script->LoadMesh());
	script->EndBlock();

	script->EndBlock();

	//	読み込み処理完了
	//-----------------------------------------------------------------------

	scriptName = "DATA\\SCRIPT\\" + stageName + "\\SET.txt";
	script.reset(new SCRIPT((char*)scriptName.c_str()));
	//----------------------------------------------------------------------
	//	セット処理
	script->BeginBlock("RENDER");

	index = 0;
	while (index < stage.size())
	{
		string str = "NO." + to_string(index + 1);
		script->BeginBlock((char*)str.c_str());

		Vector3 v;

		//位置情報セット
		if (script->Search("POS"))
			v = script->LoadVector3();
		stage[index].SetPos(v);

		//回転情報セット
		if (script->Search("ROT"))
			v = script->LoadVector3();
		stage[index].SetRot(v);

		//スケール情報セット
		if (script->Search("SCALE"))
			v = script->LoadVector3();
		stage[index].SetScale(v);

		//種類セット
		int num = script->LoadInt2("KIND");
		stage[index].SetKind(num);


		if (num == (int)STAGE::KIND_GROUND::MOVING)
		{
			//移動始点セット
			if (script->Search("BEGIN"))
				v = script->LoadVector3();
			stage[index].SetBegin(v);

			//移動終点セット
			if (script->Search("END"))
				v = script->LoadVector3();
			stage[index].SetEnd(v);

			float dist = script->LoadFloat2("STARTDIST");
			stage[index].SetMoveDist(dist);
			//移動時間セット
			float second = script->LoadFloat2("SECOND");
			stage[index].SetSecond(second);

			//初期移動方向セット
			int direct = script->LoadInt2("DIRECT");
			stage[index].SetMoveDirect((float)direct);
		}

		index++;
		script->EndBlock();
	}

	script->EndBlock();

	index = 0;
	while (index < COLLISION::GetLowStage()->size())
	{
		string str = "NO." + to_string(index + 1);
		script->BeginBlock((char*)str.c_str());

		Vector3 v;

		//位置情報セット
		if (script->Search("POS"))
			v = script->LoadVector3();
		COLLISION::GetLowStage()->at(index)->SetPos(v);

		//回転情報セット
		if (script->Search("ROT"))
			v = script->LoadVector3();
		COLLISION::GetLowStage()->at(index)->SetRot(v);

		//スケール情報セット
		if (script->Search("SCALE"))
			v = script->LoadVector3();
		COLLISION::GetLowStage()->at(index)->SetScale(v);

		int num = script->LoadInt2("KIND");
		COLLISION::GetLowStage()->at(index)->SetKind(num);


		COLLISION::GetLowStage()->at(index)->MeshUpdate();
		index++;

		script->EndBlock();


	}
	script->EndBlock();


	script->BeginBlock("CRYSTAL");
	crystal.resize(script->LoadInt2("NUM"));

	float time = script->LoadFloat2("TIME");
	for (unsigned int i = 0; i < crystal.size(); i++)
	{
		string str = "NO." + to_string(i + 1);
		script->BeginBlock((char*)str.c_str());

		int num = script->LoadInt2("PARENT");
		crystal[i].Initialize(num);

		Vector3 v;
		if (script->Search("REVISIONPOS"))
			v = script->LoadVector3();
		else
			v = Vector3(0, 0, 0);
		crystal[i].SetRevision(v);

		script->EndBlock();
		crystal[i].SetMax(time);
		crystal[i].SetMin(-time);

	}

	script->EndBlock();

	script->EndBlock();


	CRYSTAL::SetHoldCnt((int)PLAYER::NO::P1, 0);
	CRYSTAL::SetHoldCnt((int)PLAYER::NO::P2, 0);


}

//キャラクター初期化
void SCENE_MAIN::CharaInit()
{

	std::string filename = "DATA\\SCRIPT\\PLAYER.txt";
	std::unique_ptr<SCRIPT> script;
	script = std::make_unique<SCRIPT>((char*)filename.c_str());
	std::string sn = script->LoadStringB();


	Vector3 v;
	script->BeginBlock("PLAYER1");

	while (true)
	{
		if (script->Search((char*)stageName.c_str()))
			if (script->Search("POS"))
			{
				v = script->LoadVector3();
				break;
			}
	}

	//	プレイヤー初期化
	if (Char_no1 == 0)
		pl1 = std::make_unique<PLAYER>((char*)"DATA\\MODEL\\CHR\\PLAYER\\pl1\\PLAYER1.IEM");
	else
		pl1 = std::make_unique<PLAYER>((char*)"DATA\\MODEL\\CHR\\PLAYER\\pl2\\PLAYER2.IEM");

	//プレイヤー初期化　　　（pos , angle(0,1) , no）
	pl1->Initialize(v, 0, Player_no1, Char_no1);

	script->EndBlock();

	if (Char_no2 == 0)
		pl2 = std::make_unique<PLAYER>((char*)"DATA\\MODEL\\CHR\\PLAYER\\pl1\\PLAYER1.IEM");
	else
		pl2 = std::make_unique<PLAYER>((char*)"DATA\\MODEL\\CHR\\PLAYER\\pl2\\PLAYER2.IEM");
	//プレイヤー初期化　　　（pos , angle(0,1) , no）


	script->BeginBlock("PLAYER2");

	while (true)
	{
		if (script->Search((char*)stageName.c_str()))
			if (script->Search("POS"))
			{
				v = script->LoadVector3();
				break;
			}
	}

	script->EndBlock();

	pl2->Initialize(v, 1, Player_no2, Char_no2);



	//  パートナー初期化
	//  第一引数:味方プレイヤー,第二引数:相手プレイヤー
	//変更
	if (partner_no1 == 0)
	{
		if (Char_no1 == 0)
			plPartner1[Player_no1] = std::make_unique<ATTACK_PARTNER>
			(pl1.get(), pl2.get(), camera.get(), (char*)"DATA\\MODEL\\CHR\\PARTNER\\DRAGON\\A\\dragon3.IEM", Player_no1);
		else
			plPartner1[Player_no1] = std::make_unique<ATTACK_PARTNER>
			(pl1.get(), pl2.get(), camera.get(), (char*)"DATA\\MODEL\\CHR\\PARTNER\\DRAGON\\B\\dragon3.IEM", Player_no1);

		plPartner1[Player_no1]->Initialize(pl1->GetPos() + Vector3(-10, 5, 10.0f));

	}
	else
	{
		if (Char_no1 == 0)
			plPartner2[Player_no1] = std::make_unique<TRAP_PARTNER>
			(pl1.get(), pl2.get(), camera.get(), (char*) "DATA\\MODEL\\CHR\\PARTNER\\CAT\\A\\Cat3.IEM", Player_no1);
		else
			plPartner2[Player_no1] = std::make_unique<TRAP_PARTNER>
			(pl1.get(), pl2.get(), camera.get(), (char*) "DATA\\MODEL\\CHR\\PARTNER\\CAT\\B\\Cat3.IEM", Player_no1);
		plPartner2[Player_no1]->Initialize(pl1->GetPos() + Vector3(-10, 0, 10.0f));
	}

	if (partner_no2 == 0)
	{
		if (Char_no2 == 0)
			plPartner1[Player_no2] = std::make_unique<ATTACK_PARTNER>
			(pl2.get(), pl1.get(), camera.get(), (char*)"DATA\\MODEL\\CHR\\PARTNER\\DRAGON\\A\\dragon3.IEM", Player_no2);
		else
			plPartner1[Player_no2] = std::make_unique<ATTACK_PARTNER>
			(pl2.get(), pl1.get(), camera.get(), (char*) "DATA\\MODEL\\CHR\\PARTNER\\DRAGON\\B\\dragon3.IEM", Player_no2);
		plPartner1[Player_no2]->Initialize(pl2->GetPos() + Vector3(5, 5, 10.0f));
	}
	else
	{
		if (Char_no2 == 0)
			plPartner2[Player_no2] = std::make_unique<TRAP_PARTNER>
			(pl2.get(), pl1.get(), camera.get(), (char*) "DATA\\MODEL\\CHR\\PARTNER\\CAT\\A\\Cat3.IEM", Player_no2);
		else
			plPartner2[Player_no2] = std::make_unique<TRAP_PARTNER>
			(pl2.get(), pl1.get(), camera.get(), (char*) "DATA\\MODEL\\CHR\\PARTNER\\CAT\\B\\Cat3.IEM", Player_no2);

		plPartner2[Player_no2]->Initialize(pl2->GetPos() + Vector3(5, 0, 10.0f));
	}
	//ここまで
}

//初期化(全体)
bool SCENE_MAIN::Initialize()
{
	if (loadFlg) return true;
	StageInit();

	//	環境設定
	iexLight::SetFog(800, 1000, 0);

	//	環境光
	iexLight::SetAmbient(0xffffffff);


	//	カメラ設定
	camera = std::make_unique<CAMERA>();
	camera->Initialize();


	step = 0;


	CharaInit();

	//UI初期化
	timer.reset(new TIME);
	timer->Initialize("DATA\\IMAGE\\UI\\judge.png");
	timer->Setcnt(3);

	lifegauge.reset(new LIFEGAUGE);
	lifegauge->Initialize(pl1->Getlife(), pl1->GetNo());
	lifegauge->Initialize(pl2->Getlife(), pl2->GetNo());

	icon.reset(new ICON);
	icon->Initialize();

	/*lifegauge2p.reset(new LIFEGAUGE);
	lifegauge2p->Initialize( pl2->Getlife(),pl2->GetNo());*/

	crystal_ui.resize(crystal.size());
	for (unsigned int index = 0; index < crystal_ui.size(); index++)
	{
		crystal_ui[index].Initialize("DATA\\IMAGE\\UI\\CRYSTAL_UI.PNG");
	}







	//リザルト用変数初期化
	result_bg.reset(new iex2DObj("DATA\\IMAGE\\UI\\result_BG.PNG"));
	num = 0;
	vec = Vector3(0, 0, 0);
	t = 0;
	tmax = 20;
	check = false;
	n0 = 0; n1 = 0; n2 = 0;
	check2 = false;
	width_Y = 0;
	moveY = 0;
	winY = 800;
	loseY = 800;

	menu_image.reset(new iex2DObj("DATA\\IMAGE\\menu.PNG"));
	menu_no = 0;
	Before_step = 0;

	start_finish = std::make_unique<iex2DObj>((char*)"DATA\\IMAGE\\UI\\display.png");
	loadFlg = true;

	size = Vector2(580, 260);

	resultPng.resize(6);
	std::unique_ptr<iex2DObj> tmp;

	tmp = std::make_unique<iex2DObj>((char*)"DATA\\IMAGE\\Battle\\result_1pwin_A.png");
	resultPng[0] = std::move(tmp);

	tmp = std::make_unique<iex2DObj>((char*)"DATA\\IMAGE\\Battle\\result_1pwin_B.png");
	resultPng[1] = std::move(tmp);

	tmp = std::make_unique<iex2DObj>((char*)"DATA\\IMAGE\\Battle\\result_2pwin_A.png");
	resultPng[2] = std::move(tmp);

	tmp = std::make_unique<iex2DObj>((char*)"DATA\\IMAGE\\Battle\\result_2pwin_B.png");
	resultPng[3] = std::move(tmp);

	tmp = std::make_unique<iex2DObj>((char*)"DATA\\IMAGE\\Battle\\result_draw_A.png");
	resultPng[4] = std::move(tmp);

	tmp = std::make_unique<iex2DObj>((char*)"DATA\\IMAGE\\Battle\\result_draw_B.png");
	resultPng[5] = std::move(tmp);

	resultIndex = -1;


	pause[0] = std::make_unique<iex2DObj>((char*)"DATA\\IMAGE\\battle\\pause.png");
	pause[1] = std::make_unique<iex2DObj>((char*)"DATA\\IMAGE\\battle\\pause2.png");
	cursor = std::make_unique<iex2DObj>((char*)"DATA\\IMAGE\\Cursor.png");
	menu_no = 0;
	cursorY = 350;

	g_sound->SetSoundVolume("BGM1", -1000);
	g_sound->SetSoundVolume("BGM2", -1000);
	g_sound->PlayBGM("BGM1");

	return true;
}



SCENE_MAIN::~SCENE_MAIN()
{
	for (unsigned int index = 0; index < crystal.size(); index++)
		crystal[index].Release();
	crystal.clear();
	CRYSTAL::ReleaseObject();
	COLLISION::Release();
	STAGE::ReleaseBG();
	loadFlg = false;

	if (timer->Getcnt() <= 30)
		g_sound->StopBGM("BGM2");
	else
		g_sound->StopBGM("BGM1");

}

//*****************************************************************************************************************************
//
//		更新
//
//*****************************************************************************************************************************
void SCENE_MAIN::StageUpdate()
{
	Vector3 local1, local2;

	int rideNum1 = -1, rideNum2 = -1;


	//プレイヤーをステージのそれぞれのパーツへローカル化
	for (unsigned int index = 0; index < COLLISION::GetLowStage()->size(); index++)
	{
		if (COLLISION::ToLocal(index, pl1.get()->GetPos(), local1, Vector3(0, -1, 0)) && pl1->Getmove().y < 0)
		{
			pl1.get()->SetJumpFlg(0);
			pl1.get()->SetmoveY(0);
			rideNum1 = index;
			break;
		}
	}

	for (unsigned int index = 0; index < COLLISION::GetLowStage()->size(); index++)
	{

		if (COLLISION::ToLocal(index, pl2.get()->GetPos(), local2, Vector3(0, -1, 0)) && pl2->Getmove().y < 0)
		{
			pl2.get()->SetJumpFlg(0);
			pl2.get()->SetmoveY(0);
			rideNum2 = index;
			break;
		}
	}

	//  追加
	if (plPartner1[Player_no1])
		plPartner1[Player_no1]->ToLocal();
	if (plPartner2[Player_no1])
		plPartner2[Player_no1]->ToLocal();

	if (plPartner1[Player_no2])
		plPartner1[Player_no2]->ToLocal();
	if (plPartner2[Player_no2])
		plPartner2[Player_no2]->ToLocal();


	//	ステージ更新
	//描画用ステージ
	for (unsigned int index = 0; index < stage.size(); index++)
		stage[index].Update();

	//当たり判定用ステージ
	for (unsigned int index = 0; index < COLLISION::GetLowStage()->size(); index++)
		COLLISION::Update(index, stage[index].GetPos(), stage[index].GetRot(), stage[index].GetScale());

	//1P側のワールド空間へ戻す
	if (rideNum1 >= 0 && pl1->Getmove().y == 0)
	{
		COLLISION::ToWorld(rideNum1, local1);
		pl1.get()->Setpos(local1);
	}

	//2P側のワールド空間へ戻す
	if (rideNum2 >= 0 && pl2->Getmove().y == 0)
	{
		COLLISION::ToWorld(rideNum2, local2);
		pl2.get()->Setpos(local2);
	}

	//  追加
	if (plPartner1[Player_no1])
		plPartner1[Player_no1]->ToWorld();
	if (plPartner2[Player_no1])
		plPartner2[Player_no1]->ToWorld();

	if (plPartner1[Player_no2])
		plPartner1[Player_no2]->ToWorld();
	if (plPartner2[Player_no2])
		plPartner2[Player_no2]->ToWorld();

}

void SCENE_MAIN::CrystalUpdate()
{
	for (unsigned int index = 0; index < crystal.size(); index++)
	{
		crystal[index].SetPos(stage.at(crystal[index].GetParentNum()).GetPos() + crystal[index].GetRevision());
		crystal[index].Update();
		crystal[index].SetCharge(false);
		crystal[index].GetEffect()->SetHoldFlg(false);
	}

	float mostNearDist = FLT_MAX;
	int mostNearIndex = -1;
	for (unsigned int index = 0; index < crystal.size(); index++)
	{
		if (crystal[index].GetisHold() != -1)	continue;
		Vector3 v = crystal[index].GetPos() - pl1->GetPos();
		if (mostNearDist > v.LengthSq())
		{
			mostNearDist = v.LengthSq();
			mostNearIndex = index;
		}
	}
	bool canHold = false;
	if (mostNearIndex > -1)
		canHold = COLLISION::PlayerToCrystal(pl1.get(), &crystal[mostNearIndex]);
	if (canHold)
	{
		if (pl1->Gethold())
		{
			crystal[mostNearIndex].SetState(CRYSTAL::STATE::CHARGE);
			crystal[mostNearIndex].GetEffect()->SetHoldFlg(true);
			crystal[mostNearIndex].PlusGage(1, Char_no1);
		}
		else
		{
			crystal[mostNearIndex].SetState(CRYSTAL::STATE::NONE);
			crystal[mostNearIndex].GetEffect()->SetHoldFlg(true);
		}
	}


	mostNearDist = FLT_MAX;
	mostNearIndex = -1;
	for (unsigned int index = 0; index < crystal.size(); index++)
	{
		if (crystal[index].GetisHold() != -1)	continue;
		Vector3 v = crystal[index].GetPos() - pl2->GetPos();
		if (mostNearDist > v.LengthSq())
		{
			mostNearDist = v.LengthSq();
			mostNearIndex = index;
		}
	}
	canHold = false;
	if (mostNearIndex > -1)
		canHold = COLLISION::PlayerToCrystal(pl2.get(), &crystal[mostNearIndex]);
	if (canHold)
	{
		if (pl2->Gethold())
		{
			crystal[mostNearIndex].SetState(CRYSTAL::STATE::CHARGE);
			crystal[mostNearIndex].MinusGage(1, Char_no2);
			crystal[mostNearIndex].GetEffect()->SetHoldFlg(true);
		}
		else
		{
			crystal[mostNearIndex].SetState(CRYSTAL::STATE::NONE);
			crystal[mostNearIndex].GetEffect()->SetHoldFlg(true);
		}
	}


}

void	SCENE_MAIN::Update()
{
	switch (step)
	{
	case Preparation:
	{
		StageUpdate();
		CrystalUpdate();
		//	ステージ更新
		for (unsigned int index = 0; index < stage.size(); index++)
			stage[index].Update();
		//	カメラ更新
		camera->Update(pl1->GetPos(), pl2->GetPos());
		timer->Update();

		if (timer->Getcnt() <= 0)
		{
			Before_step = Preparation;
			step = Main;
			timer->Setcnt(120);
		}
		break;
	}

	case Main:
	{
		//ステージ更新
		StageUpdate();

		//クリスタル更新
		CrystalUpdate();


		//	プレイヤー更新
		pl1->UPdate(pl2.get());
		pl2->UPdate(pl1.get());

		//使い魔更新
		if (plPartner1[Player_no1])
			plPartner1[Player_no1]->Update(/*timer->Getcnt()*/);
		if (plPartner2[Player_no1])
			plPartner2[Player_no1]->Update(timer->Getcnt());

		if (plPartner1[Player_no2])
			plPartner1[Player_no2]->Update(/*timer->Getcnt()*/);
		if (plPartner2[Player_no2])
			plPartner2[Player_no2]->Update(timer->Getcnt());


		Vector3 local1, local2;

		int rideNum1 = -1, rideNum2 = -1;

		for (unsigned int index = 0; index < COLLISION::GetLowStage()->size(); index++)
		{
			if (COLLISION::ToLocal(index, pl1.get()->GetPos(), local1, Vector3(0, -1, 0)))
			{
				pl1.get()->SetJumpFlg(0);
				rideNum1 = index;
				break;
			}
		}

		for (unsigned int index = 0; index < COLLISION::GetLowStage()->size(); index++)
		{

			if (COLLISION::ToLocal(index, pl2.get()->GetPos(), local2, Vector3(0, -1, 0)))
			{
				pl2.get()->SetJumpFlg(0);
				rideNum2 = index;
				break;
			}
		}

		//  追加
		if (plPartner1[Player_no1])
			plPartner1[Player_no1]->ToLocal();
		if (plPartner2[Player_no1])
			plPartner2[Player_no1]->ToLocal();

		if (plPartner1[Player_no2])
			plPartner1[Player_no2]->ToLocal();
		if (plPartner2[Player_no2])
			plPartner2[Player_no2]->ToLocal();


		//	ステージ更新
		for (unsigned int index = 0; index < stage.size(); index++)
			stage[index].Update();

		for (unsigned int index = 0; index < COLLISION::GetLowStage()->size(); index++)
		{
			COLLISION::Update(index, stage[index].GetPos(), stage[index].GetRot(), stage[index].GetScale());
		}


		if (rideNum1 >= 0)
		{
			COLLISION::ToWorld(rideNum1, local1);
			pl1.get()->Setpos(local1);
		}
		if (rideNum2 >= 0)
		{
			COLLISION::ToWorld(rideNum2, local2);
			pl2.get()->Setpos(local2);

		}



		//  追加
		if (plPartner1[Player_no1])
			plPartner1[Player_no1]->ToWorld();
		if (plPartner2[Player_no1])
			plPartner2[Player_no1]->ToWorld();

		if (plPartner1[Player_no2])
			plPartner1[Player_no2]->ToWorld();
		if (plPartner2[Player_no2])
			plPartner2[Player_no2]->ToWorld();




		//魔法同士の判定
		int check = COLLISION::MagicMagic(pl1->GetMagic(), pl2->GetMagic());
		if (0 == check)
		{
			pl1->GetMagic()->erase();
			pl2->GetMagic()->erase();
		}
		else if (1 == check)
			pl1->GetMagic()->erase();
		else if (2 == check)
			pl2->GetMagic()->erase();



		// プレイヤー死亡処理
		DeadPlayer();

		//	カメラ更新
		camera->Update(pl1->GetPos(), pl2->GetPos());

		//UI更新
		timer->Update();
		lifegauge->Update(pl1->Getlife(), pl1->GetNo());
		lifegauge->Update(pl2->Getlife(), pl2->GetNo());

#ifdef _DEBUG
		if (KEY_Get(KEY_SELECT, 0) == 3)
		{
			if (timer->Getcnt() > 30)
				timer->Setcnt(31);
			else
				timer->Setcnt(1);
		}
#endif // _DEBUG



		//ゲーム終了
		if (timer->Getcnt() <= 0 ||
			CRYSTAL::GetHoldCnt((int)PLAYER::NO::P1) >= crystal.size() ||
			CRYSTAL::GetHoldCnt((int)PLAYER::NO::P2) >= crystal.size())
		{
			g_sound->PlaySE("finish");
			Before_step = Main;
			step = Result;
		}

		//メニュー
		if (KEY_Get(KEY_SELECT, 0) == 3 || KEY_Get(KEY_SELECT, 1) == 3)
		{
			Before_step = Main;
			step = Menu;
		}
	}
	break;

	case Result:
	{
		if (CRYSTAL::GetHoldCnt((int)PLAYER::NO::P1) == crystal.size() ||
			CRYSTAL::GetHoldCnt((int)PLAYER::NO::P1) > CRYSTAL::GetHoldCnt((int)PLAYER::NO::P2))
		{
			resultIndex = 0;
			if (Char_no1)
				resultIndex = 1;
			winX = 350;

			loseX = 1920 / 2 + 300;

		}
		else if (CRYSTAL::GetHoldCnt((int)PLAYER::NO::P2) == crystal.size() ||
			CRYSTAL::GetHoldCnt((int)PLAYER::NO::P2) > CRYSTAL::GetHoldCnt((int)PLAYER::NO::P1))
		{
			resultIndex = 3;
			if (Char_no1)
				resultIndex = 2;
			loseX = 350;

			winX = 1920 / 2 + 300;

		}
		else
		{
			resultIndex = 4;
			if (Char_no1)
				resultIndex = 5;
		}
		result_time++;

		static int n = 0;
		if (n == 0 && Before_step == Main)
		{
			pl1->Setpos(Vector3(-70, 30, -20));
			pl2->Setpos(Vector3(70, 30, -20));

			for (unsigned int index = 0; index < crystal.size(); index++)
			{
				crystal[index].SetPos(Vector3(-5.0f + index * 3.0f, -10.0f, 5.0f));
			}

			n++;
		}

		if (result_time >= 60 && num < crystal.size())
		{
			if (false == check)
			{
				check = true;
				/*if (crystal[num].GetisHold() == 0)
				{
				vec = (pl1->GetPos() + Vector3(-30.0f, -15.0f * n1 + 15.0f, 0.0f)) - crystal[num].GetPos();
				vec /= tmax;
				n1++;
				}
				else if (crystal[num].GetisHold() == 1)
				{
				vec = (pl2->GetPos() + Vector3(35.0f, -15.0f * n2 + 15.0f, 0.0f)) - crystal[num].GetPos();
				vec /= tmax;
				n2++;
				}
				else*/
				{
					vec = ((pl1->GetPos() + pl2->GetPos()) / 2.0f + Vector3(-20.0f, -15.0f * n0 +5.0f, 0.0f)) - crystal[num].GetPos();
					vec /= tmax;
					n0++;
				}
			}

			t++;
			crystal[num].SetPos(crystal[num].GetPos() + vec);
			if (t >= tmax)
			{
				num++;
				t = 0;
				vec = Vector3(0, 0, 0);
				check = false;
			}
		}
		else if (pl1->Result() && pl2->Result())
		{

			pl1->Update();
			pl2->Update();
		}

		n++;
		if (n >= 180)
		{
			if (KEY_Get(KEY_SELECT, 0) == 3 || KEY_Get(KEY_SELECT, 1) == 3)
			{
				MainFrame->ChangeScene(new SCENE_TITLE());
				n = 0;
			}
		}

		/*if (n >= 120)
		{
		n = 0;
		}*/
		/*if (KEY_Get(KEY_B, 0) == 3 || KEY_Get(KEY_B, 1) == 3)
		{
		n = 0;
		Before_step = Result;
		step = Menu;
		}*/
	}

	break;

	case Menu:
	{
		for (int i = 0; i < 2; i++)
		{
			if (KEY_Get(KEY_UP, i) == 3)
			{
				menu_no--;
				if (menu_no < 0)
					menu_no = 1;
				break;
			}
		}
		for (int i = 0; i < 2; i++)
		{
			if (KEY_Get(KEY_DOWN, i) == 3)
			{
				menu_no++;
				if (menu_no > 1)
					menu_no = 0;
				break;
			}
		}
		for (int i = 0; i < 2; i++)
		{
			if (KEY_Get(KEY_SELECT, i) == 3)
			{
				step = GAME_STEP::Main;
				break;
			}
		}

		switch (menu_no)
		{
		case 0:
			cursorY = 420;
			for (int i = 0; i < 2; i++)
			{
				if (KEY_Get(KEY_B, i) == 3)
				{
					step = GAME_STEP::Main;
					break;
				}
			}
			break;
		case 1:
			cursorY = 520;
			for (int i = 0; i < 2; i++)
			{
				if (KEY_Get(KEY_B, i) == 3)
				{
					MainFrame->ChangeScene(new SCENE_TITLE());
					break;
				}
			}
			break;
		}
	}
	}

}

void SCENE_MAIN::DeadPlayer()
{
	if (pl1->GetPos().y < -10.0f)pl1->Respawn(Vector3(0.0f, 30.0f, 0.0f), 1);
	if (pl2->GetPos().y < -10.0f)pl2->Respawn(Vector3(0.0f, 30.0f, 0.0f), 1);
	//プレイヤー死亡時リスポーン & クリスタルのロスト
	if (pl1->GetMode() == CHARACTOR::STATE::DEAD)
	{
		float min = FLT_MAX;
		int lostNo_noneHold = -1;
		int lostNo_Hold = -1;
		for (int index = 0; index < crystal.size(); index++)
		{
			float length = (crystal[index].GetPos() - pl1->GetPos()).LengthSq();
			if (crystal[index].GetGage() > 0.0f)
			{
				if (length < min)
				{
					if (crystal[index].GetisHold() == (int)PLAYER::NO::P1)
					{
						min = length;
						lostNo_Hold = index;
					}
					lostNo_noneHold = index;
				}
			}
		}
		if (lostNo_Hold > -1)
			crystal[lostNo_Hold].LostCrystal((int)PLAYER::NO::P1);
		else if (lostNo_noneHold > -1)
			crystal[lostNo_noneHold].LostCrystal((int)PLAYER::NO::P1);

		pl1->Respawn(Vector3(-40, 50, 0), 0);
		//使い魔リスポーン
		if (plPartner1[Player_no1])
			plPartner1[Player_no1]->Initialize(pl1->GetPos() + Vector3(-5, 5, 10.0f));
		if (plPartner2[Player_no1])
			plPartner2[Player_no1]->Initialize(pl1->GetPos() + Vector3(-5, 5, 10.0f));

	}

	if (pl2->GetMode() == CHARACTOR::STATE::DEAD)
	{
		float min = FLT_MAX;
		int lostNo_noneHold = -1;
		int lostNo_Hold = -1;
		for (int index = 0; index < crystal.size(); index++)
		{
			float length = (crystal[index].GetPos() - pl2->GetPos()).LengthSq();
			if (crystal[index].GetGage() < 0.0f)
			{
				if (length < min)
				{
					if (crystal[index].GetisHold() == (int)PLAYER::NO::P2)
					{
						min = length;
						lostNo_Hold = index;
					}
					lostNo_noneHold = index;
				}
			}
		}
		if (lostNo_Hold > -1)
			crystal[lostNo_Hold].LostCrystal((int)PLAYER::NO::P2);
		else if (lostNo_noneHold > -1)
			crystal[lostNo_noneHold].LostCrystal((int)PLAYER::NO::P2);

		pl2->Respawn(Vector3(-40, 50, 0), 0);
		//使い魔リスポーン
		if (plPartner1[Player_no2])
			plPartner1[Player_no2]->Initialize(pl2->GetPos() + Vector3(-5, 5, 10.0f));
		if (plPartner2[Player_no2])
			plPartner2[Player_no2]->Initialize(pl2->GetPos() + Vector3(-5, 5, 10.0f));

	}



}

//*****************************************************************************************************************************
//
//		描画関連
//
//*****************************************************************************************************************************

void	SCENE_MAIN::Render()
{
	//	画面クリア
	camera->Render();

	//ステージ描画
	D3DCAPS9 caps;
	HRESULT result = iexSystem::Device->GetDeviceCaps(&caps);
	if (FAILED(result))OutputDebugString("Error: キャプスの取得に失敗");

	unsigned long shoudAlphaTest = caps.AlphaCmpCaps & D3DPCMPCAPS_GREATEREQUAL;
	if (shoudAlphaTest)
	{
		iexSystem::Device->SetRenderState(D3DRS_ALPHAREF, (DWORD)0x000000A0);
		iexSystem::Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		iexSystem::Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	}
	switch (step)
	{
	case GAME_STEP::Preparation:
		for (auto it = bg.begin(); it != bg.end(); ++it)
			it->Render(0, 0, 1920, 1080, 0, 0, 1920, 1080, RS_COPY, 0xBBBBBBBB, 50);
		for (unsigned int index = 0; index < stage.size(); index++)
			stage[index].Render();

		timer->Render();
		break;
	case GAME_STEP::Main:
		for (auto it = bg.begin(); it != bg.end(); ++it)
			it->Render(0, 0, 1920, 1080, 0, 0, 1920, 1080, RS_COPY, 0xBBBBBBBB, 50);

		for (unsigned int index = 0; index < stage.size(); index++)
			stage[index].Render();

		//クリスタル描画
		for (unsigned int num = 0; num < crystal.size(); num++)
			crystal[num].Render(camera.get(), Char_no1, Char_no2);

		//使い魔描画
		if (plPartner1[Player_no1])
			plPartner1[Player_no1]->Render(Char_no1);
		if (plPartner2[Player_no1])
			plPartner2[Player_no1]->Render(Char_no1);

		if (plPartner1[Player_no2])
			plPartner1[Player_no2]->Render(Char_no2);
		if (plPartner2[Player_no2])
			plPartner2[Player_no2]->Render(Char_no2);

		//プレイヤー
		pl1->Render(camera.get());
		pl2->Render(camera.get());

		//UI描画
		if (timer->Getcnt() > 119)
		{
			static Vector2 size = Vector2(580, 260);
			start_finish->Render(
				(int)(970 - size.x / 2), (int)(480 - size.y / 2),
				(int)size.x, (int)size.y, 0, 256 * 3, 1024, 256);
			size.x += 1.0f; size.y += 1.0f;

			if (size.x >= 640)
				size.x = 580;
			if (size.y >= 320)
				size.y = 260;

		}

		lifegauge->Render(Player_no1);
		lifegauge->Render(Player_no2);

		timer->Render();

		icon->Render(camera.get(), pl1->GetPos() + Vector3(0, 21, 0), Player_no1);
		icon->Render(camera.get(), pl2->GetPos() + Vector3(0, 21, 0), Player_no2);
		break;

	case GAME_STEP::Result:
		for (auto it = bg.begin(); it != bg.end(); ++it)
			it->Render(0, 0, 1920, 1080, 0, 0, 1920, 1080, RS_COPY, 0xB0777777, 50);
		for (unsigned int index = 0; index < stage.size(); index++)
			stage[index].ResultRender();

		if (result_time < 60)
		{
			start_finish->Render(
				(int)(970 - size.x / 2), (int)(480 - size.y / 2), (int)size.x, (int)size.y,
				0, 0, 1024, 256);
			size.x++; size.y++;

			if (size.x >= 640)
				size.x = 580;
			if (size.y >= 320)
				size.y = 260;
		}
		else
		{
			if (!(num < crystal.size()))
			{
				if (-1 < resultIndex  && resultIndex < 4)
				{
					timer->Render(winX, winY + width_Y, 256, 256, 0, 512, 256, 256);
					timer->Render(loseX, loseY + width_Y, 256, 256, 256, 512, 256, 256);
				}
				else if (resultIndex == 4 || resultIndex == 5)
					timer->Render(720, winY, 512, 256, 512, 768, 512,256 );
				if (resultIndex > -1)
				{
					//1P
					resultPng[resultIndex]->Render(300, 300, 1920 / 4, 1080 / 2, 0, 0, 1920 / 2, 1080);
					//2P
					resultPng[resultIndex]->Render(1920 / 2 + 250, 300, 1920 / 4, 1080 / 2, 1920 / 2, 0, 1920 / 2, 1080);
				}

				if (check2 == false)
				{
					winY -= 15;
					loseY -= 15;
					if (winY < 100) { winY = 100; check2 = true; ti = 0; moveY = 1; }
					if (loseY < 100) { loseY = 100; check2 = true; ti = 0; moveY = 1; }
				}
				else
				{
					ti++;
					if (45 < ti)
					{
						width_Y += moveY;
						if (width_Y >= 16 || width_Y <= -16)moveY *= -1;
					}
				}

			}

			for (unsigned int num = 0; num < crystal.size(); num++)
			{
				crystal[num].ResultRender();
			}
		}
		break;
	case GAME_STEP::Menu:
		for (auto it = bg.begin(); it != bg.end(); ++it)
			it->Render(0, 0, 1920, 1080, 0, 0, 1920, 1080, RS_COPY, 0xBBBBBBBB, 50);
		for (unsigned int index = 0; index < stage.size(); index++)
			stage[index].ResultRender();

		menu_image->Render(0, 0, 1920, 1080, 0, 0, 1920, 1080, RS_ADD, 0xAAFFFFFF);
		pause[0]->Render(700, 250, 512, 512, 0, 0, 512, 512);
		pause[1]->Render(825, 350, 256, 256, 0, 0, 512, 256);
		pause[1]->Render(825, 450, 256, 256, 0, 256, 512, 256);
		cursor->Render(750, cursorY, 128, 128, 256, 0, 256, 256);
		break;
	}

	if (shoudAlphaTest)
		iexSystem::Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}



