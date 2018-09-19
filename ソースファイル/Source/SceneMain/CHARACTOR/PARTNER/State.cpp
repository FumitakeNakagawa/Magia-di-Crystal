#include	"State.h"
#include	"SceneMain\CHARACTOR\PARTNER\Partner.h"
#include	"SceneMain\CHARACTOR\Character.h"
#include	"SceneMain\COLISSION\COLLISION.h"

//*****************************************************************
//
//  攻撃型の使い魔のプレイヤーを追従するクラス
//
//*****************************************************************
//-----------------------------------------------------------------
//    なんかステートに入った瞬間にやりたいことかあったら・・・
//-----------------------------------------------------------------
void TRACKING_A::Enter(ATTACK_PARTNER* lpPartner)
{
	lpPartner->GetObj()->SetMotion(1);
	lpPartner->SetMove(Vector3(0, 0, 0));
}

//-----------------------------------------------------------------
//    ステート実行関数
//-----------------------------------------------------------------
void TRACKING_A::Execute(ATTACK_PARTNER* lpPartner)
{
	//  プレイヤーと使い魔とのベクトル
	//  UNDONE:z値変更
	Vector3 vec = (lpAllyPlayer->GetPos() + Vector3(0, 5, 10.0f)) - PARTNER_POS;
	float d = vec.Length();
	vec.Normalize();

	move += vec / 20;

	if (move.x >= 0.5f) move.x = 0.5f;
	if (move.x <= -0.5f) move.x = -0.5f;
	if (move.y >= 0.5f) move.y = 0.5f;
	if (move.y <= -0.5f) move.y = -0.5f;

	lpPartner->SetMove(move);

	//  プレイヤーとの距離が一定値以下ならステートを変える
	if (d <= 10)
	{
		lpPartner->ChangeState(lpPartner->GetWait());
	}
}

