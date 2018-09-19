#pragma once

#include	"iextreme.h"
#include	<memory>


class STAGE
{
public:
	static iex2DObj* bg;
	enum class KIND_GROUND : int
	{
		FIXED,	//ŒÅ’è‚µ‚½’n–Ê
		MOVING,	//“®‚­’n–Ê

		KIND_MAX
	};

private:


protected:
	std::shared_ptr<iex2DObj> effect;
	std::shared_ptr<iexShader> shader;
	KIND_GROUND kindGround;
	std::shared_ptr<iexMesh> mesh;
	Vector3 pos, scale, rot;
	Matrix invMat;



	Vector3 begin, end;
	float speed;
	float moveDist;
	float moveDirect;
	float second;
public:
	void Load(iexMesh*mesh);
	void Set(Vector3& pos, Vector3& rot, Vector3& scale);

	void Initialize(iexMesh* mesh, int k);
	void Update();
	void Render();
	void ResultRender();



	void SetPos(Vector3 v) { pos = v; }
	void SetScale(Vector3 v) { scale = v; }
	void SetRot(Vector3 v) { rot = v; }

	void SetSpeed(float s) { speed = s; }
	void SetMoveDist(float dist) { moveDist = dist; }
	void SetMoveDirect(float direct) { moveDirect = direct; }
	void SetSecond(float s) { second = s; }

	void SetBegin(Vector3& v) { begin = v; }
	void SetEnd(Vector3& v) { end = v; }

	void SetKind(int num) { kindGround = (KIND_GROUND)num; }

	void MeshUpdate();

	Vector3 GetPos() { return pos; }
	Vector3 GetScale() { return scale; }
	Vector3 GetRot() { return rot; }

	KIND_GROUND GetKindGround() { return kindGround; }

	iexMesh* GetMesh() { return mesh.get(); }
	Matrix& GetInverseMatrix() { return invMat; }

	static void SetBG(iex2DObj* bg) { STAGE::bg = bg; }
	static iex2DObj* GetBG() { return bg; }
	static void ReleaseBG() { if (bg) { delete bg; bg = nullptr; } }
	//void Release() {if (cloud) { delete cloud; cloud = nullptr; } }
};