#include	"SceneSelect.h"
#include	"SYSTEM\Framework.h"
#include	"SceneLoad\Load.h"
#include	"SceneMain\SceneMain.h"

int cnt[2];

int controller_no[2];
int partner_no[2];
int char1;
int char2;
char* stageName;

SCENE_SELECT_CHAR::~SCENE_SELECT_CHAR()
{

}

bool SCENE_SELECT_CHAR::Initialize()
{
	iexLight::SetAmbient(0xFFFFFFFF);
	iexLight::SetFog(100, 200, 0xFFFFFFFF);


	bg.reset(new iex2DObj("DATA\\IMAGE\\SELECT\\bg.png"));
	Char[0].reset(new iex3DObj("DATA\\MODEL\\CHR\\PLAYER\\pl1\\PLAYER1.IEM"));
	Char[1].reset(new iex3DObj("DATA\\MODEL\\CHR\\PLAYER\\pl2\\PLAYER2.IEM"));
	choseIcon.reset(new iex2DObj("DATA\\IMAGE\\UI\\icon.png"));
	decide.reset(new iex2DObj("DATA\\IMAGE\\SELECT\\Decide.png"));

	view.reset(new iexView());
	//
	//
	for (int i = 0; i < 2; i++)
	{
		controller_no[i] = i;
		cnt[i] = 1;
	}

	Char[0]->SetPos(Vector3(-40, -15, 20));
	Char[0]->SetAngle(Vector3(0, 0, 0));
	Char[0]->SetScale(Vector3(1.5, 1.5, -1));
	Char[0]->SetMotion(0);
	Char[0]->Update();

	Char[1]->SetPos(Vector3(40, -15, 20));
	Char[1]->SetAngle(Vector3(0, PI, 0));
	Char[1]->SetScale(Vector3(1.5, 1.5, 1));
	Char[1]->SetMotion(0);
	Char[1]->Update();



	iconX[0] = 200;
	iconX[1] = 200;
	iconY = 250;

	char1 = 0;
	char2 = 0;
	frame = 0;
	de1 = de2 = false;
	return true;
}

void SCENE_SELECT_CHAR::Update()
{
	/*if (cnt[0] + cnt[1] < 1)
	{
	if (KEY_Get(KEY_SELECT, 0) == 3)
	{
	cnt[0] = cnt[1] + 1;
	controller_no[0] = cnt[0];
	}

	if (KEY_Get(KEY_SELECT, 1) == 3)
	{
	cnt[1] = cnt[0] + 1;
	controller_no[1] = cnt[1];
	}
	}*/


	if (cnt[0] + cnt[1] >= 1)
	{
		if (KEY_Get(KEY_RIGHT, controller_no[0]) == 3 && char1 <= 0 && cnt[controller_no[0]] < 2 && cnt[controller_no[1]] < 2)
		{
			char1 = 1;
		}

		if (KEY_Get(KEY_LEFT, controller_no[0]) == 3 && char1 > 0 && cnt[controller_no[0]] < 2 && cnt[controller_no[1]] < 2)
		{
			char1 = 0;
		}

		iconX[controller_no[0]] = 200 + char1 * 1175;

		if (KEY_Get(KEY_B, controller_no[0]) == 3 && cnt[controller_no[0]] < 2)
		{
			Char[char1]->SetMotion(10);
			cnt[controller_no[0]] = 2;
			if (char1 == 0)
			{
				char2 = 1;
				x1 = 100;
			}
			else
			{
				char2 = 0;
				x1 = 1600;
			}
			//cnt[controller_no[1]] = 2;
			frame = 0;
			de1 = true;
		}
		if (KEY_Get(KEY_A, controller_no[0]) == 3 && cnt[controller_no[0]] == 2)
		{
			Char[char1]->SetMotion(0);
			cnt[controller_no[0]] = 1;
			de1 = false;
		}




		if (KEY_Get(KEY_RIGHT, controller_no[1]) == 3 && char2 <= 0 && cnt[controller_no[1]] < 2 && cnt[controller_no[0]] < 2)
		{
			char2 = 1;
		}

		if (KEY_Get(KEY_LEFT, controller_no[1]) == 3 && char2 > 0 && cnt[controller_no[1]] < 2 && cnt[controller_no[0]] < 2)
		{
			char2 = 0;
		}

		iconX[controller_no[1]] = 200 + char2 * 1175;

		if (KEY_Get(KEY_B, controller_no[1]) == 3 && cnt[controller_no[1]] < 2)
		{
			Char[char2]->SetMotion(10);
			cnt[controller_no[1]] = 2;
			if (char2 == 0)
			{
				char1 = 1;
				x2 = 100;
			}
			else
			{
				char1 = 0;
				x2 = 1600;
			}
			//cnt[controller_no[0]] = 2;
			de2 = true;
		}

		if (KEY_Get(KEY_A, controller_no[1]) == 3 && cnt[controller_no[1]] == 2)
		{
			Char[char2]->SetMotion(0);
			cnt[controller_no[1]] = 1;
			de2 = false;
		}
	}




	Char[0]->Animation();
	Char[0]->Update();

	Char[1]->Animation();
	Char[1]->Update();


	view->Set(Vector3(0, 0, -20), Vector3(0, 0, 0));

	if ((cnt[0] + cnt[1]) >= 4)
	{
		static int t = 0;
		t++;
		if (t >= 60) {
			MainFrame->ChangeScene(new SCENE_SELECT_PARTNER(controller_no[0], controller_no[1]));
		}
	}

}



