#include	"Crystal.h"
#include	"SceneMain\CHARACTOR\PLAYER\PLAYER.h"

int CRYSTAL::holdCnt[2];
iexMesh* CRYSTAL::obj = nullptr;

//-----------------------------------------------------------------
//
//		初期化・コンストラクタ
//
//-----------------------------------------------------------------
CRYSTAL::CRYSTAL()
{
	pos = Vector3(0, 0, 0);
	rot = Vector3(0, 0, 0);
	scale = Vector3(1, 1, 1);
	holdGage = 0.0f;
	min = -180;
	max = 180;
	shader.reset(new iexShader("DATA\\Shader\\3DEx.fx"));
}

void CRYSTAL::Initialize(int ParentNum)
{
	clone = obj->Clone();
	pos = Vector3(0, 0, 0);
	rot = Vector3(0, PI, 0);
	scale = Vector3(0.75f, 1.0f, 1.0f);
	holdGage = 0;
	isHold = -1;
	parentNum = ParentNum;
	charge = false;
	state = STATE::NONE;


	shader.reset(new iexShader("DATA\\Shader\\3DEx.fx"));
	effect.reset(new _EFFECT);
	effect->Initialize();
	shader->SetValue("uv_top", 0.5f);

	lost.flg = false;
	lost.step = 0;

	boost = 1.0f;
}

void CRYSTAL::Release()
{
	if (clone)
	{
		delete clone;
		clone = nullptr;
	}
}


//------------------------------------------------------------------------
//
//		クリスタルの非占領化
//
//-------------------------------------------------------------------------
void CRYSTAL::LostCrystal(int no)
{
	switch (lost.step)
	{
	case 0:
		if (isHold > -1)
		{
			isHold = -1;
			if (holdCnt[no])	holdCnt[no]--;
		}
		lost.flg = true;
		lost.step++;
		state = STATE::LOST;
		lost.number = no;

	case 1:
		switch (lost.number)
		{
		case 0:
			for (int i = 0; i < 3; i++)
			{
				if (holdGage > 0.0f)
					MinusGage(3, -1);
				else
					holdGage = 0.0f;
			}
			break;
		case 1:
			for (int i = 0; i < 3; i++)
				if (holdGage < 0.0f)
					PlusGage(3, -1);
				else
					holdGage = 0.0f;
		default:
			break;
		}
		if (holdGage == 0.0f)
		{
			lost.flg = false;
			lost.step = 0;
			state = STATE::NONE;
		}

	}
}

//------------------------------------------------------------------
//
//		更新
//
//-----------------------------------------------------------------
void CRYSTAL::Update()
{
#ifdef  _DEBUG
	if (GetAsyncKeyState('I') & 0x8000)boost += 0.1f;
	if (GetAsyncKeyState('O') & 0x8000)boost -= 0.1f;
#endif //  _DEBUG



	if (lost.flg)
		LostCrystal(-1);

	shader->SetValue("uv_height", 0.5f);
	shader->SetValue("uv_width", 0.5f);

	effect->hold_Update(isHold);


	clone->SetPos(pos);
	clone->SetAngle(rot);
	clone->SetScale(scale);
	clone->Update();
}





//--------------------------------------------------------------
//
//		描画
//
//--------------------------------------------------------------
void CRYSTAL::Render(CAMERA* camera, int char_no1, int char_no2)
{
	clone->SetPos(pos);
	clone->SetAngle(rot);
	clone->SetScale(scale);
	clone->Update();
	clone->Render(shader.get(), "uv_anime");

	if (state == STATE::CHARGE)
		effect->hold_Render(pos + Vector3(-8.0f, +13.0f, 0.0f), camera, isHold, char_no1, char_no2);


#ifdef _DEBUG
	char str[32];
	sprintf(str, "boost = %.1f", boost);
	IEX_DrawText(str, 10, 350, 100, 100, 0xFFFFFFFF);

#endif
}

void CRYSTAL::Render()
{
	clone->SetPos(pos);
	clone->SetAngle(rot);
	clone->SetScale(scale);
	clone->Update();
	clone->Render(shader.get(), "uv_anime");
}


void CRYSTAL::ResultRender()
{
	if (isHold == -1)
	{
		shader->SetValue("uv_top", 0.5f);
	}
	clone->SetPos(pos);
	clone->SetAngle(rot);
	clone->SetScale(scale);
	clone->Update();


	clone->Render(shader.get(), "uv_anime");

}
void CRYSTAL::PlusGage(int speed, int no)
{
	charge = true;

	holdGage += (speed*boost);
	float plus = 0.5f / max;

	float top;
	if (holdGage < 0.0f)
		top = 0.5f - holdGage*plus;
	else
		top = 0.5f + holdGage*plus;

	//MAXまで溜まった時
	if (holdGage >= max)
	{
		holdGage = max;
		top = 0.9999f;
		holdCnt[(int)PLAYER::NO::P1]++;
		isHold = (int)PLAYER::NO::P1;
	}
	if (holdGage > 0 && no == 0)
		shader->SetValue("uv_left", 0.5f);
	if (holdGage > 0 && no == 1)
		shader->SetValue("uv_left", 0.0f);

	shader->SetValue("uv_top", top);
}

void CRYSTAL::MinusGage(int speed, int no)
{
	charge = true;
	holdGage -= (speed*boost);
	float plus = 0.5f / (min*-1.0f);

	float top;
	if (holdGage > 0.0f)
		top = 0.5f - (holdGage*-1.0f)*plus;
	else
		top = 0.5f + (holdGage*-1.0f)*plus;

	if (holdGage <= min)
	{
		holdGage = min;
		top = 0.9999f;
		holdCnt[(int)PLAYER::NO::P2]++;
		isHold = (int)PLAYER::NO::P2;
	}
	if (holdGage < 0 && no == 0)
		shader->SetValue("uv_left", 0.5f);
	if (holdGage < 0 && no == 1)
		shader->SetValue("uv_left", 0.0f);

	shader->SetValue("uv_top", top);
}
