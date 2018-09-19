#pragma once

class TRAP
{
private:
	iex3DObj*	obj;
	std::unique_ptr<iex2DObj> image;
	Vector3		pos;
	Vector3		move;
	iexShader*  shader;
	enum State
	{
		MOVE,         //  ˆÚ“®’†
		INSTALLATION  //  Ý’u’†
	};
	State       state;

	int   playerNum;
	float scale;
	float angle;
	float redUpdate;
	float imagePosMove;
	float Displace;
	bool  imageMoveFlag;
	bool  redUpdateFlag;
	unsigned int explosionTimer;
	unsigned char damage;

	int rideNum;
	Vector3 local;

public:
	//  UNDONE:ˆø”•ÏX
	TRAP(iexShader* shader,Vector3& pos,Vector3& move,int num,float angle,iex3DObj* Obj) :state(MOVE), pos(pos), move(move), scale(1.0f), angle(angle), damage(1),
		redUpdate(0.5f), redUpdateFlag(true), explosionTimer(500),
		Displace(0.009f), playerNum(num), imagePosMove(0.0f), imageMoveFlag(true)
	{
		this->shader = shader;
		//image.reset(new iex2DObj("DATA\\IMAGE\\BOMBCOLOR.PNG"));
		obj = nullptr;
		obj = Obj;
		image = nullptr;
	};
	~TRAP()
	{
		if (obj) { delete obj; obj = nullptr; }
		image.release();
	};

	void Initialize(/*const Vector3&, const Vector3&,int*/);
	void Append();
	void Move();
	void Catch();
	void Erase();

	void ColorUpdate();
	void Update();
	void Render(const Vector3&);

	Vector3 GetPos() { return pos; }
	int GetExpTimer() { return explosionTimer; }
	unsigned char GetDamage() { return damage; }
	void ToLocal();
	void ToWorld();
};