//-----------------------------------------------------------------
//    なんかステートでる瞬間にやることがあったら・・・
//-----------------------------------------------------------------
void TRACKING_A::Exit(ATTACK_PARTNER* lpPartner)
{
	move = Vector3(0, 0, 0);
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//*****************************************************************
//
//  攻撃型の使い魔の待機クラス
//
//*****************************************************************
//-----------------------------------------------------------------
//    なんかステートに入った瞬間にやりたいことかあったら・・・
//-----------------------------------------------------------------
void WAIT_A::Enter(ATTACK_PARTNER* lpPartner)
{
}

//-----------------------------------------------------------------
//    ステート実行関数
//-----------------------------------------------------------------
void WAIT_A::Execute(ATTACK_PARTNER* lpPartner)
{
	//  味方プレイヤーと使い魔とのベクトル
	Vector3 vec = (lpAllyPlayer->GetPos() + Vector3(0, 5, 0)) - PARTNER_POS;
	float d = vec.Length();

	//  使い魔浮いたり落ちたり
	if (flyFlag)  fly += 0.02;
	if (!flyFlag) fly -= 0.01;

	if (fly > 0.3f) flyFlag = false;
	if (fly < -0.3f) flyFlag = true;

	lpPartner->SetMoveY(fly);

	if (d > 15.0f) lpPartner->ChangeState(lpPartner->GetTracking());
}

//-----------------------------------------------------------------
//    なんかステートでる瞬間にやることがあったら・・・
//-----------------------------------------------------------------
void WAIT_A::Exit(ATTACK_PARTNER* lpPartner)
{
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//*****************************************************************
//
//  攻撃型の使い魔の相手プレイヤーを追跡するクラス
//
//*****************************************************************
//-----------------------------------------------------------------
//    なんかステートに入った瞬間にやりたいことかあったら・・・
//-----------------------------------------------------------------
void CHASE_A::Enter(ATTACK_PARTNER* lpPartner)
{
	//  念のため・・・
	move = Vector3(0, 0, 0);

	lpPartner->GetObj()->SetMotion(1);

}

//-----------------------------------------------------------------
//    ステート実行関数
//-----------------------------------------------------------------
void CHASE_A::Execute(ATTACK_PARTNER* lpPartner)
{
	//  相手プレイヤーと使い魔とのベクトル
	Vector3 vec = (lpOpponentPlayer->GetPos() + Vector3(0, 5, PARTNER_POS.z)) - PARTNER_POS;
	float d = vec.Length();
	vec.Normalize();

	move += vec / 8;

	if (move.x >= CHASE_MOVE_MAX)  move.x = CHASE_MOVE_MAX;
	if (move.x <= -CHASE_MOVE_MAX) move.x = -CHASE_MOVE_MAX;
	if (move.y >= CHASE_MOVE_MAX)  move.y = CHASE_MOVE_MAX;
	if (move.y <= -CHASE_MOVE_MAX) move.y = -CHASE_MOVE_MAX;

	lpPartner->SetMove(move);

	//  プレイヤーとの距離が一定値以下ならステートを変える
	//if (d <= 10)
	//{
	//	lpPartner->ChangeState(lpPartner->GetRush());
	//}
}

//-----------------------------------------------------------------
//    なんかステートでる瞬間にやることがあったら・・・
//-----------------------------------------------------------------
void CHASE_A::Exit(ATTACK_PARTNER* lpPartner)
{
	move = Vector3(0, 0, 0);
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//*****************************************************************
//
//  攻撃型の使い魔のプレイヤーを探索するクラス
//
//*****************************************************************
//-----------------------------------------------------------------
//    なんかステートに入った瞬間にやりたいことかあったら・・・
//-----------------------------------------------------------------
void SEARCH::Enter(ATTACK_PARTNER* lpPartner)
{
}

//-----------------------------------------------------------------
//    ステート実行関数
//-----------------------------------------------------------------
void SEARCH::Execute(ATTACK_PARTNER* lpPartner)
{
}

//-----------------------------------------------------------------
//    なんかステートでる瞬間にやることがあったら・・・
//-----------------------------------------------------------------
void SEARCH::Exit(ATTACK_PARTNER* lpPartner)
{
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//*****************************************************************
//
//  攻撃型の使い魔の物理攻撃するクラス
//
//*****************************************************************
//-----------------------------------------------------------------
//    なんかステートに入った瞬間にやりたいことかあったら・・・
//-----------------------------------------------------------------
void RUSH_A::Enter(ATTACK_PARTNER* lpPartner)
{
	//  念のため・・・
	attackTimer = 150;

	Vector3 v = lpOpponentPlayer->GetPos() - lpPartner->GetPos();
	//  相手プレイヤーの方を向く
	(v.x >= 0) ? lpPartner->SetAngle(RIGHT) :
		lpPartner->SetAngle(LEFT);

	lpPartner->GetObj()->SetMotion(4);
}

//-----------------------------------------------------------------
//    ステート実行関数
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
//    なんかステートでる瞬間にやることがあったら・・・
//-----------------------------------------------------------------
void RUSH_A::Exit(ATTACK_PARTNER* lpPartner)
{
	attackTimer = 150;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//*****************************************************************
//
//  攻撃型の使い魔の魔法攻撃するクラス
//
//*****************************************************************
//-----------------------------------------------------------------
//    なんかステートに入った瞬間にやりたいことかあったら・・・
//-----------------------------------------------------------------
void MAGIC_A::Enter(ATTACK_PARTNER* lpPartner)
{
	Vector3 v = lpOpponentPlayer->GetPos() - lpPartner->GetPos();

	magicTimer = 150;
	shotFlag = false;

	lpPartner->GetObj()->SetMotion(2);

	//  相手プレイヤーの方を向く
	(v.x >= 0) ? lpPartner->SetAngle(RIGHT) :
		lpPartner->SetAngle(LEFT);
}

//-----------------------------------------------------------------
//    ステート実行関数
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
//    なんかステートでる瞬間にやることがあったら・・・
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
//  攻撃型の使い魔のグローバルステートクラス
//
//*****************************************************************
void GLOBAL_STATE_A::Execute(ATTACK_PARTNER* lpPartner)
{
	//  相手プレイヤーと使い魔とのベクトル
	Vector3 vec = (lpOpponentPlayer->GetPos() + Vector3(0, 5, 0)) - PARTNER_POS;
	float d = vec.Length();

	vec.Normalize();

	if (d >= 25 && d <= 30)
	{
		Vector3 v = lpOpponentPlayer->GetPos() - lpPartner->GetPos();
		//  相手プレイヤーの方を向く
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

	//  使い魔と相手プレイヤーの距離が一定以下になると物理攻撃ステートに移る
	if (d <= 10 && lpPartner->GetFSM()->GetCurrentState() != lpPartner->GetRush())
	{
		lpPartner->GetFSM()->ChangeState(lpPartner->GetRush());
	}

	if (rad < 2.0f) return;

	//  使い魔と相手プレイヤーの距離が一定以下になると
	//  ステートが何であろうと魔法攻撃ステートに移る
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
//  罠設置型使い魔のプレイヤーを追従するクラス
//
//*****************************************************************
//-----------------------------------------------------------------
//    なんかステートに入った瞬間にやりたいことかあったら・・・
//-----------------------------------------------------------------
void TRACKING_T::Enter(TRAP_PARTNER* lpPartner)
{
	lpPartner->GetObj()->SetMotion(1);
}

//-----------------------------------------------------------------
//    ステート実行関数
//-----------------------------------------------------------------
void TRACKING_T::Execute(TRAP_PARTNER* lpPartner)
{
	//  y座標を使い魔に合わせたプレイヤーの座標
	Vector3 PlayerPos2 = Vector3(PLAYER_POS.x, PARTNER_POS.y, PLAYER_POS.z);

	//  プレイヤー(PlayerPos2)と使い魔との距離用のベクトル
	//  UNDONE:↓ちょっとだけ変更
	Vector3 vec = PlayerPos2 + Vector3(0, 0, 10.0f) - PARTNER_POS;
	float d = sqrtf(vec.x*vec.x + vec.y*vec.y);
	vec.Normalize();

	//  距離(x)が一定値を超えていたら横移動
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

	//  プレイヤーが使い魔より20高い位置にいて
	//  使い魔がジャンプしていなかったらジャンプフラグ立てる
	if (PLAYER_POS.y - PARTNER_POS.y> 15 &&
		!lpPartner->GetJumpFlg())
	{
		lpPartner->SetJumpFlg(true);
	}

	//  使い魔のジャンプフラグ立ってたら上移動する
	if (lpPartner->GetJumpFlg())
	{
		lpPartner->SetMoveY(1.0f);
	}

	//  使い魔のほうがプレイヤーより-5高くなったらジャンプフラグ折る
	if (PARTNER_POS.y - PLAYER_POS.y > -5&& lpPartner->GetJumpFlg())
	{
		lpPartner->SetMoveX(vec.x*0.8f);
		if (d <= 2.0f&&d >= -2.0f)
		{
			lpPartner->SetMoveX(0);
		}
		lpPartner->SetJumpFlg(false);
	}

	//  プレイヤーとの距離が一定値超えたらステートを変える
	Vector3 vec2 = PLAYER_POS - PARTNER_POS;
	float l = vec2.Length();

	//  UNDONE:数値変更
	if (l >= 50)
	{
		lpPartner->SetJumpFlg(false);
		//lpPartner->ChangeState(lpPartner->GetSearch());
		lpPartner->ChangeState(lpPartner->GetWaitState());
	}
}

//-----------------------------------------------------------------
//    なんかステートでる瞬間にやることがあったら・・・
//-----------------------------------------------------------------
void TRACKING_T::Exit(TRAP_PARTNER* lpPartner)
{

}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//*****************************************************************
//
//  罠設置型使い魔のプレイヤーを探索するクラス
//
//*****************************************************************
//-----------------------------------------------------------------
//    なんかステートに入った瞬間にやりたいことかあったら・・・
//-----------------------------------------------------------------
//void SEARCH_T::Enter(TRAP_PARTNER* lpPartner)
//{
//	flagCheckFlag = NO_CHECK;
//	lpPartner->GetObj()->SetMotion(0);
//}

//-----------------------------------------------------------------
//    ステート実行関数
//-----------------------------------------------------------------
//void SEARCH_T::Execute(TRAP_PARTNER* lpPartner)
//{
//	//  プレイヤーと使い魔とのベクトル
//	Vector3 vec = PLAYER_POS - PARTNER_POS;
//
//	//  特に意味のない変数
//	//  レイピックの引数用にとりあえず・・・
//	Vector3 out;
//
//	//  使い魔が床に着いた瞬間左右判定フラグ建てる
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
//	//  プレイヤーと使い魔の距離が一定値以下なる,且つ
//	//  プレイヤーと使い魔の高さがほぼ同じになったら追従ステートに変更
//	float l = vec.Length();
//	if (l <= 10 && (PARTNER_POS.y <= PLAYER_POS.y + 5.0f && PARTNER_POS.y >= PLAYER_POS.y - 5.0f))
//		lpPartner->ChangeState(lpPartner->GetTracking());
//}

//-----------------------------------------------------------------
//    なんかステートでる瞬間にやることがあったら・・・
//-----------------------------------------------------------------
//void SEARCH_T::Exit(TRAP_PARTNER* lpPartner)
//{
//
//}

//*****************************************************************
//
//  罠設置型使い魔の待機クラス
//
//*****************************************************************
//-----------------------------------------------------------------
//    なんかステートに入った瞬間にやりたいことかあったら・・・
//-----------------------------------------------------------------
void WAIT_T::Enter(TRAP_PARTNER* lpPartner)
{
	lpPartner->GetObj()->SetMotion(0);
}

//-----------------------------------------------------------------
//    ステート実行関数
//-----------------------------------------------------------------
void WAIT_T::Execute(TRAP_PARTNER* lpPartner)
{
	//  プレイヤーと使い魔とのベクトル
	Vector3 vec = PLAYER_POS - PARTNER_POS;

	float l = vec.Length();

	//  UNDONE:値変更
	if (l <= 10 && (PARTNER_POS.y <= PLAYER_POS.y + 8.0f && PARTNER_POS.y >= PLAYER_POS.y - 8.0f))
		lpPartner->ChangeState(lpPartner->GetTracking());
}

//-----------------------------------------------------------------
//    なんかステートでる瞬間にやることがあったら・・・
//-----------------------------------------------------------------
void WAIT_T::Exit(TRAP_PARTNER* lpPartner)
{
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//*****************************************************************
//
//  罠設置型使い魔の魔法攻撃するクラス
//
//*****************************************************************
//-----------------------------------------------------------------
//    なんかステートに入った瞬間にやりたいことかあったら・・・
//-----------------------------------------------------------------
void MAGIC_T::Enter(TRAP_PARTNER* lpPartner)
{
	magicTimer = 120;
	shotFlag = false;
}
//-----------------------------------------------------------------
//    ステート実行関数
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
//    なんかステートでる瞬間にやることがあったら・・・
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
//  罠設置型使い魔の罠設置するクラス
//
//*****************************************************************
//-----------------------------------------------------------------
//    なんかステートに入った瞬間にやりたいことかあったら・・・
//-----------------------------------------------------------------
void TRAP_T::Enter(TRAP_PARTNER* lpPartner)
{

}

//-----------------------------------------------------------------
//    ステート実行関数
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

	//  罠投げたら前のステートに戻る
	lpPartner->GetFSM()->ChangePreviousState();
}

//-----------------------------------------------------------------
//    なんかステートでる瞬間にやることがあったら・・・
//-----------------------------------------------------------------
void TRAP_T::Exit(TRAP_PARTNER* lpPartner)
{

}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//*****************************************************************
//
//  罠設置型使い魔のグローバルステートクラス
//
//*****************************************************************
//-----------------------------------------------------------------
//    ステート実行関数
//-----------------------------------------------------------------
void GLOBAL_STATE_T::Execute(TRAP_PARTNER* lpPartner)
{
	//  相手プレイヤーと使い魔とのベクトル
	Vector3 vec = (lpOpponentPlayer->GetPos() + Vector3(0, 5, 0)) - PARTNER_POS;
	float d = vec.Length();

	//  罠排出時間チェック用
	int checkTime = lpPartner->GetTimer() % 5;

	//  ステート変更処理
	//  10秒に一回罠排出します
	if (lpPartner->GetFSM()->GetCurrentState() != lpPartner->GetTrapState())
	{
		if (lpPartner->GetTimer() != 120 && checkTime == 0 && !trapFlag)
		{
			lpPartner->ChangeState(lpPartner->GetTrapState());
			trapFlag = true;
		}
	}

	if (checkTime == 1) trapFlag = false;

	//  相手プレイヤーと使い魔との距離が一定以下になったら
	//  ステートを魔法攻撃ステートに変える
	//if (d <= 10 && lpPartner->GetFSM()->GetCurrentState() != lpPartner->GetMagicState())
	//{
	//	lpPartner->ChangeState(lpPartner->GetMagicState());
	//}
}