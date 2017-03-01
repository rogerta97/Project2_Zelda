#ifndef __PLAYER_MANAGER_H__
#define __PLAYER_MANAGER_H__

#include "j1Module.h"
#include "p2Defs.h"
#include "p2List.h"
#include "Entity.h"
#include "j1Entity.h"

class b2Fixture;
class PhysBody;
class Quest;
class Entity;

struct Player
{
	Player() {  };
	Player(Entity* _entity, uint _index)
	{
		entity = _entity; state = states::idle_down; index = _index;
	}

	Entity*  entity = nullptr;
	states   state = states_null;
	movement move = stop;
	uint	 index = 0;
};

class PlayerManager
{
public:

	PlayerManager();

	// Destructor
	virtual ~PlayerManager();

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

	Player* AddPlayer(entity_name name, int index, iPoint pos, int team, int show_life_bar = true);
	void ChangePlayer(entity_name name, int index);
	void DeletePlayer(int index);
	void ClearPlayers();

	std::vector<Entity*> GetTeamPlayers(int team);
	std::vector<int> GetTeamViewports(int team);
	int GetEntityViewportIfIsPlayer(Entity* entity);

private:

private:
	vector<Player*> players;

};

#endif // __PLAYER_MANAGER_H__