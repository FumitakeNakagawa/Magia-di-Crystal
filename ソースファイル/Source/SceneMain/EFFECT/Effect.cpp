#include	"Effect.h"
#include	<iostream>
#include	<string>


#include	"../CHARACTOR/PLAYER/PLAYER.h"
#include	"../CHARACTOR/PLAYER/Magic.h"
#include    "../../SYSTEM/System.h"


//static int frame = 0;
//std::unique_ptr<EFFECT> effect;


std::unique_ptr<PLAYER>	player;
std::unique_ptr<MAGIC>	magic;

//std::vector<iex2DObj*> EFFECT::image;


void EFFECT::SetPng(char* filename, const int max)
{

	/*image.resize(max);
	for (int num = 0; num < max; num++)
	{
	string name = filename;
	string number = to_string(num);
	name += "\\" + number + ".png";
	image[num] = (new iex2DObj((char*)name.c_str()));
	}*/

}

void EFFECT::ReleaseImage()
{
	/*for (int num = 0; num < image.size(); num++)
	{
	if (image[num])
	{
	delete image[num];
	image[num] = nullptr;
	}
	}
	image.clear();*/
}

EFFECT::EFFECT()
{
	obj.reset(new iex2DObj("DATA\\IMAGE\\EFFECT\\ice.PNG"));
	//  pos = Vector3(0, 0, 0);
	//  move = Vector3(0, 0, 0);
	flg = 0;
	angle = 0;
	frame = 0;

	effect_animation = 0;

	//	1番目の画像を設定
	current_image_id = 20;

}

EFFECT::EFFECT(char* filename) :EFFECT()
{

}



void EFFECT::Initialize(int no)
{
	if (no == 0)
		obj.reset(new iex2DObj("DATA\\IMAGE\\EFFECT\\flame.PNG"));
	else
		obj.reset(new iex2DObj("DATA\\IMAGE\\EFFECT\\ice.PNG"));
	//  pos = Vector3(0, 0, 0);
	//  move = Vector3(0, 0, 0);
	flg = 0;
	angle = 0;
	frame = 0;

	effect_animation = 0;

	//	1番目の画像を設定
	current_image_id = 20;
}




void EFFECT::Update()
{
	frame++;
	if (frame % 2 == 0)
	{
		effect_animation++;
		if (12 <= effect_animation)effect_animation = 4;

		//magic->Charge(frame);

	}

	/*if (effect_animation == 0)
	{
	if (effect_frame >= EFFECT_TIMER) effect_frame = 0;
	}*/
}



void EFFECT::Render(Vector3 p, float pow, CAMERA* camera)
{
	camera->Screentransformation(power, p);

	pow /= 3.5f;
	//  エフェクトが映っているかどうか
	if (power.x<-256 || power.x>iexSystem::ScreenWidth + 256)return;
	if (power.y<-256 || power.y>iexSystem::ScreenHeight)return;
	if (power.z < 0)return;

	power.z = 0.5f;		//	誤魔化し

	if (angle == 0)
	{
		obj->Render((int)(power.x - 188 + (386 + pow) / 2 - (pow * 256)), (int)(power.y - 188 + (386 + pow) / 2 - (pow * 188)), (int)(386 * pow), (int)(386 * pow), (int)(effect_animation % 4 * 256), (int)(effect_animation / 4 * 256), 256, 256, shader2D, "copy", 0xFFFFFFFF, power.z);
	}
	else if (angle > 0)
	{
		obj->Render(int(power.x - 188 + (386 + pow) / 2 - (pow * 156)), (int)(power.y - 188 + (386 + pow) / 2 - (pow * 188)), (int)(386 * pow), (int)(386 * pow), (int)((effect_animation % 5) + 1 * 256), (int)(effect_animation / 4 * 256), -256, 256, shader2D, "copy", 0xFFFFFFFF, power.z);
	}


}

void EFFECT::Set_Effect(int f, int a)  //  (位置, 移動量,flg番号)
{

	flg = f;
	angle = a;

}


//チャージエフェクト
void CHARGE::ChargeInitialize()
{
	charge_image = new iex2DObj("DATA\\EFFECT\\Charge\\Charge_main.PNG");
	range = 5.0f;

	for (int n = 0; n < 16; n++)
	{
		data[n].pos = Vector3(0, 0, 0);
		data[n].vec = Vector3(0, 0, 0);
		data[n].value = 0.0f;
		data[n].flag = 0;
		data[n].maxtime = 0;
		data[n].time = 0;
	}
	increment = 1;
	timer = 0;
}

