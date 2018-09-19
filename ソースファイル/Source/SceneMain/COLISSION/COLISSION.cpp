#include	"COLLISION.h"
#include	"SceneMain\CHARACTOR\Character.h"
#include	"SceneMain\CRYSTAL\Crystal.h"
#include	"..\\CHARACTOR\PLAYER\Magic.h"
#include	"SceneMain\CHARACTOR\PLAYER\PLAYER.h"
#include	"..\\STAGE\STAGE.h"
#include	"SceneMain\CHARACTOR\PARTNER\PartnerMagic.h"
#include	"SceneMain\CHARACTOR\PARTNER\Partner.h"


std::vector<STAGE*> COLLISION::lowStage;
iex2DObj* COLLISION::image=nullptr;

void COLLISION::Load(unsigned int index, iexMesh* mesh)
{
	lowStage.push_back(new STAGE());
	lowStage[index]->Load(mesh);
	if(!image)
		image = new iex2DObj("DATA\\DEBUG.PNG");
}

void COLLISION::Initialize(DWORD32 index, iexMesh* mesh, int kind, const Vector3& pos, const Vector3& rot, const Vector3& scale)
{
	if (index >= lowStage.size())
		lowStage.push_back(new STAGE);

	lowStage[index]->Initialize(mesh, kind);
	lowStage[index]->SetPos(pos);
	lowStage[index]->SetRot(rot);
	lowStage[index]->SetScale(scale);
	lowStage[index]->Update();

}

void COLLISION::Release()
{
	for (unsigned int index = 0; index < lowStage.size(); index++)
	{
		if (lowStage[index])
		{
			delete lowStage[index];
			lowStage[index] = nullptr;
		}
	}
	lowStage.clear();

	if (image)
	{
		delete image;
		image = nullptr;
	}
}

void COLLISION::Update(int index, Vector3& pos, Vector3& rot, Vector3& scale)
{
	if (lowStage[index]->GetKindGround() == STAGE::KIND_GROUND::MOVING)
	{
		lowStage[index]->SetPos(Vector3(pos.x, pos.y, 0.0f));
		lowStage[index]->SetRot(rot);
		lowStage[index]->SetScale(scale);

		lowStage[index]->GetMesh()->SetPos(lowStage[index]->GetPos());
		lowStage[index]->GetMesh()->SetAngle(lowStage[index]->GetRot());
		lowStage[index]->GetMesh()->SetScale(lowStage[index]->GetScale());
		lowStage[index]->GetMesh()->Update();
	}
}


void COLLISION::Render()
{
	for (unsigned int index = 0; index < lowStage.size(); index++)
	{
		lowStage[index]->Render();
	}
}

//	縦方向判定
bool COLLISION::HeightRay(int index, Vector3 &out, Vector3 pos, Vector3 vec, float dist, float revision)
{
	Vector3 outVec;
	pos.z = 0.0f;
	if (0 > lowStage.at(index)->GetMesh()->RayPick(&outVec, &pos, &vec, &dist))
		return false;
	outVec.y += revision;
	out = outVec;
	return true;
}


//  地面判定用
bool	COLLISION::OnGround(int index, Vector3& out, const Vector3& pos, float delLen, const Vector3& gravity)
{
	if (lowStage[index] == NULL)		return	false;
	Vector3	v = gravity;		v.Normalize();
	Vector3	p = pos + Vector3(0, 1.5f, 0);
	Vector3	o;
	float	dist = 1000.0f;

	int		n = lowStage[index]->GetMesh()->RayPick(&o, &p, &v, &dist);
	if (n < 0)						return	false;

	Vector3	vec = o - p;
	if (vec.Length() > delLen)		return	false;

	out = o;
	return	true;
}

