#pragma once

#include "../IEX/iextreme.h"
#include	"SceneMain\CAMERA\CAMERA.h"
#include <memory>


#define MAXSPEED 2.0
#define GRAVITY -0.06f
#define MAXLIFE	12

class CHARACTOR
{
public:
	enum class STATE
	{
		WAIT,		//待機（初期状態）
		MOVE,		//移動（基本状態）
		JUMP,		//ジャンプ
		ATTACK,		//攻撃（未使用）
		HOLD,		//クリスタル獲得中
		INVINCIBLE,	//無敵
		DAMAGE,		//ダメージをうけた
		DEATH,		//死んでいる
		DEAD,		//死んだ
		
		

		MODE_MAX
	};

	enum class NO
	{
		P1 = 0,
		P2 = 1,

		PLAYER_MAX
	};

	enum class DIST
	{
		RIGHT = 0,
		LEFT = 1,
	};

protected:
	std::unique_ptr<iex3DObj> obj;

	Vector3 pos;	//座標
	Vector3 move;	//移動量
	DIST angle;	//向き（Y軸のみ）
	float life;		//ライフ


	STATE state;	//状態

	NO no;	//番号（1：1PLAYER　2：2PLAYER）

public:
	//コンストラクタ・デストラクタ
	CHARACTOR() :obj(nullptr), pos(Vector3(0, 0, 0)), move(Vector3(0, 0, 0)), angle(DIST::RIGHT), life(MAXLIFE), state(STATE::WAIT) {}
	CHARACTOR(char* filename);

	//メンバ関数
	virtual void Initialize() = 0;
	virtual void Initialize(const Vector3&) = 0;
	virtual void Update() = 0;
	virtual void Update(const Vector3& epos) {};

	virtual void Render(int char_no) = 0;
	virtual void Render(CAMERA* camera) = 0;

	//セッター・ゲッター
	void Setpos(Vector3 p) { pos = p; }
	void Setpos(float x, float y, float z) { pos = Vector3(x, y, z); }
	void Setmove(Vector3 m) { move = m; }
	void Setmove(float x, float y, float z) { move = Vector3(x, y, z); }


	void SetMode(int m) { state = static_cast<STATE>(m); }


	//追加---------------------------------------
	Vector3 GetPos() const { return pos; }
	DIST    GetAngle()const { return angle; }
	//-------------------------------------------

	Vector3 GetPos() { return pos; }
	Vector3* GetPosAddres() { return &pos; }
	//int GetMode() { return static_cast<int>(state); }
	STATE GetMode() { return state; }


};
