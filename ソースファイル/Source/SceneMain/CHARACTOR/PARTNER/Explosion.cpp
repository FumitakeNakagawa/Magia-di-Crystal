#include	"iextreme.h"
#include	"SceneMain\CAMERA\CAMERA.h"
#include	"Explosion.h"

EXPLOSION::EXPLOSION(const Vector3& pos, CAMERA* c):animation(0), timer(0)
{
	camera = c;
	obj.reset(new iex2DObj("DATA\\IMAGE\\EFFECT\\EFFECT1.PNG"));
	trapPos = pos;
}

EXPLOSION::~EXPLOSION()
{

}

void EXPLOSION::Update()
{
	camera->Screentransformation(Output, trapPos);
}

void EXPLOSION::Render()
{
	timer++;
	if (timer % 3 == 0) animation += 128;
	//if (animation > 640) animation = 0;
	obj->Render(Output.x-230, Output.y-300, 450, 450, animation, 512, 128, 128);
}