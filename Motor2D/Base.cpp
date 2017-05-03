#include "Base.h"
#include "GameObject.h"
#include "j1Entity.h"
#include "j1Scene.h"
#include "MainScene.h"
#include "j1Viewports.h"
#include "BaseManager.h"
#include "Quest_Manager.h"
#include "j1XMLLoader.h"
#include "j1Pathfinding.h"
#include "j1Map.h"

Base::Base(iPoint pos)
{
	game_object = new GameObject(iPoint(pos.x, pos.y), iPoint(242, 180), App->cf->CATEGORY_PLAYER, App->cf->MASK_PLAYER, pbody_type::p_t_npc, 0);

	int Base_entity[56] = {
		2, 106,
		16, 99,
		25, 99,
		28, 45,
		75, 46,
		99, 37,
		103, 17,
		123, 9,
		142, 19,
		143, 42,
		215, 45,
		217, 101,
		240, 104,
		239, 133,
		239, 189,
		217, 196,
		217, 248,
		207, 260,
		164, 260,
		144, 288,
		103, 288,
		82, 260,
		38, 260,
		28, 248,
		32, 194,
		10, 194,
		1, 182,
		1, 147
	};

	game_object->CreateCollision(iPoint(-242/2, -290/2), Base_entity, 56, fixture_type::f_t_hit_box);
	game_object->SetListener((j1Module*)App->entity);
	game_object->SetFixedRotation(true);
	game_object->SetKinematic();

	uchar* matrix = new uchar[80];
	uchar data[80] =   { 1,1,1,0,0,1,1,1,
						 1,0,0,0,0,0,0,1,
				 		 1,0,0,0,0,0,0,1,
						 0,0,0,0,0,0,0,0,
						 0,0,0,0,0,0,0,0,
						 0,0,0,0,0,0,0,0,
						 1,0,0,0,0,0,0,1,
						 1,0,0,0,0,0,0,1,
						 1,0,0,0,0,0,0,1,
						 1,1,1,0,0,1,1,1
	};

	memcpy_s(matrix, 80, data, 80);

	iPoint map_pos = App->map->WorldToMap(GetPos().x - 242 / 2, GetPos().y - 290 / 2);
	App->pathfinding->ChangeWalkability(matrix, map_pos.x, map_pos.y, 8, 10);

	RELEASE_ARRAY(matrix);

	pugi::xml_document doc;
	App->xml->LoadXML("base.xml", doc);
	pugi::xml_node stats_node = doc.child("file").child("stats");

	stats.life = stats.base_hp = stats.max_life = stats_node.attribute("hp").as_int();

	game_object->SetTexture(game_object->LoadAnimationsFromXML(doc, "animations"));

	name = "base";
}

Base::~Base()
{
}

bool Base::Start()
{
	show_life_bar = true;

	game_object->SetAnimation("idle");

	return true;
}

bool Base::Update(float dt)
{
	Entity* entity = nullptr;
	Ability* ability = nullptr;
	Spell* spell = nullptr;

	if (GotHit(entity, ability, spell) && stats.life)
	{
		// Enemy attacks
		if (entity != nullptr && ability != nullptr && entity->GetTeam() != GetTeam())
		{
			if (!invulnerable)
			{
				if (spell == nullptr)
					DealDamage((entity->stats.power * ability->damage_multiplicator) + ability->damage);

				if (stats.life <= 0)
				{
					App->entity->AddRupeesIfPlayer(entity, 1);
					App->scene->main_scene->base_manager->KillBase(this);
					App->scene->main_scene->EndGame((GetTeam() == 1) ? 2 : 1);
				}
			}
		}
	}

	return true;
}

bool Base::Draw(float dt)
{
	LifeBar(iPoint(120, 10), iPoint(-55, -160));

	App->view->LayerBlit(game_object->GetPos().y, game_object->GetTexture(), { game_object->GetPos().x - 120, game_object->GetPos().y - 144}, game_object->GetCurrentAnimationRect(dt), 0, -1.0f, true, SDL_FLIP_NONE);
	return true;
}

bool Base::CleanUp()
{
	return true;
}

iPoint Base::GetPos() const
{
	return game_object->GetPos();
}
