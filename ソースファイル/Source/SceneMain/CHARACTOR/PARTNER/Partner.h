#pragma once
#include	"SceneMain\CHARACTOR\PARTNER\State.h"
#include	"StateMachine.h"
#include	"PartnerMagic.h"
#include	"Trap.h"
#include	"Explosion.h"

//***************************************************************************
//
//    �g�����̊��N���X
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
//    �U���^�̎g�����N���X
//
//***************************************************************************
class ATTACK_PARTNER :public BASE_PARTNER, public CHARACTOR
{
private:
	STATE_MACHIN<ATTACK_PARTNER>* lpStateMachine; //  �e�X�e�[�g�i�[
	PLAYER*	   lpAllyPlayer;     //  �Ǐ]����v���C���[
	PLAYER*    lpOpponentPlayer; //  ����̃v���C���[
	PARTNER_MAGIC*	magic;		 //  �Ƃ肠�����̖��@�U��
	TRACKING_A trackingState;    //  �ǐ�
	WAIT_A     waitState;        //  �ҋ@(�Ƃ肠����)
	CHASE_A    chaseState;       //  ����v���C���[�ǐ�
	SEARCH     searchState;      //  �v���C���[�T��
	RUSH_A     rushState;        //  �g�����̕����U��
	MAGIC_A    magicState;       //  ���@�U���X�e�[�g
	GLOBAL_STATE_A globalState;  //  �O���[�o���X�e�[�g

	iexShader* shader;
	CAMERA* camera;
	Vector3 scale;

	unsigned char hp;  //  �g�����̗̑�

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

	//  �Z�b�^�[
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
	//  �X�e�[�g��ς������Ƃ���
	void ChangeState(PARTNER_STATE<ATTACK_PARTNER>* NewState);

	//  �Q�b�^�[
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
//    㩐ݒu�^�̎g�����N���X
//
//***************************************************************************
class TRAP_PARTNER :public BASE_PARTNER, public CHARACTOR
{
private:
	STATE_MACHIN<TRAP_PARTNER>* lpStateMachine;  //  �e�X�e�[�g�i�[
	EXPLOSION* explosion[3];
	iexShader* shader;

	PLAYER*	   lpAllyPlayer;     //  �Ǐ]����v���C���[
	PLAYER*    lpOpponentPlayer; //  ����̃v���C���[
	PARTNER_MAGIC* magic;        //  �Ƃ肠�����̖��@�U��
	TRAP*      trap[3];			 //  �
	TRACKING_T trackingState;	 //  �ǐ�
								 //SEARCH_T   searchState;		 //  �T��
	WAIT_T     waitState;        //  �ҋ@
	MAGIC_T    magicState;		 //  ���@�U��
	TRAP_T     trapState;		 //  㩓�����
	GLOBAL_STATE_T globalState;  //  �O���[�o���X�e�[�g

	CAMERA* camera;
	Vector3 scale;
	iex3DObj* clone;
	bool       jumpFlg;   //  �Efalse:���łȂ��Etrue:���ł�
	unsigned char  hp;    //  �g�����̗̑�
	int		   time;      //  �퓬�̎c�莞��

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

	//  �g���b�v������
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

	//  �Z�b�^�[
	void SetPos(const Vector3& p) { pos = p; }
	void SetMove(const Vector3& m) { move = m; }
	void SetMoveX(float x) { move.x = x; }
	void SetMoveY(float y) { move.y = y; }
	void SetJumpFlg(bool flg) { jumpFlg = flg; }

	void SetTrap(int i)
	{
		//  UNDONE:�ύX
		if (angle == DIST::RIGHT)
			trap[i] = new TRAP(shader, pos + Vector3(0, 2.0f, 0), Vector3(0.5f, 1.5f, 0), playerNum, PI * 2, clone->Clone());
		if (angle == DIST::LEFT)
			trap[i] = new TRAP(shader, pos + Vector3(0, 2.0f, 0), Vector3(-0.5f, 1.5f, 0), playerNum, PI, clone->Clone());
		trap[i]->Initialize();
	}

	void SetMagic(PARTNER_MAGIC* m) { magic = m; }
	//void SetMagicPosMove(const Vector3& p, const Vector3& v) { magic->Append(p, v); }
	//  �X�e�[�g��ς������Ƃ���
	void ChangeState(PARTNER_STATE<TRAP_PARTNER>* lpNewState);

	//  �Q�b�^�[
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
//    �N���X�^���̎g�����N���X
//
//***************************************************************************
class CLYSTAL_PARTNER :public CHARACTOR
{
private:
	PLAYER*	lpOpponentPlayer;		  //  ����v���C���[
	std::unique_ptr<PARTNER_MAGIC>	magic;

	bool shotFlag;
	bool shotCheckFlag;
	bool motionFlag;
	int  shotTimer;     //  �g���������@�łԊu  

	CAMERA* camera;
	unsigned char hp;	//  �g�����̗̑�

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

	//  �Z�b�^�[
	void SetShotFlag(bool s) { shotFlag = s; }
	void SetShotCheckFlag(bool s) { shotCheckFlag = s; }

	//  �Q�b�^�[
	bool GetShotFlag() { return shotFlag; }
	bool GetShotCheckFlag() { return shotCheckFlag; }
	PLAYER* GetOpponentPlayer() { return lpOpponentPlayer; }
};