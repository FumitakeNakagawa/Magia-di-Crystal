#include	"stage.h"
#include	<time.h>

iex2DObj* STAGE::bg = nullptr;

void STAGE::Load(iexMesh* mesh)
{
	this->mesh.reset(mesh);
	effect.reset(new iex2DObj("DATA\\STAGE\\RUIN_EFFECT.png"));
	shader = std::make_unique<iexShader>((char*)"DATA\\Shader\\3DEX.fx");
	srand((unsigned int)time(NULL));
}

void STAGE::Set(Vector3& pos, Vector3& rot, Vector3& scale)
{
	this->pos = pos;
	this->rot = rot;
	this->scale = scale;
}

void STAGE::Initialize(iexMesh* mesh, int k)
{
	this->mesh.reset(mesh);
	pos = Vector3(0.0f, 0.0f, 0.0f);
	scale = Vector3(1.0f, 1.0f, 1.0f);
	rot = Vector3(0.0f, 0.0f, 0.0f);



	kindGround = (KIND_GROUND)k;
}

void STAGE::Update()
{
	if (kindGround == KIND_GROUND::MOVING)
	{
		Vector3 move = end - begin;
		float length = move.Length();
		move.Normalize();

		if (moveDist<.0f || moveDist > length)
			moveDirect *= -1;

		speed = ((length / second) / 60.0f)*moveDirect;
		moveDist += speed;
		pos = begin + move*moveDist;
	}


	mesh->SetPos(pos);
	mesh->SetScale(scale);
	mesh->SetAngle(rot);
	mesh->Update();
}

void STAGE::MeshUpdate()
{
	mesh->SetPos(pos);
	mesh->SetScale(scale);
	mesh->SetAngle(rot);
	mesh->Update();

	D3DXMatrixInverse(&invMat, NULL, &mesh->TransMatrix);
}

void STAGE::Render()
{
	mesh->SetPos(pos);
	mesh->SetScale(scale);
	mesh->SetAngle(rot);
	mesh->Update();

	mesh->Render();
}


void STAGE::ResultRender()
{
	if (kindGround == KIND_GROUND::MOVING)return;
	mesh->SetPos(pos);
	mesh->SetScale(scale);
	mesh->SetAngle(rot);
	mesh->Update();

	mesh->Render(shader.get(), "copy2");
}