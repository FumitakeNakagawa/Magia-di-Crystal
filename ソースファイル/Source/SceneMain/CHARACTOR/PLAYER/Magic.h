#pragma once

#include	"iextreme.h"
#include	"SceneMain\EFFECT\Effect.h"
#include "SceneMain\CHARACTOR\Character.h"
#include	<memory>

#define		MAGICSPEED		1.5		//弾速
#define		MAXPOWER		4.0f	//最大ダメージ量
#define		ALLIVETIME		45		//生存時間
#define		MAXNUM			3		//最大数

class MAGIC
{
private:


protected:
	std::unique_ptr<iex3DObj>	obj;	//モデル
	std::unique_ptr<EFFECT> effect;
	float		move;					//移動量（ｘ軸のみ）
	CHARACTOR::DIST		angle;					//向き
	Vector3		pos;					//位置
	Vector3		scale;					//サイズ

	int			time;					//生存時間

	float		power;					//強さ
	bool		alive;					//出ている(true) or 出ていない(false)
	int			num;					//魔法の数（弱いと最大３発）

public:
	MAGIC() {}
	MAGIC(int no);

	bool	Append(Vector3 p, CHARACTOR::DIST a);	//出現（位置 , 向き）
	void	Charge(int time);			//チャージ（時間（フレーム））
	bool	Move();						//動作（aliveを返す）


	bool	eraseOK();					//消していい（true） or 消さない(false)
	void	erase();					//削除（メンバ変数初期化）


	void	Render(CAMERA* camera);					//描画

													//ゲッター
	float GetPower() { return power; }
	Vector3 GetPos() { return pos; }
	Vector3 GetScale() { return scale; }
	CHARACTOR::DIST GetAngle() { return angle; }
	bool GetAlive() { return alive; }



};