bool COLLISION::ToLocal(const int index, const Vector3& pos, Vector3& local, Vector3& vec)
{
	Matrix mat;
	//逆行列
	if (lowStage[index]->GetKindGround() == STAGE::KIND_GROUND::MOVING)
		D3DXMatrixInverse(&mat, NULL, &lowStage[index]->GetMesh()->TransMatrix);
	else
		mat = lowStage[index]->GetInverseMatrix();

	//ローカル化
	local.x = pos.x*mat._11 + pos.y*mat._21 + pos.z*mat._31 + mat._41;
	local.y = pos.x*mat._12 + pos.y*mat._22 + pos.z*mat._32 + mat._42;
	local.z = pos.x*mat._13 + pos.y*mat._23 + pos.z*mat._33 + mat._43;


	//ローカル位置でのレイピック
	Vector3 p = local + Vector3(0, 3, 0);

	float d = 3.0f;
	Vector3 out;

	if (lowStage[index]->GetMesh()->RayPick(&out, &p, &vec, &d) >= 0)
	{
		local = out;
		return true;
	}
	return false;
}

void COLLISION::ToWorld(const int index, Vector3& inout)
{
	Matrix mat = lowStage[index]->GetMesh()->TransMatrix;
	Vector3 pos = inout;
	inout.x = pos.x*mat._11 + pos.y*mat._21 + pos.z*mat._31 + mat._41;
	inout.y = pos.x*mat._12 + pos.y*mat._22 + pos.z*mat._32 + mat._42;
	inout.z = pos.x*mat._13 + pos.y*mat._23 + pos.z*mat._33 + mat._43;
}

//-----------------------------------------------------------------------------------
//
//		プレイヤーとクリスタルの範囲判定
//
//-----------------------------------------------------------------------------------
bool COLLISION::PlayerToCrystal(PLAYER* player, CRYSTAL* crystal)
{
	struct
	{
		Vector2 lb, lt;
		Vector2 rb, rt;
	}p, c;

	p.lb = Vector2(player->GetPos()) + Vector2(-1.5f, 3.0f);
	p.lt = Vector2(player->GetPos()) + Vector2(-1.5f, 5.0f);
	p.rb = Vector2(player->GetPos()) + Vector2(+1.5f, 3.0f);
	p.rt = Vector2(player->GetPos()) + Vector2(+1.5f, 5.0f);

	c.lb = Vector2(crystal->GetPos()) + Vector2(-5.0f, 0.0f);
	c.lt = Vector2(crystal->GetPos()) + Vector2(-5.0f, 10.0f);
	c.rb = Vector2(crystal->GetPos()) + Vector2(+5.0f, 0.0f);
	c.rt = Vector2(crystal->GetPos()) + Vector2(+5.0f, +10.0f);


#ifdef _DEBUG
	LVERTEX px[4];
	LVERTEX cx[4];

	px[0].x = p.lt.x;
	px[0].y = p.lt.y;
	px[0].z = -5.0f;
	px[0].color = 0xFFFFFFFF;

	px[1].x = p.rt.x;
	px[1].y = p.rt.y;
	px[1].z = -5.0f;
	px[1].color = 0xFFFFFFFF;

	px[2].x = p.lb.x;
	px[2].y = p.lb.y;
	px[2].z = -5.0f;
	px[2].color = 0xFFFFFFFF;

	px[3].x = p.rb.x;
	px[3].y = p.rb.y;
	px[3].z = -5.0f;
	px[3].color = 0xFFFFFFFF;

	cx[0].x = c.lt.x;
	cx[0].y = c.lt.y;
	cx[0].z = 5.0f;
	cx[0].color = 0xFFFFFFFF;

	cx[1].x = c.rt.x;
	cx[1].y = c.rt.y;
	cx[1].z = 5.0f;
	cx[1].color = 0xFFFFFFFF;

	cx[2].x = c.lb.x;
	cx[2].y = c.lb.y;
	cx[2].z = 5.0f;
	cx[2].color = 0xFFFFFFFF;

	cx[3].x = c.rb.x;
	cx[3].y = c.rb.y;
	cx[3].z = 5.0f;
	cx[3].color = 0xFFFFFFFF;

	if (p.lt.x >= c.lt.x &&p.lt.x <= c.rt.x&&p.lt.y >= c.lb.y&&p.lt.y <= c.lt.y)
	{
		for (int i = 0; i < 4; i++)
			px[i].color = 0xFFFF0000;
	}
	if (p.rt.x >= c.lt.x &&p.rt.x <= c.rt.x&&p.rt.y >= c.lb.y&&p.rt.y <= c.lt.y)
	{
		for (int i = 0; i < 4; i++)
			px[i].color = 0xFFFF0000;
	}
	if (p.lb.x >= c.lt.x &&p.lb.x <= c.rt.x&&p.lb.y >= c.lb.y&&p.lb.y <= c.lt.y)	
	{ 
		for (int i = 0; i < 4; i++)
			px[i].color = 0xFFFF0000;
	}
	if (p.rb.x >= c.lt.x &&p.rb.x <= c.rt.x&&p.rb.y >= c.lb.y&&p.rb.y <= c.lt.y)
	{
		for (int i = 0; i < 4; i++)
			px[i].color = 0xFFFF0000;
	}

	iexPolygon::Render3D(px, 2, nullptr, RS_ADD);
	iexPolygon::Render3D(cx, 2, nullptr, RS_ADD);

#endif


	//クリスタル占領
	if (p.lt.x >= c.lt.x &&p.lt.x <= c.rt.x&&p.lt.y >= c.lb.y&&p.lt.y <= c.lt.y)
		return true;
	if (p.rt.x >= c.lt.x &&p.rt.x <= c.rt.x&&p.rt.y >= c.lb.y&&p.rt.y <= c.lt.y)
		return true;
	if (p.lb.x >= c.lt.x &&p.lb.x <= c.rt.x&&p.lb.y >= c.lb.y&&p.lb.y <= c.lt.y)
		return true;
	if (p.rb.x >= c.lt.x &&p.rb.x <= c.rt.x&&p.rb.y >= c.lb.y&&p.rb.y <= c.lt.y)
		return true;
	return false;
}

