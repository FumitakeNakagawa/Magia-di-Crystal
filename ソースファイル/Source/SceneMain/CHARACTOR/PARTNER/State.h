#pragma once
#include	"SceneMain\CHARACTOR\PLAYER\PLAYER.h"

#define PLAYER_POS  lpAllyPlayer->GetPos()
#define PARTNER_POS lpPartner->GetPos()
#define CHASE_MOVE_MAX 0.25f

class ATTACK_PARTNER;
class TRAP_PARTNER;
class CLYSTAL_PARTNER;

//*******************************************************
//  ステート基底クラス
//*******************************************************
template <class partnerType>
class PARTNER_STATE
{
protected:
	PLAYER* lpAllyPlayer;     //  追従するプレイヤー
	PLAYER* lpOpponentPlayer; //  相手のプレイヤー

public:
	void SetAllyPlayer(PLAYER* p) { lpAllyPlayer = p; }
	void SetOpponentPlayer(PLAYER* p) { lpOpponentPlayer = p; }

	virtual void Enter(partnerType*) = 0;    //  継承先で絶対実装する
	virtual void Execute(partnerType*) = 0;  //  継承先で絶対実装する
	virtual void Exit(partnerType*) = 0;     //  継承先で絶対実装する
	virtual ~PARTNER_STATE()
	{
		lpAllyPlayer = nullptr;
		lpOpponentPlayer = nullptr;
	}
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//*******************************************************
//  攻撃型の使い魔の追従クラス
//*******************************************************
class TRACKING_A :public PARTNER_STATE<ATTACK_PARTNER>
{
private:
	Vector3 move;
public:
	TRACKING_A() :move(0, 0, 0) {};
	~TRACKING_A() {};

	void Enter(ATTACK_PARTNER*);
	void Execute(ATTACK_PARTNER*);
	void Exit(ATTACK_PARTNER*);
};

//*******************************************************
//  攻撃型の使い魔の待機クラス
//*******************************************************
class WAIT_A :public PARTNER_STATE<ATTACK_PARTNER>
{
private:
	bool flyFlag;
	float fly;

public:
	WAIT_A() :fly(0), flyFlag(true) {};
	~WAIT_A() {};

	void Enter(ATTACK_PARTNER*);
	void Execute(ATTACK_PARTNER*);
	void Exit(ATTACK_PARTNER*);
};

//*******************************************************
//  攻撃型の使い魔の相手プレイヤー追跡クラス
//*******************************************************
class CHASE_A :public PARTNER_STATE<ATTACK_PARTNER>
{
private:
	Vector3 move;
public:
	CHASE_A() :move(0, 0, 0) {};
	~CHASE_A() {};

	void Enter(ATTACK_PARTNER*);
	void Execute(ATTACK_PARTNER*);
	void Exit(ATTACK_PARTNER*);
};

//*******************************************************
//  攻撃型の使い魔のプレイヤー探索クラス
//*******************************************************
class SEARCH :public PARTNER_STATE<ATTACK_PARTNER>
{
private:
public:
	SEARCH() {};
	~SEARCH() {};

	void Enter(ATTACK_PARTNER*);
	void Execute(ATTACK_PARTNER*);
	void Exit(ATTACK_PARTNER*);
};

//*******************************************************
//  攻撃型の使い魔の物理攻撃するクラス
//*******************************************************
class RUSH_A :public PARTNER_STATE<ATTACK_PARTNER>
{
private:
	//Vector3 opponentPos;
	//Vector3 attackVec;
	int attackTimer;

	enum
	{
		RIGHT,
		LEFT
	};

public:
	RUSH_A() :attackTimer(150) {};
	~RUSH_A() {};


