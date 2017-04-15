#include "Entity.h"
#include "j1Viewports.h"
#include "GameObject.h"
#include "PlayerManager.h"
#include "p2Log.h"
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

Ability* Entity::AddAbility(int number, int cooldow, int base_damage, int damage_multiplier, char* name)
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
	if (stats.life > 0)
		stats.life -= damage;
	if (stats.life < 0)
		stats.life = 0;
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
	stats.speed *= speed_multiplicator;
	slow s(time, this);
	App->entity->slowed_entities.push_back(s);
}

void Entity::Stun(float time)
{
	stuned = true;
	stun s(time, this);
	App->entity->stuned_entities.push_back(s);
}

void Entity::BoomerangEffects(Entity* entity, Ability * ability, Spell * spell)
{
	if (spell->stats.slow_duration > 0)
		Slow(spell->stats.slow_multiplicator, spell->stats.slow_duration);
	if (spell->stats.stun_duration > 0)
		Stun(spell->stats.stun_duration);
}


void Entity::LifeBar(iPoint size, iPoint offset)
{
	if (show_life_bar && game_object != nullptr)
	{
		SDL_Rect rect = { game_object->GetPos().x + offset.x, game_object->GetPos().y + offset.y, size.x, size.y };
		SDL_Rect life = rect;

		// Rule of thirds
		life.w = (rect.w*stats.life) / stats.max_life;
		if (life.w < 0)
			life.w = 0;

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
						enemy = false;
						break;
					}
				}

				// Enemy printing (red)
				if(enemy)
					App->view->LayerDrawQuad(life, 255, 0, 0, 255, true, 10, viewport, true);
			}
		}
	}
}

void Entity::UpdateStats(int extra_power, int extra_hp, int extra_speed)
{
	stats.speed = stats.restore_speed = stats.base_speed + extra_speed;
	stats.max_life = stats.base_hp + extra_hp;
	switch (team)
	{
	case 1:
	{
		stats.power = (stats.base_power + extra_power)*(1 + (App->scene->main_scene->quest_manager->get_progress(1,GetTeam()))*0.1);
		stats.max_life = (stats.base_hp + extra_hp)*(10 * (App->scene->main_scene->quest_manager->get_progress(3, GetTeam())));
		break;
	}
	case 2:
	{
		stats.power = (stats.base_power + extra_power)*(1 + (App->scene->main_scene->quest_manager->get_progress(1, GetTeam()))*0.1);
		stats.max_life = (stats.base_hp + extra_hp)*(10 * (App->scene->main_scene->quest_manager->get_progress(3, GetTeam())));
		break;
	}
	default:
		break;
	}
}

float Ability::GetCdTimeLeft()
{
	float ret = cd - cd_timer.ReadSec();

	if (ret < 0)
		ret = 0;

	return ret;
}
