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
class ShopManager;

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

private:
	void CreateMapCollisions();

public:
	UI_Window*			main_window_1 = nullptr;
	UI_Image*			princess_1 = nullptr;
	UI_Window*			main_window_2 = nullptr;
	UI_Image*			princess_2 = nullptr;
	UI_Window*			main_window_3 = nullptr;
	UI_Image*			princess_3 = nullptr;
	UI_Window*			main_window_4 = nullptr;
	UI_Image*			princess_4 = nullptr;

	MinionManager*		minion_manager = nullptr;
	TowerManager*	    tower_manager = nullptr;
	QuestManager*		quest_manager = nullptr;
	ShopManager*		shop_manager = nullptr;

private:
	//UI elements
	// P1
	UI_Image*			progress_bar_1 = nullptr; 
	UI_Image*			rupiees_img_1 = nullptr;
	UI_Image*			minimap_icon_1 = nullptr;
	list<UI_Image*>		habilities_1; 

	// P2
	UI_Image*			progress_bar_2 = nullptr;
	UI_Image*			rupiees_img_2 = nullptr;
	UI_Image*			minimap_icon_2 = nullptr;
	list<UI_Image*>		habilities_2;

	// P3
	UI_Image*			progress_bar_3 = nullptr;
	UI_Image*			rupiees_img_3 = nullptr;
	UI_Image*			minimap_icon_3 = nullptr;
	list<UI_Image*>		habilities_3;

	// P4
	UI_Image*			progress_bar_4 = nullptr;
	UI_Image*			rupiees_img_4 = nullptr;
	UI_Image*			minimap_icon_4 = nullptr;
	list<UI_Image*>		habilities_4;

	vector<PhysBody*>	map_collisions;
};


#endif // !_MAINSCENE_H_