void SCENE_SELECT_CHAR::Render()
{
	view->Activate();
	view->Clear();


	bg->Render(0, 0, 1920, 1080, 0, 0, 1920, 1080, RS_COPY, 0xFFFFFFFF, 3.0f);

	if (controller_no[0] >= 0)
		choseIcon->Render(iconX[0] + controller_no[0] * 128, iconY, 200, 256, controller_no[0] * 256, 0, 256, 256, RS_COPY, 0xFFFFFFFF, 3.0f);
	if (controller_no[1] >= 0)
		choseIcon->Render(iconX[1] + controller_no[1] * 128, iconY, 200, 256, controller_no[1] * 256, 0, 256, 256, RS_COPY, 0xFFFFFFFF, 3.0f);

	if (de1)
		decide->Render(x1, 100, 256, 256, frame * 256, 0, 256, 256);
	if (de2)
		decide->Render(x2, 100, 256, 256, (frame ^ 0x01) * 256, 0, 256, 256);

	//if (cnt[0] == 0 && cnt[1] == -1)
	//char_1->Render(200, 412, 512, 512, 0, 0, 1024, 1024);
	//char_2->Render(220 + 990, 412, 512, 512, 0, 0, 1024, 1024);

	D3DCAPS9 caps;
	HRESULT result = iexSystem::Device->GetDeviceCaps(&caps);
	if (FAILED(result))OutputDebugString("Error: キャプスの取得に失敗");

	DWORD shoudAlphaTest = caps.AlphaCmpCaps & D3DPCMPCAPS_GREATEREQUAL;
	if (shoudAlphaTest)
	{
		iexSystem::Device->SetRenderState(D3DRS_ALPHAREF, (DWORD)0x000000A0);
		iexSystem::Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		iexSystem::Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	}

	iexSystem::Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);	//カリングオフ

	Char[0]->Render();
	Char[1]->Render();

	if (shoudAlphaTest) iexSystem::Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}

#define ANGLE 0.01745f

