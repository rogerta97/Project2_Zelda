#ifndef QUEST_MANAGER
#define QUEST_MANAGER

#include <string>
#include <vector>
#include "PugiXml\src\pugixml.hpp"
#include "j1Gui.h"
#include "Cuco.h"

class Entity;

struct objectives 
{
	int current_progress = 0;
	int requirement = 0;
	int times_completed = 0;
};

enum quest_state 
{
	inactive,
	active,
	q_s_null,
};

struct quest 
{
	quest() {};
	~quest() {};

	int         id = 0;
	std::string name;
	std::string description;
	quest_state state = q_s_null;
	std::vector<objectives*> task;
};

struct PlayerText
{
	void CleanUp()
	{
		for (int i = 0; i < quest_balls_animator.size(); i++)
		{
			quest_balls_animator.at(i)->CleanUp();
			RELEASE(quest_balls_animator.at(i));
		}
		quest_balls_animator.clear();
	}

	vector<UI_Text*> player_text;

	UI_Text* active_quest_text;

	vector<Animator*> quest_balls_animator;

	vector<UI_Image*> quest_balls_images;
};

class QuestManager 
{
public:
	QuestManager();
	~QuestManager();

	void Update();

	void CleanUp();

	void DeathQuestEvent(Entity* attacant, Entity* victim);

	void reset_progress(int id);
	void change_state(int id, quest_state new_state);
	void add_progress(int id, int team);
	int get_progress(int id, int team);
	void update_progress();
	void SpawnCucos(int num);
	void SwitchWindowState(int player);
	void UpdateWindows();
	void UpdateQuestAnimations(float dt);

public:
	pugi::xml_document  quests_file;
	pugi::xml_node      quests_node;

	pugi::xml_document  quests_animations_file;

	std::vector<quest*> vquest;

	vector<PlayerText*> player_text_list; 
	vector<UI_Image*>   player_quest_windows;
	vector<UI_Image*>	player_remap_button;
	iPoint              placer = NULLPOINT;

	vector<Entity*>     cucos;
	vector<bool>        windows_to_move;

	bool                quests_enabled = true;
	int				    active_quest = -1;

private:
	int                 quest_fx = 0;
	vector<bool>	    stop_window;
	uint			    timer_read = 0;
	SDL_Rect		    test_rect = NULLRECT;
	
};

#endif