#ifndef __SCENE_H__
#define __SCENE_H__

//*****************************************************************************
//
//		�V�[�����N���X
//
//*****************************************************************************

class Scene
{
protected:
	bool loadFlg;
public:
	//	�����E���
	Scene( ) :loadFlg( false ) {}
	virtual ~Scene( ) {}
	//	������
	virtual bool Initialize( ) { return true; }
	//	�X�V�E�`��
	virtual void Update( ) {}
	virtual void Render( ) {}
};

//*****************************************************************************
#endif
