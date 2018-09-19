#include	"UI.h"
#include	"OTHER\Sound.h"



//*****************************************************************************************************************************
//
//	É^ÉCÉÄ
//
//*****************************************************************************************************************************
TIME::TIME() :cnt(0), second(0) {}

TIME::~TIME()
{
	if (timer)delete timer;
	timer = nullptr;
}


bool TIME::Initialize(char* filename)
{
	timer = new iex2DObj(filename);
	start_finish.reset(new iex2DObj("DATA\\IMAGE\\UI\\display.PNG"));
	//black.reset(new iex2DObj("DATA\\Menu.PNG"));

	cnt = TIME_MAX;
	second = 60;

	flg = false;

	sizeX = 580;
	sizeY = 260;

	x = 1980;

	return true;
}


void TIME::Update()
{
	second--;
	if (second == 0)
	{
		cnt--;
		second = 60;
		if (cnt == 30)
		{
			g_sound->PlaySE("30second");
			g_sound->StopBGM("BGM1");
			g_sound->PlayBGM("BGM2");
		}
	}




	//if (GetKeyState('O') < 0)


	if (cnt >= TIME_MAX || cnt < 1)
	{
		sizeX++;
		sizeY++;
	}

	else
	{
		sizeX = 580;
		sizeY = 260;
	}



	if (cnt <= 30 && cnt > 28)
	{
		x -= 15;
		if (x < 700)
			x = 700;
	}
	if (cnt <= 28 && flg)
	{
		x -= 15;
	}
	if (x < 1980 && x > -512)
	{
		flg = true;
	}
	else
	{
		flg = false;
	}
}


void TIME::Render()
{
	int t = cnt;
	int num = 0;

	if (cnt > 3)
	{
		for (int kcnt = 0; t != 0; kcnt++)
		{
			num = t % 10;
			timer->Render(920 - (kcnt * 96), 0, 128, 128,
				(num % 7) * 128, (num / 7) * 128,
				128, 128);
			t -= num;
			t /= 10;
		}
	}

	else
	{
		for (int kcnt = 0; t != 0; kcnt++)
		{
			num = t % 10;
			timer->Render(850 - (kcnt * 96), 320, 256, 256,
				(num % 7) * 128, (num / 7) * 128,
				128, 128, RS_COPY, 0xFFFFFFFF);
			t -= num;
			t /= 10;
		}
	}

	if (flg)
	{
		timer->Render(x, 380, 512, 512, 0, 768, 256, 256);
	}

	if (cnt >= TIME_MAX)
	{
		start_finish->Render((int)(970 - sizeX / 2),
			(int)(480 - sizeY / 2),
			(int)sizeX,
			(int)sizeY,
			0,
			256 * 3,
			1024,
			256);
	}

	if (cnt < 1)
	{
		start_finish->Render((int)(970 - sizeX / 2),
			(int)(480 - sizeY / 2),
			(int)sizeX,
			(int)sizeY,
			0,
			0,
			1024,
			256);
	}



#ifdef _DEBUG

	/*char	str[64];
	sprintf(str, "fadetime %d\nfadealpha %d", (int)fadetime, (int)fadealpha);
	IEX_DrawText(str, 10, 100, 100, 50, 0xFFFFFF00);*/

#endif // DEBUG


}


void TIME::Render(int x, int y, int dx, int dy, int sx, int sy, int w, int h)
{
	timer->Render(x, y, dx, dy, sx, sy, w, h);
}


//*****************************************************************************************************************************
//
//	ÇgÇoÉQÅ[ÉW
//
//*****************************************************************************************************************************
LIFEGAUGE::LIFEGAUGE() {}


LIFEGAUGE::~LIFEGAUGE()
{

}


bool LIFEGAUGE::Initialize(float h, int index)
{
	frame = std::make_unique<iex2DObj>((char*) "DATA\\IMAGE\\UI\\FRAME.PNG");
	gauge = std::make_unique<iex2DObj>((char*)"DATA\\IMAGE\\UI\\GAUGE.PNG");
	hp[index] = h;
	maxhp[index] = h;

	view.reset(new iexView());

	return true;
}


void LIFEGAUGE::Update(float h, int index)
{
	if (hp[index] > h)
		hp[index] -= 0.2f;

	if (hp[index] < h)
		hp[index] += 0.2f;

	if (hp[index] < 0)
	{
		hp[index] = 0;
	}
}


