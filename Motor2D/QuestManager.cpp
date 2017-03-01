#include "j1Gui.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Scene.h"
#include "MainScene.h"
#include "QuestManager.h"
#include "j1Viewports.h"
#include "p2Log.h"


#define QUEST_NUMBER 4


//	QUEST  -----------------------------------------
Quest::Quest(string& task, int id) 
{
	this->task = task;
	this->id = id;
}

bool Quest::Update() 
{	
	return true;
}

//	QUEST MANAGER  ---------------------------------

QuestManager::QuestManager()
{
	memset(this, 0, quest_list.size());
	tasks_done = 0; 

	SDL_Rect screen = App->view->GetViewportRect(1);
	placer = iPoint(screen.w - 30, screen.h - 30);
}

QuestManager::~QuestManager() 
{
	quest_list.clear();
}

// Called before render is available
bool QuestManager::Awake(pugi::xml_node&) { return true; }

// Called before the first frame
bool QuestManager::Start() 
{ 
	bool ret = true;



	return ret;
}

// Called before all Updates
bool QuestManager::PreUpdate()
{
	bool ret = true;

	return ret;
}

// Called each loop iteration
bool QuestManager::Update(float dt) 
{
	bool ret = true;


	
	// Update Quests ----- 

	for (int i = 0; i < quest_list.size(); i++)
	{
		quest_list.at(i).Update();
	}

	//// Print each type ----
	//for (int i = 0; i < QUEST_NUMBER; i++) {
	//	//abilitie_icons.at(i)->; 
	//}

	return ret;
}

// Called before all Updates
bool QuestManager::PostUpdate() 
{ 
	bool ret = true;

	return ret; 
}

// Called before quitting
bool QuestManager::CleanUp() 
{
	bool ret = true;



	return ret;
}

void QuestManager::CreateQuest(string& task, int id) 
{
	if (quest_list.size() < 5) 
	{
		Quest* new_quest = new Quest(task, id);	
		quest_list.push_back(*new_quest);
		abilitie_icons.push_back(App->scene->main_scene->main_window->CreateImage(placer, { 182, 78, 25 ,25 }, false));
		placer.y -= 35; 
	}
}

QuestObjective::QuestObjective(objective_type)
{
}

QuestObjective::~QuestObjective()
{
}

bool QuestObjective::Update()
{
	bool ret = true;



	return ret;
}

objective_type QuestObjective::GetType()
{
	return objective_type();
}

CountObjective::~CountObjective()
{
}

CountObjective::CountObjective() : QuestObjective(objective_type::count)
{

}

TravelObjective::TravelObjective() : QuestObjective(travel)
{

}

bool TravelObjective::Update()
{
	bool ret = true;



	return ret;
}

TravelObjective::~TravelObjective()
{

}
