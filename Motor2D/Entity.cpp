#include "Entity.h"
#include "j1Viewports.h"
#include "GameObject.h"
#include "PlayerManager.h"
#include "p2Log.h"

#define LIFE_BAR_COLOR_1 {30, 30, 30, 255}

void Entity::CleanEntity()
{
	RELEASE(game_object);
	CleanAbilities();
}

bool Entity::GotHit(Entity *& entity, Ability *& ability)
{
	if (hit)
	{
		entity = hit_by;
		ability = hit_ability;
		hit_by = nullptr;
		hit_ability = nullptr;
		hit = false;

		return true;
	}
	return false;
}

void Entity::AddAbility(int number, int damage, int cooldow, int duration)
{
	Ability* ability = new Ability(number, damage, cooldow, duration); abilities.push_back(ability);
}

Ability * Entity::GetAbility(int number)
{
	Ability* ret = nullptr;

	for (int i = 0; i < abilities.size(); i++)
	{
		if (number == abilities.at(i)->index)
		{
			ret = abilities.at(i);
			break;
		}
	}

	return ret;
}

void Entity::CleanAbilities()
{
	for (int i = 0; i < abilities.size(); i++)
	{
		RELEASE(abilities.at(i));
	}
	abilities.clear();
}

void Entity::LifeBar(iPoint size, iPoint offset)
{
	if (show_life_bar && game_object != nullptr)
	{
		SDL_Rect rect = { game_object->GetPos().x + offset.x, game_object->GetPos().y + offset.y, size.x, size.y };
		SDL_Rect life = rect;

		// Rule of thirds
		life.w = (rect.w*stats.life) / stats.max_life;

		// Back bar
		App->view->LayerDrawQuad(rect, 30, 30, 30, 255, true, 10, 0, true);

		vector<int> my_team = App->entity->player_manager->GetTeamViewports(team);

		int main_view = 0;

		// Get the viewports of my team
		if (is_player)
		{
			main_view = App->entity->player_manager->GetEntityViewportIfIsPlayer(this);
		}
	
		// Print to viewports depending the team
		for (int viewport = 1; viewport <= App->view->GetViews(); viewport++)
		{
			// Current viewport is my viewport
			if (viewport == main_view)
			{
				App->view->LayerDrawQuad(life, 255, 255, 51, 255, true, 10, viewport, true);
			}
			else
			{
				// Team viewport
				bool enemy = true;
				for (int i = 0; i < my_team.size(); i++)
				{
					if (viewport == my_team.at(i))
					{
						App->view->LayerDrawQuad(life, 51, 153, 255, 255, true, 10, viewport, true);
						enemy = false;
						break;
					}
				}

				// Enemy viewport
				if(enemy)
					App->view->LayerDrawQuad(life, 255, 0, 0, 255, true, 10, viewport, true);
			}
		}
	}
}

