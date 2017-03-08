#ifndef _MAINSCENE_H_
#define _MAINSCENE_H_

#include "Scene.h"
#include "j1Gui.h"
#include "j1Render.h"

class GameObject;
class b2Fixture;
class Parallax;
class Link;
class QuestManager; 
class Minion;
class MinionManager;
class TowerManager;

enum GameStates
{

};

class MainScene : public Scene 
{
public:
	MainScene();
	~MainScene();

	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	void OnColl(PhysBody* bodyA, PhysBody* bodyB, b2Fixture* fixtureA, b2Fixture* fixtureB);
	void OnCommand(std::list<std::string>& tokens);

public:
	UI_Window*			main_window = nullptr;


	MinionManager*		minion_manager = nullptr;

	TowerManager*	    tower_manager = nullptr;

	QuestManager*		quest_manager = nullptr;

private:
	//UI elements
	UI_Image*			progress_bar = nullptr; 
	UI_Image*			rupiees_img = nullptr;
	UI_Image*			minimap_icon = nullptr;
	UI_Text*			rupiees_numb = nullptr;
	list<UI_Image*>		habilities; 
};


#endif // !_MAINSCENE_H_