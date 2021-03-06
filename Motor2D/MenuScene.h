#ifndef _MENUSCENE_H_
#define _MENUSCENE_H_

#include "Scene.h"
#include "j1Gui.h"
#include "j1Render.h"
#include "j1Timer.h"

class Animator;

enum button_action 
{
	START,
	OPTIONS,
	CREDITS,
	QUIT,
	FX,
	MUSIC, 
	QUESTS,
	REMAP
};

class MenuScene : public Scene
{
public:
	MenuScene();
	~MenuScene();

	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	void OnColl(PhysBody* bodyA, PhysBody* bodyB, b2Fixture* fixtureA, b2Fixture* fixtureB);
	void OnCommand(std::list<std::string>& tokens);

	void GoOptions();
	void GoMenu(); 

	bool QuestsEnabled();

private:
	void FadeOut();

public:
	fPoint			   pos = NULLPOINT;

	UI_Window*		   menu_window = nullptr;

	bool			   quests_enabled = false;

private:
	UI_Image*		   triforce = nullptr;

	UI_Image*		   cursor_1 = nullptr; 
	UI_Image*		   cursor_2 = nullptr;

	UI_Image*		   princess = nullptr; 

	UI_Button*		   start_button = nullptr;
	UI_Button*		   options_button = nullptr; 
	UI_Button*		   credits_button = nullptr;
	UI_Button*		   quit_button = nullptr; 

	UI_Button*		   fx_button = nullptr;
	UI_Button*		   music_button = nullptr;

	UI_Button*		   remap_button = nullptr;
	UI_Button*		   quests_button = nullptr;

	UI_Text*		   start_text = nullptr; 
	UI_Text*		   options_text = nullptr;
	UI_Text*		   credits_text = nullptr; 
	UI_Text*           quit_text = nullptr;

	UI_Text*		   fx_text = nullptr;
	UI_Text*		   music_text = nullptr;

	UI_Text*		   remap_text = nullptr;
	UI_Text*		   quests_text = nullptr;

	UI_Check_Box*	   options_checkbox = nullptr; 

	vector<UI_Button*> button_list;
	vector<UI_Image*>  cursors;

	bool			   is_options = false;
	button_action	   current_button = START;

	j1Timer			   music_time;

	// Main banner
	Animator*		   main_banner = nullptr;
	iPoint			   main_banner_pos = NULLPOINT;
	SDL_Texture*	   main_banner_texture = nullptr;

	// Background image
	SDL_Texture*	   background_image = nullptr;
	iPoint			   background_pos = NULLPOINT;
	SDL_Rect		   background_image_rect = NULLRECT;
	float              fade_value = 255.0f;
};


#endif