void SCENE_SELECT_PARTNER::move(int pl_no)
{
	if (right_flg[pl_no]) {

		static int a[2] = { 0 };
		static int step[2] = { 0 };
		switch (step[pl_no])
		{

		case 0:
			if (partner_no[pl_no] == 0)
			{
				pos[pl_no][0].x++;
				pos[pl_no][0].z++;
				pos[pl_no][0].y -= 0.2f;
				scale[pl_no][0].x -= 0.1f;
				scale[pl_no][0].y -= 0.1f;
				SetMotion(pl_no, 0, 0);


				pos[pl_no][1].x--;
				pos[pl_no][1].z--;
				pos[pl_no][1].y += 0.1f;
				scale[pl_no][1].x += 0.1f;
				scale[pl_no][1].y += 0.1f;
				SetMotion(pl_no, 1, 2);

			}

			if (partner_no[pl_no] == 1)
			{
				pos[pl_no][0].x--;
				pos[pl_no][0].z--;
				pos[pl_no][0].y += 0.2f;
				scale[pl_no][0].x += 0.1f;
				scale[pl_no][0].y += 0.1f;
				SetMotion(pl_no, 0, 1);

				pos[pl_no][1].x++;
				pos[pl_no][1].z++;
				pos[pl_no][1].y -= 0.1f;
				scale[pl_no][1].x -= 0.1f;
				scale[pl_no][1].y -= 0.1f;
				SetMotion(pl_no, 1, 0);
			}


			a[pl_no]++;

			if (a[pl_no] >= 15) {
				step[pl_no]++;
			}
			break;

		case 1:
			if (partner_no[pl_no] == 1)
			{
				pos[pl_no][0].x++;
				pos[pl_no][0].z++;
				scale[pl_no][0].x += 0.1f;
				scale[pl_no][0].y += 0.1f;


				pos[pl_no][1].x--;
				pos[pl_no][1].z--;
				scale[pl_no][1].x -= 0.1f;
				scale[pl_no][1].y -= 0.1f;


			}

			if (partner_no[pl_no] == 0)
			{
				pos[pl_no][0].x--;
				pos[pl_no][0].z--;
				scale[pl_no][0].x -= 0.1f;
				scale[pl_no][0].y -= 0.1f;

				pos[pl_no][1].x++;
				pos[pl_no][1].z++;
				scale[pl_no][1].x += 0.1f;
				scale[pl_no][1].y += 0.1f;
			}

			a[pl_no]++;

			if (a[pl_no] >= 30)
			{
				step[pl_no]++;
			}
			break;
		case 2:
			partner_no[pl_no]++;
			if (partner_no[pl_no] > 1)
				partner_no[pl_no] = 0;
			right_flg[pl_no] = false;
			step[pl_no] = 0;
			a[pl_no] = 0;

		default:
			break;
		}
	}

	if (left_flg[pl_no]) {
		static int a[2] = { 0 };
		static int step[2] = { 0 };

		switch (step[pl_no])
		{

		case 0:
			if (partner_no[pl_no] == 0)
			{
				pos[pl_no][0].x--;
				pos[pl_no][0].z--;
				pos[pl_no][0].y -= 0.2f;
				scale[pl_no][0].x -= 0.1f;
				scale[pl_no][0].y -= 0.1f;
				SetMotion(pl_no, 0, 1);

				pos[pl_no][1].x++;
				pos[pl_no][1].z++;
				pos[pl_no][1].y += 0.1f;
				scale[pl_no][1].x += 0.1f;
				scale[pl_no][1].y += 0.1f;
				SetMotion(pl_no, 1, 0);
			}

			if (partner_no[pl_no] == 1)
			{
				pos[pl_no][0].x++;
				pos[pl_no][0].z++;
				pos[pl_no][0].y += 0.2f;
				scale[pl_no][0].x += 0.1f;
				scale[pl_no][0].y += 0.1f;
				SetMotion(pl_no, 1, 0);

				pos[pl_no][1].x--;
				pos[pl_no][1].z--;
				pos[pl_no][1].y -= 0.1f;
				scale[pl_no][1].x -= 0.1f;
				scale[pl_no][1].y -= 0.1f;
				SetMotion(pl_no, 1, 1);
			}


			a[pl_no]++;

			if (a[pl_no] >= 15) {
				step[pl_no]++;
			}
			break;

		case 1:
			if (partner_no[pl_no] == 1)
			{
				pos[pl_no][0].x--;
				pos[pl_no][0].z--;
				scale[pl_no][0].x += 0.1f;
				scale[pl_no][0].y += 0.1f;

				pos[pl_no][1].x++;
				pos[pl_no][1].z++;
				scale[pl_no][1].x -= 0.1f;
				scale[pl_no][1].y -= 0.1f;
			}

			if (partner_no[pl_no] == 0)
			{
				pos[pl_no][0].x++;
				pos[pl_no][0].z++;
				scale[pl_no][0].x -= 0.1f;
				scale[pl_no][0].y -= 0.1f;

				pos[pl_no][1].x--;
				pos[pl_no][1].z--;
				scale[pl_no][1].x += 0.1f;
				scale[pl_no][1].y += 0.1f;
			}

			a[pl_no]++;

			if (a[pl_no] >= 30)
			{
				step[pl_no]++;
			}
			break;
		case 2:
			partner_no[pl_no]--;
			if (partner_no[pl_no] < 0)
				partner_no[pl_no] = 1;
			left_flg[pl_no] = false;
			step[pl_no] = 0;
			a[pl_no] = 0;

		default:
			break;
		}
	}
}

