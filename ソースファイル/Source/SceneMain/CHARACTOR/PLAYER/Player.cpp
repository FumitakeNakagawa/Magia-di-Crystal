#include	"Player.h"
#include	"..\..\\COLISSION\COLLISION.h"



CHARACTOR::CHARACTOR(char* filename) :CHARACTOR()
{
	obj.reset(new iex3DObj(filename));
}



//������
void PLAYER::Initialize(Vector3 pos, int angle, int n, int char_no)
{

	this->pos = pos;
	move = Vector3(0, 0, 0);
	scale = Vector3(1.0f, 1.0f, 1.0f);
	state = STATE::WAIT;
	this->angle = DIST(angle);
	jump_flg = 0;
	shot_flg = 0;
	no = NO(n);
	magicPower = 0.0f;
	life = MAXLIFE;
	Char = char_no;

	//���@����
	magic.reset(new MAGIC(Char));

	//���G�t�F�N�g�p�摜
	//particle.reset(new iex2DObj("DATA\\PARTICLE.PNG"));

	//�G�t�F�N�g�`���[�W������
	charge.reset(new CHARGE());
	charge->ChargeInitialize();

	_effect.reset(new _EFFECT());
	_effect->Initialize();

	obj->SetMotion(0);
	obj->SetPos(pos);
	obj->SetAngle(0, (int)angle*PI, 0);
	if (this->angle == DIST::RIGHT)
		scale = Vector3(scale.x, scale.y, -1.0f);
	else
		scale = Vector3(scale.x, scale.y, 1.0f);
	obj->SetScale(scale);
	obj->Update();
}

//������
void PLAYER::Respawn(Vector3 pos, int angle)
{
	this->pos = pos;
	move = Vector3(0, 0, 0);
	state = STATE::WAIT;
	this->angle = DIST(angle);
	jump_flg = 0;
	shot_flg = 0;
	magicPower = 0.0f;
	magic_timer = 0;
	life = MAXLIFE;
	inv_timer = 120;
	state = STATE::INVINCIBLE;
	obj->SetMotion(0);
	obj->Update();
}



void PLAYER::Wait()
{
	SetMotion(0);
	state = STATE::MOVE;

}

//����
void PLAYER::Control()
{
	float axisX, axisY;
	axisX = KEY_GetAxisX((int)no)*0.01f;
	axisY = KEY_GetAxisY((int)no)*0.01f;

	if (KEY_Get(KEY_RIGHT, (int)no) == 3 || axisX > 3)
	{
		DIST ang = angle;
		//SetMotion(3);
		while (ang == DIST::LEFT)
		{
			if (_effect->Getsmoke_flg() == false && move.y == 0)
				_effect->Setsmoke_flg(true);
			/*scale.x -= 0.1f;
			if (scale.x <= 0.0f)*/
			scale.x *= -1;
			ang = DIST::RIGHT;
		}

	}

	if (KEY_Get(KEY_LEFT, (int)no) == 3 || axisX < -3)
	{
		DIST ang = angle;
		//SetMotion(3);
		while (ang == DIST::RIGHT)
		{
			if (_effect->Getsmoke_flg() == false && move.y == 0)
				_effect->Setsmoke_flg(true);
			/*scale.x -= 0.1f;
			if (scale.x <= 0.0f)*/
			scale.x *= -1;
			ang = DIST::LEFT;
		}
	}

	if (KEY_Get(KEY_RIGHT, (int)no))	//�E
	{
		angle = DIST::RIGHT;


		if (move.x < MAXSPEED)
		{
			move.x += 0.1f;
		}

	}

	else if (axisX > 3)
	{
		angle = DIST::RIGHT;

		if (move.x < MAXSPEED)
		{
			move.x += axisX*0.01f;
		}

	}






	if (KEY_Get(KEY_LEFT, (int)no))	//��
	{

		angle = DIST::LEFT;

		if (move.x > -MAXSPEED)
		{
			move.x -= 0.1f;
		}

	}
	else if (axisX < -3)
	{

		angle = DIST::LEFT;

		if (move.x > -MAXSPEED)
		{
			move.x += axisX*0.01f;
		}

	}





	if (state <= STATE::INVINCIBLE)
	{
		if (KEY_Get(KEY_A, (int)no) == 3)	//�~�{�^��
		{
			//state = STATE::JUMP;
			SetMotion(1);
			if (jump_flg < 1)
			{
				move.y = jump_power;
				jump_flg++;
			}
		}
	}



	if ((move.x > 0.1 || move.x < -0.1) && state < STATE::INVINCIBLE)
	{
		SetMotion(3);
	}



	if (axisX < 3.0 && axisX > -3.0 && (!KEY_Get(KEY_RIGHT, (int)no) && !KEY_Get(KEY_LEFT, (int)no)))
	{
		if (state < STATE::JUMP)
		{
			if (obj->GetMotion() == 3)
				SetMotion(0);
		}
	}

	//�N���X�^���l��
	if (KEY_Get(KEY_B, (int)no) == 3 && state <= STATE::INVINCIBLE)
	{
		crystalhold = true;

		state = STATE::HOLD;
	}



}