//------------------------------------------------------------------------------------
//
//		プレイヤーと魔法の衝突判定
//
//------------------------------------------------------------------------------------
float COLLISION::PlayerToMagic(const Vector3& pos, MAGIC* magic)
{
	if (!magic->GetAlive()) return 0.0f;

	MOVER sphere;
	sphere.pos.x = magic->GetPos().x;
	sphere.pos.y = magic->GetPos().y;

	sphere.radius = magic->GetScale().x;

	Vector2 min_pos(pos.x - 3.0f, pos.y - 1.0f);
	Vector2 max_pos(pos.x + 3.0f, pos.y + 15.0f);

	Vector2 square[4] =
	{
		{ min_pos.x, max_pos.y },
		{ max_pos.x, max_pos.y },
		{ min_pos.x, min_pos.y },
		{ max_pos.x, min_pos.y }
	};


	LVERTEX cv[4];

	for (int i = 0; i < 4; i++)
	{
		cv[i].x = square[i].x;
		cv[i].y = square[i].y;
		cv[i].z = -2.0f;
		cv[i].color = 0xFFFFFFFF;
	}

	if (max_pos.y <= (sphere.pos.y - sphere.radius) || (sphere.pos.y + sphere.radius) <= min_pos.y) return 0.0f;

	//各頂点へのベクトル
	Vector2 v0 = sphere.pos - square[1];

	//射影ベクトル作成
	Vector2 vy = Vector2(0.0f, sphere.pos.y - square[1].y);
	Vector2 nv = v0;
	nv.Normalize();
	vy.Normalize();
	Vector2 vx = v0*acosf(nv.Dot(vy));
	float l = vx.Length() - sphere.radius;
	if (l < .0f)
		magic->GetPower();

	v0 = sphere.pos - square[0];
	vy = Vector2(0.0f, sphere.pos.y - square[0].y);
	nv = v0;
	nv.Normalize();
	vy.Normalize();
	vx = v0*acosf(nv.Dot(vy));
	l = vx.Length() - sphere.radius;
	if (l < .0f)
		return magic->GetPower();

	return 0.0f;

}

