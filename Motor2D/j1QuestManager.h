//#ifndef __j1QUESTMANAGER_H__
//#define __j1QUESTMANAGER_H__
//
//#include "j1Module.h"
//#include "p2Point.h"
//#include "j1Gui.h"//#include "SDL/include/SDL.h"
//
//class Quest {
//	
//public:
//	 SDL_Rect* images = nullptr; //first position stands for task not done, second for task done
//};
//
//class j1QuestManager : public j1Module
//{
//public: 
//	j1QuestManager();
//
//	virtual ~j1QuestManager();
//
//	bool Awake(pugi::xml_node&);
//
//	bool Start();
//
//	bool Update(float dt);
//
//	bool PreUpdate();
//
//	bool PostUpdate();
//
//	bool CleanUp();
//
//	Quest* CreateQuest(list<SDL_Rect*> image, iPoint pos); 
//
//private: 
//
//	list<Quest*> quest_list; 
//
//
//};
//
//#endif