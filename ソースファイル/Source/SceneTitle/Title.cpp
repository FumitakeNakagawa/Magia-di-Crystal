#include	"Title.h"
#include	"SceneLoad\Load.h"
#include	"SceneMain\SceneMain.h"
#include	"SceneSelect\SceneSelect.h"
#include	"OTHER\Sound.h"
#include	"SYSTEM\Framework.h"





bool SCENE_TITLE::Initialize( )
{
	
	bg = std::make_unique<iex2DObj>((char*)"DATA\\IMAGE\\TITLE.PNG");
	push = std::make_unique<iex2DObj>((char*)"DATA\\IMAGE\\UI\\PUSH.PNG");
	//view = std::make_unique<iexView>();

	color = 0xFFFFFFFF;
	alpha = 255;
	
	plus = -5;

	

	iexLight::SetAmbient(0xFFFFFFFF);
	iexLight::SetFog(100, 200, 0xFFFFFFFF);
	return true;
}

SCENE_TITLE::~SCENE_TITLE()
{


}




void SCENE_TITLE::Update( )
{
	//view->Set(0, 0, -20, 0, 0, 0);

	alpha += plus;

	if (alpha < 10 || alpha>250)plus *= -1;

	color = (alpha << 24) | (color & 0x00FFFFFF);

	if ( KEY_Get( KEY_SELECT,0 ) == 3 || KEY_Get(KEY_SELECT, 1) == 3)
		MainFrame->ChangeScene( new SCENE_SELECT_CHAR);

}


void SCENE_TITLE::Render()
{
	
	//view->Activate();
	//view->Clear();

	bg->Render(0,0,1920,1080,0,0,1920,1080);

	
	push->Render(500, 500, 960, 720, 0, 0, 512, 512, RS_COPY, color);
	
	

	

}
