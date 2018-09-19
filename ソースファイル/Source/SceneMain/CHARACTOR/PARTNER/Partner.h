#pragma once
#include	"SceneMain\CHARACTOR\PARTNER\State.h"
#include	"StateMachine.h"
#include	"PartnerMagic.h"
#include	"Trap.h"
#include	"Explosion.h"

//***************************************************************************
//
//    使い魔の基底クラス
//
//***************************************************************************
class BASE_PARTNER
{
protected:
	Vector3 local;
	int rideNum;
	int playerNum;
public:
	virtual ~BASE_PARTNER() {};
	virtual void Initialize(const Vector3&) = 0;
	virtual void Update() = 0;
	virtual void Update(int) = 0;

	virtual void ToLocal() = 0;
	virtual void ToWorld() = 0;

	virtual void Render(int char_no) = 0;
};

//***************************************************************************
//
//    攻撃型の使い魔クラス
//
//***************************************************************************
class ATTACK_PARTNER :public BASE_PARTNER, public CHARACTOR
{
private:
	STATE_MACHIN<ATTACK_PARTNER>* lpStateMachine; //  各ステート格納
	PLAYER*	   lpAllyPlayer;     //  追従するプレイヤー
	PLAYER*    lpOpponentPlayer; //  相手のプレイヤー
	PARTNER_MAGIC*	magic;		 //  とりあえずの魔法攻撃
	TRACKING_A trackingState;    //  追跡
	WAIT_A     waitState;        //  待機(とりあえず)
	CHASE_A    chaseState;       //  相手プレイヤー追跡
	SEARCH     searchState;      //  プレイヤー探索
	RUSH_A     rushState;        //  使い魔の物理攻撃
	MAGIC_A    magicState;       //  魔法攻撃ステート
	GLOBAL_STATE_A globalState;  //  グローバルステート

	iexShader* shader;
	CAMERA* camera;
	Vector3 scale;

	unsigned char hp;  //  使い魔の体力

public:
	ATTACK_PARTNER() :CHARACTOR() {}
	ATTACK_PARTNER(PLAYER* p, PLAYER* p2, CAMERA* c, char* filename, int plNum) :
		CHARACTOR(filename), lpAllyPlayer(p), lpOpponentPlayer(p2),
		lpStateMachine(nullptr), hp(5), magic(nullptr), camera(c)
	{
		playerNum = plNum;
		lpStateMachine = new STATE_MACHIN<ATTACK_PARTNER>(this, &trackingState, &globalState, lpAllyPlayer, lpOpponentPlayer);
		shader = new iexShader("DATA/SHADER/3DEX.FX");
	};

	~ATTACK_PARTNER()
	{
		lpAllyPlayer = nullptr;
		lpOpponentPlayer = nullptr;
		if (magic)
		{
			delete magic;
			magic = nullptr;
		}
		if (lpStateMachine)
		{
			delete lpStateMachine;
			lpStateMachine = nullptr;
		}
		if (shader)
		{
			delete shader;
			shader = nullptr;
		}
	}

	//void Initialize(char* filename)override;
	void Initialize(const Vector3&);
	void Initialize() {};
	void Initialize(Vector3 pos) {};

	void MagicUpdate();
	void RushCollision();

	void Update()override;
	void Update(int t) {};
	void Render(int char_no)override;
	void Render(CAMERA* camera) {}

	void ToLocal()override;
	void ToWorld()override;

	void ReduceHp()
	{
		hp--;
		if (hp < 0) hp = 0;
	}

	//  セッター
	void SetPos(const Vector3& p) { pos = p; }
	void SetMove(const Vector3& m) { move = m; }
	void SetMoveX(float x) { move.x = x; }
	void SetMoveY(float y) { move.y = y; }
	void SetMagic(PARTNER_MAGIC* s) { magic = s; }
	void SetAngle(int a) { angle = (DIST)a; }
	void SetMagicPosMove(const Vector3& p, const Vector3& v)
	{
		magic = new PARTNER_MAGIC(p, v, shader, playerNum);
	}
	//  ステートを変えたいときに
	void ChangeState(PARTNER_STATE<ATTACK_PARTNER>* NewState);