void CHARGE::ChargeUpdate(int c_time)
{
	//charge_vec = player->GetPos()+Vector3();

	timer = c_time;

	//  エフェクトの削除
	for (int n = 0; n < 16; n++)
	{
		if (1 == data[n].flag&&data[n].time >= data[n].maxtime)
		{
			data[n].flag = 0;
		}
	}

	if (c_time >= 90)
	{
		SetIncrement(16);
	}
	if (c_time < 90)
	{
		SetIncrement(8);
	}
	if (c_time < 60)
	{
		SetIncrement(4);
	}
	if (c_time < 30)
	{
		SetIncrement(1);
	}

	//  エフェクト出現
	if (10 <= timer)
	{
		//timer = 0;
		for (int n = 0, i = 0; n < 16; n++)
		{
			if (0 == data[n].flag)
			{
				data[n].flag = 1;
				data[n].maxtime = 30;
				data[n].time = 0;
				data[n].value = 0.1f;
				while (1)
				{
					data[n].pos = Vector3((float)rand() / RAND_MAX * 8.0f - 4.0f, (float)rand() / RAND_MAX * 8.0f - 4.0f, 0.0f);
					data[n].vec = Vector3(0, 0, 0) - data[n].pos;
					if (range >= data[n].vec.Length())break;
				}
				i++;
			}
			if (i >= increment)break;
		}
	}

	//  エフェクト更新
	for (int n = 0; n < 16; n++)
	{
		if (1 == data[n].flag)
		{
			data[n].pos += data[n].vec*(1.0f / data[n].maxtime);
			data[n].value += 0.1f;
			if (1.0f <= data[n].value)data[n].value = 1.0f;
			data[n].time++;
		}
	}

	timer++;
}

void CHARGE::ChargeRender(Vector3 p, CAMERA* camera)
{
	DWORD color;
	//  エフェクト量の変更
	if (timer >= 90)
	{
		color = 0xFFFF0000;
	}
	if (timer < 90)
	{
		color = 0xFF0000FF;
	}
	if (timer < 60)
	{
		color = 0xFF00FF00;
	}
	if (timer < 30)
	{
		color = 0xFFFFFFFF;
	}

	for (int n = 0; n < 16; n++)
	{
		if (0 != data[n].flag)
		{
			camera->Screentransformation(power, p + data[n].pos);

			//  エフェクトが映っているかどうか
			if (power.x<-256 || power.x>iexSystem::ScreenWidth + 256)return;
			if (power.y<-256 || power.y>iexSystem::ScreenHeight)return;
			//if (power.z < 0)return;

			power.z = 0.0f;		//	誤魔化し


			charge_image->Render((int)(power.x - 20 * data[n].value), (int)(power.y - 20 * data[n].value),
				(int)(40 * data[n].value), (int)(40 * data[n].value),
				0, 0, 256, 256, shader2D, "copy", color, power.z);
		}

	}


}

void CHARGE::DataRelease()
{
	for (int n = 0; n < 16; n++)
	{
		data[n].flag = 0;
	}
	timer = 0;
}

void _EFFECT::Initialize()
{
	image1.reset(new iex2DObj("DATA\\IMAGE\\EFFECT\\EFFECT1.png"));
	image2.reset(new iex2DObj("DATA\\IMAGE\\EFFECT\\EFFECT2.png"));
	//追加
	chargeimage.reset(new iex2DObj("DATA\\IMAGE\\EFFECT\\CHARGE.png"));

	smoke.anime = 0;
	smoke.frame = 0;
	smoke.flg = false;

	charge.anime = 0;
	charge.frame = 0;
	charge.flg = false;

	magic_Vanish.anime = 0;
	magic_Vanish.frame = 0;
	magic_Vanish.flg = false;
}

void _EFFECT::Update()
{
}

void _EFFECT::Render(Vector3 pl, CAMERA* camera)
{

}

void _EFFECT::Smoke_Update()
{
	smoke.frame++;
	smoke.anime = smoke.frame / 2;
	if (smoke.frame > 20)
	{
		smoke.flg = false;
	}
	if (!smoke.flg)
		smoke.frame = 0;
}

void _EFFECT::Smoke_Render(Vector3 p, CAMERA* camera)
{
	Vector3 power;
	camera->Screentransformation(power, p);

	//  エフェクトが映っているかどうか
	if (power.x<-256 || power.x>iexSystem::ScreenWidth + 256)return;
	if (power.y<-256 || power.y>iexSystem::ScreenHeight)return;
	//if (power.z < 0)return;

	power.z = 0.0f;		//	誤魔化し

	if (smoke.flg == true)
	{
		image1->Render((int)power.x, (int)power.y, 128, 128,
			(int)(smoke.anime % 5 * 128), (int)(768 + (smoke.anime / 5)), 128, 128, shader2D, "copy", 0xA0FFFFFF, -2.0f);

	}
}

void _EFFECT::Charge_Update()
{
	charge.frame++;
	if (charge.frame % 120 == 0)
		charge.frame = 80;
	charge.anime = charge.frame / 6;

	if (!charge.flg)
		charge.frame = 0;
}