void LIFEGAUGE::Render(int index)
{
	if (index == 0)
	{
		frame->Render(0, -120, 512, 256 + 128, 0, 0, 1024, 1024);
		gauge->float_Render(0, -120, (512 * hp[index] / maxhp[index]), 256 + 128, 0, 0, (1024 * hp[index] / maxhp[index]), 1024);

		//frame->Render(0, 900-150, 512, 512, 0, 0, 1024, 1024);
		//gauge->Render(18, 900-150, int(512 * hp / maxhp), 512, 0, 0, -int(1024 * hp / maxhp), 1024);
	}

	else
	{
		frame->Render(1980 - 580, -120, 512, 256 + 128, 1024, 0, -1024, 1024);
		gauge->float_Render(1980 - 580 + 512, -120, -(512 * hp[index] / maxhp[index]), 256 + 128, 1024, 0, (1024 * hp[index] / maxhp[index]), 1024);

		//frame->Render(1980-580, 900-150, 512, 512, 1024, 0, -1024, 1024);
		//gauge->Render(1980-580-18, 900-150, int(512 * hp / maxhp), 512, 1024, 0, int(1024 * hp / maxhp), 1024);

	}
}

void LIFEGAUGE::Render(CAMERA* camera, const Vector3 & pos)
{

	/*Vector3 Outp;

	camera->Screentransformation(Outp, pos);
	if (Outp.x < -256 / 2 || Outp.x > iexSystem::ScreenWidth + 72 / 2)return;
	if ( Outp.y > iexSystem::ScreenHeight + 72 / 2)return;
	if (Outp.z < 0) return;

	Outp.x -= 256 / 2;

	frame->Render(Outp.x, Outp.y, 256, 256, 0, 0, 1024, 1024, RS_COPY, 0xFFFFFFFF, Outp.z);

	gauge->Render((int)Outp.x, (int)Outp.y+2, (int)(256 * hp) / maxhp, 256-4, 0, 0, (1024 * hp) / maxhp, 1024,RS_COPY,0xFFFFFFFF,Outp.z);
	*/
}

void LIFEGAUGE::Sethp(float l)
{

}

void CRYSTAL_UI::Initialize(char * filename)
{
	image.reset(new iex2DObj(filename));
	color = 0xFFFFFFFF;
}

void CRYSTAL_UI::Render(CAMERA * camera, const Vector3 & pos)
{
	//Vector3 Outp;

	//camera->Screentransformation(Outp, pos);
	//if ( Outp.x > iexSystem::ScreenWidth + 128 / 2)return;
	//if (Outp.y > iexSystem::ScreenHeight + 128 / 2)return;
	//if (Outp.z < 0) return;

	//Outp.x -= 128 / 2;

	//if(hold == 1)
	//	image->Render(Outp.x, Outp.y, 128, 128, hold * 128, hold * 128, 128, 128, RS_COPY, 0xA00042C5, Outp.z);
	//if (hold == 2)
	//	image->Render(Outp.x, Outp.y, 128, 128, hold * 128, hold * 128, 128, 128, RS_COPY, 0xA0FF0000, Outp.z);
	////if (hold == 0)
	//	//image->Render(Outp.x, Outp.y, 128, 128, hold * 128, hold * 128, 128, 128, RS_COPY, 0xA000FF00, Outp.z + 0.1f);


}

void CRYSTAL_UI::Render(int x, int y)
{
	image->Render(x, y, 128, 128, 0, 0, 256, 256, RS_COPY, color);
}

void ICON::Initialize()
{
	image.reset(new iex2DObj("DATA\\IMAGE\\UI\\icon.png"));
}

void ICON::Render(CAMERA * camera, const Vector3 & pos,int no)
{
	Vector3 Outp;

	camera->Screentransformation(Outp, pos);
	if (Outp.x < -256 / 2 || Outp.x > iexSystem::ScreenWidth + 72 / 2)return;
	if (Outp.y > iexSystem::ScreenHeight + 72 / 2)return;
	if (Outp.z < 0) return;

	Outp.x -= 256 / 2;

	image->Render(Outp.x+43, Outp.y, 128, 128, 256*no, 0, 256, 256);


}