	//  ゲッター
	DIST           GetAngle() { return angle; }
	TRACKING_A*    GetTracking() { return &trackingState; }
	WAIT_A*        GetWait() { return &waitState; }
	CHASE_A*       GetChase() { return &chaseState; }
	SEARCH*        GetSearch() { return &searchState; }
	RUSH_A*        GetRush() { return &rushState; }
	MAGIC_A*       GetMagicState() { return &magicState; }
	Vector3        GetMove() { return move; }
	PARTNER_MAGIC* GetMagic() { return magic; }
	STATE_MACHIN<ATTACK_PARTNER>* GetFSM()const { return lpStateMachine; }
	iex3DObj* GetObj() { return obj.get(); }
};

//***************************************************************************
//***************************************************************************

//***************************************************************************
//
//    罠設置型の使い魔クラス
//
//***************************************************************************
class TRAP_PARTNER :public BASE_PARTNER, public CHARACTOR
{
private:
	STATE_MACHIN<TRAP_PARTNER>* lpStateMachine;  //  各ステート格納
	EXPLOSION* explosion[3];
	iexShader* shader;

	PLAYER*	   lpAllyPlayer;     //  追従するプレイヤー
	PLAYER*    lpOpponentPlayer; //  相手のプレイヤー
	PARTNER_MAGIC* magic;        //  とりあえずの魔法攻撃
	TRAP*      trap[3];			 //  罠
	TRACKING_T trackingState;	 //  追跡
								 //SEARCH_T   searchState;		 //  探索
	WAIT_T     waitState;        //  待機
	MAGIC_T    magicState;		 //  魔法攻撃
	TRAP_T     trapState;		 //  罠投げる
	GLOBAL_STATE_T globalState;  //  グローバルステート

	CAMERA* camera;
	Vector3 scale;
	iex3DObj* clone;
	bool       jumpFlg;   //  ・false:飛んでない・true:飛んでる
	unsigned char  hp;    //  使い魔の体力
	int		   time;      //  戦闘の残り時間

public:
	TRAP_PARTNER() :CHARACTOR() {}
	TRAP_PARTNER(PLAYER* p, PLAYER* p2, CAMERA* c, char*filename, int plNum) :
		CHARACTOR(filename), lpAllyPlayer(p), camera(c),
		lpOpponentPlayer(p2), lpStateMachine(nullptr), hp(5),
		time(0), magic(nullptr)
	{
		playerNum = plNum;
		for (int i = 0; i < 3; i++)
		{
			trap[i] = nullptr;
			explosion[i] = nullptr;

		}
		clone = new iex3DObj("DATA\\MODEL\\CHR\\PARTNER\\CAT\\A\\Cat3.IEM");
		shader = new iexShader("DATA/SHADER/3DEX.FX");
		lpStateMachine = new STATE_MACHIN<TRAP_PARTNER>(this, &trackingState, &globalState, lpAllyPlayer, lpOpponentPlayer);
	};
	~TRAP_PARTNER()
	{
		lpAllyPlayer = nullptr;
		lpOpponentPlayer = nullptr;
		if (lpStateMachine)
		{
			delete lpStateMachine;
			lpStateMachine = nullptr;
		}
		for (int i = 0; i < 3; i++)
		{
			if (trap[i])
			{
				delete trap[i]; trap[i] = nullptr;
			}
			if (explosion[i])
			{
				delete explosion[i];
				explosion[i] = nullptr;
			}
		}
		if (magic)
		{
			delete magic;
			magic = nullptr;
		}
		if (clone) { delete clone; clone = nullptr; }
		if (shader)
		{
			delete shader;
			shader = nullptr;
		}
	}

	//void Initialize(char* filename)override;
	void Initialize(const Vector3& pos);
	void Initialize() {};
	void Initialize(Vector3 pos) {};

	//  トラップ初期化
	//void TrapInit(int i)
	//{
	//	if (angle == DIST::RIGHT) trap[i]->Initialize(pos + Vector3(0, 2.0f, 0), Vector3( 0.5f, 1.5f, 0), playerNum);
	//	if (angle == DIST::LEFT)  trap[i]->Initialize(pos + Vector3(0, 2.0f, 0), Vector3(-0.5f, 1.5f, 0), playerNum);
	//}

