#include	"iextreme.h"
#include	"SceneMain\CHARACTOR\PARTNER\Partner.h"
#include	"SceneMain\CHARACTOR\PLAYER\PLAYER.h"
#include	"..\\..\\COLISSION\COLLISION.h"
#include	"Explosion.h"

//***************************************************************************
//
//    �U���^�̎g�����N���X
//
//***************************************************************************
void ATTACK_PARTNER::Initialize(const Vector3& pos)
{
	this->pos = pos;
	move = Vector3(0, 0, 0);
	angle = DIST::RIGHT;
	scale = Vector3(1.5f, 1.5f, -0.75f);
	obj->SetMotion(1);
}

//-----------------------------------------------------------------
//    ���@�X�V
//-----------------------------------------------------------------
void ATTACK_PARTNER::MagicUpdate()
{
	if (magic)
	{
		magic->Move();
		Vector3 v = magic->GetPos() - pos;
		if (v.Length() > 30 || COLLISION::PartnerMagicToPlayer(lpOpponentPlayer, *magic, camera))
		{
			magic->Erase();
			delete magic;
			magic = nullptr;
		}
	}
}

//-----------------------------------------------------------------
//    �X�e�[�g�������U���̂Ƃ������蔻��
//-----------------------------------------------------------------
void ATTACK_PARTNER::RushCollision()
{
	if (lpStateMachine->GetCurrentState() == &rushState)
	{
		//  255�t���[������260�t���[���̊Ԕ�����
		if (obj->GetFrame() >= 255 && obj->GetFrame() <= 260)
		{
			COLLISION::PartnerAttackToPlayer(lpOpponentPlayer, *this, camera);
		}
	}
}

//-----------------------------------------------------------------
//    �U���^�g�����X�V
//-----------------------------------------------------------------
void ATTACK_PARTNER::Update()
{
	//if (pos.y > 0) move.y += GRAVITY;
	if (move.x <= 0.01f&&move.x >= -0.01f)
		move.x = 0;

	if (lpStateMachine->GetCurrentState() != &magicState)
	{
		Vector3 vec = lpAllyPlayer->GetPos() - pos;
		if (vec.x > 0) angle = DIST::RIGHT;
		else angle = DIST::LEFT;
	}


	RushCollision();

	//  ���݂̃X�e�[�g���s
	lpStateMachine->Update();

	MagicUpdate();

	move *= 0.8f;
	pos += move;

	obj->SetPos(pos);
	obj->SetAngle(0, (int)angle*PI, 0);
	if (angle == DIST::RIGHT)
		scale = Vector3(scale.x, scale.y, -0.75f);
	else
		scale = Vector3(scale.x, scale.y, 0.75f);

	obj->SetScale(scale);
	obj->Animation();
	obj->Update();
}

