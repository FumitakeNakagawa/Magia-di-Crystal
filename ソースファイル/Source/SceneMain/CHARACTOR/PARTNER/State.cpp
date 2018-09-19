#include	"State.h"
#include	"SceneMain\CHARACTOR\PARTNER\Partner.h"
#include	"SceneMain\CHARACTOR\Character.h"
#include	"SceneMain\COLISSION\COLLISION.h"

//*****************************************************************
//
//  �U���^�̎g�����̃v���C���[��Ǐ]����N���X
//
//*****************************************************************
//-----------------------------------------------------------------
//    �Ȃ񂩃X�e�[�g�ɓ������u�Ԃɂ�肽�����Ƃ���������E�E�E
//-----------------------------------------------------------------
void TRACKING_A::Enter(ATTACK_PARTNER* lpPartner)
{
	lpPartner->GetObj()->SetMotion(1);
	lpPartner->SetMove(Vector3(0, 0, 0));
}

//-----------------------------------------------------------------
//    �X�e�[�g���s�֐�
//-----------------------------------------------------------------
void TRACKING_A::Execute(ATTACK_PARTNER* lpPartner)
{
	//  �v���C���[�Ǝg�����Ƃ̃x�N�g��
	//  UNDONE:z�l�ύX
	Vector3 vec = (lpAllyPlayer->GetPos() + Vector3(0, 5, 10.0f)) - PARTNER_POS;
	float d = vec.Length();
	vec.Normalize();

	move += vec / 20;

	if (move.x >= 0.5f) move.x = 0.5f;
	if (move.x <= -0.5f) move.x = -0.5f;
	if (move.y >= 0.5f) move.y = 0.5f;
	if (move.y <= -0.5f) move.y = -0.5f;

	lpPartner->SetMove(move);

	//  �v���C���[�Ƃ̋��������l�ȉ��Ȃ�X�e�[�g��ς���
	if (d <= 10)
	{
		lpPartner->ChangeState(lpPartner->GetWait());
	}
}

