#ifndef _MENUSCENE_H_
#define _MENUSCENE_H_


#include "Scene.h"
#include "j1Gui.h"
#include "j1Render.h"

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

	fPoint pos;

	UI_Window* menu_window = nullptr;

private:

	UI_Image* triforce = nullptr;

	UI_Button* start_button = nullptr;
	UI_Button* options_button = nullptr; 
	UI_Button* quit_button = nullptr; 

	UI_Text* start_text = nullptr; 
	UI_Text* options_text = nullptr;
	UI_Text* quit_text = nullptr;

	vector<UI_Button*> button_list;

};


#endif
