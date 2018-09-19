#pragma once
#include	"../IEX/iextreme.h"
#include	"SCENEMAIN\CHARACTOR\Character.h"
#include	"Magic.h"
#include	"SceneMain\CAMERA\CAMERA.h"
#include	"SceneMain\EFFECT\Effect.h"


#define jump_power 1.3f


class PLAYER :public CHARACTOR
{
private:
	void Wait();
	void Control();
	void Attack();
	void Damage(float power, PLAYER* p);	//�i�������F�_���[�W�ʁ@�������F�G�v���C���[�j
	void Dead();
	void Invincible(int t);					//�i�����F���G����(�t���[����)�j

	int jump_time = 0;
	int magic_timer = 0;
	int inv_timer = 0;						//���G���ԗp�i�X�e�[�g�ύX�O�ɕύX����j
	bool inv_jump = false;
	bool crystalhold = false;

	std::shared_ptr<MAGIC>	magic;
	float magicPower;

	//std::unique_ptr<iex2DObj> particle;		//���G�t�F�N�g�p
	std::unique_ptr<CHARGE> charge;
	std::unique_ptr<_EFFECT> _effect;


protected:
	int  jump_flg;		//�W�����v�p�i0�F�n�ʁ@1�F�󒆁j
	int shot_flg;	//�V���b�g�p�i0�F�e�łĂȂ��@1�F�`���[�W���@2�F�e�łĂ�j

	Vector3 scale;

	int Char;	//�L�����ԍ��i0�@or�@1�j

public:
	PLAYER() :CHARACTOR() {}
	PLAYER(char* filename) :CHARACTOR(filename) {}
	~PLAYER() {  }


	void Initialize() {};
	void Initialize(const Vector3&)override {}				//�������i�ʒu�j
	void Initialize(Vector3 pos, int angle, int n, int char_no);		//�������i�ʒu,�@����,�@�R���g���[���[�ԍ�,�L�����ԍ��j
	void Respawn(Vector3 pos, int angle);				//�ċN�i�ʒu, �����j
	void Update()override;
	void Update(const Vector3& epos)override {};			//�X�V�i�G�ʒu�j
	void UPdate(PLAYER* p);								//�X�V�i�G�v���C���[�j
	void Render(int char_no)override{}
	void Render(CAMERA* camera);
	void SetMotion(int num) { if (obj->GetMotion() != num) obj->SetMotion(num); }

	int Result();
	void ResultRender();

	//void Effect_Render(CAMERA* camera, Vector3 pos);	//���G�t�F�N�g�i�J�����@, �ʒu�j

	//�Q�b�^�[
	MAGIC* GetMagic() { return magic.get(); };
	int Getmagic_timer() { return magic_timer; }
	float Getlife() { return life; }
	float GetmagicPower() { return magicPower; }
	DIST GetmagicAngle() { return GetMagic()->GetAngle(); }
	int GetDist() { return (int)angle; }
	bool Gethold() { return crystalhold; }
	Vector3 Getmove() { return move; }
	int GetNo() { return (int)no; }

	//�Z�b�^�[
	void Setstate(STATE s) { state = s; }
	void SetmagicPower(float p) { magicPower = p; }
	void SetJumpFlg(int n) { jump_flg = n; }
	void SetmoveY(float y) { move.y = y; }
	void Sethold(bool f) { crystalhold = f; }

	void SubtractHP(unsigned char damage)
	{
		life -= damage;
	}

};

