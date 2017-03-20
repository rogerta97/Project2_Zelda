#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "j1Gui.h"
#include "Scene.h"
#include "j1Entity.h"

struct SDL_Texture;
class MenuScene; 
class MainScene;
class CharacterSelectionScene; 
class TeamSelectScene;

struct player_info
{
	uint			gamepad = 5;
	uint			viewport = 5;
	uint			team = 3;
	uint			player = 5;
	entity_name		character = e_n_null;
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

public:
	// Scenes
	MenuScene*					 menu_scene = nullptr; 
	MainScene*					 main_scene = nullptr;
	CharacterSelectionScene*	 charselect_screen = nullptr; 
	TeamSelectScene*			 team_select = nullptr;

	player_info					 players[4];

private:
	// Scenes list
	list<Scene*>				 scenes;

	// Current scene
	Scene*						 current_scene = nullptr;
		
};

#endif // __j1SCENE_H__