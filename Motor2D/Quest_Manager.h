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
	vector<UI_Text*> player_text;

	vector<UI_Text*> active_quest_text;
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
	pugi::xml_document quests_file;
	pugi::xml_node quests_node;

public:
	void change_state(int id, quest_state new_state);
	void add_progress(int id, int team);
	int get_progress(int id, int team);
	void update_progress();
	void SpawnCucos(int num);
	std::vector<quest*> vquest;

	vector<PlayerText*> player_text_list; 

	iPoint           placer = NULLPOINT;

	vector<Entity*> cucos;

	bool quests_enabled = true;
private:
	int				active_quest = -1;
	uint			timer_read = 0;

};


























#endif