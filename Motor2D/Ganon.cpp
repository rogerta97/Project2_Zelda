#include "Ganon.h"
#include "GameObject.h"
#include "j1App.h"
#include "CollisionFilters.h"
#include "Functions.h"
#include "j1Textures.h"
#include "p2Log.h"
#include "j1Viewports.h"
#include "j1Entity.h"
#include "j1Spell.h"
#include "Boomerang.h"
#include "PlayerManager.h"
#include "j1Pathfinding.h"
#include "j1Map.h"
#include "Quest_Manager.h"
#include "j1XMLLoader.h"

Ganon::Ganon(iPoint pos)
{
	game_object = new GameObject(iPoint(pos.x, pos.y), iPoint(30, 40), App->cf->CATEGORY_PLAYER, App->cf->MASK_PLAYER, pbody_type::p_t_player, 0);
	game_object->SetHitBoxSize(80, 80); 

	game_object->CreateCollisionSensor(iPoint(5,2), game_object->GetHitBoxSize().x, game_object->GetHitBoxSize().y, fixture_type::f_t_hit_box);

	uint w = 70, h = 30; 
	ganon_collision = game_object->CreateCollision(iPoint(0,10), w, h, fixture_type::f_t_collision_box);
	game_object->SetListener((j1Module*)App->entity);
	game_object->SetFixedRotation(true);
	game_object->pbody->body->SetBullet(true);

	pugi::xml_document doc;
	App->xml->LoadXML("ganon.xml", doc);

	// Loading Abilities ----------------
	pugi::xml_node stats_node = doc.child("file").child("stats");
	rupee_reward = stats_node.attribute("rupees").as_int();

	stats.life = stats.base_hp = stats.max_life = stats_node.attribute("hp").as_int();
	stats.base_power = stats.power = stats_node.attribute("power").as_int();
	stats.base_speed = stats.speed = stats.restore_speed = stats_node.attribute("speed").as_int();

	float dmg_mult = stats_node.child("ability1").attribute("mult").as_float();
	float cd = stats_node.child("ability1").attribute("cd").as_float();
	int bd = stats_node.child("ability1").attribute("bd").as_int();

	Ability* a1 = AddAbility(0, cd, bd, dmg_mult);
	a1->SetImages({ 816, 351, 80, 48 }, { 816, 473, 80, 48 }, { 1013, 1960, 80, 48 }, {0,0,0,0});

	dmg_mult = stats_node.child("ability2").attribute("mult").as_float();
	cd = stats_node.child("ability2").attribute("cd").as_float();
	bd = stats_node.child("ability2").attribute("bd").as_int();
	Ability* a2 = AddAbility(1, cd, bd, dmg_mult);
	a2->SetImages({ 896, 351, 80, 48 }, { 896, 473, 80, 48 }, { 1093, 1960, 80, 48 }, { 0,0,0,0 });

	dmg_mult = stats_node.child("ability3").attribute("mult").as_float();
	cd = stats_node.child("ability3").attribute("cd").as_float();
	bd = stats_node.child("ability3").attribute("bd").as_int();
	Ability* a3 = AddAbility(2, cd, bd, dmg_mult, "boomerang"); // Name references to the Spell name
	a3->SetImages({ 816, 399, 48, 73 }, { 816, 521, 48, 73 }, { 1013, 2008, 48, 73 }, { 0,0,0,0 });

	dmg_mult = stats_node.child("ability4").attribute("mult").as_float();
	cd = stats_node.child("ability4").attribute("cd").as_float();
	bd = stats_node.child("ability4").attribute("bd").as_int();
	Ability* a4 = AddAbility(3, cd, bd, dmg_mult);
	a4->SetImages({ 864, 399, 48, 73 }, { 864, 521,48, 73 }, { 1061, 2008, 48, 73 }, { 0,0,0,0 });
	// -------------------------------------

	game_object->SetTexture(game_object->LoadAnimationsFromXML(doc, "animations"));

	draw_offset = restore_draw_offset = {  50,  48 };

	blit_layer = 2;

	name = "ganon";
}

Ganon::~Ganon()
{
}

bool Ganon::Start()
{
	bool ret = true;

	if (GetTeam() == 1)
		game_object->SetAnimation("idle_down");
	else
		game_object->SetAnimation("idle_down_2");

	can_move = true;

	return ret;
}

bool Ganon::PreUpdate()
{
	bool ret = true;


	return ret;
}