//-----------------------------------------------------------------
//    �Ȃ񂩃X�e�[�g�ł�u�Ԃɂ�邱�Ƃ���������E�E�E
//-----------------------------------------------------------------
void TRACKING_A::Exit(ATTACK_PARTNER* lpPartner)
{
	move = Vector3(0, 0, 0);
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//*****************************************************************
//
//  �U���^�̎g�����̑ҋ@�N���X
//
//*****************************************************************
//-----------------------------------------------------------------
//    �Ȃ񂩃X�e�[�g�ɓ������u�Ԃɂ�肽�����Ƃ���������E�E�E
//-----------------------------------------------------------------
void WAIT_A::Enter(ATTACK_PARTNER* lpPartner)
{
}

//-----------------------------------------------------------------
//    �X�e�[�g���s�֐�
//-----------------------------------------------------------------
void WAIT_A::Execute(ATTACK_PARTNER* lpPartner)
{
	//  �����v���C���[�Ǝg�����Ƃ̃x�N�g��
	Vector3 vec = (lpAllyPlayer->GetPos() + Vector3(0, 5, 0)) - PARTNER_POS;
	float d = vec.Length();

	//  �g�����������藎������
	if (flyFlag)  fly += 0.02;
	if (!flyFlag) fly -= 0.01;

	if (fly > 0.3f) flyFlag = false;
	if (fly < -0.3f) flyFlag = true;

	lpPartner->SetMoveY(fly);

	if (d > 15.0f) lpPartner->ChangeState(lpPartner->GetTracking());
}

//-----------------------------------------------------------------
//    �Ȃ񂩃X�e�[�g�ł�u�Ԃɂ�邱�Ƃ���������E�E�E
//-----------------------------------------------------------------
void WAIT_A::Exit(ATTACK_PARTNER* lpPartner)
{
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//*****************************************************************
//
//  �U���^�̎g�����̑���v���C���[��ǐՂ���N���X
//
//*****************************************************************
//-----------------------------------------------------------------
//    �Ȃ񂩃X�e�[�g�ɓ������u�Ԃɂ�肽�����Ƃ���������E�E�E
//-----------------------------------------------------------------
void CHASE_A::Enter(ATTACK_PARTNER* lpPartner)
{
	//  �O�̂��߁E�E�E
	move = Vector3(0, 0, 0);

	lpPartner->GetObj()->SetMotion(1);

}

//-----------------------------------------------------------------
//    �X�e�[�g���s�֐�
//-----------------------------------------------------------------
void CHASE_A::Execute(ATTACK_PARTNER* lpPartner)
{
	//  ����v���C���[�Ǝg�����Ƃ̃x�N�g��
	Vector3 vec = (lpOpponentPlayer->GetPos() + Vector3(0, 5, PARTNER_POS.z)) - PARTNER_POS;
	float d = vec.Length();
	vec.Normalize();

	move += vec / 8;

	if (move.x >= CHASE_MOVE_MAX)  move.x = CHASE_MOVE_MAX;
	if (move.x <= -CHASE_MOVE_MAX) move.x = -CHASE_MOVE_MAX;
	if (move.y >= CHASE_MOVE_MAX)  move.y = CHASE_MOVE_MAX;
	if (move.y <= -CHASE_MOVE_MAX) move.y = -CHASE_MOVE_MAX;

	lpPartner->SetMove(move);

	//  �v���C���[�Ƃ̋��������l�ȉ��Ȃ�X�e�[�g��ς���
	//if (d <= 10)
	//{
	//	lpPartner->ChangeState(lpPartner->GetRush());
	//}
}

//-----------------------------------------------------------------
//    �Ȃ񂩃X�e�[�g�ł�u�Ԃɂ�邱�Ƃ���������E�E�E
//-----------------------------------------------------------------
void CHASE_A::Exit(ATTACK_PARTNER* lpPartner)
{
	move = Vector3(0, 0, 0);
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//*****************************************************************
//
//  �U���^�̎g�����̃v���C���[��T������N���X
//
//*****************************************************************
//-----------------------------------------------------------------
//    �Ȃ񂩃X�e�[�g�ɓ������u�Ԃɂ�肽�����Ƃ���������E�E�E
//-----------------------------------------------------------------
void SEARCH::Enter(ATTACK_PARTNER* lpPartner)
{
}

//-----------------------------------------------------------------
//    �X�e�[�g���s�֐�
//-----------------------------------------------------------------
void SEARCH::Execute(ATTACK_PARTNER* lpPartner)
{
}

//-----------------------------------------------------------------
//    �Ȃ񂩃X�e�[�g�ł�u�Ԃɂ�邱�Ƃ���������E�E�E
//-----------------------------------------------------------------
void SEARCH::Exit(ATTACK_PARTNER* lpPartner)
{
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//*****************************************************************
//
//  �U���^�̎g�����̕����U������N���X
//
//*****************************************************************
//-----------------------------------------------------------------
//    �Ȃ񂩃X�e�[�g�ɓ������u�Ԃɂ�肽�����Ƃ���������E�E�E
//-----------------------------------------------------------------
void RUSH_A::Enter(ATTACK_PARTNER* lpPartner)
{
	//  �O�̂��߁E�E�E
	attackTimer = 150;

	Vector3 v = lpOpponentPlayer->GetPos() - lpPartner->GetPos();
	//  ����v���C���[�̕�������
	(v.x >= 0) ? lpPartner->SetAngle(RIGHT) :
		lpPartner->SetAngle(LEFT);

	lpPartner->GetObj()->SetMotion(4);
}

//-----------------------------------------------------------------
//    �X�e�[�g���s�֐�
//-----------------------------------------------------------------
void RUSH_A::Execute(ATTACK_PARTNER* lpPartner)
{
	attackTimer--;

	if (attackTimer <= 0)
	{
		lpPartner->ChangeState(lpPartner->GetTracking());
	}
}

//-----------------------------------------------------------------
//    �Ȃ񂩃X�e�[�g�ł�u�Ԃɂ�邱�Ƃ���������E�E�E
//-----------------------------------------------------------------
void RUSH_A::Exit(ATTACK_PARTNER* lpPartner)
{
	attackTimer = 150;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//*****************************************************************
//
//  �U���^�̎g�����̖��@�U������N���X
//
//*****************************************************************
//-----------------------------------------------------------------
//    �Ȃ񂩃X�e�[�g�ɓ������u�Ԃɂ�肽�����Ƃ���������E�E�E
//-----------------------------------------------------------------
void MAGIC_A::Enter(ATTACK_PARTNER* lpPartner)
{
	Vector3 v = lpOpponentPlayer->GetPos() - lpPartner->GetPos();

	magicTimer = 150;
	shotFlag = false;

	lpPartner->GetObj()->SetMotion(2);

	//  ����v���C���[�̕�������
	(v.x >= 0) ? lpPartner->SetAngle(RIGHT) :
		lpPartner->SetAngle(LEFT);
}

//-----------------------------------------------------------------
//    �X�e�[�g���s�֐�
//-----------------------------------------------------------------
void MAGIC_A::Execute(ATTACK_PARTNER* lpPartner)
{
	magicTimer--;

	Vector3 vec = lpOpponentPlayer->GetPos() - lpPartner->GetPos();
	vec.Normalize();
	vec *= 0.5f;

	if (!lpPartner->GetMagic() && !shotFlag&&lpPartner->GetObj()->GetFrame() == 121)
	{
		//lpPartner->SetMagic(new PARTNER_MAGIC());
		if (lpPartner->GetAngle() == CHARACTOR::DIST::RIGHT)
		{
			lpPartner->SetMagicPosMove(lpPartner->GetPos() + Vector3(2, -5.2, 0), vec);
		}
		else if (lpPartner->GetAngle() == CHARACTOR::DIST::LEFT)
		{
			lpPartner->SetMagicPosMove(lpPartner->GetPos() + Vector3(-2, -5.2, 0), vec);
		}

		shotFlag = true;
	}

	if (magicTimer <= 0)
	{
		lpPartner->ChangeState(lpPartner->GetTracking());
	}
}

//-----------------------------------------------------------------
//    �Ȃ񂩃X�e�[�g�ł�u�Ԃɂ�邱�Ƃ���������E�E�E
//-----------------------------------------------------------------
void MAGIC_A::Exit(ATTACK_PARTNER* lpPartner)
{
	magicTimer = 150;
	shotFlag = false;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//*****************************************************************
//
//  �U���^�̎g�����̃O���[�o���X�e�[�g�N���X
//
//*****************************************************************
void GLOBAL_STATE_A::Execute(ATTACK_PARTNER* lpPartner)
{
	//  ����v���C���[�Ǝg�����Ƃ̃x�N�g��
	Vector3 vec = (lpOpponentPlayer->GetPos() + Vector3(0, 5, 0)) - PARTNER_POS;
	float d = vec.Length();

	vec.Normalize();

	if (d >= 25 && d <= 30)
	{
		Vector3 v = lpOpponentPlayer->GetPos() - lpPartner->GetPos();
		//  ����v���C���[�̕�������
		(v.x >= 0) ? lpPartner->SetAngle(RIGHT) :
			lpPartner->SetAngle(LEFT);
	}
	//-----------------------------------------------

	Vector3 front0 = (PARTNER_POS + Vector3(1.0f, 0, 0)) - PARTNER_POS;
	Vector3 front1 = (PARTNER_POS + Vector3(-1.0f, 0, 0)) - PARTNER_POS;

	float l, rad;

	if (lpPartner->GetAngle() == CHARACTOR::DIST::LEFT) l = Vector3Dot(vec, front0);
	if (lpPartner->GetAngle() == CHARACTOR::DIST::RIGHT) l = Vector3Dot(vec, front1);

	rad = acos(l);

	//  �g�����Ƒ���v���C���[�̋��������ȉ��ɂȂ�ƕ����U���X�e�[�g�Ɉڂ�
	if (d <= 10 && lpPartner->GetFSM()->GetCurrentState() != lpPartner->GetRush())
	{
		lpPartner->GetFSM()->ChangeState(lpPartner->GetRush());
	}

	if (rad < 2.0f) return;

	//  �g�����Ƒ���v���C���[�̋��������ȉ��ɂȂ��
	//  �X�e�[�g�����ł��낤�Ɩ��@�U���X�e�[�g�Ɉڂ�
	if (d >= 25 && d <= 30)
	{
		if (lpPartner->GetFSM()->GetCurrentState() != lpPartner->GetMagicState())
		{
			lpPartner->GetFSM()->ChangeState(lpPartner->GetMagicState());
		}
	}
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//*****************************************************************
//
//  㩐ݒu�^�g�����̃v���C���[��Ǐ]����N���X
//
//*****************************************************************
//-----------------------------------------------------------------
//    �Ȃ񂩃X�e�[�g�ɓ������u�Ԃɂ�肽�����Ƃ���������E�E�E
//-----------------------------------------------------------------
void TRACKING_T::Enter(TRAP_PARTNER* lpPartner)
{
	lpPartner->GetObj()->SetMotion(1);
}

//-----------------------------------------------------------------
//    �X�e�[�g���s�֐�
//-----------------------------------------------------------------
void TRACKING_T::Execute(TRAP_PARTNER* lpPartner)
{
	//  y���W���g�����ɍ��킹���v���C���[�̍��W
	Vector3 PlayerPos2 = Vector3(PLAYER_POS.x, PARTNER_POS.y, PLAYER_POS.z);

	//  �v���C���[(PlayerPos2)�Ǝg�����Ƃ̋����p�̃x�N�g��
	//  UNDONE:��������Ƃ����ύX
	Vector3 vec = PlayerPos2 + Vector3(0, 0, 10.0f) - PARTNER_POS;
	float d = sqrtf(vec.x*vec.x + vec.y*vec.y);
	vec.Normalize();

	//  ����(x)�����l�𒴂��Ă����牡�ړ�
	if (d >= 10.0f)
	{
		lpPartner->SetMoveX(vec.x / 2);
		if (lpPartner->GetObj()->GetMotion() != 1)
			lpPartner->GetObj()->SetMotion(1);
	}
	else if (d < 9 && lpPartner->GetObj()->GetMotion() != 0)
	{
		lpPartner->GetObj()->SetMotion(0);
	}

	//  �v���C���[���g�������20�����ʒu�ɂ���
	//  �g�������W�����v���Ă��Ȃ�������W�����v�t���O���Ă�
	if (PLAYER_POS.y - PARTNER_POS.y> 15 &&
		!lpPartner->GetJumpFlg())
	{
		lpPartner->SetJumpFlg(true);
	}

	//  �g�����̃W�����v�t���O�����Ă����ړ�����
	if (lpPartner->GetJumpFlg())
	{
		lpPartner->SetMoveY(1.0f);
	}

	//  �g�����̂ق����v���C���[���-5�����Ȃ�����W�����v�t���O�܂�
	if (PARTNER_POS.y - PLAYER_POS.y > -5&& lpPartner->GetJumpFlg())
	{
		lpPartner->SetMoveX(vec.x*0.8f);
		if (d <= 2.0f&&d >= -2.0f)
		{
			lpPartner->SetMoveX(0);
		}
		lpPartner->SetJumpFlg(false);
	}

	//  �v���C���[�Ƃ̋��������l��������X�e�[�g��ς���
	Vector3 vec2 = PLAYER_POS - PARTNER_POS;
	float l = vec2.Length();

	//  UNDONE:���l�ύX
	if (l >= 50)
	{
		lpPartner->SetJumpFlg(false);
		//lpPartner->ChangeState(lpPartner->GetSearch());
		lpPartner->ChangeState(lpPartner->GetWaitState());
	}
}

//-----------------------------------------------------------------
//    �Ȃ񂩃X�e�[�g�ł�u�Ԃɂ�邱�Ƃ���������E�E�E
//-----------------------------------------------------------------
void TRACKING_T::Exit(TRAP_PARTNER* lpPartner)
{

}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//*****************************************************************
//
//  㩐ݒu�^�g�����̃v���C���[��T������N���X
//
//*****************************************************************
//-----------------------------------------------------------------
//    �Ȃ񂩃X�e�[�g�ɓ������u�Ԃɂ�肽�����Ƃ���������E�E�E
//-----------------------------------------------------------------
//void SEARCH_T::Enter(TRAP_PARTNER* lpPartner)
//{
//	flagCheckFlag = NO_CHECK;
//	lpPartner->GetObj()->SetMotion(0);
//}

//-----------------------------------------------------------------
//    �X�e�[�g���s�֐�
//-----------------------------------------------------------------
//void SEARCH_T::Execute(TRAP_PARTNER* lpPartner)
//{
//	//  �v���C���[�Ǝg�����Ƃ̃x�N�g��
//	Vector3 vec = PLAYER_POS - PARTNER_POS;
//
//	//  ���ɈӖ��̂Ȃ��ϐ�
//	//  ���C�s�b�N�̈����p�ɂƂ肠�����E�E�E
//	Vector3 out;
//
//	//  �g���������ɒ������u�ԍ��E����t���O���Ă�
//	if (COLLISION::OnGround(out, PARTNER_POS, 2.0f)
//		&& flagCheckFlag == NO_CHECK)
//	{
//		if (PARTNER_POS.x < PLAYER_POS.x) LR_Flag = RIGHT;
//		else  LR_Flag = LEFT;
//		flagCheckFlag = CHECK;
//	}
//
//	//if (!COLLISION::OnGround(out, PARTNER_POS, 2.0f))
//	if (COLLISION::OnGround(out, Vector3(PARTNER_POS.x, PARTNER_POS.y, 0), 2.0f))
//	{
//		flagCheckFlag = NO_CHECK;
//	}
//
//	if (PARTNER_POS.y - PLAYER_POS.y<0) lpPartner->SetJumpFlg(true);
//
//	if (lpPartner->GetJumpFlg())
//	{
//		if (PARTNER_POS.x < PLAYER_POS.x) LR_Flag = RIGHT;
//		else  LR_Flag = LEFT;
//		lpPartner->SetMoveY(1.0f);
//	}
//	if (PARTNER_POS.y - PLAYER_POS.y <= 0) lpPartner->SetJumpFlg(false);
//
//	if (LR_Flag == RIGHT) lpPartner->SetMoveX(0.6f);
//	if (LR_Flag == LEFT)  lpPartner->SetMoveX(-0.6f);
//
//	//  �v���C���[�Ǝg�����̋��������l�ȉ��Ȃ�,����
//	//  �v���C���[�Ǝg�����̍������قړ����ɂȂ�����Ǐ]�X�e�[�g�ɕύX
//	float l = vec.Length();
//	if (l <= 10 && (PARTNER_POS.y <= PLAYER_POS.y + 5.0f && PARTNER_POS.y >= PLAYER_POS.y - 5.0f))
//		lpPartner->ChangeState(lpPartner->GetTracking());
//}

//-----------------------------------------------------------------
//    �Ȃ񂩃X�e�[�g�ł�u�Ԃɂ�邱�Ƃ���������E�E�E
//-----------------------------------------------------------------
//void SEARCH_T::Exit(TRAP_PARTNER* lpPartner)
//{
//
//}

//*****************************************************************
//
//  㩐ݒu�^�g�����̑ҋ@�N���X
//
//*****************************************************************
//-----------------------------------------------------------------
//    �Ȃ񂩃X�e�[�g�ɓ������u�Ԃɂ�肽�����Ƃ���������E�E�E
//-----------------------------------------------------------------
void WAIT_T::Enter(TRAP_PARTNER* lpPartner)
{
	lpPartner->GetObj()->SetMotion(0);
}

//-----------------------------------------------------------------
//    �X�e�[�g���s�֐�
//-----------------------------------------------------------------
void WAIT_T::Execute(TRAP_PARTNER* lpPartner)
{
	//  �v���C���[�Ǝg�����Ƃ̃x�N�g��
	Vector3 vec = PLAYER_POS - PARTNER_POS;

	float l = vec.Length();

	//  UNDONE:�l�ύX
	if (l <= 10 && (PARTNER_POS.y <= PLAYER_POS.y + 8.0f && PARTNER_POS.y >= PLAYER_POS.y - 8.0f))
		lpPartner->ChangeState(lpPartner->GetTracking());
}

//-----------------------------------------------------------------
//    �Ȃ񂩃X�e�[�g�ł�u�Ԃɂ�邱�Ƃ���������E�E�E
//-----------------------------------------------------------------
void WAIT_T::Exit(TRAP_PARTNER* lpPartner)
{
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//*****************************************************************
//
//  㩐ݒu�^�g�����̖��@�U������N���X
//
//*****************************************************************
//-----------------------------------------------------------------
//    �Ȃ񂩃X�e�[�g�ɓ������u�Ԃɂ�肽�����Ƃ���������E�E�E
//-----------------------------------------------------------------
void MAGIC_T::Enter(TRAP_PARTNER* lpPartner)
{
	magicTimer = 120;
	shotFlag = false;
}
//-----------------------------------------------------------------
//    �X�e�[�g���s�֐�
//-----------------------------------------------------------------
void MAGIC_T::Execute(TRAP_PARTNER* lpPartner)
{
	magicTimer--;

	Vector3 vec = lpOpponentPlayer->GetPos() - lpPartner->GetPos();
	vec.Normalize();
	vec *= 0.5f;

	if (!lpPartner->GetMagic() && !shotFlag)
	{
		//lpPartner->SetMagic(new PARTNER_MAGIC());
		//lpPartner->SetMagicPosMove(lpPartner->GetPos(), vec);
		shotFlag = true;
	}

	if (magicTimer <= 0)
	{
		lpPartner->GetFSM()->ChangeState(lpPartner->GetTracking());
	}
}

//-----------------------------------------------------------------
//    �Ȃ񂩃X�e�[�g�ł�u�Ԃɂ�邱�Ƃ���������E�E�E
//-----------------------------------------------------------------
void MAGIC_T::Exit(TRAP_PARTNER* lpPartner)
{
	magicTimer = 120;
	shotFlag = false;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//*****************************************************************
//
//  㩐ݒu�^�g������㩐ݒu����N���X
//
//*****************************************************************
//-----------------------------------------------------------------
//    �Ȃ񂩃X�e�[�g�ɓ������u�Ԃɂ�肽�����Ƃ���������E�E�E
//-----------------------------------------------------------------
void TRAP_T::Enter(TRAP_PARTNER* lpPartner)
{

}

//-----------------------------------------------------------------
//    �X�e�[�g���s�֐�
//-----------------------------------------------------------------
void TRAP_T::Execute(TRAP_PARTNER* lpPartner)
{
	for (int i = 0; i < 3; i++)
	{
		if (!lpPartner->GetTrap(i))
		{
			lpPartner->SetTrap(i);
			break;
		}
	}

	//  㩓�������O�̃X�e�[�g�ɖ߂�
	lpPartner->GetFSM()->ChangePreviousState();
}

//-----------------------------------------------------------------
//    �Ȃ񂩃X�e�[�g�ł�u�Ԃɂ�邱�Ƃ���������E�E�E
//-----------------------------------------------------------------
void TRAP_T::Exit(TRAP_PARTNER* lpPartner)
{

}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//*****************************************************************
//
//  㩐ݒu�^�g�����̃O���[�o���X�e�[�g�N���X
//
//*****************************************************************
//-----------------------------------------------------------------
//    �X�e�[�g���s�֐�
//-----------------------------------------------------------------
void GLOBAL_STATE_T::Execute(TRAP_PARTNER* lpPartner)
{
	//  ����v���C���[�Ǝg�����Ƃ̃x�N�g��
	Vector3 vec = (lpOpponentPlayer->GetPos() + Vector3(0, 5, 0)) - PARTNER_POS;
	float d = vec.Length();

	//  㩔r�o���ԃ`�F�b�N�p
	int checkTime = lpPartner->GetTimer() % 5;

	//  �X�e�[�g�ύX����
	//  10�b�Ɉ��㩔r�o���܂�
	if (lpPartner->GetFSM()->GetCurrentState() != lpPartner->GetTrapState())
	{
		if (lpPartner->GetTimer() != 120 && checkTime == 0 && !trapFlag)
		{
			lpPartner->ChangeState(lpPartner->GetTrapState());
			trapFlag = true;
		}
	}

	if (checkTime == 1) trapFlag = false;

	//  ����v���C���[�Ǝg�����Ƃ̋��������ȉ��ɂȂ�����
	//  �X�e�[�g�𖂖@�U���X�e�[�g�ɕς���
	//if (d <= 10 && lpPartner->GetFSM()->GetCurrentState() != lpPartner->GetMagicState())
	//{
	//	lpPartner->ChangeState(lpPartner->GetMagicState());
	//}
}