float COLLISION::PlayerMagic(PLAYER* p, MAGIC* magic)
{
	if (!magic->GetAlive()) return 0.0f;

	MOVER sphere;
	sphere.pos.x = magic->GetPos().x;
	sphere.pos.y = magic->GetPos().y;

	sphere.radius = magic->GetScale().x;

	Vector2 min_pos(p->GetPos().x - 3.0f, p->GetPos().y - 8.5f);
	Vector2 max_pos(p->GetPos().x + 3.0f, p->GetPos().y + 3.0f);

	Vector2 square[4] =
	{
		{ min_pos.x, max_pos.y },
		{ max_pos.x, max_pos.y },
		{ min_pos.x, min_pos.y },
		{ max_pos.x, min_pos.y }
	};


	LVERTEX cv[4];

	for (int i = 0; i < 4; i++)
	{
		cv[i].x = square[i].x;
		cv[i].y = square[i].y;
		cv[i].z = -2.0f;
		cv[i].color = 0xFFFFFFFF;

	}


	if (max_pos.y <= (sphere.pos.y - sphere.radius) || (sphere.pos.y + sphere.radius) <= min_pos.y) return 0.0f;

	//各頂点へのベクトル
	Vector2 v0 = sphere.pos - square[1];

	//射影ベクトル作成
	Vector2 vy = Vector2(0.0f, sphere.pos.y - square[1].y);
	Vector2 nv = v0;
	nv.Normalize();
	vy.Normalize();
	Vector2 vx = v0*acosf(nv.Dot(vy));
	float l = vx.Length() - sphere.radius;
	if (l < .0f)
		magic->GetPower();

	v0 = sphere.pos - square[0];
	vy = Vector2(0.0f, sphere.pos.y - square[0].y);
	nv = v0;
	nv.Normalize();
	vy.Normalize();
	vx = v0*acosf(nv.Dot(vy));
	l = vx.Length() - sphere.radius;
	if (l < .0f)
		return magic->GetPower();

	return 0.0f;
}

//  魔法と魔法の当たり判定
int COLLISION::MagicMagic(MAGIC* m1, MAGIC* m2)
{
	if (!m1->GetAlive()) return -1;
	if (!m2->GetAlive()) return -1;

	MOVER sphere1, sphere2;

	sphere1.pos.x = m1->GetPos().x;
	sphere1.pos.y = m1->GetPos().y;
	sphere1.radius = m1->GetScale().x;

	sphere2.pos.x = m2->GetPos().x;
	sphere2.pos.y = m2->GetPos().y;
	sphere2.radius = m2->GetScale().x;

	if (sphere1.pos.x + sphere1.radius<sphere2.pos.x - sphere2.radius || sphere1.pos.x - sphere1.radius>sphere2.pos.x + sphere2.radius)return -1;
	if (sphere1.pos.y + sphere1.radius<sphere2.pos.y - sphere2.radius || sphere1.pos.y - sphere1.radius>sphere2.pos.y + sphere2.radius)return -1;

	Vector2 v1 = sphere1.pos - sphere2.pos;
	Vector2 v2 = v1;
	v2.Normalize();
	v2 = v2*(sphere1.radius + sphere2.radius);
	if (v1.Length() <= v2.Length())
	{
		if (sphere1.radius > sphere2.radius)return 2;
		if (sphere1.radius < sphere2.radius)return 1;
		return 0;
	}
	return -1;
}