	void Enter(ATTACK_PARTNER*);
	void Execute(ATTACK_PARTNER*);
	void Exit(ATTACK_PARTNER*);
};

//*******************************************************
//  攻撃型の使い魔の魔法攻撃するクラス
//*******************************************************
class MAGIC_A :public PARTNER_STATE<ATTACK_PARTNER>
{
private:
	int  magicTimer;
	bool shotFlag;  //  true:撃った、false:撃ってない
	enum
	{
		RIGHT,
		LEFT
	};

public:
	MAGIC_A() :magicTimer(150), shotFlag(false) {};
	void Enter(ATTACK_PARTNER*);
	void Execute(ATTACK_PARTNER*);
	void Exit(ATTACK_PARTNER*);
};

//*******************************************************
//  攻撃型の使い魔のグローバルステートクラス
//*******************************************************
class GLOBAL_STATE_A :public PARTNER_STATE<ATTACK_PARTNER>
{
private:
	//  UNDONE:追加
	enum
	{
		RIGHT,
		LEFT
	};
public:
	GLOBAL_STATE_A() {};
	~GLOBAL_STATE_A() {};
	void Enter(ATTACK_PARTNER*) {};
	void Execute(ATTACK_PARTNER*);
	void Exit(ATTACK_PARTNER*) {};
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//*******************************************************
//  罠設置型使い魔の追従クラス
//*******************************************************
class TRACKING_T :public PARTNER_STATE<TRAP_PARTNER>
{
private:
public:
	TRACKING_T() {};
	~TRACKING_T() {};

	void Enter(TRAP_PARTNER*);
	void Execute(TRAP_PARTNER*);
	void Exit(TRAP_PARTNER*);
};

//*******************************************************
//  罠設置型使い魔のプレイヤー探索クラス
//*******************************************************
//class SEARCH_T :public PARTNER_STATE<TRAP_PARTNER>
//{
//private:
//	int LR_Flag;
//	int flagCheckFlag;
//
//	enum
//	{
//		RIGHT,
//		LEFT, 
//		CHECK,
//		NO_CHECK
//	};
//public:
//	SEARCH_T() {};
//	~SEARCH_T() {};
//
//	void Enter(TRAP_PARTNER*);
//	void Execute(TRAP_PARTNER*);
//	void Exit(TRAP_PARTNER*);
//};

//*******************************************************
//  罠設置型使い魔の待機クラス
//*******************************************************
class WAIT_T :public PARTNER_STATE<TRAP_PARTNER>
{
private:
public:
	WAIT_T() {};
	~WAIT_T() {};

	void Enter(TRAP_PARTNER*);
	void Execute(TRAP_PARTNER*);
	void Exit(TRAP_PARTNER*);
};

//*******************************************************
//  罠設置型使い魔の魔法攻撃するクラス
//*******************************************************
class MAGIC_T :public PARTNER_STATE<TRAP_PARTNER>
{
private:
	int magicTimer;
	bool shotFlag;  //  true:撃った、false:撃ってない
public:
	MAGIC_T() :magicTimer(120), shotFlag(false) {};
	void Enter(TRAP_PARTNER*);
	void Execute(TRAP_PARTNER*);
	void Exit(TRAP_PARTNER*);
};

//*******************************************************
//  罠設置型使い魔の罠設置するクラス
//*******************************************************
class TRAP_T :public PARTNER_STATE<TRAP_PARTNER>
{
private:
public:
	void Enter(TRAP_PARTNER*);
	void Execute(TRAP_PARTNER*);
	void Exit(TRAP_PARTNER*);
};

//*******************************************************
//  罠設置型使い魔のグローバルステートクラス
//*******************************************************
class GLOBAL_STATE_T :public PARTNER_STATE<TRAP_PARTNER>
{
private:
	bool trapFlag;
public:
	GLOBAL_STATE_T() :trapFlag(false) {};
	~GLOBAL_STATE_T() {};
	void Enter(TRAP_PARTNER*) {};
	void Execute(TRAP_PARTNER*);
	void Exit(TRAP_PARTNER*) {};
};