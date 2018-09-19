#pragma once

#include	<memory>
struct Vector3;

class PARTNER_MAGIC
{
private:
	std::unique_ptr<iex3DObj>	obj;
	iexShader* shader;

	Vector3	pos;
	Vector3	move;
	float angle;
	float scale;

	unsigned int playerNum;

	unsigned char magicPower;
public:
	PARTNER_MAGIC(const Vector3& p, const Vector3& v,iexShader* shader,int plNum) :pos(p + Vector3(0, 10, 0)), move(v), angle(PI),
		scale(1.0f), magicPower(1) 
	{
		obj.reset(new iex3DObj("DATA\\magic\\magic.IEM"));
		this->shader = shader;
		playerNum = plNum;
	};

	~PARTNER_MAGIC()
	{
		this->shader = nullptr;
	}

	//void Append(const Vector3&, const Vector3&);
	void Move();

	void Erase();
	void Render(int char_no);

	//  ÉQÉbÉ^Å[
	Vector3 GetPos() { return pos; }
	Vector3 GetPos() const { return pos; }

	float   GetScale() { return scale; }
	unsigned char getPower()const { return magicPower; }
};