//  使い魔の魔法とプレイヤーの衝突判定
bool COLLISION::PartnerMagicToPlayer(PLAYER* player, const PARTNER_MAGIC& magic, CAMERA* camera)
{
	//  使い魔の弾とプレイヤーのスクリーン座標
	Vector3 Output0, Output1;
	camera->Screentransformation(Output0, magic.GetPos());
	camera->Screentransformation(Output1, player->GetPos());

	//  使い魔の弾とプレイヤーの矩形
	Vector3 magicCorner[4], playerCorner[4];

	magicCorner[0] = Vector3(Output0.x + 10.0f, Output0.y + 7.0f, 0);
	magicCorner[1] = Vector3(Output0.x + 10.0f, Output0.y - 7.0f, 0);
	magicCorner[2] = Vector3(Output0.x - 10.0f, Output0.y + 7.0f, 0);
	magicCorner[3] = Vector3(Output0.x - 10.0f, Output0.y - 7.0f, 0);

	playerCorner[0] = Vector3(Output1.x + 40.0f, Output1.y - 30.0f, 0);
	playerCorner[1] = Vector3(Output1.x + 40.0f, Output1.y - 170.0f, 0);
	playerCorner[2] = Vector3(Output1.x - 40.0f, Output1.y - 30.0f, 0);
	playerCorner[3] = Vector3(Output1.x - 40.0f, Output1.y - 170.0f, 0);

#ifdef _DEBUG
	//  当たり判定用描画
	image->Render(magicCorner[0].x, magicCorner[0].y, 5, 5, 0, 0, 10, 10);
	image->Render(magicCorner[1].x, magicCorner[1].y, 5, 5, 0, 0, 10, 10);
	image->Render(magicCorner[2].x, magicCorner[2].y, 5, 5, 0, 0, 10, 10);
	image->Render(magicCorner[3].x, magicCorner[3].y, 5, 5, 0, 0, 10, 10);

	image->Render(playerCorner[0].x, playerCorner[0].y, 5, 5, 0, 0, 10, 10);
	image->Render(playerCorner[1].x, playerCorner[1].y, 5, 5, 0, 0, 10, 10);
	image->Render(playerCorner[2].x, playerCorner[2].y, 5, 5, 0, 0, 10, 10);
	image->Render(playerCorner[3].x, playerCorner[3].y, 5, 5, 0, 0, 10, 10);
#endif  //  _DEBUG

	if (magicCorner[0].x < playerCorner[2].x) return false;
	if (magicCorner[2].x > playerCorner[0].x) return false;
	if (magicCorner[1].y > playerCorner[0].y) return false;
	if (magicCorner[0].y < playerCorner[1].y) return false;

#ifdef _DEBUG
	//  当たり判定用描画
	image->Render(magicCorner[0].x, magicCorner[0].y, 5, 5, 16, 0, 10, 10);
	image->Render(magicCorner[1].x, magicCorner[1].y, 5, 5, 16, 0, 10, 10);
	image->Render(magicCorner[2].x, magicCorner[2].y, 5, 5, 16, 0, 10, 10);
	image->Render(magicCorner[3].x, magicCorner[3].y, 5, 5, 16, 0, 10, 10);

	image->Render(playerCorner[0].x, playerCorner[0].y, 5, 5, 16, 0, 10, 10);
	image->Render(playerCorner[1].x, playerCorner[1].y, 5, 5, 16, 0, 10, 10);
	image->Render(playerCorner[2].x, playerCorner[2].y, 5, 5, 16, 0, 10, 10);
	image->Render(playerCorner[3].x, playerCorner[3].y, 5, 5, 16, 0, 10, 10);
#endif  //  _DEBUG

	if (player->GetMode() < CHARACTOR::STATE::INVINCIBLE) {
		player->SubtractHP(magic.getPower());
		player->Setstate(CHARACTOR::STATE::DAMAGE);
	}
	return true;
}

