#include "Magic.h"
#include "SceneMain\EFFECT\Effect.h"




bool MAGIC::eraseOK()
{
	if (time > ALLIVETIME || alive == false)	//��莞�Ԉُ�o������
		return true;
	return false;
}

//�폜�i�����o�ϐ��������j
void MAGIC::erase()
{
	obj.release();

	effect->SetFlg(0);

	time = 0;
	scale = Vector3(0.0f, 0.0f, 0.0f);
	alive = false;
	power = 0;

}

//�R���X�g���N�^
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

//�o��
bool MAGIC::Append(Vector3 p, CHARACTOR::DIST a)
{
	if (!obj)										//�I�u�W�F�N�g���Ȃ��Ƃ���������
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

//�`���[�W
void MAGIC::Charge(int time)
{
	int t;
	if (time >= 120)
	{
		t = 120;
		scale.x = 1.0f + t*0.05f;		//�T�C�Y�X�V
		scale.y = 1.0f + t*0.05f;		//�T�C�Y�X�V
		scale.z = 1.0f + t*0.05f;		//�T�C�Y�X�V


	}

	if (time < 120)
	{
		t = 80;

		scale.x = 1.0f + 60 * 0.05f;		//�T�C�Y�X�V
		scale.y = 1.0f + 60 * 0.05f;		//�T�C�Y�X�V
		scale.z = 1.0f + 60 * 0.05f;		//�T�C�Y�X�V
	}

	if (time < 80)
	{
		t = 40;
		scale.x = 1.0f + 30 * 0.05f;		//�T�C�Y�X�V
		scale.y = 1.0f + 30 * 0.05f;		//�T�C�Y�X�V
		scale.z = 1.0f + 30 * 0.05f;		//�T�C�Y�X�V
	}

	if (time < 40)
	{
		t = 5;
		scale.x = 1.0f + 5 * 0.05f;		//�T�C�Y�X�V
		scale.y = 1.0f + 5 * 0.05f;		//�T�C�Y�X�V
		scale.z = 1.0f + 5 * 0.05f;		//�T�C�Y�X�V
	}


	power = t * MAXPOWER / 120;		//��������
	if (power < 1)					//�ŏ���1�ɂ���
		power = 1.0f;

}

//���@���o�Ă���Ƃ�true �o�Ă��Ȃ��Ƃ�false��Ԃ�
bool MAGIC::Move()
{


	if (eraseOK())
	{
		erase();
	}

	else				//�����Ȃ��Ƃ�����
	{
		if (angle == CHARACTOR::DIST::LEFT)
			pos.x -= move;
		if (angle == CHARACTOR::DIST::RIGHT)
			pos.x += move;
		time++;
	}



	return alive;
}



//�`��
void MAGIC::Render(CAMERA*camera)
{

	if (alive)		//�I�u�W�F�N�g������Ƃ�
	{
		//�I�u�W�F�N�g�X�V
		obj->SetPos(pos + Vector3(0, 10, 0));
		obj->SetAngle((int)angle*PI);
		obj->SetScale(scale);
		obj->Update();

		//�`��
		//obj->Render();
		effect->Update();
		if (obj)
			effect->Render(pos + Vector3(0, 10, 0), power, camera);

	}



}
