#ifndef QUEST_MANAGER
#define QUEST_MANAGER

#include <string>
#include <vector>
#include "PugiXml\src\pugixml.hpp"
struct Objectives {
	int current_progress;
	int requirement;
};
enum Quest_State {
	unavailable,
	active,
	completed
};
struct Quest {
	int id;
	std::string name;
	std::string description;
	Quest_State state;
	Objectives task;
};
class QuestManager {
public:
	QuestManager();
	~QuestManager();
	pugi::xml_document quests_file;
	pugi::xml_node quests_node;

public:
	void change_state(int id,Quest_State new_state);
	void add_progress(int id);
	void update_progress();
	std::vector<Quest*> vquest;


};


























#endif