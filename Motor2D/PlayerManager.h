#ifndef __PLAYER_MANAGER_H__
#define __PLAYER_MANAGER_H__

#include "j1Module.h"
#include "p2Defs.h"
#include "p2List.h"
#include "Entity.h"
#include "j1Entity.h"
#include "Item.h"

class b2Fixture;
class PhysBody;
class Quest;
class Entity;

struct Player
{
	Player() {  };
	Player(Entity* _entity, uint _controller_index, uint _viewport)
	{
		entity = _entity; state = states::idle_down; controller_index = _controller_index, viewport = _viewport;
	}

	Entity*  entity = nullptr;
	states   state = states::states_null;
	shows	 show = shows::show_null;
	movement move = stop;
	Item*	 items[3] = { nullptr,nullptr,nullptr };
	uint	 controller_index = 0;
	uint	 viewport = 0;
};

class PlayerManager
{
public:

	PlayerManager();

	// Destructor
	~PlayerManager();

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

	Player* AddPlayer(entity_name name, iPoint pos, int controller_index, int viewport, int team, bool on_spawn = true, int show_life_bar = true);
	void ChangePlayer(entity_name name, int controller_index, int viewport);
	void DeletePlayer(int index);
	void ClearPlayers();

	std::vector<Entity*> GetTeamPlayers(int team);
	std::vector<int> GetTeamViewports(int team);
	int GetEntityViewportIfIsPlayer(Entity* entity);
	bool CheckIfSpawnPointIsUsed(int team, iPoint pos);

private:

private:
	vector<Player*> players;
	vector<iPoint> spawn_points_used_team1;
	vector<iPoint> spawn_points_used_team2;

};

#endif // __PLAYER_MANAGER_H__