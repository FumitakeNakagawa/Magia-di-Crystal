#pragma once
#include	"iextreme.h"
#include	"SYSTEM\Scene.h"

class SCENE_TITLE : public Scene
{
private: 
	std::unique_ptr<iex2DObj> bg;
	std::unique_ptr<iex2DObj>push;
	std::unique_ptr<iexView> view;

	DWORD color;
	BYTE alpha;
	int plus;
public:

	bool Initialize();
	~SCENE_TITLE();
	void Update();
	void Render();

};