#include	"CAMERA.h"


//
CAMERA::CAMERA()
{
	view = nullptr;
	pos = Vector3(0, 32, -87.0f);
	target = Vector3(0, 32, 0);
	width = 128;
	height = 72;
	n = 0;
	f = 140;

}

void CAMERA::Initialize()
{
	view.reset(new iexView());
}


void CAMERA::Update()
{


	view->Set(pos, target);
}

void CAMERA::Update(Vector3 pos1, Vector3 pos2)
{
	Vector3 d = pos1 - pos2;
	float l = d.Length();
	d.Normalize();

	
	//static int time = 0;
	//if (KEY_Get(KEY_R2, 0))
	//{
	//	//pos.z++;
	//	time++;
	//	if (time % 40 == 0)
	//	{
	//		width -= 16;
	//		height -= 9;
	//		f--;
	//	}
	//}

	//if (KEY_Get(KEY_L2, 0))
	//{
	//	//pos.z--;
	//	time++;
	//	if (time % 40 == 0)
	//	{
	//		width += 16;
	//		height += 9;
	//		f++;
	//	}
	//}

	/*if (KEY_Get(KEY_R2, 0) == 2 || KEY_Get(KEY_L2, 0) == 2)
	{
		time = 0;
	}*/

	view->Set(pos, target);

}

void CAMERA::Render()
{
	view->Activate(width, height, n, f);
	view->Clear();

	/*char str[256];
	wsprintf(str, "width = %d\nheight = %d", width, height);
	IEX_DrawText(str, 10, 200, 100, 150, 0xFFFFFF00);*/
}