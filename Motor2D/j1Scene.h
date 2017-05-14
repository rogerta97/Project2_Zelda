#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "j1Gui.h"
#include "Scene.h"
#include "j1Entity.h"

struct SDL_Texture;
class MenuScene; 
class MainScene;
class FinalScreen; 
class CharacterSelectionScene; 
class TeamSelectScene;
class TransitionScene; 
class LogoScene;
class RemapingScene; 
class Mapping;

struct player_info
{
	uint			gamepad = 5;
	uint			viewport = 5;
	uint			team = 3;
	uint			player = 5;
	entity_name		character = e_n_null;
	Mapping*		mapping = nullptr;

	uint			kills = 0;
	uint			deaths = 0;
	uint			minions = 0;
	uint			towers = 0;

	SDL_Rect		items_rects[3] = { {0,0,0,0},{ 0,0,0,0 },{ 0,0,0,0 } };

	void Reset() { character = e_n_null; kills = 0; deaths = 0; minions = 0; towers = 0; items_rects[0] = { 0,0,0,0 }; items_rects[1] = { 0,0,0,0 }; items_rects[2] = { 0,0,0,0 };	}
	
};																						 
																						 
class j1Scene : public j1Module															  
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake();

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

	// Change scene
	void ChangeScene(Scene* new_scene);
	Scene* GetCurrentScene();

	// Blit choosing the layer
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB, b2Fixture* fixtureA, b2Fixture* fixtureB);
	void OnCollisionEnter(PhysBody* bodyA, PhysBody* bodyB, b2Fixture* fixtureA, b2Fixture* fixtureB);
	void OnCollisionOut(PhysBody* bodyA, PhysBody* bodyB, b2Fixture* fixtureA, b2Fixture* fixtureB);

	void OnCommand(std::list<std::string>& tokens);

	void OnCVar(std::list<std::string>& tokens);

	void SaveCVar(std::string& cvar_name, pugi::xml_node& node) const;

	void ListenEvent(int type, EventThrower * origin, int id);

public:
	// Scenes
	MenuScene*					 menu_scene = nullptr; 
	MainScene*					 main_scene = nullptr;
	CharacterSelectionScene*	 charselect_screen = nullptr; 
	TeamSelectScene*			 team_select = nullptr;
	FinalScreen*				 final_screen = nullptr; 
	LogoScene*					 logo_scene = nullptr;
	RemapingScene*				 remaping_scene = nullptr; 
	TransitionScene*		     transition_scene = nullptr;
	
	player_info					 players[4];

	Scene*						 last_scene = nullptr;

private:
	// Scenes list
	list<Scene*>				 scenes;

	// Current scene
	Scene*						 current_scene = nullptr;

		
};

#endif // __j1SCENE_H__