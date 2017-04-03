#ifndef __PLAYER_MANAGER_H__
#define __PLAYER_MANAGER_H__

#include "j1Module.h"
#include "p2Defs.h"
#include "p2List.h"
#include "Entity.h"
#include "j1Entity.h"
#include "Item.h"
#include "j1Window.h"
#include "j1Fonts.h"
#include "j1Scene.h"
#include "j1Gui.h"
#include "MainScene.h"
#include "ShopManager.h"

class b2Fixture;
class PhysBody;
class Quest;
class Entity;

class Player
{
public:
	Player() {  };
	Player(Entity* _entity, uint _controller_index, uint _viewport, iPoint _respawn)
	{
		entity = _entity; state = states::idle_down; controller_index = _controller_index, viewport = _viewport;
		uint win_w, win_h;
		App->win->GetWindowSize(win_w, win_h);
		int x = 20 + ((viewport - 1) % 2)*win_w / 2;
		int y = 30 + ((viewport - 1) / 2)*win_h / 2;
		rupees_num = App->scene->main_scene->shop_manager->shop_window->CreateText(iPoint(x, y), App->font->game_font_small);
		UpdateRupees();
		team = entity->GetTeam();
		respawn = _respawn;
	}

	void BuyItem(Item* item, int price);
	void Kill();
	void Respawn();
	void ApplyItemStats();
	void AddRupees(int add);

private:
	void UpdateRupees();

public:

	Entity*     entity = nullptr;
	states      state = states::states_null;
	shows	    show = shows::show_null;
	movement    move = stop;
	entity_name type = entity_name::e_n_null;

	Item*	    items[3] = { nullptr,nullptr,nullptr };

	uint	    controller_index = 0;
	uint		viewport = 0;

	UI_Text*    rupees_num = nullptr;
	uint	    rupees = 20000;

	j1Timer     death_timer;
	float		death_time = 5.0f;

	bool		is_dead = false;

	iPoint		respawn = NULLPOINT;
	int			team = 0;	

	bool		disable_controller = false;
};

class PlayerManager
{
public:

	PlayerManager();

	// Destructor
	~PlayerManager();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	Player* AddPlayer(entity_name name, iPoint pos, int controller_index, int viewport, int team, int respawn = 1, int show_life_bar = true);
	void DeletePlayer(int index);
	void ClearPlayers();
	
	std::vector<Entity*> GetTeamPlayers(int team);
	std::vector<int> GetTeamViewports(int team);
	int GetEntityViewportIfIsPlayer(Entity* entity);
	iPoint GetFreePlayerSpawn(int team, int respawn);

	// Ability goes from 1 to 4
	bool IsAbilityCdCompleted(Player* player, int ability);

	void ResetAbilityTimer(Player* player, int ability);

	int GetPlayerTeamFromBody(PhysBody* body);
	Player* GetPlayerFromBody(PhysBody* body);

	//Disable player input. 0 for all
	void DisableInput(int player);

	//Allow player input. 0 to allow all
	void AllowInput(int player);

private:
	void PlayerInput(Player* player);
	void MoveCamera(Player* player);
	void CheckIfRespawn(Player* player);
	void CheckIfDeath(Player* player);
	void UpdateUI(Player* player);
	void UpdateDeathUI(Player* player);
	void PasiveHP(Player* player);

public:
	vector<Player*>     players;

private:
	// UI
	vector<UI_Image*>	abilities_1;
	vector<UI_Text*>    abilities_cd_1;
	UI_Text*			death_text_1 = nullptr;

	vector<UI_Image*>	abilities_2;
	vector<UI_Text*>    abilities_cd_2;
	UI_Text*			death_text_2 = nullptr;

	vector<UI_Image*>	abilities_3;
	vector<UI_Text*>    abilities_cd_3;
	UI_Text*			death_text_3 = nullptr;

	vector<UI_Image*>   abilities_4;
	vector<UI_Text*>    abilities_cd_4;
	UI_Text*			death_text_4 = nullptr;

	EventThrower*       event_thrower = nullptr;

	SDL_Color           death_rect_color = NULLRECT;
	SDL_Rect			death_rect = NULLRECT;

	uint				last_heal_time = 0;
};

#endif // __PLAYER_MANAGER_H__