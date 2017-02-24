#include "j1Gui.h"
#include "j1Render.h"
#include "j1Input.h"
#include "QuestManager.h"
#include "j1Viewports.h"


//	QUEST  -----------------------------------------
Quest::Quest(UI_Text* task, UI_Image* image, bool done, SDL_Rect rect_task_done, int id) {

	this->task = task;
	this->image = image;
	this->done = false;
	this->done_rect = rect_task_done;
	this->id = id;
	this->ball_pos = iPoint(image->rect.x, image->rect.h); 
}

bool Quest::Update(iPoint placer) 
{
	
	if (done) 
		image->Set(placer, done_rect);
	
	else
		image->Set(placer, image->image); 

	image->update(); 
		
	return true;
}

//	QUEST MANAGER  ---------------------------------

QuestManager::QuestManager()
{
	memset(this, 0, quest_list.size());
	tasks_done = 0; 
	placer = iPoint(0, 0); 
	quest_list.clear(); 
}

QuestManager::~QuestManager() {}

// Called before render is available
bool QuestManager::Awake(pugi::xml_node&) { return true; }

// Called before the first frame
bool QuestManager::Start() { return true; }

// Called before all Updates
bool QuestManager::PreUpdate() { return true; }

// Called each loop iteration
bool QuestManager::Update(float dt) {

	SDL_Rect screen = App->view->GetViewportRect(1);
	placer = iPoint(screen.w - 20, screen.h - 20);

	for (vector<Quest>::iterator it = quest_list.begin(); it != quest_list.end(); it++) {
		it->Update(placer); 
		placer.y -= 20; 
	}

	return true;
}

// Called before all Updates
bool QuestManager::PostUpdate() { return true; }

// Called before quitting
bool QuestManager::CleanUp() { return true; }

void QuestManager::CreateQuest(UI_Text* task, UI_Image* image, SDL_Rect rect_task_done, int id) {

	if (quest_list.size() < 5) 
	{
		Quest* new_quest = new Quest(task, image, false, rect_task_done, id);
		quest_list.push_back(*new_quest);
	}

}

