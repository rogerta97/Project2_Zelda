#include "j1Gui.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Scene.h"
#include "MainScene.h"
#include "QuestManager.h"
#include "j1Viewports.h"


//	QUEST  -----------------------------------------
Quest::Quest(string& task, int id) {

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
	quest_list.clear(); 

	SDL_Rect screen = App->view->GetViewportRect(1);
	placer = iPoint(screen.w - 20, screen.h - 20);
}

QuestManager::~QuestManager() {
}

// Called before render is available
bool QuestManager::Awake(pugi::xml_node&) { return true; }

// Called before the first frame
bool QuestManager::Start() 
{ 



	return true;
}

// Called before all Updates
bool QuestManager::PreUpdate() { return true; }

// Called each loop iteration
bool QuestManager::Update(float dt) {


	for (vector<Quest>::iterator it = quest_list.begin(); it != quest_list.end(); it++) {
		it->Update(); 
	}

	return true;
}

// Called before all Updates
bool QuestManager::PostUpdate() { return true; }

// Called before quitting
bool QuestManager::CleanUp() { return true; }

void QuestManager::CreateQuest(string& task, int id) {

	if (quest_list.size() < 5) 
	{
		Quest* new_quest = new Quest(task, id);	
		quest_list.push_back(*new_quest);
		quest_balls.push_back(App->scene->main_scene->main_window->CreateImage(placer, { 112, 2, 12, 12 }, false));
		placer.y -= 20;
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
	return true;
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
	return false;
}

TravelObjective::~TravelObjective()
{

}
