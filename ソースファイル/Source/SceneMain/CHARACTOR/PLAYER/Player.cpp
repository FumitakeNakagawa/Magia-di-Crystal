#include	"Player.h"
#include	"..\..\\COLISSION\COLLISION.h"



CHARACTOR::CHARACTOR(char* filename) :CHARACTOR()
{
	obj.reset(new iex3DObj(filename));
}



//初期化
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

	//魔法生成
	magic.reset(new MAGIC(Char));

	//仮エフェクト用画像
	//particle.reset(new iex2DObj("DATA\\PARTICLE.PNG"));

	//エフェクトチャージ初期化
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

//復活時
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

//操作
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

	if (KEY_Get(KEY_RIGHT, (int)no))	//右
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






	if (KEY_Get(KEY_LEFT, (int)no))	//左
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
		if (KEY_Get(KEY_A, (int)no) == 3)	//×ボタン
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

	//クリスタル獲得
	if (KEY_Get(KEY_B, (int)no) == 3 && state <= STATE::INVINCIBLE)
	{
		crystalhold = true;

		state = STATE::HOLD;
	}



}




//攻撃
void PLAYER::Attack()
{

	//if (state != STATE::HOLD)
	{
		if (KEY_Get(KEY_C, (int)no) == 1 && shot_flg <= 1)	//□を押してる間
		{

			if (magic_timer < 120)							//最大溜め時間（90フレーム（1.5秒））
				magic_timer++;

			shot_flg = 1;
			magic->Charge(magic_timer);						//魔法チャージ

															//  エフェクト量の変更

															//チャージエフェクト更新
			_effect->Charge_Update();
			_effect->Setcharge_flg(true);
			charge->ChargeUpdate(magic_timer);
		}

		if (KEY_Get(KEY_C, (int)no) == 2)					//□を離す
		{
			magic->Append(pos, angle);					//魔法出現
			shot_flg = 2;
			SetMotion(4);
			obj->SetFrame(136);
			_effect->Setcharge_flg(false);
		}
	}

	if (shot_flg == 2)									//魔法出現中
	{
		if (!magic->Move())								//魔法が出ているか
		{
			shot_flg = 0;
			magic_timer = 0;
		}
	}


}




//ダメージを受けた
void PLAYER::Damage(float power, PLAYER * p)
{

	life -= power;							//パワー分体力マイナス
	magic_timer = 0;						//チャージをキャンセル
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

	p->SetmagicPower(0);					//魔法のパワーを戻す
											//ホールドをキャンセル
	Sethold(false);


	if (life <= 0)							//死んだら
	{
		state = STATE::DEATH;				//死亡処理へ
		return;
	}

	else
	{
		inv_timer = 45;					//30フレーム（0.5秒）無敵	
		state = STATE::INVINCIBLE;
	}

}

//死んだら
void PLAYER::Dead()
{
	SetMotion(6);

	if (magic->GetAlive())
		magic->erase();

	static int time = 90;				//90フレーム（1.5秒）硬直
	time--;

	if (time <= 0)
	{
		state = STATE::DEAD;
		time = 90;
	}
}

void PLAYER::Invincible(int t)
{
	static int time = 0;			//引数”ｔ”フレーム無敵
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
	case STATE::WAIT:		//待機
		Wait();
		break;

	case STATE::MOVE:		//移動
		break;

	case STATE::JUMP:		//ジャンプ
		break;

	case STATE::INVINCIBLE:	//無敵状態
		Invincible(inv_timer);
		break;

	case STATE::DAMAGE:		//ダメージを受ける
		Damage(p->GetmagicPower(), p);
		break;

	case STATE::DEATH:		//ライフ0
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

	//重力加算
	//move.y += GRAVITY;
	move.y += GRAVITY;
	//加速度マイナス
	move.x *= 0.8f;

	//移動
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






	//画面端判定
	if (pos.x > 58)
		pos.x = 58;
	if (pos.x < -58)
		pos.x = -58;


	//魔法当り判定
	magicPower = COLLISION::PlayerMagic(p, magic.get());
	if (magicPower && p->GetMode() != STATE::INVINCIBLE) {
		GetMagic()->erase();//魔法削除

		p->Setstate(STATE::DAMAGE);
		_effect->Setmagic_Vanish_flg(true);
	}



	if (scale.x < 1.0f)
	{
		scale.x = 1.0;
	}

	//オブジェクト更新
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
	//オブジェクト更新
	obj->SetPos(pos);
	obj->SetAngle(0, (int)angle* PI, 0);
	obj->SetScale(scale);
	obj->Update();





	D3DCAPS9 caps;
	HRESULT result = iexSystem::Device->GetDeviceCaps(&caps);
	if (FAILED(result))OutputDebugString("Error: キャプスの取得に失敗");

	unsigned long shoudAlphaTest = caps.AlphaCmpCaps & D3DPCMPCAPS_GREATEREQUAL;
	if (shoudAlphaTest)
	{
		iexSystem::Device->SetRenderState(D3DRS_ALPHAREF, (DWORD)0x000000A0);
		iexSystem::Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		iexSystem::Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	}

	//描画
	iexSystem::Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);	//カリングオフ
	if (state == STATE::INVINCIBLE)				//無敵中点滅
	{
		static int time = 0;
		time++;
		if (time % 6 == 0)
		{
			obj->Render(/*shader3D.get(),"toon"*/);
		}
	}

	else if (state == STATE::DEATH)				//死亡時点滅（無敵より間隔が早い）
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



	magic->Render(camera);				//魔法描画
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
	//オブジェクト更新
	obj->SetPos(pos);
	obj->SetAngle(0, (int)angle* PI, 0);
	obj->SetScale(scale);
	obj->Update();





	D3DCAPS9 caps;
	HRESULT result = iexSystem::Device->GetDeviceCaps(&caps);
	if (FAILED(result))OutputDebugString("Error: キャプスの取得に失敗");

	unsigned long shoudAlphaTest = caps.AlphaCmpCaps & D3DPCMPCAPS_GREATEREQUAL;
	if (shoudAlphaTest)
	{
		iexSystem::Device->SetRenderState(D3DRS_ALPHAREF, (DWORD)0x000000A0);
		iexSystem::Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		iexSystem::Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	}

	//描画
	iexSystem::Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);	//カリングオフ

	obj->Render(/*shader3D.get(),"toon"*/);


}






