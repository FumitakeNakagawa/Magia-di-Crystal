#include	"iextreme.h"
#include	"SceneMain\CHARACTOR\Character.h"
#include	"Trap.h"
#include	"..\\..\\COLISSION\COLLISION.h"


void TRAP::Initialize()
{
	//this->pos = pos;
	//this->move = move;
	//this->playerNum = plNum;
	//angle = PI * 2;
	//obj.reset(new iex3DObj("DATA\\MODEL\\CHR\\PARTNER\\CAT\\CAT.IEM"));
	image.reset(new iex2DObj("DATA\\IMAGE\\BOMBCOLOR.PNG"));
}

void TRAP::Append()
{

}

//  罠が空中にあるときだけ実行される
void TRAP::Move()
{
	move.y += GRAVITY;
	//move.x *= 0.8f;
	pos += move;

	if (pos.x > 63 || pos.x < -63) move.x *= -1;
}

//  罠が設置後のみ実行される
void TRAP::Catch()
{
	//move.y += GRAVITY;
	//move *= 0.8f;
	//pos += move;
}

//  罠消す
void TRAP::Erase()
{
	//obj.release();
}

//  罠更新
void TRAP::Update()
{
	//  罠の状態で挙動分岐
	switch (state)
	{
	case MOVE:        //  移動中
		Move();
		break;
	case INSTALLATION://  地面設置後
		Catch();
		break;
	}

	ColorUpdate();
	obj->SetPos(pos);
	obj->SetAngle(angle);
	obj->SetScale(scale);
	obj->Update();
}

void TRAP::Render(const Vector3& imagePos)
{
	if (imageMoveFlag) imagePosMove += 0.6f;
	if (!imageMoveFlag) imagePosMove -= 0.6f;

	if (imagePosMove > 15) imageMoveFlag = false;
	if (imagePosMove < 0) imageMoveFlag = true;

	image->Render(imagePos.x - 15, imagePos.y - 150 + imagePosMove, 35, 35,
		playerNum * 128, 0, 128, 128);

	obj->Render(shader,"copy3");
}

void TRAP::ColorUpdate()
{
	explosionTimer--;

	if (redUpdateFlag)	redUpdate += Displace;
	if (!redUpdateFlag)  redUpdate -= Displace;

	if (explosionTimer <= 300 && explosionTimer > 130) Displace = 0.03f;
	else if (explosionTimer <= 130) Displace = 0.05f;
	
	if (redUpdate >= 0.95f) redUpdateFlag = false;
	if (redUpdate <= 0.5f) redUpdateFlag = true;

	shader->SetValue("redValue", redUpdate);
}

void TRAP::ToLocal()
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

}

void TRAP::ToWorld()
{
	if (rideNum >= 0)
	{
		COLLISION::ToWorld(rideNum, local);
		//  UNDONE:↓変更
		pos = Vector3(local.x, local.y, pos.z);
		state = INSTALLATION;
	}
}
