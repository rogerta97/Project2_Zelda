#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Scene.h"
#include "RemapingScene.h"
#include "MainScene.h"
#include "MenuScene.h"
#include "TransitionScene.h"
#include "j1Console.h"
#include "TeamSelectScene.h"
#include "FinalScreen.h"
#include "CharacterSelectionScene.h"
#include "LogoScene.h"
#include "Mapping.h"

#define NUMBER_OF_PLAYERS 4

j1Scene::j1Scene() : j1Module()
{
	name = "scene";
}

// Destructor
j1Scene::~j1Scene()
{

}

// Called before render is available
bool j1Scene::Awake()
{
	bool ret = true;

	LOG("Loading SceneManager");

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	bool ret = false;

	LOG("Start module scene");

	players[0].player = players[0].gamepad = players[0].viewport = 1;
	players[1].player = players[1].gamepad = players[1].viewport = 2;
	players[2].player = players[2].gamepad = players[2].viewport = 3;
	players[3].player = players[3].gamepad = players[3].viewport = 4;
	players[0].team = 1;
	players[1].team = 2;
	players[2].team = 1;
	players[3].team = 2;

	//Create mapping for players
	players[0].mapping = new Mapping();
	players[1].mapping = new Mapping();
	players[2].mapping = new Mapping();
	players[3].mapping = new Mapping();

	// Create scenes
	menu_scene = new MenuScene(); 
	scenes.push_back(menu_scene);
	main_scene = new MainScene();
	scenes.push_back(main_scene);
	team_select = new TeamSelectScene();
	scenes.push_back(team_select);
	charselect_screen = new CharacterSelectionScene();
	scenes.push_back(charselect_screen); 
	logo_scene = new LogoScene();
	scenes.push_back(logo_scene);
	final_screen = new FinalScreen(); 
	scenes.push_back(final_screen); 
	remaping_scene = new RemapingScene();
	scenes.push_back(remaping_scene); 
	transition_scene = new TransitionScene(); 
	scenes.push_back(transition_scene);
	// -------------

	// Starting scene
	current_scene = logo_scene;

	if(current_scene != nullptr)
		ret = current_scene->Start();

	return ret;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	bool ret = false;

	if (current_scene != nullptr)
		ret = current_scene->PreUpdate();

	return ret;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	bool ret = false;

	if (current_scene != nullptr)
		ret = current_scene->Update(dt);
	
	return ret;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = false;

	if (current_scene != nullptr)
		ret = current_scene->PostUpdate();

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	bool ret = false;
	if (current_scene != nullptr)
		ret = current_scene->CleanUp();

	return ret;
}

void j1Scene::ChangeScene(Scene * new_scene)
{
	LOG("Changing current scene");

	last_scene = current_scene;
	current_scene = new_scene;
	last_scene->CleanUp();
	current_scene->Start();
}

Scene * j1Scene::GetCurrentScene()
{
	return current_scene;
}


void j1Scene::OnCollision(PhysBody * bodyA, PhysBody * bodyB, b2Fixture * fixtureA, b2Fixture * fixtureB)
{
	current_scene->OnColl(bodyA, bodyB, fixtureA, fixtureB);
}

void j1Scene::OnCollisionEnter(PhysBody * bodyA, PhysBody * bodyB, b2Fixture * fixtureA, b2Fixture * fixtureB)
{
	current_scene->OnCollEnter(bodyA, bodyB, fixtureA, fixtureB);
}

void j1Scene::OnCollisionOut(PhysBody * bodyA, PhysBody * bodyB, b2Fixture * fixtureA, b2Fixture * fixtureB)
{
	current_scene->OnCollOut(bodyA, bodyB, fixtureA, fixtureB);
}

void j1Scene::OnCommand(std::list<std::string>& tokens)
{
	current_scene->OnCommand(tokens);
}

void j1Scene::OnCVar(std::list<std::string>& tokens)
{
	current_scene->OnCVar(tokens);
}

void j1Scene::SaveCVar(std::string & cvar_name, pugi::xml_node & node) const
{
	current_scene->SaveCVar(cvar_name,node);
}

void j1Scene::ListenEvent(int type, EventThrower * origin, int id)
{
	current_scene->ListenEvent(type, origin, id);
}



