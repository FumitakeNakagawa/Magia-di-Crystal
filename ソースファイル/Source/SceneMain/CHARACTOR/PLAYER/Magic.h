#pragma once

#include	"iextreme.h"
#include	"SceneMain\EFFECT\Effect.h"
#include "SceneMain\CHARACTOR\Character.h"
#include	<memory>

#define		MAGICSPEED		1.5		//�e��
#define		MAXPOWER		4.0f	//�ő�_���[�W��
#define		ALLIVETIME		45		//��������
#define		MAXNUM			3		//�ő吔

class MAGIC
{
private:


protected:
	std::unique_ptr<iex3DObj>	obj;	//���f��
	std::unique_ptr<EFFECT> effect;
	float		move;					//�ړ��ʁi�����̂݁j
	CHARACTOR::DIST		angle;					//����
	Vector3		pos;					//�ʒu
	Vector3		scale;					//�T�C�Y

	int			time;					//��������

	float		power;					//����
	bool		alive;					//�o�Ă���(true) or �o�Ă��Ȃ�(false)
	int			num;					//���@�̐��i�ア�ƍő�R���j

public:
	MAGIC() {}
	MAGIC(int no);

	bool	Append(Vector3 p, CHARACTOR::DIST a);	//�o���i�ʒu , �����j
	void	Charge(int time);			//�`���[�W�i���ԁi�t���[���j�j
	bool	Move();						//����ialive��Ԃ��j


	bool	eraseOK();					//�����Ă����itrue�j or �����Ȃ�(false)
	void	erase();					//�폜�i�����o�ϐ��������j


	void	Render(CAMERA* camera);					//�`��

													//�Q�b�^�[
	float GetPower() { return power; }
	Vector3 GetPos() { return pos; }
	Vector3 GetScale() { return scale; }
	CHARACTOR::DIST GetAngle() { return angle; }
	bool GetAlive() { return alive; }



};


