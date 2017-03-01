#ifndef _QUESTMANAGER_H_
#define _QUESTMANAGER_H_

#include "PugiXml\src\pugixml.hpp"
#include "j1Gui.h"
#include "SDL\include\SDL.h"

enum objective_type
{
	count,
	travel,
	o_t_null,
};

// That does not need to be a class !!!!!!!!!!!!!!!!!!!!!!!!!!!
class QuestObjective 
{
public: 
	QuestObjective(objective_type); 

	~QuestObjective();

	virtual bool Update(); 

	objective_type GetType();

private: 
	objective_type type = o_t_null;
	bool           done = false; 
};

// That does not need to be a class !!!!!!!!!!!!!!!!!!!!!!!!!!!
class TravelObjective : public QuestObjective 
{
public: 
	TravelObjective(); 

	bool Update(); 

	~TravelObjective(); 

private:
	iPoint destination;
};

// That does not need to be a class !!!!!!!!!!!!!!!!!!!!!!!!!!!
class CountObjective : public QuestObjective 
{
	CountObjective();

	~CountObjective();

private:
	int total = 0;
	int current = 0; 
};


// That does not need to be a class !!!!!!!!!!!!!!!!!!!!!!!!!!!
class Quest
{
public: 

	Quest(string& task, int id);

	bool Update();

	~Quest() {};

private: 
	bool                    done = false; 
	vector<QuestObjective*> objectives; 
	string                  task;
	int                     progress = -1; 
	int                     id = -1;
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

	void CreateQuest(string& task, int id);

	int tasks_done = 0;

private:

private:
	vector<Quest>     quest_list;
	vector<UI_Image*> quest_balls; 

	iPoint            placer = NULLPOINT; 
	SDL_Rect          done_rect = NULLRECT;
};

#endif