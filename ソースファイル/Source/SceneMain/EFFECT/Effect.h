#pragma once


#include	"iextreme.h"
#include	<list>
//#include	<vector>
#include	<stdlib.h>
#include	<time.h>
#include    "../CAMERA/CAMERA.h"

using namespace std;


#define EFFECT_TIMER 30

class EFFECT
{
	//static vector<iex2DObj*>	image;             //画像

	std::unique_ptr<iex2DObj> obj;           //png画像用
	int effect_animation;    //animationの種類
	int effect_frame;       //コマ送り用
private:
	int					current_image_id;  //画像

										   //Vector3 pos;             //位置
										   //Vector3 move;            //移動量
	Vector3 power;
	Vector3 scale;             //大きさ
	int flg;                // 0:消滅　1:消滅描画　2:その他
	int frame;
	int angle;
	float effect_power;

	int num;

	/*typedef */struct /*DATA*/
	{
		int coordinateX1;        //量(右向き)
		int coordinateY1;
		int coordinateX2;        //量(左向き)
		int coordinateY2;
		int scale;               //描画の幅
	}data[4];

public:
	//コンストラクタ
	EFFECT()/* : pos(Vector3(0, 0, 0)), move(Vector3(0, 0, 0))*/;
	EFFECT(char* filename);

	//デストラクタ
	~EFFECT() { /*image.erase(image.begin, image.end);*/ }

	//メンバ関数
	void Initialize(int no);

	void Update();
	void Render(/*CAMERA *camera*/);
	void Render(Vector3 p, float pow, CAMERA* camera);

	void GetNum(int n) { num = n; }

	//エフェクトチャージ用
	void Charge_Effect(int time);

	//  発射設定
	void Set_Effect(int f, int a);

	//セッター・ゲッター
	//void SetPos(Vector3 p){ pos = p; }

	//Vector3 GetPos(){ return pos; }
	Vector3 GetPower() { return power; }

	int GetFlg() { return flg; }
	void SetFlg(int f) { flg = f; }

	//  bool setScreenPos(Vector3 &power);
	//  bool setScreenPos(Vector3 &power, const Vector3 pos, const Matrix &mat);


	static void SetPng(char* filenaeme, const int max);
	//static iex2DObj* GetPng(int num){ image[num]; }
	static void ReleaseImage();

};

//チャージエフェクト
class CHARGE
{
private:
	iex2DObj* charge_image;   //画像
							  //std::unique_ptr<iex2DObj>

	struct
	{
		Vector3 pos;
		Vector3 vec;
		float   value;
		int     flag;
		int     maxtime;
		int     time;
	}data[16];

	Vector3 power;
	float   range;
	int     timer;
	int     increment;
public:
	//コンストラクタ
	CHARGE() :charge_image(NULL) { /*srand((unsigned int)time(NULL));*/ }
	//デストラクタ
	~CHARGE() { if (charge_image) delete charge_image; }

	void ChargeInitialize();
	void ChargeUpdate(int c_time);
	void ChargeRender(Vector3 p, CAMERA* camera);

	void DataRelease();

	void SetIncrement(int i) { increment = i; }
};

class _EFFECT
{
private:
	std::unique_ptr<iex2DObj> image1;
	std::unique_ptr<iex2DObj> image2;
	std::unique_ptr<iex2DObj> chargeimage;


	struct variable
	{
		int anime;
		int frame;
		bool flg;

	}smoke,charge,magic_Vanish,hold,holded;
	int loop;
	Vector3 pos;


public:
	void Initialize();
	void Update();
	void Render(Vector3 pl, CAMERA* camera);

	void Smoke_Update();
	void Smoke_Render(Vector3 p, CAMERA* camera);

	void Charge_Update();
	void Charge_Render(Vector3 p, CAMERA* camera);

	void magic_Vanish_Update(Vector3 p);
	void magic_Vanish_Render(CAMERA* camera, int no);

	void hold_Update(int no);
	void hold_Render(Vector3& p, CAMERA* camera,int no,int c1,int c2);

	bool Getsmoke_flg() { return smoke.flg; }
	void Setsmoke_flg(bool f) { smoke.flg = f; }

	bool Getcharge_flg() { return charge.flg; }
	void Setcharge_flg(bool f) { charge.flg = f; }

	bool Getmagic_Vanish_flg() { return magic_Vanish.flg;}
	void Setmagic_Vanish_flg(bool f) { magic_Vanish.flg = f; }

	void SetHoldFlg(bool flg) { hold.flg = flg; }
};

//extern std::unique_ptr<EFFECT> effect;