#ifndef _MENUSCENE_H_
#define _MENUSCENE_H_

#include "Scene.h"
#include "j1Gui.h"
#include "j1Render.h"

enum button_action 
{
	START,
	OPTIONS,
	CREDITS,
	QUIT,
	FX,
	MUSIC

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

public:
	fPoint			   pos = NULLPOINT;

	UI_Window*		   menu_window = nullptr;

private:
	UI_Image*		   triforce = nullptr;

	UI_Image*		   cursor_1 = nullptr; 
	UI_Image*		   cursor_2 = nullptr;

	UI_Image*		   princess = nullptr;

	button_action	   current_button = START; 

	UI_Button*		   start_button = nullptr;
	UI_Button*		   options_button = nullptr; 
	UI_Button*		   credits_button = nullptr;
	UI_Button*		   quit_button = nullptr; 

	UI_Button*		   fx_button = nullptr;
	UI_Button*		   music_button = nullptr;

	UI_Text*		   start_text = nullptr; 
	UI_Text*		   options_text = nullptr;
	UI_Text*		   credits_text = nullptr; 
	UI_Text*           quit_text = nullptr;

	UI_Text*		   fx_text = nullptr;
	UI_Text*		   music_text = nullptr;

	UI_Check_Box*	   options_checkbox = nullptr; 

	vector<UI_Button*> button_list;
	vector<UI_Image*>  cursors;

	bool			   is_options = false; 

};


#endif
