#pragma once
#include	"iextreme.h"
#include	"OTHER\Vector2.h"
#include	<vector>

//  UNDONE:追加しました
//  ドラゴンの床判定戻したら削除してください
#include	"SceneMain\STAGE\Stage.h"


class CRYSTAL;
class CHARACTOR;
class MAGIC;
class PLAYER;
class STAGE;
class PARTNER_MAGIC;
class CAMERA;
class ATTACK_PARTNER;
class CLYSTAL_PARTNER;
class TRAP;

class COLLISION
{
private:
	struct MOVER
	{
		Vector2 pos;
		float radius;
	};
	static std::vector<STAGE*> lowStage;//当たり判定用モデル(低ポリゴン)
	static iex2DObj* image;

public:
	static void Load(unsigned int index, iexMesh* mesh);
	static void Initialize(iexMesh* mesh, Vector3& pos, Vector3& rot, Vector3& scale);
	static void Initialize(DWORD32 index, iexMesh* mesh, int kind, const Vector3& pos, const Vector3& rot, const Vector3& scale);
	static void Release();
	static void Update(int index, Vector3& pos, Vector3& rot, Vector3& scale);
	static void Render();

	//プレイヤと地面との判定
	static bool ToLocal(const int index, const Vector3& pos, Vector3& local, Vector3& vec);
	static void ToWorld(const int index, Vector3& inout);


	static bool HeightRay(int index, Vector3& out, Vector3 pos, Vector3 vec, float dist, float revision);
	static bool OnGround(int index, Vector3& out, const Vector3&pos,
		float delLen = 2.0f, const Vector3& gravity = Vector3(0, -1.0f, 0));

	//プレイヤとクリスタルとの判定
	static bool PlayerToCrystal(PLAYER* p1, CRYSTAL* crystal);

	
	//  使い魔の魔法とプレイヤーとの衝突判定
	static bool PartnerMagicToPlayer(PLAYER*, const PARTNER_MAGIC&, CAMERA*);

	//  攻撃型使い魔の物理攻撃とプレイヤーとの衝突判定
	static bool PartnerAttackToPlayer(PLAYER*, const CHARACTOR&, CAMERA*);

	//  クリスタルの使い魔とプレイヤーの魔法攻撃との衝突判定
	static bool ClystalPartnerToMagic(CLYSTAL_PARTNER*, CAMERA*);

	//  罠と敵プレイヤーとの衝突判定
	static bool TrapToPlayer(PLAYER*, TRAP*, CAMERA*);
	
	//  爆発とプレイヤーの衝突判定
	static bool ExpToPlayer(PLAYER*, const Vector3&, CAMERA*);

	//プレイヤーと魔法の衝突判定
	static float PlayerToMagic(const Vector3& pos, MAGIC* magic);
	static float PlayerMagic(PLAYER* p, MAGIC* magic);

	static std::vector<STAGE*>* GetLowStage() { return &lowStage; }

	//  UNDONE:追加しました
	//  ドラゴンの動く床の判定がいるのであれば削除してください。
	static STAGE::KIND_GROUND GetLowStageKindGround(int i) { return lowStage[i]->GetKindGround(); }


	//  魔法と魔法の当たり判定
	static int MagicMagic(MAGIC* m1, MAGIC* m2);

};