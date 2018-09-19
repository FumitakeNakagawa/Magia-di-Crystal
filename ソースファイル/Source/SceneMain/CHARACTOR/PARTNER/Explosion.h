#pragma once

class EXPLOSION
{
private:
	std::unique_ptr<iex2DObj>   obj;
	CAMERA* camera;
	Vector3 trapPos;
	Vector3 Output;
	unsigned int animation;
	int timer;

public:
	EXPLOSION(const Vector3&,CAMERA*);
	~EXPLOSION();
	void Update();
	void Render();

	Vector3 GetPos() { return Output; }
	int GetAnimation() { return animation; }
};
