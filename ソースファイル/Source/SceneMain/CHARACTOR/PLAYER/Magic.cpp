#include "Magic.h"
#include "SceneMain\EFFECT\Effect.h"




bool MAGIC::eraseOK()
{
	if (time > ALLIVETIME || alive == false)	//一定時間異常経ったら
		return true;
	return false;
}

//削除（メンバ変数初期化）
void MAGIC::erase()
{
	obj.release();

	effect->SetFlg(0);

	time = 0;
	scale = Vector3(0.0f, 0.0f, 0.0f);
	alive = false;
	power = 0;

}

//コンストラクタ
MAGIC::MAGIC(int no)
{
	obj.reset(nullptr);

	effect.reset(new EFFECT());
	effect->Initialize(no);

	pos = Vector3(0, 0, 0);
	move = MAGICSPEED;
	angle = CHARACTOR::DIST::RIGHT;
	scale = Vector3(1.0f, 1.0f, 1.0f);

	time = 0;
	power = 0;
	alive = false;
}

//出現
bool MAGIC::Append(Vector3 p, CHARACTOR::DIST a)
{
	if (!obj)										//オブジェクトがないとき生成する
	{
		obj.reset(new iex3DObj("DATA\\magic\\magic.IEM"));
		pos = p;
		angle = a;
		alive = true;
		effect->Set_Effect(2, (int)angle);
		return true;
	}

	else
	{
		return false;
	}

}

//チャージ
void MAGIC::Charge(int time)
{
	int t;
	if (time >= 120)
	{
		t = 120;
		scale.x = 1.0f + t*0.05f;		//サイズ更新
		scale.y = 1.0f + t*0.05f;		//サイズ更新
		scale.z = 1.0f + t*0.05f;		//サイズ更新


	}

	if (time < 120)
	{
		t = 80;

		scale.x = 1.0f + 60 * 0.05f;		//サイズ更新
		scale.y = 1.0f + 60 * 0.05f;		//サイズ更新
		scale.z = 1.0f + 60 * 0.05f;		//サイズ更新
	}

	if (time < 80)
	{
		t = 40;
		scale.x = 1.0f + 30 * 0.05f;		//サイズ更新
		scale.y = 1.0f + 30 * 0.05f;		//サイズ更新
		scale.z = 1.0f + 30 * 0.05f;		//サイズ更新
	}

	if (time < 40)
	{
		t = 5;
		scale.x = 1.0f + 5 * 0.05f;		//サイズ更新
		scale.y = 1.0f + 5 * 0.05f;		//サイズ更新
		scale.z = 1.0f + 5 * 0.05f;		//サイズ更新
	}


	power = t * MAXPOWER / 120;		//強さ決定
	if (power < 1)					//最小を1にする
		power = 1.0f;

}

//魔法が出ているときtrue 出ていないときfalseを返す
bool MAGIC::Move()
{


	if (eraseOK())
	{
		erase();
	}

	else				//消せないとき動く
	{
		if (angle == CHARACTOR::DIST::LEFT)
			pos.x -= move;
		if (angle == CHARACTOR::DIST::RIGHT)
			pos.x += move;
		time++;
	}



	return alive;
}



//描画
void MAGIC::Render(CAMERA*camera)
{

	if (alive)		//オブジェクトがあるとき
	{
		//オブジェクト更新
		obj->SetPos(pos + Vector3(0, 10, 0));
		obj->SetAngle((int)angle*PI);
		obj->SetScale(scale);
		obj->Update();

		//描画
		//obj->Render();
		effect->Update();
		if (obj)
			effect->Render(pos + Vector3(0, 10, 0), power, camera);

	}



}
