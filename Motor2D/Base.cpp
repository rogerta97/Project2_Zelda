#include "Base.h"
#include "GameObject.h"
#include "j1Entity.h"
#include "Scene.h"
#include "j1Scene.h"
#include "MainScene.h"
#include "j1Viewports.h"
#include "BaseManager.h"

Base::Base(iPoint pos)
{
	game_object = new GameObject(iPoint(pos.x, pos.y), iPoint(242, 180), App->cf->CATEGORY_PLAYER, App->cf->MASK_PLAYER, pbody_type::p_t_npc, 0);

	game_object->CreateCollision(iPoint(0, 0), game_object->GetHitBoxSize().x, game_object->GetHitBoxSize().y, fixture_type::f_t_hit_box);
	game_object->SetListener((j1Module*)App->entity);
	game_object->SetFixedRotation(true);
	game_object->SetKinematic();

	pugi::xml_document doc;
	App->LoadXML("base.xml", doc);
	game_object->SetTexture(game_object->LoadAnimationsFromXML(doc, "animations"));
	App->UnloadXML(doc);

	name = "base";
}

Base::~Base()
{
}

bool Base::Start()
{
	show_life_bar = true;

	game_object->SetAnimation("idle");

	stats.max_life = stats.life = 400;

	return true;
}

bool Base::Update(float dt)
{
	LifeBar(iPoint(120, 10), iPoint(-55, -160));

	Entity* entity = nullptr;
	Ability* ability = nullptr;
	Spell* spell = nullptr;

	if (GotHit(entity, ability, spell) && stats.life)
	{
		// Enemy attacks
		if (entity != nullptr && ability != nullptr && entity->GetTeam() != GetTeam())
		{
			DealDamage(ability->damage * ability->damage_multiplicator);

			if (spell != nullptr && TextCmp(spell->name.c_str(), "boomerang"))
			{
				DealDamage(ability->damage * (spell->stats.damage_multiplicator - 1)); // Spells control their own damage mutiplicator

				if (spell->stats.slow_duration > 0)
					Slow(spell->stats.slow_multiplicator, spell->stats.slow_duration);
				if (spell->stats.stun_duration > 0)
					Stun(spell->stats.stun_duration);
			}

			if (stats.life <= 0)
			{
				App->scene->main_scene->base_manager->KillBase(this);
				App->scene->main_scene->EndGame((GetTeam() == 1) ? 1 : 2);
			}
		}
	}

	return true;
}

bool Base::Draw(float dt)
{
	App->view->LayerBlit(game_object->GetPos().y, game_object->GetTexture(), { game_object->GetPos().x - 120, game_object->GetPos().y - 144}, game_object->GetCurrentAnimationRect(dt), 0, -1.0f, true, SDL_FLIP_NONE);
	return true;
}

bool Base::CleanUp()
{
	return true;
}
