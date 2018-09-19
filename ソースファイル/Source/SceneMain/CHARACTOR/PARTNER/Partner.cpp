#include	"iextreme.h"
#include	"SceneMain\CHARACTOR\PARTNER\Partner.h"
#include	"SceneMain\CHARACTOR\PLAYER\PLAYER.h"
#include	"..\\..\\COLISSION\COLLISION.h"
#include	"Explosion.h"

//***************************************************************************
//
//    攻撃型の使い魔クラス
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
//    魔法更新
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
//    ステートが物理攻撃のとき当たり判定
//-----------------------------------------------------------------
void ATTACK_PARTNER::RushCollision()
{
	if (lpStateMachine->GetCurrentState() == &rushState)
	{
		//  255フレームから260フレームの間判定取る
		if (obj->GetFrame() >= 255 && obj->GetFrame() <= 260)
		{
			COLLISION::PartnerAttackToPlayer(lpOpponentPlayer, *this, camera);
		}
	}
}

//-----------------------------------------------------------------
//    攻撃型使い魔更新
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

	//  現在のステート実行
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
//    攻撃型使い魔描画
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
//    ステート変えます
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

//  UNDNOE:追加
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
//    罠設置型の使い魔クラス
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
//    魔法更新
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
//    罠の更新
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
//    罠設置型使い魔更新
//-----------------------------------------------------------------
void TRAP_PARTNER::Update(int timer)
{
	time = timer;

	//  y座標をプレイヤーに合わせた使い魔の座標
	Vector3 PlayerVec = Vector3(pos.x, lpAllyPlayer->GetPos().y, pos.z);
	Vector3 Vec = lpAllyPlayer->GetPos() - PlayerVec;

	move.x *= 0.8f;
	pos += move;

	if (pos.y > 0) move.y += GRAVITY;
	if (move.x <= 0.01f&&move.x >= -0.01f) move.x = 0;


	//  使い魔の向き変更
	if (move.x > 0) angle = DIST::RIGHT;
	else if (move.x < 0) angle = DIST::LEFT;
	else if (move.x == 0)
	{
		if (Vec.x > 0)  angle = DIST::RIGHT;
		if (Vec.x <= 0) angle = DIST::LEFT;
	}

	//  現在のステート実行
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
//    罠設置型使い魔描画
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
			//  罠のスクリーン座標算出
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
//    ステート変えます
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
//    クリスタルの使い魔クラス
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
//    クリスタル使い魔更新
//-----------------------------------------------------------------
void CLYSTAL_PARTNER::Update()
{
	//  相手プレイヤーから使い魔へのベクトル
	Vector3 Vec = lpOpponentPlayer->GetPos() - pos;
	float length = Vec.Length();
	Vec.Normalize();

	if (pos.y > 0) move.y += GRAVITY;

	//  地面判定
	Vector3 out;
	if (COLLISION::OnGround(0, out, Vector3(pos.x, pos.y, 0), 2.0f))
	{
		pos.y = out.y;
		move.y = .0f;
	}

	//  相手プレイヤーの方へ向くように向き更新
	if (Vec.x > 0)  angle = DIST::RIGHT;
	if (Vec.x <= 0) angle = DIST::LEFT;

	//  弾を撃つ
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
//    クリスタル使い魔描画
//-----------------------------------------------------------------
void CLYSTAL_PARTNER::Render(int char_no)
{
	D3DCAPS9 caps;
	HRESULT result = iexSystem::Device->GetDeviceCaps(&caps);
	if (FAILED(result))OutputDebugString("Error: キャプスの取得に失敗");

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
//    魔法攻撃発射処理
//-----------------------------------------------------------------
void CLYSTAL_PARTNER::Shot()
{
	//  相手プレイヤーから使い魔へのベクトル
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

	//  魔法とクリスタルの使い魔との距離
	if (magic)
	{
		Vector3 vec2 = magic->GetPos() - pos;
		float length2 = vec2.Length();

		magic->Move();

		//  弾が一定距離以上進むか相手プレイヤーに衝突したら弾を消す
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