//�U��
void PLAYER::Attack()
{

	//if (state != STATE::HOLD)
	{
		if (KEY_Get(KEY_C, (int)no) == 1 && shot_flg <= 1)	//���������Ă��
		{

			if (magic_timer < 120)							//�ő嗭�ߎ��ԁi90�t���[���i1.5�b�j�j
				magic_timer++;

			shot_flg = 1;
			magic->Charge(magic_timer);						//���@�`���[�W

															//  �G�t�F�N�g�ʂ̕ύX

															//�`���[�W�G�t�F�N�g�X�V
			_effect->Charge_Update();
			_effect->Setcharge_flg(true);
			charge->ChargeUpdate(magic_timer);
		}

		if (KEY_Get(KEY_C, (int)no) == 2)					//���𗣂�
		{
			magic->Append(pos, angle);					//���@�o��
			shot_flg = 2;
			SetMotion(4);
			obj->SetFrame(136);
			_effect->Setcharge_flg(false);
		}
	}

	if (shot_flg == 2)									//���@�o����
	{
		if (!magic->Move())								//���@���o�Ă��邩
		{
			shot_flg = 0;
			magic_timer = 0;
		}
	}


}




//�_���[�W���󂯂�
void PLAYER::Damage(float power, PLAYER * p)
{

	life -= power;							//�p���[���̗̓}�C�i�X
	magic_timer = 0;						//�`���[�W���L�����Z��
	SetMotion(5);

	if (p->GetmagicPower() >= 2) {
		if (p->GetmagicAngle() == DIST::RIGHT)
		{
			move.x = p->GetmagicPower() / 2.0f;
		}

		else
		{
			move.x = -p->GetmagicPower() / 2.0f;
		}
	}

	p->SetmagicPower(0);					//���@�̃p���[��߂�
											//�z�[���h���L�����Z��
	Sethold(false);


	if (life <= 0)							//���񂾂�
	{
		state = STATE::DEATH;				//���S������
		return;
	}

	else
	{
		inv_timer = 45;					//30�t���[���i0.5�b�j���G	
		state = STATE::INVINCIBLE;
	}

}

//���񂾂�
void PLAYER::Dead()
{
	SetMotion(6);

	if (magic->GetAlive())
		magic->erase();

	static int time = 90;				//90�t���[���i1.5�b�j�d��
	time--;

	if (time <= 0)
	{
		state = STATE::DEAD;
		time = 90;
	}
}

void PLAYER::Invincible(int t)
{
	static int time = 0;			//�����h���h�t���[�����G
	time++;

	if (time > t)
	{
		state = STATE::WAIT;
		time = 0;
	}
}

void PLAYER::Update()
{
	obj->Update();
}


void PLAYER::UPdate(PLAYER * p)
{
	switch (state) {
	case STATE::WAIT:		//�ҋ@
		Wait();
		break;

	case STATE::MOVE:		//�ړ�
		break;

	case STATE::JUMP:		//�W�����v
		break;

	case STATE::INVINCIBLE:	//���G���
		Invincible(inv_timer);
		break;

	case STATE::DAMAGE:		//�_���[�W���󂯂�
		Damage(p->GetmagicPower(), p);
		break;

	case STATE::DEATH:		//���C�t0
		Dead();
		break;

	case STATE::HOLD:
		SetMotion(8);
		if (KEY_Get(KEY_B, (int)no) == 2)
		{
			crystalhold = false;
			if (obj->GetMotion() == 8)
				SetMotion(9);
			state = STATE::MOVE;
		}
		break;


	}

	if (state < STATE::HOLD || state == STATE::INVINCIBLE)
		Control();
	if (state < STATE::DAMAGE)
		Attack();

	//�d�͉��Z
	//move.y += GRAVITY;
	move.y += GRAVITY;
	//�����x�}�C�i�X
	move.x *= 0.8f;

	//�ړ�
	pos += move;

	if (move.y != 0)
	{
		jump_flg = 1;
	}
	else
	{
		state = STATE::WAIT;
	}

	if (move.y > 0)
	{
		SetMotion(1);
	}






	//��ʒ[����
	if (pos.x > 58)
		pos.x = 58;
	if (pos.x < -58)
		pos.x = -58;


	//���@���蔻��
	magicPower = COLLISION::PlayerMagic(p, magic.get());
	if (magicPower && p->GetMode() != STATE::INVINCIBLE) {
		GetMagic()->erase();//���@�폜

		p->Setstate(STATE::DAMAGE);
		_effect->Setmagic_Vanish_flg(true);
	}



	if (scale.x < 1.0f)
	{
		scale.x = 1.0;
	}

	//�I�u�W�F�N�g�X�V
	obj->SetPos(pos);
	obj->SetAngle(0, (int)angle*PI, 0);
	if (angle == DIST::RIGHT)
		scale = Vector3(scale.x, scale.y, -1.0f);
	else
		scale = Vector3(scale.x, scale.y, 1.0f);
	obj->SetScale(scale);
	obj->Animation();
	obj->Update();



	_effect->Smoke_Update();
	_effect->magic_Vanish_Update(p->GetPos() + Vector3(-5.0f, 10.0f, 0.0f));
}