SCENE_SELECT_PARTNER::SCENE_SELECT_PARTNER(int n1, int n2)
{
	pl1 = n1;
	pl2 = n2;
}


SCENE_SELECT_PARTNER::~SCENE_SELECT_PARTNER()
{

}

bool SCENE_SELECT_PARTNER::Initialize()
{
	bg.reset(new iex2DObj("DATA\\IMAGE\\SELECT\\bg.png"));
	decide.reset(new iex2DObj("DATA\\IMAGE\\SELECT\\Decide.png"));
	view.reset(new iexView());
	next.reset(new iex2DObj("DATA\\IMAGE\\SELECT\\option.png"));


	for (int i = 0; i < 2; i++)
	{
		cnt[i] = -1;

	}

	partner_no[controller_no[0]] = 0;
	partner_no[pl2] = 0;

	/*******************************************************/
	/*1P                                                   */
	/*******************************************************/
	if (char1 == 0)
	{
		partner[controller_no[0]][0].reset(new iex3DObj("DATA\\MODEL\\CHR\\PARTNER\\DRAGON\\A\\dragon3.IEM"));
		partner[controller_no[0]][1].reset(new iex3DObj("DATA\\MODEL\\CHR\\PARTNER\\CAT\\A\\Cat3.IEM"));
		frame = 0;
	}

	else
	{
		partner[controller_no[0]][0].reset(new iex3DObj("DATA\\MODEL\\CHR\\PARTNER\\DRAGON\\B\\dragon3.IEM"));
		partner[controller_no[0]][1].reset(new iex3DObj("DATA\\MODEL\\CHR\\PARTNER\\CAT\\B\\Cat3.IEM"));
		frame = 1;
	}
	pos[controller_no[0]][0] = Vector3(-35, -5.0, 0);
	pos[controller_no[0]][1] = Vector3(-35, -7.5, 3);
	angle[controller_no[0]] = Vector3(0, 0, 0);
	scale[controller_no[0]][0] = Vector3(3.5f, 3.5f, -1);
	scale[controller_no[0]][1] = Vector3(0.5f, 0.5f, -1);
	partner[controller_no[0]][0]->SetMotion(1);
	partner[pl1][1]->SetMotion(0);

	/*******************************************************/
	/*2P                                                   */
	/*******************************************************/
	if (char2 == 0)
	{
		partner[pl2][0].reset(new iex3DObj("DATA\\MODEL\\CHR\\PARTNER\\DRAGON\\A\\dragon3.IEM"));
		partner[pl2][1].reset(new iex3DObj("DATA\\MODEL\\CHR\\PARTNER\\CAT\\A\\Cat3.IEM"));
	}
	else
	{
		partner[pl2][0].reset(new iex3DObj("DATA\\MODEL\\CHR\\PARTNER\\DRAGON\\B\\dragon3.IEM"));
		partner[pl2][1].reset(new iex3DObj("DATA\\MODEL\\CHR\\PARTNER\\CAT\\B\\Cat3.IEM"));
	}
	pos[pl2][0] = Vector3(35, -5.0f, 0);
	pos[pl2][1] = Vector3(35, -7.5f, 3);
	angle[pl2] = Vector3(0, PI, 0);
	scale[pl2][0] = Vector3(3.5f, 3.5f, 1);
	scale[pl2][1] = Vector3(0.5f, 0.5f, 1);
	partner[pl2][0]->SetMotion(1);
	partner[pl2][1]->SetMotion(0);


	return true;
}


