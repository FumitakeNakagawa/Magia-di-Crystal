#include	"iextreme.h"
#include	"SceneMain\CHARACTOR\PLAYER\PLAYER.h"
#include	"PartnerMagic.h"


void PARTNER_MAGIC::Erase()
{
	//obj.reset();
}

//void PARTNER_MAGIC::Append(const Vector3& p, const Vector3& m)
//{
//	obj.reset(new iex3DObj("DATA\\magic\\magic.IEM"));
//	pos = p + Vector3(0, 10, 0);
//	move = m;
//}

//  ˆÚ“®ˆ—
//  ˆø”‚Í‘ŠŽèƒvƒŒƒCƒ„[
void PARTNER_MAGIC::Move()
{
	pos += move;

	obj->SetPos(pos);
	obj->SetAngle(angle);
	obj->SetScale(scale);
	obj->Update();
}

void PARTNER_MAGIC::Render(int char_no)
{
	if (char_no == 0) obj->Render(shader, "copy7");
	else obj->Render(shader, "copy8");
}