void PLAYER::Render(CAMERA* camera)
{
	//�I�u�W�F�N�g�X�V
	obj->SetPos(pos);
	obj->SetAngle(0, (int)angle* PI, 0);
	obj->SetScale(scale);
	obj->Update();





	D3DCAPS9 caps;
	HRESULT result = iexSystem::Device->GetDeviceCaps(&caps);
	if (FAILED(result))OutputDebugString("Error: �L���v�X�̎擾�Ɏ��s");

	unsigned long shoudAlphaTest = caps.AlphaCmpCaps & D3DPCMPCAPS_GREATEREQUAL;
	if (shoudAlphaTest)
	{
		iexSystem::Device->SetRenderState(D3DRS_ALPHAREF, (DWORD)0x000000A0);
		iexSystem::Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		iexSystem::Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	}

	//�`��
	iexSystem::Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);	//�J�����O�I�t
	if (state == STATE::INVINCIBLE)				//���G���_��
	{
		static int time = 0;
		time++;
		if (time % 6 == 0)
		{
			obj->Render(/*shader3D.get(),"toon"*/);
		}
	}

	else if (state == STATE::DEATH)				//���S���_�Łi���G���Ԋu�������j
	{
		static int time = 0;
		time++;
		if (time % 4 == 0)
		{
			obj->Render();

		}
	}
	else
		obj->Render();



	magic->Render(camera);				//���@�`��
	if (shot_flg == 1 /*&& state < STATE::DAMAGE && state != STATE::HOLD*/)
		//charge->ChargeRender(GetPos() + Vector3(0, 6, 0), camera);
		_effect->Charge_Render(GetPos() + Vector3(-9.0f, 14.0f, 0), camera);

	_effect->Smoke_Render(pos + Vector3(-6.5f + ((int)angle * 4), 4.5f, 0.0f), camera);
	_effect->magic_Vanish_Render(camera, Char);

#ifdef _DEBUG
	char	str[64];
	sprintf(str, "magictime %d", (int)magic_timer);
	IEX_DrawText(str, 10, 100, 100, 50, 0xFFFFFF00);
#endif // _DEBUG

}

int PLAYER::Result()
{
	if (pos.x < -20)
	{
		pos.x++;
		angle = DIST::RIGHT;
		SetMotion(3);
	}
	if (pos.x > 20)
	{
		pos.x--;
		angle = DIST::LEFT;
		SetMotion(3);
	}

	state = STATE::MOVE;
	obj->SetPos(pos);
	obj->SetAngle(0, (int)angle*PI, 0);
	if (angle == DIST::RIGHT)
		scale = Vector3(1.0f, 1.0f, -1.0f);
	else
		scale = Vector3(1.0f, 1.0f, 1.0f);
	obj->SetScale(scale);
	obj->Animation();
	obj->Update();

	if (pos.x >= -20 && pos.x <= 20)
	{
		return 1;
	}
	return 0;
}

void PLAYER::ResultRender()
{
	//�I�u�W�F�N�g�X�V
	obj->SetPos(pos);
	obj->SetAngle(0, (int)angle* PI, 0);
	obj->SetScale(scale);
	obj->Update();





	D3DCAPS9 caps;
	HRESULT result = iexSystem::Device->GetDeviceCaps(&caps);
	if (FAILED(result))OutputDebugString("Error: �L���v�X�̎擾�Ɏ��s");

	unsigned long shoudAlphaTest = caps.AlphaCmpCaps & D3DPCMPCAPS_GREATEREQUAL;
	if (shoudAlphaTest)
	{
		iexSystem::Device->SetRenderState(D3DRS_ALPHAREF, (DWORD)0x000000A0);
		iexSystem::Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		iexSystem::Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	}

	//�`��
	iexSystem::Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);	//�J�����O�I�t

	obj->Render(/*shader3D.get(),"toon"*/);


}






