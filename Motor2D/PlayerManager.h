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
		int x = 27 + ((viewport - 1) % 2)*win_w / 2;
		int y = 30 + ((viewport - 1) / 2)*win_h / 2;
		rupees_num = App->scene->main_scene->shop_manager->shop_window->CreateText(iPoint(x, y), App->font->game_font_25);
		UpdateRupees();
		team = entity->GetTeam();
		respawn = _respawn;
		death_timer = App->AddGameplayTimer();
		base_travel_timer = App->AddGameplayTimer();
	}

	void BuyItem(Item* item, int price);
	void Kill();
	void Respawn();
	void BaseTravel();
	void ApplyItemStats();
	void AddRupees(int add);
	void CleanUp();

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
	uint	    rupees = 400;

	j1Timer*    death_timer = nullptr;
	float		death_time = 5.0f;

	bool		is_dead = false;

	iPoint		respawn = NULLPOINT;
	int			team = 0;	

	bool		disable_controller = false;

	bool		base_travel = false;
	j1Timer*    base_travel_timer = nullptr;

	float		last_heal_time = 0.0f;

	float		last_rupee_time = 60.0f;
};

struct PlayerManagerUI
{
	vector<UI_Image*>	abilities_button;
	vector<UI_Image*>	abilities_icon;
	vector<UI_Text*>    abilities_cd;
	UI_Text*			death_time = nullptr;
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

	// Get player from index 
	Entity* GetPlayer(int index);

private:
	void PlayerInput(Player* player, int index);
	void MoveCamera(Player* player);
	void CheckIfRespawn(Player* player);
	void CheckIfDeath(Player* player);
	void UpdateUI(Player* player);
	void UpdateDeathUI(Player* player, float dt);
	void PasiveHP(Player* player);
	void PasiveRupee(Player* player);
	void SetAbilitiesIcon(int index);
	void CheckBomb(Player* player);

public:
	vector<Player*>     players;

private:
	// UI Elements
	vector<PlayerManagerUI> p_manager_ui_elements; 

	EventThrower*           event_thrower = nullptr;

	uint					last_heal_time = 0;

	uint			     	death_sound_effect = 100;

	Animator*				death_text_anim = nullptr;
	SDL_Texture*		    death_text_texture = nullptr;
	iPoint                  death_text_pos = NULLPOINT;

	iPoint					death_quad_pos = NULLPOINT;
	SDL_Rect				death_quad_rect = NULLRECT;
};


#endif // __PLAYER_MANAGER_H__