//  使い魔の物理攻撃とプレイヤーの衝突判定
bool COLLISION::PartnerAttackToPlayer(PLAYER* player, const CHARACTOR& partner, CAMERA* camera)
{
	//  使い魔の弾とプレイヤーのスクリーン座標
	Vector3 Output0, Output1;
	camera->Screentransformation(Output0, partner.GetPos());
	camera->Screentransformation(Output1, player->GetPos());

	//  使い魔の弾とプレイヤーの矩形
	Vector3 partnerCorner[4], playerCorner[4];

	if (partner.GetAngle() == CHARACTOR::DIST::RIGHT)
	{
		partnerCorner[0] = Vector3(Output0.x + 80.0f, Output0.y + 7.0f, 0);
		partnerCorner[1] = Vector3(Output0.x + 80.0f, Output0.y - 40.0f, 0);
		partnerCorner[2] = Vector3(Output0.x + 40.0f, Output0.y + 7.0f, 0);
		partnerCorner[3] = Vector3(Output0.x + 40.0f, Output0.y - 40.0f, 0);

		playerCorner[0] = Vector3(Output1.x + 40.0f, Output1.y - 30.0f, 0);
		playerCorner[1] = Vector3(Output1.x + 40.0f, Output1.y - 170.0f, 0);
		playerCorner[2] = Vector3(Output1.x - 40.0f, Output1.y - 30.0f, 0);
		playerCorner[3] = Vector3(Output1.x - 40.0f, Output1.y - 170.0f, 0);
	}

	if (partner.GetAngle() == CHARACTOR::DIST::LEFT)
	{
		partnerCorner[0] = Vector3(Output0.x - 40.0f, Output0.y + 7.0f, 0);
		partnerCorner[1] = Vector3(Output0.x - 40.0f, Output0.y - 40.0f, 0);
		partnerCorner[2] = Vector3(Output0.x - 80.0f, Output0.y + 7.0f, 0);
		partnerCorner[3] = Vector3(Output0.x - 80.0f, Output0.y - 40.0f, 0);

		playerCorner[0] = Vector3(Output1.x + 40.0f, Output1.y - 30.0f, 0);
		playerCorner[1] = Vector3(Output1.x + 40.0f, Output1.y - 170.0f, 0);
		playerCorner[2] = Vector3(Output1.x - 40.0f, Output1.y - 30.0f, 0);
		playerCorner[3] = Vector3(Output1.x - 40.0f, Output1.y - 170.0f, 0);
	}

#ifdef _DEBUG
	//  当たり判定用描画
	image->Render(partnerCorner[0].x, partnerCorner[0].y, 5, 5, 0, 0, 10, 10);
	image->Render(partnerCorner[1].x, partnerCorner[1].y, 5, 5, 0, 0, 10, 10);
	image->Render(partnerCorner[2].x, partnerCorner[2].y, 5, 5, 0, 0, 10, 10);
	image->Render(partnerCorner[3].x, partnerCorner[3].y, 5, 5, 0, 0, 10, 10);

	image->Render(playerCorner[0].x, playerCorner[0].y, 5, 5, 0, 0, 10, 10);
	image->Render(playerCorner[1].x, playerCorner[1].y, 5, 5, 0, 0, 10, 10);
	image->Render(playerCorner[2].x, playerCorner[2].y, 5, 5, 0, 0, 10, 10);
	image->Render(playerCorner[3].x, playerCorner[3].y, 5, 5, 0, 0, 10, 10);
#endif  //  _DEBUG

	if (partnerCorner[0].x < playerCorner[2].x) return false;
	if (partnerCorner[2].x > playerCorner[0].x) return false;
	if (partnerCorner[1].y > playerCorner[0].y) return false;
	if (partnerCorner[0].y < playerCorner[1].y) return false;

	if (player->GetMode() != CHARACTOR::STATE::INVINCIBLE) {
		player->SubtractHP(2);
		player->Setstate(CHARACTOR::STATE::DAMAGE);
	}

	return true;
}

//  クリスタルの使い魔とプレイヤーの魔法攻撃との衝突判定
bool COLLISION::ClystalPartnerToMagic(CLYSTAL_PARTNER* partner, CAMERA* camera)
{
	//  使い魔と、プレイヤーの魔法のスクリーン座標
	Vector3 Output0, Output1;
	camera->Screentransformation(Output0, partner->GetPos());
	camera->Screentransformation(Output1, partner->GetOpponentPlayer()->GetMagic()->GetPos());

	//  使い魔の弾とプレイヤーの矩形
	Vector3 partnerCorner[4], playerMagicCorner[4];

	partnerCorner[0] = Vector3(Output0.x + 70.0f, Output0.y - 20.0f, 0);
	partnerCorner[1] = Vector3(Output0.x + 70.0f, Output0.y - 130.0f, 0);
	partnerCorner[2] = Vector3(Output0.x - 70.0f, Output0.y - 20.0f, 0);
	partnerCorner[3] = Vector3(Output0.x - 70.0f, Output0.y - 130.0f, 0);

	playerMagicCorner[0] = Vector3(Output1.x + 40.0f, Output1.y - 30.0f, 0);
	playerMagicCorner[1] = Vector3(Output1.x + 40.0f, Output1.y - 170.0f, 0);
	playerMagicCorner[2] = Vector3(Output1.x - 40.0f, Output1.y - 30.0f, 0);
	playerMagicCorner[3] = Vector3(Output1.x - 40.0f, Output1.y - 170.0f, 0);

#ifdef _DEBUG
	//  当たり判定用描画
	image->Render(partnerCorner[0].x, partnerCorner[0].y, 5, 5, 0, 0, 10, 10);
	image->Render(partnerCorner[1].x, partnerCorner[1].y, 5, 5, 0, 0, 10, 10);
	image->Render(partnerCorner[2].x, partnerCorner[2].y, 5, 5, 0, 0, 10, 10);
	image->Render(partnerCorner[3].x, partnerCorner[3].y, 5, 5, 0, 0, 10, 10);

	image->Render(playerMagicCorner[0].x, playerMagicCorner[0].y, 5, 5, 0, 0, 10, 10);
	image->Render(playerMagicCorner[1].x, playerMagicCorner[1].y, 5, 5, 0, 0, 10, 10);
	image->Render(playerMagicCorner[2].x, playerMagicCorner[2].y, 5, 5, 0, 0, 10, 10);
	image->Render(playerMagicCorner[3].x, playerMagicCorner[3].y, 5, 5, 0, 0, 10, 10);
#endif  //  _DEBUG

	if (partnerCorner[0].x < playerMagicCorner[2].x) return false;
	if (partnerCorner[2].x > playerMagicCorner[0].x) return false;
	if (partnerCorner[1].y > playerMagicCorner[0].y) return false;
	if (partnerCorner[0].y < playerMagicCorner[1].y) return false;

#ifdef _DEBUG
	image->Render(partnerCorner[0].x, partnerCorner[0].y, 5, 5, 16, 0, 10, 10);
	image->Render(partnerCorner[1].x, partnerCorner[1].y, 5, 5, 16, 0, 10, 10);
	image->Render(partnerCorner[2].x, partnerCorner[2].y, 5, 5, 16, 0, 10, 10);
	image->Render(partnerCorner[3].x, partnerCorner[3].y, 5, 5, 16, 0, 10, 10);

	image->Render(playerMagicCorner[0].x, playerMagicCorner[0].y, 5, 5, 16, 0, 10, 10);
	image->Render(playerMagicCorner[1].x, playerMagicCorner[1].y, 5, 5, 16, 0, 10, 10);
	image->Render(playerMagicCorner[2].x, playerMagicCorner[2].y, 5, 5, 16, 0, 10, 10);
	image->Render(playerMagicCorner[3].x, playerMagicCorner[3].y, 5, 5, 16, 0, 10, 10);
#endif  //  _DEBUG

	return true;
}

