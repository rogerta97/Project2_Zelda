#ifndef _MAINSCENE_H_
#define _MAINSCENE_H_

#include "Scene.h"
#include "j1Gui.h"
#include "Trunk.h"
#include "j1Render.h"
#include "j1Timer.h"
#include "Animation.h"

class GameObject;
class b2Fixture;
class Parallax;
class Link;
class QuestManager; 
class Minion;
class MinionManager;
class AestheticsManager;
class TowerManager;
class ShopManager;
class ZeldaManager;
class BaseManager;
class QuestManager;
class JungleCampManager;
class MinimapManager;

enum GameStates
{

};

enum PauseUIStates
{
	p_e_resume, 
	p_e_quit, 
	p_e_null, 
};

struct PauseUI
{
	UI_Image*			resume_background = nullptr;
	UI_Text*			resume_text = nullptr;

	UI_Image*			quit_background = nullptr;
	UI_Text*			quit_text = nullptr;

	UI_Image*			cursor_1 = nullptr;
	UI_Image*			cursor_2 = nullptr;

	PauseUIStates		cursor_state = p_e_null; 

	void				SetPauseUI(bool ui_state); 
	void				MoveCursor(); 
	void				UpdatePause(); 
};

struct MinimapState 
{
	UI_Image* stats_back_image = nullptr; 

	UI_Text* hp_text = nullptr;
	UI_Text* power_text = nullptr;
	UI_Text* speed_text = nullptr;
	UI_Text* kills_text = nullptr;
	UI_Text* minions_text = nullptr;

	void Enable();
	void Disable();

};

struct MainSceneViewport
{
	UI_Window*			viewport_window = nullptr;
	UI_Image*			princess = nullptr;
	UI_Image*			progress_bar = nullptr;
	UI_Image*			rupiees_img = nullptr;
	UI_Image*			minimap_icon = nullptr;
	UI_Image*			win_text = nullptr;

	MinimapState		minimapstate;
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
	void ListenEvent(int type, EventThrower* origin, int id);

private:
	void CreateMapCollisions();
	void DrawScreenSeparation();

	void UpdateWinnerAnim(uint winner, float dt);

	void GetPlayerItemsRects();

public:
	// Managers
	MinionManager*		minion_manager = nullptr;
	TowerManager*	    tower_manager = nullptr;;
	ShopManager*		shop_manager = nullptr;
	AestheticsManager*  aest_manager = nullptr;
	ZeldaManager*		zelda_manager = nullptr;
	BaseManager*		base_manager = nullptr;
	QuestManager*		quest_manager = nullptr;
	JungleCampManager*	jungleCamp_manager = nullptr;
	PlayerManager*      player_manager = nullptr;
	MinimapManager*     minimap_manager = nullptr;

	// UI Elements
	vector<MainSceneViewport>	ui_viewports;

private:
	UI_Window*			main_scene_window = nullptr;
	
	vector<PhysBody*>	map_collisions;

	j1Timer*			game_timer = nullptr;
	j1Timer*			quest_timer = nullptr;
	bool				first_quest_completed = false;
	uint				end_delay = 10;

	uint				winner = 0;

	Animator*			victory = nullptr;
	Animator*			defeat = nullptr;

	// Pause UI
	PauseUI				pause_ui; 
};


#endif // !_MAINSCENE_H_