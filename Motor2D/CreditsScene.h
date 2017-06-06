#pragma oncezz
#ifndef _CREDITSCENE_H_
#define _CREDITSCENE_H_

#include "Scene.h"
#include "j1Gui.h"
#include "j1Render.h"

struct Credits_UI 
{
	SDL_Texture* logo = nullptr; 
	SDL_Texture* icons_tex = nullptr; 
	SDL_Texture* background_image = nullptr; 

	SDL_Rect logo_rect = NULLRECT; 
	iPoint logo_pos = NULLPOINT; 

	SDL_Rect twitter_logo_rect = NULLRECT; 
	iPoint twitter_pos = NULLPOINT; 

	SDL_Rect github_logo_rect = NULLRECT;
	iPoint github_pos = NULLPOINT;

	SDL_Rect mail_logo_rect = NULLRECT;
	iPoint mail_pos = NULLPOINT;

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

	UI_Button* twitter_button = nullptr; 
	UI_Button* github_button = nullptr;
	UI_Button* mail_button = nullptr;
	
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

	bool		 is_in_twitter = false; 
	bool		 is_in_github = false; 
	bool		 is_in_mail = false; 

	SDL_Rect	 logo_rect = NULLRECT; 
	Credits_UI	 content; 
};


#endif // !_LOGO_H_