void SCENE_SELECT_PARTNER::Update()
{


	//********************************************
	//1P
	//********************************************
	if (KEY_Get(KEY_RIGHT, pl1) == 3 && cnt[pl1] < 0 && !left_flg[pl1])
	{

		right_flg[pl1] = true;
	}

	if (KEY_Get(KEY_LEFT, pl1) == 3 && cnt[pl1] < 0 && !right_flg[pl1])
	{
		left_flg[pl1] = true;
	}

	move(pl1);



	if (KEY_Get(KEY_B, pl1) == 3 && cnt[pl1] < 0)
	{
		cnt[pl1]++;
		if (partner_no[pl1] == 0)
			SetMotion(pl1, 0, 2);
		else
			SetMotion(pl1, 1, 1);
	}
	if (KEY_Get(KEY_A, pl1) == 3 && cnt[pl1] >= 0)
	{
		cnt[pl1]--;
		if (partner_no[pl1] == 0)
			SetMotion(pl1, 0, 1);
		else
			SetMotion(pl1, 1, 2);
	}

	partner[pl1][0]->SetPos(pos[pl1][0]);
	partner[pl1][0]->SetAngle(angle[pl1]);
	partner[pl1][0]->SetScale(scale[pl1][0]);
	partner[pl1][0]->Animation();
	partner[pl1][0]->Update();

	partner[pl1][1]->SetPos(pos[pl1][1]);
	partner[pl1][1]->SetAngle(angle[pl1]);
	partner[pl1][1]->SetScale(scale[pl1][1]);
	partner[pl1][1]->Animation();
	partner[pl1][1]->Update();


	//********************************************
	//2P
	//********************************************
	if (KEY_Get(KEY_RIGHT, pl2) == 3 && cnt[pl2] < 0 && !left_flg[pl2])
	{
		right_flg[pl2] = true;
	}

	if (KEY_Get(KEY_LEFT, pl2) == 3 && cnt[pl2] < 0 && !right_flg[pl2])
	{
		left_flg[pl2] = true;
	}

	move(pl2);

	if (KEY_Get(KEY_B, pl2) == 3 && cnt[pl2] < 0)
	{
		cnt[pl2]++;
		if (partner_no[pl2] == 0)
			SetMotion(pl2, 0, 3);
		else
			SetMotion(pl2, 1, 1);
	}
	if (KEY_Get(KEY_A, pl2) == 3 && cnt[pl2] >= 0)
	{
		cnt[pl2]--;
		if (partner_no[pl2] == 0)
			SetMotion(pl2, 0, 0);
		else
			SetMotion(pl2, 1, 2);
	}



	partner[pl2][0]->SetPos(pos[pl2][0]);
	partner[pl2][0]->SetAngle(angle[pl2]);
	partner[pl2][0]->SetScale(scale[pl2][0]);
	partner[pl2][0]->Animation();
	partner[pl2][0]->Update();

	partner[pl2][1]->SetPos(pos[pl2][1]);
	partner[pl2][1]->SetAngle(angle[pl2]);
	partner[pl2][1]->SetScale(scale[pl2][1]);
	partner[pl2][1]->Animation();
	partner[pl2][1]->Update();


	if ((cnt[0] + cnt[1]) >= 0 && (KEY_Get(KEY_SELECT, 1) == 3 || KEY_Get(KEY_SELECT, 0) == 3))
	{
		MainFrame->ChangeScene(new SCENE_SELECT_STAGE());
	}

}

