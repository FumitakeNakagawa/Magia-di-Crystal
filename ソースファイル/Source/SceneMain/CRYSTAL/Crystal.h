#pragma once
#include	"iextreme.h"
#include	"SceneMain\EFFECT\Effect.h"
#include	<memory>

class CRYSTAL
{
public:
	enum class STATE : unsigned int
	{
		NONE,		//何もしていない
		CHARGE, //チャージ中
		HOLD,		//占拠されている
		LOST,			//ロスト中

		END
	};
private:
	static int holdCnt[2];
	static iexMesh* obj;
	STATE	state;
	std::unique_ptr<iexShader> shader;
	std::unique_ptr<_EFFECT> effect;
	iexMesh* clone;
	Vector3 pos, rot, scale;
	Vector3 revision;//高さ補正値
	float holdGage;

	bool charge;
	float boost;//クリスタル増減加速


	struct
	{
		int step;
		bool flg;
		int number;
	}lost;

	//占領されているか
	//-1:占領されていない
	//0:1P
	//1:2P
	int isHold;

	float min, max;

	int parentNum;
private:
	//占拠されているクリスタルの判定

public:
	CRYSTAL();
	//	~CRYSTAL() { clone = nullptr; }
	void LostCrystal(int no);
	virtual void Initialize(int parenNum);
	virtual void Update();
	virtual void Render(CAMERA* camera, int char_no1, int char_no2);
	void ResultRender();
	virtual void Render();
	virtual void Release();

	//占拠するまでの時間のインクリメント・デクリメント
	void PlusGage(int speed, int no);
	void	MinusGage(int speed, int no);

	iexMesh* GetMesh() { return clone; }

	void SetPos(Vector3 v) { pos = v; }
	Vector3 GetPos() { return pos; }

	void SetRevision(Vector3& v) { revision = v; }
	Vector3& GetRevision() { return revision; }


	STATE GetState() { return state; }
	void SetState(STATE s) { state = s; }

	float GetGage() { return holdGage; }


	int GetisHold() { return isHold; }	//UI用（α用）


	int GetParentNum() { return parentNum; }
	void SetParent(int num) { parentNum = num; }

	bool GetCharge() { return charge; }
	void SetCharge(bool ch) { charge = ch; }


	void SetMax(float max) { this->max =max*60.0f; }
	void SetMin(float min) { this->min =min*60.0f; }

	_EFFECT* GetEffect() { return effect.get(); }

	static void SetObject(char* filename) { obj = (new iexMesh(filename)); }
	static void SetObject(iexMesh* mesh) { obj = mesh; }

	static void ReleaseObject() { if (obj) { delete obj; obj = nullptr; } }
	static void SetHoldCnt(int n, int value) { holdCnt[n] = value; }
	static unsigned int GetHoldCnt(int n) { return holdCnt[n]; }
};


