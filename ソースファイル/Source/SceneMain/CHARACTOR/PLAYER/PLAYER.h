#pragma once
#include	"../IEX/iextreme.h"
#include	"SCENEMAIN\CHARACTOR\Character.h"
#include	"Magic.h"
#include	"SceneMain\CAMERA\CAMERA.h"
#include	"SceneMain\EFFECT\Effect.h"


#define jump_power 1.3f


class PLAYER :public CHARACTOR
{
private:
	void Wait();
	void Control();
	void Attack();
	void Damage(float power, PLAYER* p);	//（第一引数：ダメージ量　第二引数：敵プレイヤー）
	void Dead();
	void Invincible(int t);					//（引数：無敵時間(フレーム数)）

	int jump_time = 0;
	int magic_timer = 0;
	int inv_timer = 0;						//無敵時間用（ステート変更前に変更する）
	bool inv_jump = false;
	bool crystalhold = false;

	std::shared_ptr<MAGIC>	magic;
	float magicPower;

	//std::unique_ptr<iex2DObj> particle;		//仮エフェクト用
	std::unique_ptr<CHARGE> charge;
	std::unique_ptr<_EFFECT> _effect;


protected:
	int  jump_flg;		//ジャンプ用（0：地面　1：空中）
	int shot_flg;	//ショット用（0：弾でてない　1：チャージ中　2：弾でてる）

	Vector3 scale;

	int Char;	//キャラ番号（0　or　1）

public:
	PLAYER() :CHARACTOR() {}
	PLAYER(char* filename) :CHARACTOR(filename) {}
	~PLAYER() {  }


	void Initialize() {};
	void Initialize(const Vector3&)override {}				//初期化（位置）
	void Initialize(Vector3 pos, int angle, int n, int char_no);		//初期化（位置,　向き,　コントローラー番号,キャラ番号）
	void Respawn(Vector3 pos, int angle);				//再起（位置, 向き）
	void Update()override;
	void Update(const Vector3& epos)override {};			//更新（敵位置）
	void UPdate(PLAYER* p);								//更新（敵プレイヤー）
	void Render(int char_no)override{}
	void Render(CAMERA* camera);
	void SetMotion(int num) { if (obj->GetMotion() != num) obj->SetMotion(num); }

	int Result();
	void ResultRender();

	//void Effect_Render(CAMERA* camera, Vector3 pos);	//仮エフェクト（カメラ　, 位置）

	//ゲッター
	MAGIC* GetMagic() { return magic.get(); };
	int Getmagic_timer() { return magic_timer; }
	float Getlife() { return life; }
	float GetmagicPower() { return magicPower; }
	DIST GetmagicAngle() { return GetMagic()->GetAngle(); }
	int GetDist() { return (int)angle; }
	bool Gethold() { return crystalhold; }
	Vector3 Getmove() { return move; }
	int GetNo() { return (int)no; }

	//セッター
	void Setstate(STATE s) { state = s; }
	void SetmagicPower(float p) { magicPower = p; }
	void SetJumpFlg(int n) { jump_flg = n; }
	void SetmoveY(float y) { move.y = y; }
	void Sethold(bool f) { crystalhold = f; }

	void SubtractHP(unsigned char damage)
	{
		life -= damage;
	}

};