bool Ganon::Update(float dt)
{
	bool ret = true;

	if (to_delete)
		return true;

	App->view->CenterCamera(camera, game_object->GetPos().x, game_object->GetPos().y);

	Entity* entity = nullptr;
	Ability* ability = nullptr;
	Spell* spell = nullptr;
	if (GotHit(entity, ability, spell) && stats.life)
	{
		// Enemy attacks
		if (entity != nullptr && ability != nullptr && entity->GetTeam() != GetTeam())
		{
			if (spell != nullptr)
			{
				DealDamage((entity->stats.power * spell->stats.damage_multiplicator) + ability->damage); // Spells control their own damage mutiplicator

				spell->Effects(entity, this, ability);
			}
			else
				DealDamage((entity->stats.power * ability->damage_multiplicator) + ability->damage);

			// Base travel cancel
			Player* p = App->scene->main_scene->player_manager->GetPlayerFromBody(game_object->pbody);
			if (p != nullptr)
				p->base_travel = false;
			// -------------
		}

		// Friendly attacks
		else
		{
			if (spell != nullptr && TextCmp(spell->name.c_str(), "boomerang"))
			{
				if (spell->owner == this)
				{
					if (spell->can_delete)
					{
						GetAbility(2)->cd_timer->SubstractTimeFromStart(7);
						App->spell->DeleteSpell(spell);
					}
				}
			}
		}

		// Dies
		if (stats.life <= 0)
		{
			if (entity->is_player)
			{
				// Update quests
				App->scene->main_scene->quest_manager->DeathQuestEvent(entity, this);

				//Add kill to killer
				App->scene->players[App->scene->main_scene->player_manager->GetEntityViewportIfIsPlayer(entity) - 1].kills++;
			}

			App->entity->AddRupeesIfPlayer(entity, rupee_reward);
			App->scene->players[App->scene->main_scene->player_manager->GetEntityViewportIfIsPlayer(this) - 1].deaths++;
		}
	}

	
	// ------------------------------------------------

	return ret;
}

bool Ganon::Draw(float dt)
{
	bool ret = true;

	LifeBar(iPoint(60, 5), iPoint(-25, -65));

	// Blit
	if (flip)
		App->view->LayerBlit(GetPos().y, game_object->GetTexture(), { game_object->GetPos().x - draw_offset.x - 3, game_object->GetPos().y - draw_offset.y }, game_object->GetCurrentAnimationRect(dt), 0, -1.0f, true, SDL_FLIP_HORIZONTAL);
	else
		App->view->LayerBlit(GetPos().y, game_object->GetTexture(), { game_object->GetPos().x - draw_offset.x, game_object->GetPos().y - draw_offset.y }, game_object->GetCurrentAnimationRect(dt), 0, -1.0f, true, SDL_FLIP_NONE);


	// -------------
	// End atacking (It's down the blit because of a reason)
	// -------------
	if (attacking)
	{
		bool reset = false;
		// Basic atack --------------------
		if (game_object->animator->IsCurrentAnimation("basic_attack_up") || game_object->animator->IsCurrentAnimation("basic_attack_up_2")
			|| game_object->animator->IsCurrentAnimation("basic_attack_down") || game_object->animator->IsCurrentAnimation("basic_attack_down_2")
			|| game_object->animator->IsCurrentAnimation("basic_attack_left") || game_object->animator->IsCurrentAnimation("basic_attack_left_2")
			|| game_object->animator->IsCurrentAnimation("basic_attack_right") || game_object->animator->IsCurrentAnimation("basic_attack_right_2"))
		{
			if (game_object->animator->GetCurrentAnimation()->Finished())
			{
				reset = true;
				game_object->DeleteFixture(abilities.at(0)->fixture);
			}
		}

		if (game_object->animator->IsCurrentAnimation("spin_attack") || game_object->animator->IsCurrentAnimation("spin_attack_2"))
		{
			if (game_object->animator->GetCurrentAnimation()->Finished())
			{
				reset = true;
				game_object->DeleteFixture(abilities.at(1)->fixture);
			}
		}

		if (reset)
		{
			game_object->animator->GetCurrentAnimation()->Reset();
			attacking = false;
			can_move = true;
			draw_offset = restore_draw_offset;
		}
		// -------------------------------
	}

	return ret;
}

bool Ganon::PostUpdate()
{
	bool ret = true;



	return ret;
}

bool Ganon::CleanUp()
{
	bool ret = true;

	return ret;
}

