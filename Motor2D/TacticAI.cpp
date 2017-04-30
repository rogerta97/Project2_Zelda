#include "TacticAI.h"
#include "Entity.h"

TacticAI::TacticAI()
{
}

TacticAI::~TacticAI()
{
}

iPoint TacticAI::GetTargetPos(Entity * actual_ent, Entity * target)
{
	iPoint ret;

	//Look if target is regidtered for ai tactics
	bool exist = false;
	for (std::list<tactic_entity*>::iterator it = ai_entity_list.begin(); it != ai_entity_list.end(); ++it)
	{
		if ((*it)->entity == target)
		{
			exist = true;
			break;
		}
	}	

	//Add to tactic ai list if target doesn't exist
	if (!exist)
	{
		tactic_entity* ent = new tactic_entity();

		ent->entity = target;

		ai_entity_list.push_back(ent);
	}

	//check side relative to target 1 right -1 left
	int side = 0;
	if (target->GetPos().x > actual_ent->GetPos().x)
		side = -1;
	else side = 1;

	//chech if target can move
	bool can_move = true;
	if(target->type == tower || target->type == base

	return ret;
}
