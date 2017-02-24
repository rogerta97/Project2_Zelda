#ifndef _QUESTMANAGER_H_
#define _QUESTMANAGER_H_

#include "PugiXml\src\pugixml.hpp"
#include "j1Gui.h"
#include "SDL\include\SDL.h"

struct Quest
{
	Quest(UI_Text* task, UI_Image* image, bool done, SDL_Rect rect_task_done, int id);

	bool Update(iPoint placer);

	~Quest() {};

	bool done = false;
	SDL_Rect done_rect;
	UI_Text* task = nullptr;
	UI_Image* image = nullptr;
	iPoint ball_pos;
	int id;
};

class QuestManager
{
public:

	QuestManager();

	// Destructor
	virtual ~QuestManager();

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

	void CreateQuest(UI_Text* task, UI_Image* image, SDL_Rect rect_task_done, int id);

	int tasks_done = 0;

private:

private:
	vector<Quest> quest_list;
	iPoint placer; 


};

#endif