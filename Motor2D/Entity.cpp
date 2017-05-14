#include "Entity.h"
#include "j1Viewports.h"
#include "GameObject.h"
#include "PlayerManager.h"
#include "p2Log.h"
#include "Mapping.h"
#include "Functions.h"
#include "Quest_Manager.h"

void Entity::CleanEntity()
{
	CleanAbilities();
	game_object->CleanUp();
	RELEASE(game_object);
}

void Entity::CleanAbilities()
{
	for (int i = 0; i < abilities.size(); i++)
	{
		abilities.at(i)->CleanUp();
		RELEASE(abilities.at(i));
	}
	abilities.clear();
}

bool Entity::GotHit(Entity *& entity, Ability *& ability, Spell* &spell)
{
	if (hit)
	{
		entity = hit_by;
		ability = hit_ability;
		spell = hit_spell;
		hit_by = nullptr;
		hit_ability = nullptr;
		hit_spell = nullptr;
		hit = false;

		return true;
	}
	return false;
}


Ability* Entity::AddAbility(int number, float cooldow, int base_damage, float damage_multiplier, char* name)
{
	Ability* ret = nullptr;

	Ability* ability = new Ability(number, base_damage, damage_multiplier, cooldow, name); abilities.push_back(ability); ret = ability;

	return ability;
}

Ability* Entity::GetAbility(int number)
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

Ability * Entity::GetAbilityByName(const char* name)
{
	Ability* ret = nullptr;

	for (int i = 0; i < abilities.size(); i++)
	{
		if (TextCmp(name, abilities.at(i)->name.c_str()))
		{
			ret = abilities.at(i);
			break;
		}
	}

	return ret;
}

int Entity::GetLife()
{
	return stats.life;
}

void Entity::DealDamage(int damage)
{
	if (invulnerable)
		return;

	if (stats.life > 0)
	{
		if (stats.shield > 0)
		{
			stats.shield -= damage;

			if (stats.shield < 0)
				stats.life += stats.shield;
		}
		else 
			stats.life -= damage;
	}

	if (stats.life < 0)
		stats.life = 0;

	if (stats.shield < 0)
		stats.shield = 0;
}

void Entity::Heal(int heal)
{
	if (stats.life > 0)
		stats.life += heal;
	if (stats.life > stats.max_life)
		stats.life = stats.max_life;
}

void Entity::Slow(float speed_multiplicator, float time)
{	
	for (list<slow>::iterator it = App->entity->slowed_entities.begin(); it != App->entity->slowed_entities.end();)
	{
		if ((*it).entity == this)
			return;
		else
			++it;
	}

	stats.speed *= speed_multiplicator;
	slow s(time, this);
	App->entity->slowed_entities.push_back(s);
}

void Entity::Stun(float time)
{
	if (time < 0 || type == entity_name::base || type == entity_name::tree || type == entity_name::tower)
		return;

	for (list<stun>::iterator it = App->entity->stuned_entities.begin(); it != App->entity->stuned_entities.end();)
	{
		if ((*it).entity == this)
			return;
		else
			++it;
	}

	stuned = true;
	stun s(time, this, App->entity->GetEntityEffectsAnimator()->GetAnimation("stun"));
	App->entity->stuned_entities.push_back(s);
}

void Entity::LifeBar(iPoint size, iPoint offset, int shield)
{
	if (game_object != nullptr)
	{
		SDL_Rect rect = { game_object->GetPos().x + offset.x, game_object->GetPos().y + offset.y, size.x, size.y };
		SDL_Rect life = rect;
		SDL_Rect shld = rect;

		// Rule of thirds
		life.w = (rect.w*stats.life) / stats.max_life;
		if (life.w < 0)
			life.w = 0;

		shld.x = life.x + life.w + 2;
		shld.w = (float)(rect.w * shield) / (float)stats.max_life;
		if (shld.w < 0)
			shld.w = 0;

		// Back bar
		App->view->LayerDrawQuad(rect, 30, 30, 30, 255, true, 9, 0, true);

		// Get the viewports of my team
		vector<int> my_team = App->scene->main_scene->player_manager->GetTeamViewports(team);

		int main_view = 0;

		if (is_player)
		{
			main_view = App->scene->main_scene->player_manager->GetEntityViewportIfIsPlayer(this);
		}
	
		// Print to viewports depending the team
		for (int viewport = 1; viewport <= App->view->GetViews(); viewport++)
		{
			// Current viewport is my viewport (yellow)
			if (viewport == main_view)
			{
				App->view->LayerDrawQuad(life, 255, 255, 51, 255, true, 10, viewport, true);
				if (shield > 0)
				{
					App->view->LayerDrawQuad({ shld.x - 2, shld.y - 2, shld.w + 4, shld.h + 4 }, 85, 85, 51, 255, true, 11, viewport, true);
					App->view->LayerDrawQuad(shld, 165, 165, 51, 255, true, 12, viewport, true);
				}
			}
			else
			{
				// Team printing (blue)
				bool enemy = true;
				for (int i = 0; i < my_team.size(); i++)
				{
					if (viewport == my_team.at(i))
					{
						App->view->LayerDrawQuad(life, 51, 153, 255, 255, true, 10, viewport, true);

						if (shield > 0)
						{
							App->view->LayerDrawQuad({ shld.x - 2, shld.y - 2, shld.w + 4, shld.h + 4 }, 51, 50, 100, 255, true, 11, viewport, true);
							App->view->LayerDrawQuad(shld, 51, 103, 165, 255, true, 12, viewport, true);
						}
						enemy = false;
						break;
					}
				}

				// Enemy printing (red)
				if (enemy)
				{
					App->view->LayerDrawQuad(life, 255, 0, 0, 255, true, 10, viewport, true);

					if (shield > 0)
					{
						App->view->LayerDrawQuad({ shld.x - 2, shld.y - 2, shld.w + 4, shld.h + 4 }, 100, 0, 0, 255, true, 11, viewport, true);
						App->view->LayerDrawQuad(shld, 165, 0, 0, 255, true, 12, viewport, true);
					}
				}
			}
		}
	}
}

void Entity::UpdateStats(int extra_power, int extra_hp, int extra_speed)
{
	switch (team)
	{
	case 1:
	{
		stats.power = (stats.base_power + extra_power) + (5 * (App->scene->main_scene->quest_manager->get_progress(1,GetTeam())));
		stats.max_life = (stats.base_hp + extra_hp) + (10 * (App->scene->main_scene->quest_manager->get_progress(3, GetTeam())));
		stats.speed = (stats.base_speed + extra_speed) + (15 * (App->scene->main_scene->quest_manager->get_progress(2, GetTeam())));
		break;
	}
	case 2:
	{
		stats.power = (stats.base_power + extra_power) + (5 * (App->scene->main_scene->quest_manager->get_progress(1, GetTeam())));
		stats.max_life = (stats.base_hp + extra_hp) + (10 * (App->scene->main_scene->quest_manager->get_progress(3, GetTeam())));
		stats.speed = (stats.base_speed + extra_speed) + (15 * (App->scene->main_scene->quest_manager->get_progress(2, GetTeam())));
		break;
	}
	default:
		break;
	}
}

void Entity::SetInvulnerable()
{
	invulnerable = true;
}

void Entity::SetAbilityImages(int ability_id, SDL_Rect icon_rect)
{
	abilities[ability_id - 1]->ability_icon = icon_rect; 
}

float Ability::GetCdTimeLeft()
{
	float ret = cd - cd_timer->ReadSec();

	if (ret < 0)
		ret = 0;

	return ret;
}
