#pragma once
#include	"iextreme.h"
#include	"OTHER\Vector2.h"
#include	<vector>

//  UNDONE:�ǉ����܂���
//  �h���S���̏�����߂�����폜���Ă�������
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
	static std::vector<STAGE*> lowStage;//�����蔻��p���f��(��|���S��)
	static iex2DObj* image;

public:
	static void Load(unsigned int index, iexMesh* mesh);
	static void Initialize(iexMesh* mesh, Vector3& pos, Vector3& rot, Vector3& scale);
	static void Initialize(DWORD32 index, iexMesh* mesh, int kind, const Vector3& pos, const Vector3& rot, const Vector3& scale);
	static void Release();
	static void Update(int index, Vector3& pos, Vector3& rot, Vector3& scale);
	static void Render();

	//�v���C���ƒn�ʂƂ̔���
	static bool ToLocal(const int index, const Vector3& pos, Vector3& local, Vector3& vec);
	static void ToWorld(const int index, Vector3& inout);


	static bool HeightRay(int index, Vector3& out, Vector3 pos, Vector3 vec, float dist, float revision);
	static bool OnGround(int index, Vector3& out, const Vector3&pos,
		float delLen = 2.0f, const Vector3& gravity = Vector3(0, -1.0f, 0));

	//�v���C���ƃN���X�^���Ƃ̔���
	static bool PlayerToCrystal(PLAYER* p1, CRYSTAL* crystal);

	
	//  �g�����̖��@�ƃv���C���[�Ƃ̏Փ˔���
	static bool PartnerMagicToPlayer(PLAYER*, const PARTNER_MAGIC&, CAMERA*);

	//  �U���^�g�����̕����U���ƃv���C���[�Ƃ̏Փ˔���
	static bool PartnerAttackToPlayer(PLAYER*, const CHARACTOR&, CAMERA*);

	//  �N���X�^���̎g�����ƃv���C���[�̖��@�U���Ƃ̏Փ˔���
	static bool ClystalPartnerToMagic(CLYSTAL_PARTNER*, CAMERA*);

	//  㩂ƓG�v���C���[�Ƃ̏Փ˔���
	static bool TrapToPlayer(PLAYER*, TRAP*, CAMERA*);
	
	//  �����ƃv���C���[�̏Փ˔���
	static bool ExpToPlayer(PLAYER*, const Vector3&, CAMERA*);

	//�v���C���[�Ɩ��@�̏Փ˔���
	static float PlayerToMagic(const Vector3& pos, MAGIC* magic);
	static float PlayerMagic(PLAYER* p, MAGIC* magic);

	static std::vector<STAGE*>* GetLowStage() { return &lowStage; }

	//  UNDONE:�ǉ����܂���
	//  �h���S���̓������̔��肪����̂ł���΍폜���Ă��������B
	static STAGE::KIND_GROUND GetLowStageKindGround(int i) { return lowStage[i]->GetKindGround(); }


	//  ���@�Ɩ��@�̓����蔻��
	static int MagicMagic(MAGIC* m1, MAGIC* m2);

};