	void MagicUpdate();
	void TrapUpdate();

	void Update()override {};
	void Update(int timer);
	void Render(int char_no)override;
	void Render(CAMERA* camera) {}

	void ToLocal();
	void ToWorld();

	void ReduceHp()
	{
		hp--;
		if (hp < 0) hp = 0;
	}

	//  セッター
	void SetPos(const Vector3& p) { pos = p; }
	void SetMove(const Vector3& m) { move = m; }
	void SetMoveX(float x) { move.x = x; }
	void SetMoveY(float y) { move.y = y; }
	void SetJumpFlg(bool flg) { jumpFlg = flg; }

	void SetTrap(int i)
	{
		//  UNDONE:変更
		if (angle == DIST::RIGHT)
			trap[i] = new TRAP(shader, pos + Vector3(0, 2.0f, 0), Vector3(0.5f, 1.5f, 0), playerNum, PI * 2, clone->Clone());
		if (angle == DIST::LEFT)
			trap[i] = new TRAP(shader, pos + Vector3(0, 2.0f, 0), Vector3(-0.5f, 1.5f, 0), playerNum, PI, clone->Clone());
		trap[i]->Initialize();
	}

	void SetMagic(PARTNER_MAGIC* m) { magic = m; }
	//void SetMagicPosMove(const Vector3& p, const Vector3& v) { magic->Append(p, v); }
	//  ステートを変えたいときに
	void ChangeState(PARTNER_STATE<TRAP_PARTNER>* lpNewState);

	//  ゲッター
	bool		   GetJumpFlg() { return jumpFlg; }
	TRACKING_T*    GetTracking() { return &trackingState; }
	//SEARCH_T*      GetSearch()		 { return &searchState;	  }
	MAGIC_T*       GetMagicState() { return &magicState; }
	TRAP_T*        GetTrapState() { return &trapState; }
	WAIT_T*        GetWaitState() { return &waitState; }
	TRAP*		   GetTrap(int i) { return trap[i]; }
	PARTNER_MAGIC* GetMagic() { return magic; }
	iex3DObj*      GetObj() { return obj.get(); }
	int			   GetTimer() { return time; }

	STATE_MACHIN<TRAP_PARTNER>* GetFSM()const { return lpStateMachine; }
};

//***************************************************************************
//***************************************************************************

//***************************************************************************
//
//    クリスタルの使い魔クラス
//
//***************************************************************************
class CLYSTAL_PARTNER :public CHARACTOR
{
private:
	PLAYER*	lpOpponentPlayer;		  //  相手プレイヤー
	std::unique_ptr<PARTNER_MAGIC>	magic;

	bool shotFlag;
	bool shotCheckFlag;
	bool motionFlag;
	int  shotTimer;     //  使い魔が魔法打つ間隔  

	CAMERA* camera;
	unsigned char hp;	//  使い魔の体力

public:
	CLYSTAL_PARTNER() :CHARACTOR() {}

	CLYSTAL_PARTNER(char* filename, PLAYER* p, CAMERA* c) :
		CHARACTOR(filename), lpOpponentPlayer(p), hp(5), camera(c), motionFlag(false) {};
	~CLYSTAL_PARTNER()
	{
		lpOpponentPlayer = nullptr;
	}

	void Initialize() {};
	void Initialize(Vector3 pos);
	void Update()override;
	void Render(int char_no)override;
	void Render(CAMERA* camera) {}

	void Shot();
	void ReduceHp()
	{
		hp--;
		if (hp < 0) hp = 0;
	}

	//  セッター
	void SetShotFlag(bool s) { shotFlag = s; }
	void SetShotCheckFlag(bool s) { shotCheckFlag = s; }

	//  ゲッター
	bool GetShotFlag() { return shotFlag; }
	bool GetShotCheckFlag() { return shotCheckFlag; }
	PLAYER* GetOpponentPlayer() { return lpOpponentPlayer; }
};