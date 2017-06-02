#pragma oncezz
#ifndef _CREDITSCENE_H_
#define _CREDITSCENE_H_

#include "Scene.h"
#include "j1Gui.h"
#include "j1Render.h"

struct Credits_UI 
{
	SDL_Texture* logo = nullptr; 

	UI_Text*  iban_line_1 = nullptr; 
	UI_Text*  iban_line_2 = nullptr;
	UI_Text*  ivan_line = nullptr; 
	UI_Text*  guillem_line = nullptr; 
	UI_Text*  domi_line = nullptr; 
	UI_Text*  sergi_line = nullptr; 
	UI_Text*  roger_line = nullptr; 

	UI_Text*  special_thanks = nullptr; 

	UI_Text*  voice_line = nullptr; 
	UI_Text*  voice_editor_line = nullptr; 

	UI_Image* twitter_logo = nullptr; 

	UI_Image* github_logo = nullptr;

	UI_Image* mail_logo = nullptr;
};

class CreditsScene : public Scene
{
public:
	CreditsScene();
	~CreditsScene();

	bool Start();
	bool Update(float dt);
	bool CleanUp();

private:

	UI_Window*   window = nullptr;
	SDL_Rect	 size = NULLRECT; 


	SDL_Rect	 logo_rect = NULLRECT; 
	Credits_UI	 content; 
};


#endif // !_LOGO_H_