void Ganon::MoveAngle(float speed, float angle)
{
	float sp = speed * App->GetDT();
	fPoint s(sp * cos(DEGTORAD *angle), sp * sin(DEGTORAD * angle));
	game_object->SetPos({ game_object->fGetPos().x - s.x, game_object->fGetPos().y - s.y });
}

void Ganon::MoveUp(float speed)
{
	if (can_move)
	{
		game_object->SetPos({ game_object->fGetPos().x, game_object->fGetPos().y - speed });
	}
}

void Ganon::MoveDown(float speed)
{
	if (can_move)
		game_object->SetPos({ game_object->fGetPos().x, game_object->fGetPos().y + speed });
}

void Ganon::MoveLeft(float speed)
{
	if (can_move)
		game_object->SetPos({ game_object->fGetPos().x - speed, game_object->fGetPos().y });
}

void Ganon::MoveRight(float speed)
{
	if (can_move)
		game_object->SetPos({ game_object->fGetPos().x + speed, game_object->fGetPos().y });
}

void Ganon::MoveUpRight(float speed)
{
	if (can_move)
	{
		fPoint s(speed * cos(DEGTORAD * 45), speed * sin(DEGTORAD * 45));
		game_object->SetPos({ game_object->fGetPos().x + s.x, game_object->fGetPos().y - s.y });
	}
}

void Ganon::MoveDownRight(float speed)
{
	if (can_move)
	{
		fPoint s(speed * cos(DEGTORAD * 45), speed * sin(DEGTORAD * 45));
		game_object->SetPos({ game_object->fGetPos().x + s.x, game_object->fGetPos().y + s.y });
	}
}

void Ganon::MoveUpLeft(float speed)
{
	if (can_move)
	{
		fPoint s(speed * cos(DEGTORAD * 45), speed * sin(DEGTORAD * 45));
		game_object->SetPos({ game_object->fGetPos().x - s.x, game_object->fGetPos().y - s.y });
	}
}

void Ganon::MoveDownLeft(float speed)
{
	if (can_move)
	{
		fPoint s(speed * cos(DEGTORAD * 45), speed * sin(DEGTORAD * 45));
		game_object->SetPos({ game_object->fGetPos().x - s.x, game_object->fGetPos().y + s.y });
	}
}

void Ganon::RunUp()
{
	if (can_move)
	{
		if (GetTeam() == ANIMATIONS_TEAM)
			game_object->SetAnimation("run_up");
		else
			game_object->SetAnimation("run_up_2");
		flip = false;
	}
}

void Ganon::RunDown()
{
	if (can_move)
	{
		if (GetTeam() == ANIMATIONS_TEAM)
			game_object->SetAnimation("run_down");
		else
			game_object->SetAnimation("run_down_2");
		flip = false;
	}
}

void Ganon::RunLeft()
{
	if (can_move)
	{
		if (GetTeam() == ANIMATIONS_TEAM)
			game_object->SetAnimation("run_lateral");
		else
			game_object->SetAnimation("run_lateral_2");
		flip = true;
	}
}

void Ganon::RunRight()
{
	if (can_move)
	{
		if (GetTeam() == ANIMATIONS_TEAM)
			game_object->SetAnimation("run_lateral");
		else
			game_object->SetAnimation("run_lateral_2");
		flip = false;
	}
}

void Ganon::IdleUp()
{
	if (can_move)
	{
		if (GetTeam() == ANIMATIONS_TEAM)
			game_object->SetAnimation("idle_up");
		else
			game_object->SetAnimation("idle_up_2");
		flip = false;
	}
}

void Ganon::IdleDown()
{
	if (can_move)
	{
		if (GetTeam() == ANIMATIONS_TEAM)
			game_object->SetAnimation("idle_down");
		else
			game_object->SetAnimation("idle_down_2");
		flip = false;
	}
}

void Ganon::IdleLeft()
{
	if (can_move)
	{
		if (GetTeam() == ANIMATIONS_TEAM)
			game_object->SetAnimation("idle_lateral");
		else
			game_object->SetAnimation("idle_lateral_2");
		flip = true;
	}
}

void Ganon::IdleRight()
{
	if (can_move)
	{
		if (GetTeam() == ANIMATIONS_TEAM)
			game_object->SetAnimation("idle_lateral");
		else
			game_object->SetAnimation("idle_lateral_2");
		flip = false;
	}
}

void Ganon::SetCamera(int id)
{
	if (id > 0 && id < 5)
	{
		camera = id;
	}
}


iPoint Ganon::GetPos() const
{
	return game_object->GetPos();
}