//-----------------------------------------------------------------
//    �U���^�g�����`��
//-----------------------------------------------------------------
void ATTACK_PARTNER::Render(int char_no)
{
#ifdef _DEBUG
	RushCollision();
#endif  //  _DEBUG
	//-----------------------------------------

	iexSystem::Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	if (magic)
	{
#ifdef _DEBUG
		COLLISION::PartnerMagicToPlayer(lpOpponentPlayer, *magic, camera);
#endif  //  _DEBUG
		magic->Render(char_no);
	}
	obj->Render();

	iexSystem::Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//-----------------------------------------------------------------
//    �X�e�[�g�ς��܂�
//-----------------------------------------------------------------
void ATTACK_PARTNER::ChangeState(PARTNER_STATE<ATTACK_PARTNER>* NewState)
{
	lpStateMachine->ChangeState(NewState);
}

void ATTACK_PARTNER::ToLocal()
{
	rideNum = -1;

	for (unsigned int index = 0; index < COLLISION::GetLowStage()->size(); index++)
	{
		if (COLLISION::GetLowStageKindGround(index) == STAGE::KIND_GROUND::MOVING) continue;

		if (COLLISION::ToLocal(index, Vector3(pos.x, pos.y, 0), local, Vector3(0, -1.0f, 0)))
		{
			rideNum = index;
			break;
		}
	}
}

//  UNDNOE:�ǉ�
void ATTACK_PARTNER::ToWorld()
{
	if (rideNum >= 0)
	{
		COLLISION::ToWorld(rideNum, local);
		pos = Vector3(local.x, local.y, pos.z);
	}
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//***************************************************************************
//
//    㩐ݒu�^�̎g�����N���X
//
//***************************************************************************
void TRAP_PARTNER::Initialize(const Vector3& pos)
{
	this->pos = pos;
	move = Vector3(0, 0, 0);
	angle = DIST::RIGHT;
	jumpFlg = false;
	scale = Vector3(1.4f, 1.4f, -0.75f);
	obj->SetMotion(0);
}

//-----------------------------------------------------------------
//    ���@�X�V
//-----------------------------------------------------------------
void TRAP_PARTNER::MagicUpdate()
{
	if (magic)
	{
		magic->Move();
		Vector3 v = magic->GetPos() - pos;
		if (v.Length() > 20)
		{
			magic->Erase();
			delete magic;
			magic = nullptr;
		}
	}
}

//-----------------------------------------------------------------
//    㩂̍X�V
//-----------------------------------------------------------------
void TRAP_PARTNER::TrapUpdate()
{
	for (int i = 0; i < 3; i++)
	{
		if (trap[i])
		{
			trap[i]->Update();
			if (COLLISION::TrapToPlayer(lpOpponentPlayer, trap[i], camera)
				|| trap[i]->GetExpTimer() <= 0)
			{
				for (int j = 0; i < 3; j++)
				{
					if (!explosion[i])
					{
						explosion[i]= new EXPLOSION(trap[i]->GetPos(), camera);
						break;
					}
				}
				trap[i]->Erase();
				delete trap[i];
				trap[i] = nullptr;
			}
		}
		if (explosion[i])
		{
			explosion[i]->Update();
			COLLISION::ExpToPlayer(lpOpponentPlayer, explosion[i]->GetPos(), camera);
		}
	}
}

//-----------------------------------------------------------------
//    㩐ݒu�^�g�����X�V
//-----------------------------------------------------------------
void TRAP_PARTNER::Update(int timer)
{
	time = timer;

	//  y���W���v���C���[�ɍ��킹���g�����̍��W
	Vector3 PlayerVec = Vector3(pos.x, lpAllyPlayer->GetPos().y, pos.z);
	Vector3 Vec = lpAllyPlayer->GetPos() - PlayerVec;

	move.x *= 0.8f;
	pos += move;

	if (pos.y > 0) move.y += GRAVITY;
	if (move.x <= 0.01f&&move.x >= -0.01f) move.x = 0;


	//  �g�����̌����ύX
	if (move.x > 0) angle = DIST::RIGHT;
	else if (move.x < 0) angle = DIST::LEFT;
	else if (move.x == 0)
	{
		if (Vec.x > 0)  angle = DIST::RIGHT;
		if (Vec.x <= 0) angle = DIST::LEFT;
	}

	//  ���݂̃X�e�[�g���s
	lpStateMachine->Update();

	TrapUpdate();

	obj->SetPos(pos);
	obj->SetAngle(0, (int)angle*PI, 0);
	if (angle == DIST::RIGHT)
		scale.z = -0.75f;
	else
		scale.z = 0.75f;
	obj->SetScale(scale);
	obj->Animation();
	obj->Update();
}

//-----------------------------------------------------------------
//    㩐ݒu�^�g�����`��
//-----------------------------------------------------------------
void TRAP_PARTNER::Render(int char_no)
{
	iexSystem::Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	if (magic) magic->Render(char_no);
	for (int i = 0; i < 3; i++)
	{
		if (trap[i])
		{
#ifdef _DEBUG
			
			COLLISION::TrapToPlayer(lpOpponentPlayer, trap[i], camera);

#endif //  _DEBUG
			//  㩂̃X�N���[�����W�Z�o
			Vector3 Output;
			camera->Screentransformation(Output, trap[i]->GetPos());
			trap[i]->Render(Output);
		}

		if (explosion[i])
		{
			explosion[i]->Render();
#ifdef _DEBUG
			COLLISION::ExpToPlayer(lpOpponentPlayer, explosion[i]->GetPos(), camera);
#endif //  _DEBUG

			if (explosion[i]->GetAnimation() > 512)
			{
				delete explosion[i];
				explosion[i] = nullptr;
			}
		}
	}

	obj->Render();
	iexSystem::Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//-----------------------------------------------------------------
//    �X�e�[�g�ς��܂�
//-----------------------------------------------------------------
void TRAP_PARTNER::ChangeState(PARTNER_STATE<TRAP_PARTNER>* NewState)
{
	lpStateMachine->ChangeState(NewState);
}

void TRAP_PARTNER::ToLocal()
{
	rideNum = -1;

	for (unsigned int index = 0; index < COLLISION::GetLowStage()->size(); index++)
	{
		if (COLLISION::ToLocal(index, Vector3(pos.x, pos.y, 0), local, Vector3(0, -1.0f, 0)))
		{
			rideNum = index;
			break;
		}
	}
	for (int i = 0; i < 3; i++)
	{
		if (trap[i])
			trap[i]->ToLocal();
	}

}

void TRAP_PARTNER::ToWorld()
{
	if (rideNum >= 0)
	{
		COLLISION::ToWorld(rideNum, local);
		pos = Vector3(local.x, local.y, pos.z);
		move.y *= 0.8f;
	}
	for (int i = 0; i < 3; i++)
	{
		if (trap[i])
			trap[i]->ToWorld();
	}
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//***************************************************************************
//
//    �N���X�^���̎g�����N���X
//
//***************************************************************************
void CLYSTAL_PARTNER::Initialize(Vector3 pos)
{
	this->pos = pos;
	move = Vector3(0, 0, 0);
	angle = DIST::RIGHT;
	shotTimer = 90;
	shotFlag = false;
	shotCheckFlag = false;
	motionFlag = false;
	magic = nullptr;
	obj->SetMotion(0);
	//magic.reset(new PARTNER_MAGIC());
}

//-----------------------------------------------------------------
//    �N���X�^���g�����X�V
//-----------------------------------------------------------------
void CLYSTAL_PARTNER::Update()
{
	//  ����v���C���[����g�����ւ̃x�N�g��
	Vector3 Vec = lpOpponentPlayer->GetPos() - pos;
	float length = Vec.Length();
	Vec.Normalize();

	if (pos.y > 0) move.y += GRAVITY;

	//  �n�ʔ���
	Vector3 out;
	if (COLLISION::OnGround(0, out, Vector3(pos.x, pos.y, 0), 2.0f))
	{
		pos.y = out.y;
		move.y = .0f;
	}

	//  ����v���C���[�̕��֌����悤�Ɍ����X�V
	if (Vec.x > 0)  angle = DIST::RIGHT;
	if (Vec.x <= 0) angle = DIST::LEFT;

	//  �e������
	Shot();

	move *= 0.8f;
	pos += move;

	obj->SetPos(pos);
	obj->SetAngle(0, (int)angle*PI, 0);
	obj->SetScale(1.0f);
	obj->Animation();
	obj->Update();
}

//-----------------------------------------------------------------
//    �N���X�^���g�����`��
//-----------------------------------------------------------------
void CLYSTAL_PARTNER::Render(int char_no)
{
	D3DCAPS9 caps;
	HRESULT result = iexSystem::Device->GetDeviceCaps(&caps);
	if (FAILED(result))OutputDebugString("Error: �L���v�X�̎擾�Ɏ��s");

	bool shoudAlphaTest = caps.AlphaCmpCaps & D3DPCMPCAPS_GREATEREQUAL;
	if (shoudAlphaTest)
	{
		iexSystem::Device->SetRenderState(D3DRS_ALPHAREF, (DWORD)0x000000A0);
		iexSystem::Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		iexSystem::Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	}

	iexSystem::Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	obj->Render();
	iexSystem::Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	if (magic)
	{
#ifdef _DEBUG
		COLLISION::PartnerMagicToPlayer(lpOpponentPlayer, *magic, camera);
#endif
		magic->Render(char_no);
	}
}

//-----------------------------------------------------------------
//    ���@�U�����ˏ���
//-----------------------------------------------------------------
void CLYSTAL_PARTNER::Shot()
{
	//  ����v���C���[����g�����ւ̃x�N�g��
	Vector3 vec = lpOpponentPlayer->GetPos() - pos;
	float length = vec.Length();
	vec.Normalize();
	if (!shotCheckFlag)
	{
		if (length < 30 && !motionFlag)
		{
			obj->SetMotion(1);
			motionFlag = true;
		}
		if (obj->GetFrame() == 106)
		{
			//magic.reset(new PARTNER_MAGIC());
			//if (angle == DIST::RIGHT) magic->Append(pos + Vector3(2.0f, -2.0f, 0), vec / 2);
			//if (angle == DIST::LEFT) magic->Append(pos + Vector3(-2.0f, -2.0f, 0), vec / 2);
			shotCheckFlag = true;
		}
	}
	else shotTimer--;

	//  ���@�ƃN���X�^���̎g�����Ƃ̋���
	if (magic)
	{
		Vector3 vec2 = magic->GetPos() - pos;
		float length2 = vec2.Length();

		magic->Move();

		//  �e����苗���ȏ�i�ނ�����v���C���[�ɏՓ˂�����e������
		if (length2 > 20 || COLLISION::PartnerMagicToPlayer(lpOpponentPlayer, *magic, camera))
		{
			magic->Erase();
			magic.release();
			magic = nullptr;
		}
	}

	if (shotTimer <= 0)
	{
		shotCheckFlag = false;
		shotTimer = 90;
		motionFlag = false;
	}
}