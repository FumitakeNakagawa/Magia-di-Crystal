#include	"BackGround.h"

std::random_device BACK_BASE::rnd;
std::mt19937 BACK_BASE::mt;
std::uniform_int_distribution<> BACK_BASE::randint;
std::uniform_real_distribution<>BACK_BASE::randfloat;


void BACK_BASE::Random()
{
	randint;
}


//*********************************************************
//
//		îwåiÉxÅ[ÉX
//
//********************************************************
void BACK_BASE::Initialize(char* filename)
{
	mt = std::mt19937(rnd());

	obj = std::make_unique<iex2DObj>(filename);
	pos = move = Vector3(0.0f, 0.0f, 0.0f);
}

void BACK_BASE::Initialize(iex2DObj* obj)
{
	mt = std::mt19937(rnd());

	this->obj.reset(obj);
	pos = move = Vector3(0.0f, 0.0f, 0.0f);
}

void BACK_BASE::Render()
{
	obj->Render((int)pos.x, (int)pos.y, dx, dy, sx, sy, width, height, RS_COPY, color, pos.z);

}

void BACK_BASE::Render(int x, int y, int dx, int dy, int sx, int sy, int w, int h, ULONG dwFlg, DWORD color, float z)
{
	/*x = 0;
	y = 0;
	dx = iexSystem::ScreenWidth;
	dy = iexSystem::ScreenHeight;
	sx = 0;
	sy = 0;
	w = iexSystem::ScreenWidth;
	h = iexSystem::ScreenWidth;*/
	obj->Render(x, y, dx, dy, sx, sy, w, h, dwFlg, color, z);

}

//********************************************************************
//
//
//
//*******************************************************************
void BACK_MOVE::Initialize(char* filename)
{
	aliveTimer = 0;
	frame = 0;
	timer = 0;
}

void BACK_MOVE::Initialize(iex2DObj* obj)
{
	BACK_BASE::Initialize(obj);
	aliveTimer = 0;
	frame = 0;
	timer = 0;
}
//******************************************************************
//
//		â_ÉNÉâÉX
//
//******************************************************************
void BACK_CLOUD::Update()
{

}

void BACK_CLOUD::Render()
{
	iexPolygon::Render3D(vertex, 2, obj.get(), shader.get(), "uv_anime2");

}

