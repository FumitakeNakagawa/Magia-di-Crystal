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
		WAIT,		//�ҋ@�i������ԁj
		MOVE,		//�ړ��i��{��ԁj
		JUMP,		//�W�����v
		ATTACK,		//�U���i���g�p�j
		HOLD,		//�N���X�^���l����
		INVINCIBLE,	//���G
		DAMAGE,		//�_���[�W��������
		DEATH,		//����ł���
		DEAD,		//����
		
		

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

	Vector3 pos;	//���W
	Vector3 move;	//�ړ���
	DIST angle;	//�����iY���̂݁j
	float life;		//���C�t


	STATE state;	//���

	NO no;	//�ԍ��i1�F1PLAYER�@2�F2PLAYER�j

public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CHARACTOR() :obj(nullptr), pos(Vector3(0, 0, 0)), move(Vector3(0, 0, 0)), angle(DIST::RIGHT), life(MAXLIFE), state(STATE::WAIT) {}
	CHARACTOR(char* filename);

	//�����o�֐�
	virtual void Initialize() = 0;
	virtual void Initialize(const Vector3&) = 0;
	virtual void Update() = 0;
	virtual void Update(const Vector3& epos) {};

	virtual void Render(int char_no) = 0;
	virtual void Render(CAMERA* camera) = 0;

	//�Z�b�^�[�E�Q�b�^�[
	void Setpos(Vector3 p) { pos = p; }
	void Setpos(float x, float y, float z) { pos = Vector3(x, y, z); }
	void Setmove(Vector3 m) { move = m; }
	void Setmove(float x, float y, float z) { move = Vector3(x, y, z); }


	void SetMode(int m) { state = static_cast<STATE>(m); }


	//�ǉ�---------------------------------------
	Vector3 GetPos() const { return pos; }
	DIST    GetAngle()const { return angle; }
	//-------------------------------------------

	Vector3 GetPos() { return pos; }
	Vector3* GetPosAddres() { return &pos; }
	//int GetMode() { return static_cast<int>(state); }
	STATE GetMode() { return state; }


};
