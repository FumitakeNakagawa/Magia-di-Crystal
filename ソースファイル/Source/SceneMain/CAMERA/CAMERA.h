#pragma once
#include	"iextreme.h"

#define WIDTH 16
#define HEIGHT 9

class CAMERA
{
private:
	std::unique_ptr<iexView> view;
	Vector3 pos, target;
	int width, height, n, f;
public:
	CAMERA( );
	void Initialize( );


	void Update( );
	void Update(Vector3 pos1, Vector3 pos2);
	void Screentransformation(Vector3& Outp, const Vector3 Pos) { view->Screentransformation(Outp, Pos); }
	void Render( );

	void Clear(DWORD color) { view->Clear(color); }


	Vector3& GetPos( ) { return pos; }
	Vector3& GetTarget( ) { return pos; }

	int Getwidth() { return width; }
	int Getheight() { return height; }
};