//  罠と敵プレイヤーとの衝突判定
bool COLLISION::TrapToPlayer(PLAYER* player, TRAP* trap, CAMERA* camera)
{
	//  使い魔と、トラップのスクリーン座標
	Vector3 Output0, Output1;
	camera->Screentransformation(Output0, player->GetPos());
	camera->Screentransformation(Output1, trap->GetPos());

	//  プレイヤーとトラップの矩形
	Vector3 playerCorner[4], TrapCorner[4];

	playerCorner[0] = Vector3(Output0.x + 40.0f, Output0.y - 30.0f, 0);
	playerCorner[1] = Vector3(Output0.x + 40.0f, Output0.y - 170.0f, 0);
	playerCorner[2] = Vector3(Output0.x - 40.0f, Output0.y - 30.0f, 0);
	playerCorner[3] = Vector3(Output0.x - 40.0f, Output0.y - 170.0f, 0);

	TrapCorner[0] = Vector3(Output1.x + 40.0f, Output1.y, 0);
	TrapCorner[1] = Vector3(Output1.x + 40.0f, Output1.y - 100.0f, 0);
	TrapCorner[2] = Vector3(Output1.x - 40.0f, Output1.y, 0);
	TrapCorner[3] = Vector3(Output1.x - 40.0f, Output1.y - 100.0f, 0);

#ifdef _DEBUG
	//  当たり判定用描画
	image->Render(playerCorner[0].x, playerCorner[0].y, 5, 5, 0, 0, 10, 10);
	image->Render(playerCorner[1].x, playerCorner[1].y, 5, 5, 0, 0, 10, 10);
	image->Render(playerCorner[2].x, playerCorner[2].y, 5, 5, 0, 0, 10, 10);
	image->Render(playerCorner[3].x, playerCorner[3].y, 5, 5, 0, 0, 10, 10);

	image->Render(TrapCorner[0].x, TrapCorner[0].y, 5, 5, 0, 0, 10, 10);
	image->Render(TrapCorner[1].x, TrapCorner[1].y, 5, 5, 0, 0, 10, 10);
	image->Render(TrapCorner[2].x, TrapCorner[2].y, 5, 5, 0, 0, 10, 10);
	image->Render(TrapCorner[3].x, TrapCorner[3].y, 5, 5, 0, 0, 10, 10);
#endif  //  _DEBUG

	if (playerCorner[0].x < TrapCorner[2].x) return false;
	if (playerCorner[2].x > TrapCorner[0].x) return false;
	if (playerCorner[1].y > TrapCorner[0].y) return false;
	if (playerCorner[0].y < TrapCorner[1].y) return false;

#ifdef _DEBUG
	image->Render(playerCorner[0].x, playerCorner[0].y, 5, 5, 16, 0, 10, 10);
	image->Render(playerCorner[1].x, playerCorner[1].y, 5, 5, 16, 0, 10, 10);
	image->Render(playerCorner[2].x, playerCorner[2].y, 5, 5, 16, 0, 10, 10);
	image->Render(playerCorner[3].x, playerCorner[3].y, 5, 5, 16, 0, 10, 10);

	image->Render(TrapCorner[0].x, TrapCorner[0].y, 5, 5, 16, 0, 10, 10);
	image->Render(TrapCorner[1].x, TrapCorner[1].y, 5, 5, 16, 0, 10, 10);
	image->Render(TrapCorner[2].x, TrapCorner[2].y, 5, 5, 16, 0, 10, 10);
	image->Render(TrapCorner[3].x, TrapCorner[3].y, 5, 5, 16, 0, 10, 10);
#endif  //  _DEBUG

	//if (player->GetMode() != CHARACTOR::STATE::INVINCIBLE) {
	//	player->SubtractHP(1);
	//	player->Setstate(CHARACTOR::STATE::DAMAGE);
	//}

	return true;
}

