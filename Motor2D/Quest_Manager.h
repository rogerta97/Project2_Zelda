#ifndef QUEST_MANAGER
#define QUEST_MANAGER

#include <string>
#include <vector>
#include "PugiXml\src\pugixml.hpp"
#include "j1Gui.h"
struct Objectives {
	int current_progress;
	int requirement;
	int times_completed;
};
enum Quest_State {
	inactive,
	active,
};
struct Quest {
	int id;
	std::string name;
	std::string description;
	Quest_State state;
	std::vector<Objectives*> task;
};
class QuestManager {
public:
	QuestManager();
	~QuestManager();
	pugi::xml_document quests_file;
	pugi::xml_node quests_node;

public:
	void change_state(int id,Quest_State new_state);
	void add_progress(int id,int team);
	int get_progress(int id,int team);
	void update_progress();
	std::vector<Quest*> vquest;

	vector<UI_Image*> abilitie_icons;

	iPoint            placer = NULLPOINT;
	SDL_Rect          done_rect = NULLRECT;

};


























#endif