void _EFFECT::Charge_Render(Vector3 p, CAMERA * camera)
{
	Vector3 power;
	camera->Screentransformation(power, p);

	//  エフェクトが映っているかどうか
	if (power.x<-256 || power.x>iexSystem::ScreenWidth + 256)return;
	if (power.y<-256 || power.y>iexSystem::ScreenHeight)return;
	//if (power.z < 0)return;

	power.z = 0.0f;		//	誤魔化し

						//if (charge.flg == true)
	{
		chargeimage->Render((int)power.x, (int)power.y, 256, 256,
			(int)(charge.anime % 5 * 128), (int)(128 * (charge.anime / 5)), 128, 128, shader2D, "copy", 0xFFFFFFFF, -2.0f);

	}
}

void _EFFECT::magic_Vanish_Update(Vector3 pos)
{
	this->pos = pos;
	magic_Vanish.frame++;
	magic_Vanish.anime = magic_Vanish.frame / 3;

	if (magic_Vanish.frame > 30)
		magic_Vanish.flg = false;

	if (!magic_Vanish.flg)
		magic_Vanish.frame = 0;

}

void _EFFECT::magic_Vanish_Render(CAMERA * camera, int no)
{
	Vector3 power;
	camera->Screentransformation(power, pos);
	no ^= 0x01;
	//  エフェクトが映っているかどうか
	if (power.x<-256 || power.x>iexSystem::ScreenWidth + 256)return;
	if (power.y<-256 || power.y>iexSystem::ScreenHeight)return;
	//if (power.z < 0)return;

	power.z = 0.0f;		//	誤魔化し

	if (magic_Vanish.flg == true)
	{
		image1->Render((int)power.x, (int)power.y, 128, 128,
			(int)(magic_Vanish.anime % 5 * 128), (int)(256 + (256 * no) + (magic_Vanish.anime / 5)), 128, 128, shader2D, "copy", 0xFFFFFFFF, -2.0f);

	}
}

void _EFFECT::hold_Update(int no)
{
	if (no == -1)
	{
		holded.anime = 0;
		holded.frame = 0;
		holded.flg = false;
		loop = 0;
		hold.frame++;
		if (hold.frame % 60 == 0)
			hold.frame = 0;
		hold.anime = hold.frame / 6;

		if (!hold.flg)
			hold.frame = 0;
	}
	if (no > -1)
	{
		holded.frame++;
		if (holded.frame % 5 == 0)
			holded.anime++;
		if (holded.anime == 10)
		{
			holded.anime = 0;
			loop++;
		}
		if (loop >= 2)
			holded.flg = true;
		//if (holded.flg)
		//	holded.anime = 5;
	}
}

void _EFFECT::hold_Render(Vector3& pos, CAMERA* camera, int no, int c1, int c2)
{
	Vector3 power;
	camera->Screentransformation(power, pos);

	//  エフェクトが映っているかどうか
	if (power.x<-256 || power.x>iexSystem::ScreenWidth + 256)return;
	if (power.y<-256 || power.y>iexSystem::ScreenHeight)return;
	//if (power.z < 0)return;

	power.z = 0.0f;		//	誤魔化し

	if (no == -1)
	{


		if (hold.flg == true)
		{
			image2->Render((int)power.x, (int)power.y, 256, 256,
				(int)(hold.anime % 5 * 128), (int)(128 * (hold.anime / 5)), 128, 128, shader2D, "copy", 0xCCFFFFFF, -2.0f);
		}
	}

	if (!holded.flg)
	{
		if (no == (int)PLAYER::CHARACTOR::NO::P1)
		{
			int x = (int)power.x;
			int y = (int)(power.y - 30);
			int dx = 256;
			int dy = 256;
			int sx = holded.anime % 5 * 128;

			int sy;
			if (c1 == 0)	sy = holded.anime / 5 * 128 + 128 * 4;
			if (c1 == 1)	sy = holded.anime / 5 * 128 + 128 * 2;

			int h = 128;
			int w = 128;
			image2->Render(x, y, dx, dy, sx, sy, w, h, RS_COPY, 0xCCFFFFFF, -2.0f);
		}
		if (no == (int)PLAYER::CHARACTOR::NO::P2)
		{
			int x = (int)power.x;
			int y = (int)(power.y - 30);
			int dx = 256;
			int dy = 256;
			int sx = holded.anime % 5 * 128;

			int sy;
			if (c2 == 0)	sy = holded.anime / 5 * 128 + 128 * 4;
			if (c2 == 1)	sy = holded.anime / 5 * 128 + 128 * 2;
			int h = 128;
			int w = 128;
			image2->Render(x, y, dx, dy, sx, sy, w, h, RS_COPY, 0xCCFFFFFF, -2.0f);
		}
	}
}

