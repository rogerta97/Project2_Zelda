#ifndef _MAINSCENE_H_
#define _MAINSCENE_H_

#include "Scene.h"
#include "j1Gui.h"
#include "j1Render.h"
#include "j1Timer.h"

class GameObject;
class b2Fixture;
class Parallax;
class Link;
class QuestManager; 
class Minion;
class MinionManager;
class TowerManager;
class ShopManager;
class ZeldaManager;

class Zelda;

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

	j1Timer* GetGameTimer();

	void OnColl(PhysBody* bodyA, PhysBody* bodyB, b2Fixture* fixtureA, b2Fixture* fixtureB);
	void OnCommand(std::list<std::string>& tokens);

	void EndGame(int winner);
	void UpdateProgressBar();


private:
	void CreateMapCollisions();
	void DrawScreenSeparation();

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
	vector<UI_Image*>	habilities_1;

	// P2
	UI_Image*			progress_bar_2 = nullptr;
	UI_Image*			rupiees_img_2 = nullptr;
	UI_Image*			minimap_icon_2 = nullptr;
	vector<UI_Image*>	habilities_2;

	// P3
	UI_Image*			progress_bar_3 = nullptr;
	UI_Image*			rupiees_img_3 = nullptr;
	UI_Image*			minimap_icon_3 = nullptr;
	vector<UI_Image*>	habilities_3;

	// P4
	UI_Image*			progress_bar_4 = nullptr;
	UI_Image*			rupiees_img_4 = nullptr;
	UI_Image*			minimap_icon_4 = nullptr;
	vector<UI_Image*>   habilities_4;

	vector<PhysBody*>	map_collisions;

	//Testing
	j1Timer				game_timer;
	ZeldaManager*		zelda_manager = nullptr;

	uint				end_delay = 10;

	uint				winner = 0;
};


#endif // !_MAINSCENE_H_