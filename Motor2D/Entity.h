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
#include "Entity.h"
#include "j1Entity.h"
#include "EventThrower.h"

class b2Fixture;
class PhysBody;
class GameObject;
class Spell;

#define ANIMATIONS_TEAM 1

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

	snake_up,
	snake_lateral,
	snake_down,	
	snake_attack_up,
	snake_attack_lateral,
	snake_attack_down,

	skeleton_idle,
	skeleton_stunned,
	skeleton_spin,
	skeleton_bone,

	mskeleton_up,
	mskeleton_lateral,
	mskeleton_down,
	mskeleton_attack_up,
	mskeleton_attack_lateral,
	mskeleton_attack_down,

	guard_up,
	guard_left,
	guard_right,
	guard_down,
	guard_attack_up,
	guard_attack_left,
	guard_attack_right,
	guard_attack_down,

	basic_chicken,
	cap_chicken,
	black_chicken,
	pink_chicken,
	cook_chicken,
	stone_chicken,

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

	int shield = 0;
};

struct Ability
{
	Ability(int _number, int _damage, float _damage_multiplicator, float _cd, char* _name)
	{
		index = _number; damage = _damage; damage_multiplicator = _damage_multiplicator, cd = _cd; cd = _cd;
		name = _name;
		cd_timer = App->AddGameplayTimer();
		cd_timer->SubstractTimeFromStart(_cd);
	};

	void SetImages(SDL_Rect _ablility_avaliable, SDL_Rect _ability_avaliable_pressed, SDL_Rect _ability_in_cd, SDL_Rect _ability_icon)
	{
		ability_avaliable_pressed = _ability_avaliable_pressed; ablility_avaliable = _ablility_avaliable; ability_in_cd = _ability_in_cd; ability_icon = _ability_icon; 
	}

	bool CdCompleted()
	{
		return cd_timer->ReadSec() >= cd;
	}

	void CdReset() 
	{
		cd_timer->Start();
	}

	void CleanUp() 
	{
		App->DeleteGameplayTimer(cd_timer);
	};

	float GetCdTimeLeft();

	int        index = 0;
	float      cd = 0;

	float      damage = 0.0f;
	float	   damage_multiplicator = 1.0f;
	float      slow_duration = 0.0f;
	float	   slow_force = 1.0f;
	float	   stun_duration = 0.0f;

	b2Fixture* fixture = nullptr;
	string     name;

	j1Timer*   cd_timer = nullptr;

	SDL_Rect   ability_avaliable_pressed = NULLRECT;
	SDL_Rect   ablility_avaliable = NULLRECT;
	SDL_Rect   ability_in_cd = NULLRECT;
	SDL_Rect   ability_icon = NULLRECT; 
};

class Entity
{
public:
	Entity(){};

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
	virtual void ListenEv(int type, EventThrower* origin, int id) {};

	Ability* AddAbility(int number, float cooldow, int base_damage, float damage_multiplier, char* name = "no_name");
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

	void LifeBar(iPoint size, iPoint offset = { 0, 0 }, int shiled = 0); 

	//Update Stats
	void UpdateStats(int extra_power, int extra_hp, int extra_speed);

	//set invulnerability
	void SetInvulnerable();

	void SetAbilityImages(int ability_id, SDL_Rect icon_rect); 

	virtual void Die(Entity* killed_by) {};

private:
	
private:
	uint		     team = 0;

public:
	// Containers
	GameObject*      game_object = nullptr;
	vector<Ability*> abilities;
	Stats	         stats;
	entity_name		 type = entity_name::e_n_null;

	// States
	bool		     can_move = false;
	bool             attacking = false;
	bool			 is_player = false;
	bool			 stuned = false;
	bool			 invulnerable = false;

	// Life bar
	bool			 show_life_bar = false;
	int				 blit_layer = 0;

	//Progress bar
	iPoint           progress_limit = iPoint(0, 0);

	// Got Hit
	bool		     hit = false;
	Entity*	         hit_by = nullptr;
	Ability*		 hit_ability = nullptr;
	Spell*			 hit_spell = nullptr;

	// Delete 
	bool			 to_delete = false;

	string			 name;
	
protected:
	// Draw
	iPoint           draw_offset = NULLPOINT;
	iPoint		     restore_draw_offset = NULLPOINT;
};

#endif