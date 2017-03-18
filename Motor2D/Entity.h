#ifndef _ENTITY_
#define _ENTITY_

#include "j1App.h"
#include "j1Render.h"
#include "PugiXml\src\pugixml.hpp"
#include "j1Physics.h"
#include "Animation.h"
#include "Functions.h"
#include "Spell.h"
#include "j1Spell.h"

class b2Fixture;
class PhysBody;
class GameObject;
class Spell;

enum states
{
	run_up,
	run_left,
	run_down,
	run_right,

	walk_up,
	walk_left,
	walk_down,
	walk_right,

	idle_up,
	idle_left,
	idle_down,
	idle_right,

	basic_atack_up,
	basic_atack_left,
	basic_atack_down,
	basic_atack_right,

	ability1_up,
	ability1_left,
	ability1_down,
	ability1_right,

	ability2_up,
	ability2_left,
	ability2_down,
	ability2_right,

	ability3_up,
	ability3_left,
	ability3_down,
	ability3_right,

	tower_idle,
	tower_attack,

	states_null,
};

enum shows
{
	show_null,
	show_basic_atack_up,
	show_basic_atack_left,
	show_basic_atack_down,
	show_basic_atack_right,

	show_ability1_up,
	show_ability1_left,
	show_ability1_down,
	show_ability1_right,

	show_ability2_up,
	show_ability2_left,
	show_ability2_down,
	show_ability2_right,

	show_ability3_up,
	show_ability3_left,
	show_ability3_down,
	show_ability3_right,
};

enum movement
{
	move_up,
	move_down,
	move_left,
	move_right,

	move_upright,
	move_downright,
	move_upleft,
	move_downleft,

	stop,
};

class Stats
{
public:
	Stats() {};
	~Stats() {};

public:
	int base_speed = 0;
	int speed = 0;
	int restore_speed = 0;

	int base_hp = 100;
	int max_life = 100;
	int life = 100;

	int base_power = 0;
	int power = 0;	
};

struct Ability
{
	Ability(int _number, int _damage, float _cd, float _duration, char* _name)
	{
		index = _number; damage = _damage;  cd = _cd; duration = _duration; cd = _cd;
		name = _name;
	};

	int        index = 0;
	float      cd = 0;
	float      duration = 0;

	float      damage = 0.0f;
	float	   damage_multiplicator = 1.0f;
	float      slow_duration = 0.0f;
	float	   slow_force = 1.0f;
	float	   stun_duration = 0.0f;

	b2Fixture* fixture = nullptr;
	string     name;
};

class Entity
{
public:
	Entity() {};

	virtual ~Entity() {};

	virtual bool Start() { return true; };
	virtual bool PreUpdate() { return true; };
	virtual bool Update(float dt) { return true; };
	virtual bool Draw(float dt) { return true; };
	virtual bool PostUpdate() { return true; };
	virtual bool CleanUp() { return true; };

	// Characters
	virtual void MoveUp(float speed) {};
	virtual void MoveDown(float speed) {};
	virtual void MoveLeft(float speed) {};
	virtual void MoveRight(float speed) {};

	virtual void MoveUpRight(float speed) {};
	virtual void MoveDownRight(float speed) {};
	virtual void MoveUpLeft(float speed) {};
	virtual void MoveDownLeft(float speed) {};

	virtual void RunUp() {};
	virtual void RunDown() {};
	virtual void RunLeft() {};
	virtual void RunRight() {}

	virtual void IdleUp() {};
	virtual void IdleDown() {};
	virtual void IdleLeft() {};
	virtual void IdleRight() {};

	virtual void BasicAttackUp() {};
	virtual void BasicAttackDown() {};
	virtual void BasicAttackLeft() {};
	virtual void BasicAttackRight() {};

	virtual void ShowBasicAttackUp() {};
	virtual void ShowBasicAttackDown() {};
	virtual void ShowBasicAttackLeft() {};
	virtual void ShowBasicAttackRight() {};

	virtual void Ability1Up() {};
	virtual void Ability1Down() {};
	virtual void Ability1Left() {};
	virtual void Ability1Right() {};

	virtual void ShowAbility1Up() {};
	virtual void ShowAbility1Down() {};
	virtual void ShowAbility1Left() {};
	virtual void ShowAbility1Right() {};

	virtual void Ability2Up() {};
	virtual void Ability2Down() {};
	virtual void Ability2Left() {};
	virtual void Ability2Right() {};

	virtual void ShowAbility2Up() {};
	virtual void ShowAbility2Down() {};
	virtual void ShowAbility2Left() {};
	virtual void ShowAbility2Right() {};

	virtual void Ability3Up() {};
	virtual void Ability3Down() {};
	virtual void Ability3Left() {};
	virtual void Ability3Right() {};

	virtual void ShowAbility3Up() {};
	virtual void ShowAbility3Down() {};
	virtual void ShowAbility3Left() {};
	virtual void ShowAbility3Right() {};

	void CleanEntity();
	void CleanAbilities();

	bool GotHit(Entity* &entity, Ability* &ability, Spell* &spell);

	virtual void SetCamera(int index) {};

	virtual iPoint GetPos() const { return{ 0,0 }; }

	virtual bool Load(pugi::xml_node&)
	{
		return true;
	}

	virtual bool Save(pugi::xml_node&) const
	{
		return true;
	}

	virtual void OnColl(PhysBody* bodyA, PhysBody* bodyB, b2Fixture* fixtureA, b2Fixture* fixtureB) {};
	virtual void OnCollEnter(PhysBody* bodyA, PhysBody* bodyB, b2Fixture* fixtureA, b2Fixture* fixtureB) {};
	virtual void OnCollOut(PhysBody* bodyA, PhysBody* bodyB, b2Fixture* fixtureA, b2Fixture* fixtureB) {};

	void AddAbility(int number, int damage, int cooldow, int duration, char* name = "no_name");
	Ability* GetAbility(int number);
	Ability* GetAbilityByName(const char* name);

	int  GetLife();
	void DealDamage(int damage);
	void Heal(int heal);
	void Slow(float speed_multiplicator, float time);
	void Stun(float time);

	//Set Team if not set already
	void SetTeam(uint _team) 
	{
		if (team == 0) {
			team = _team;
		}
	}

	//Return entity team. 0 if neutral
	uint GetTeam() const
	{
		return team;
	}

	void LifeBar(iPoint size, iPoint offset = { 0, 0 }); 

	//Update Stats
	void UpdateStats(int extra_power, int extra_hp, int extra_speed);

private:
	
private:
	uint		     team = 0;

public:
	// Containers
	GameObject*      game_object = nullptr;
	vector<Ability*> abilities;
	Stats	         stats;

	// States
	bool			 disable_controller = false;
	bool		     can_move = false;
	bool             attacking = false;
	bool			 is_player = false;
	bool			 stuned = false;

	// Life bar
	bool			 show_life_bar = false;
	int				 blit_layer = 0;

	//Progress bar
	iPoint  progress_limit = iPoint(0, 0);

	// Got Hit
	bool		     hit = false;
	Entity*	         hit_by = nullptr;
	Ability*		 hit_ability = nullptr;
	Spell*			 hit_spell = nullptr;

	// Delete 
	bool			 to_delete = false;
	
protected:
	// Draw
	iPoint           draw_offset = NULLPOINT;
	iPoint		     restore_draw_offset = NULLPOINT;

	j1Timer			 stun_slow_timer;
};

#endif