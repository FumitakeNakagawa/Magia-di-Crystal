#include	"iextreme.h"
#include	"Framework.h"
#include	"SceneMain\SceneMain.h"
#include	"SceneTitle\Title.h"
#include	"SceneLoad\Load.h"
#include	"SceneSelect\SceneSelect.h"
#include	"OTHER\Sound.h"
#include	 <crtdbg.h>
//*****************************************************************************************************************************
//
//
//
//*****************************************************************************************************************************

char*	AppTitle = "Magia di crystal";

//�E�C���h�E���[�h
//BOOL	bFullScreen = FALSE;
//DWORD	ScreenMode = SCREEN720p;

//�t���X�N���[�����[�h
BOOL	bFullScreen = FALSE;
DWORD	ScreenMode = SCREENFULL;



Framework*	MainFrame = nullptr;

SOUND* g_sound;

//*****************************************************************************************************************************
//		�A�v���P�[�V����������
//*****************************************************************************************************************************

BOOL	InitApp( HWND hWnd )
{
	//	IEX�V�X�e��������
	IEX_Initialize( hWnd, bFullScreen, ScreenMode );
	IEX_InitAudio( );
	IEX_InitInput( );

	g_sound = new SOUND();
	g_sound->Load_Sound();


	//	�V�X�e���̏�����
	SYSTEM_Initialize( );
	//	���C���t���[�����[�N����
	MainFrame = new Framework( );
	//	�����V�[���o�^
	//MainFrame->ChangeScene( new SCENE_MAIN(0,1,0,1,0,1,"RUIN" ) );
	MainFrame->ChangeScene(new SCENE_TITLE);


	return TRUE;
}

//*****************************************************************************************************************************
//		�E�B���h�E�v���V�[�W��
//*****************************************************************************************************************************
LRESULT CALLBACK WindowProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch ( message ) {
		case WM_DESTROY: PostQuitMessage( 0 ); return 0;
		case WM_KEYDOWN:
			switch ( wParam ) {
				case VK_ESCAPE: PostMessage( hWnd, WM_CLOSE, 0, 0 ); return 0;
				//case VK_RETURN: PostMessage(hWnd, WM_CLOSE, 0, 0); return 0;
				case VK_F1:		iexSystem::OpenDebugWindow( ); return 0;
			}
			break;
	}
	return DefWindowProc( hWnd, message, wParam, lParam );
}

//*****************************************************************************************************************************
//		�E�B���h�E�쐬
//*****************************************************************************************************************************
HWND InitWindow( HINSTANCE hInstance, int nCmdShow )
{
	HWND			hWnd;

	//	�X�N���[���T�C�Y�擾
	RECT	WindowSize;
	iexSystem::GetScreenRect( ScreenMode, WindowSize );

	//	�E�B���h�E�N���X�ݒ�
	WNDCLASS	wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon( hInstance, IDI_APPLICATION );
	wc.hCursor = LoadCursor( NULL, IDC_ARROW );
	wc.hbrBackground = (HBRUSH)GetStockObject( BLACK_BRUSH );
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "IEX2010";
	RegisterClass( &wc );

	//	�E�B���h�E�쐬
	if ( !bFullScreen ) {
		AdjustWindowRect( &WindowSize, WS_OVERLAPPEDWINDOW, FALSE );
		hWnd = CreateWindow( "IEX2010", AppTitle, WS_OVERLAPPEDWINDOW,
			0, 0, WindowSize.right - WindowSize.left, WindowSize.bottom - WindowSize.top,
			NULL, NULL, hInstance, NULL );
	}
	else {
		hWnd = CreateWindow( "IEX2010", AppTitle, WS_POPUP, 0, 0, WindowSize.right, WindowSize.bottom, NULL, NULL, hInstance, NULL );
		ShowCursor( FALSE );
	}
	if ( !hWnd ) return NULL;

	ShowWindow( hWnd, nCmdShow );
	UpdateWindow( hWnd );

	return hWnd;
}

//*****************************************************************************************************************************
//		WinMain
//*****************************************************************************************************************************
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	MSG		msg;
	HWND	hWnd;
	_CrtSetBreakAlloc(62);
	if ( GetAsyncKeyState( VK_CONTROL ) & 0x8000 ) bFullScreen = TRUE;

	hWnd = InitWindow( hInstance, nCmdShow );
	InitApp( hWnd );

	//	���C�����[�v
	for ( ;;) {
		if ( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) ) {
			if ( !GetMessage( &msg, NULL, 0, 0 ) ) break;
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else {
  			if ( MainFrame->Update( ) ) MainFrame->Render( );
		}
	}

	//	�S���	
	delete	MainFrame;
	delete g_sound;
	MainFrame = nullptr;
	iexParticle::Release();
	SYSTEM_Release( );
	iexSystem::CloseDebugWindow( );
	IEX_ReleaseInput( );
	IEX_ReleaseAudio( );
	g_sound = nullptr;
	IEX_Release( );

	_CrtDumpMemoryLeaks( );
	return 0;
}
