#pragma once

#include	"iextreme.h"
#include	"SYSTEM\Scene.h"
#include	<process.h>
#include	<vector>
#include	<memory>
class SCENE_LOAD :public Scene
{
private:
	std::unique_ptr<iex2DObj> push;
	std::vector<std::unique_ptr<iex2DObj>> tips;				//���[�h���(�ς�邩��)
	std::unique_ptr<iex2DObj> obj;
	std::unique_ptr<iex2DObj>number;
	std::unique_ptr<iex2DObj> cursor;
	static Scene* nextScene;//���[�h��ʂ̎��̃V�[��
	static bool loadEndflg;	//���[�h�������������ǂ���
	static int percentage;	//���[�h������
	static int time;
	int alpha;
	bool alphaFlag;

	int nowTips;
	float nowPercentage;

	DWORD pushAlpha;
	int pushPlus;
private:

	//�X���b�h(���s����������֐�)
	static void Thread(void* arg);

public:
	//�������E���
	SCENE_LOAD() :obj(nullptr) {}
	SCENE_LOAD(char* filename, Scene* nextScene);
	~SCENE_LOAD();

	//�X�V
	void Update();
	//�`��
	void Render();


	//�Z�b�g
	static void SetPercentage(int num) { percentage = num; }
	static int GetPercentage() { return percentage; }
};