bool COLLISION::ExpToPlayer(PLAYER* player, const Vector3& pos, CAMERA* camera)
{
	//  プレイヤーのスクリーン座標
	Vector3 Output;
	camera->Screentransformation(Output, player->GetPos());

	//  プレイヤーとトラップの矩形
	Vector3 playerCorner[4], ExpCorner[4];

	playerCorner[0] = Vector3(Output.x + 40.0f, Output.y - 30.0f, 0);
	playerCorner[1] = Vector3(Output.x + 40.0f, Output.y - 170.0f, 0);
	playerCorner[2] = Vector3(Output.x - 40.0f, Output.y - 30.0f, 0);
	playerCorner[3] = Vector3(Output.x - 40.0f, Output.y - 170.0f, 0);

	ExpCorner[0] = Vector3(pos.x + 80, pos.y + 50, 0);
	ExpCorner[1] = Vector3(pos.x + 80, pos.y - 100, 0);
	ExpCorner[2] = Vector3(pos.x - 80, pos.y + 50, 0);
	ExpCorner[3] = Vector3(pos.x - 80, pos.y - 100, 0);

#ifdef _DEBUG
	//  当たり判定用描画
	image->Render(playerCorner[0].x, playerCorner[0].y, 5, 5, 0, 0, 10, 10);
	image->Render(playerCorner[1].x, playerCorner[1].y, 5, 5, 0, 0, 10, 10);
	image->Render(playerCorner[2].x, playerCorner[2].y, 5, 5, 0, 0, 10, 10);
	image->Render(playerCorner[3].x, playerCorner[3].y, 5, 5, 0, 0, 10, 10);

	image->Render(ExpCorner[0].x, ExpCorner[0].y, 5, 5, 0, 0, 10, 10);
	image->Render(ExpCorner[1].x, ExpCorner[1].y, 5, 5, 0, 0, 10, 10);
	image->Render(ExpCorner[2].x, ExpCorner[2].y, 5, 5, 0, 0, 10, 10);
	image->Render(ExpCorner[3].x, ExpCorner[3].y, 5, 5, 0, 0, 10, 10);
#endif  //  _DEBUG


	if (playerCorner[0].x < ExpCorner[2].x) return false;
	if (playerCorner[2].x > ExpCorner[0].x) return false;
	if (playerCorner[1].y > ExpCorner[0].y) return false;
	if (playerCorner[0].y < ExpCorner[1].y) return false;

	if (player->GetMode() != CHARACTOR::STATE::INVINCIBLE) {
		player->SubtractHP(1);
		player->Setstate(CHARACTOR::STATE::DAMAGE);
	}

	return true;
}