void SCENE_SELECT_PARTNER::Render()
{
	//view->Activate();
	view->Clear();


	bg->Render(0, 0, 1920, 1080, 0, 0, 1920, 1080, RS_COPY, 0xFFFFFFFF, 1.0f);

	D3DCAPS9 caps;
	HRESULT result = iexSystem::Device->GetDeviceCaps(&caps);
	if (FAILED(result))OutputDebugString("Error: キャプスの取得に失敗");

	DWORD shoudAlphaTest = caps.AlphaCmpCaps & D3DPCMPCAPS_GREATEREQUAL;
	if (shoudAlphaTest)
	{
		iexSystem::Device->SetRenderState(D3DRS_ALPHAREF, (DWORD)0x000000A0);
		iexSystem::Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		iexSystem::Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	}

	iexSystem::Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);	//カリングオフ

	if (cnt[pl1] == 0)
	{
		if (partner_no[pl1] == 0)
			partner[pl1][0]->Render();
		else
			partner[pl1][1]->Render();
		decide->Render(100, 100, 256, 256, frame * 256, 0, 256, 256);
	}
	else
	{
		//if (partner_no[pl1] == 0)
		partner[pl1][0]->Render();
		//else
		partner[pl1][1]->Render();
	}

	if (cnt[pl2] == 0)
	{

		if (partner_no[pl2] == 0)
			partner[pl2][0]->Render();
		else
			partner[pl2][1]->Render();
		decide->Render(1600, 100, 256, 256, (frame ^ 0x01) * 256, 0, 256, 256);

	}
	else
	{
		//if (partner_no[pl2] == 0)
		partner[pl2][0]->Render();
		//else
		partner[pl2][1]->Render();
	}

	if (!cnt[0] && !cnt[1])
		next->Render(740, 525, 512, 512, 0, 0, 512, 512);

	if (shoudAlphaTest) iexSystem::Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);




}

bool SCENE_SELECT_STAGE::Initialize()
{
	bg.reset(new iex2DObj("DATA\\IMAGE\\SELECT\\bg2.png"));
	view.reset(new iexView());

	image1.reset(new iex2DObj("DATA\\IMAGE\\SELECT\\parts1.png"));
	image2.reset(new iex2DObj("DATA\\IMAGE\\SELECT\\parts2.png"));
	decide.reset(new iex2DObj("DATA\\IMAGE\\SELECT\\Decide.png"));
	nextIcon.reset(new iex2DObj("DATA\\IMAGE\\SELECT\\option.png"));

	stageName = "FOREST";
	number = STAGE::FOREST;
	stage.pos = stage.point = Vector2(600.0f, 100.0f);
	stage.move = Vector2(0.0f, 0.0f);

	board.pos = board.point = Vector2(740.0f, 650.0f);
	board.move = Vector2(0.0f, 0.0f);


	CursorPos[0] = Vector2(100.0f, 200.0f);
	CursorPos[1] = Vector2(1400.0f, 200.0f);

	canMove = true;
	canDecide = true;
	loop = false;
	return false;
}

