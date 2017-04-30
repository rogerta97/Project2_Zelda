#ifndef _TACTICAI_H_
#define _TACTICAI_H_

#include "p2Defs.h"
#include "p2Point.h"
#include <list>

class Entity;

enum attack_spots
{
	a_s_top = 0,
	a_s_bot,
	a_s_left,
	a_s_right,
	a_s_null,
};

struct tactic_entity
{
	Entity* entity;
	bool field_spots[a_s_null] = { false,false,false,false };
	iPoint target_pos = {0,0};
};

class TacticAI
{
public:
	TacticAI();
	~TacticAI();

	iPoint GetTargetPos(Entity* actual_ent,Entity* target);

private:

public:

private:
	std::list<tactic_entity*> ai_entity_list;
};

#endif // !_TACTICAI_H_