void SCENE_SELECT_STAGE::Select()
{
	switch (number)
	{
	case STAGE::FOREST:
		stageName = "FOREST";
		nextFrame = 0;
		if (canMove)
		{
			for (int i = 0; i < 2; i++)
			{
				if (KEY_Get(KEY_LEFT, i))
				{
					stage.move = board.move = Vector2(-30.0f, 0.0f);
					number = STAGE::RUIN;
					canMove = false;
					canDecide = false;
					break;
				}
				if (KEY_Get(KEY_RIGHT, i))
				{
					stage.move = board.move = Vector2(30.0f, 0.0f);
					number = STAGE::RUIN;
					canMove = false;
					canDecide = false;
					break;
				}
			}
		}
		break;
	case STAGE::RUIN:
		stageName = "RUIN";
		nextFrame = 1;
		if (canMove)
		{
			for (int i = 0; i < 2; i++)
			{
				if (KEY_Get(KEY_LEFT, i))
				{
					stage.move = board.move = Vector2(-30.0f, 0.0f);
					number = STAGE::FOREST;
					canMove = false;
					canDecide = false;
					break;
				}
				if (KEY_Get(KEY_RIGHT, i))
				{
					stage.move = board.move = Vector2(30.0f, 0.0f);
					number = STAGE::FOREST;
					canMove = false;
					canDecide = false;
					break;
				}
			}
		}
		break;
	default:
		break;
	}

	if (stage.move.x < .0f && stage.pos.x + 875.0 < -10.0f)
	{
		stage.pos.x = 2000.0f;

		stage.frame = nextFrame;
		loop = true;
	}
	if (stage.move.x < .0f && stage.pos.x < stage.point.x&&loop)
	{
		stage.move = Vector2(0.0f, 0.0f);
		stage.pos = stage.point;

		canMove = true;
		canDecide = true;
		loop = false;
	}
	if (stage.move.x > .0f && stage.pos.x > 2000.0f)
	{
		stage.pos.x = -800.0f;

		stage.frame = nextFrame;
		loop = true;
	}
	if (stage.move.x > .0f && stage.pos.x > stage.point.x && loop)
	{
		stage.move = Vector2(0.0f, 0.0f);
		stage.pos = stage.point;

		canDecide = true;
		canMove = true;
		loop = false;
	}

	stage.pos += stage.move;
	board.frame = nextFrame ^ 0x01;
}


void SCENE_SELECT_STAGE::Update()
{

	switch (step)
	{
	case STEP::SELECT:
		Select();
		if (canDecide)
		{
			for (int i = 0; i < 2; i++)
			{
				if (KEY_Get(KEY_B, i))
				{
					step = STEP::DECIDE;
					break;
				}
			}
		}
		break;
	case STEP::DECIDE:
		for (int i = 0; i < 2; i++)
		{
			if (KEY_Get(KEY_A, i))
			{
				step = STEP::SELECT;
				break;
			}
		}

		for (int i = 0; i < 2; i++)
		{
			if (KEY_Get(KEY_SELECT, i))
			{
				MainFrame->ChangeScene(new SCENE_LOAD("", new SCENE_MAIN(controller_no[0], controller_no[1], char1, char2, partner_no[controller_no[0]], partner_no[controller_no[1]], stageName)));
				break;
			}
		}
	}

}

void SCENE_SELECT_STAGE::Render()
{
	//view->Activate();
	view->Clear();


	bg->Render(0, 0, 1920, 1080, 0, 0, 1920, 1080, RS_COPY, 0xFFFFFFFF, 1.0f);

	image1->float_Render(stage.pos.x, stage.pos.y, 875, 750, stage.frame * 512, 0, 512, 512);
	image2->float_Render(board.pos.x, board.pos.y, 512, 512, board.frame * 512, 0, 512, 512);

	if (canMove)
	{
		image2->float_Render(CursorPos[0].x, CursorPos[0].y, 512, 512, 0, 512, 512, 512);
		image2->float_Render(CursorPos[1].x, CursorPos[1].y, 512, 512, 512, 512, 512, 512);
	}

	if (step == STEP::DECIDE)
	{
		decide->Render(1200, 100, 256, 256, 0, 256, 256, 256);
		nextIcon->Render(740, 525, 512, 512, 0, 0, 512, 512);
	}
}
