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
#include "GanonBat.h"

#define ABILITY1_RANGE 260

Ganon::Ganon(iPoint pos)
{
	game_object = new GameObject(iPoint(pos.x, pos.y), iPoint(30, 40), App->cf->CATEGORY_PLAYER, App->cf->MASK_PLAYER, pbody_type::p_t_player, 0);
	game_object->SetHitBoxSize(80, 80); 

	game_object->CreateCollisionSensor(iPoint(0,0), game_object->GetHitBoxSize().x, game_object->GetHitBoxSize().y, fixture_type::f_t_hit_box);

	ganon_collision = game_object->CreateCollision(iPoint(0, 10), 70, 30, fixture_type::f_t_collision_box);
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
	Ability* a2 = AddAbility(1, cd, bd, dmg_mult, "ganon_bat");
	a2->SetImages({ 896, 351, 80, 48 }, { 896, 473, 80, 48 }, { 1093, 1960, 80, 48 }, { 0,0,0,0 });

	dmg_mult = stats_node.child("ability3").attribute("mult").as_float();
	cd = stats_node.child("ability3").attribute("cd").as_float();
	bd = stats_node.child("ability3").attribute("bd").as_int();
	Ability* a3 = AddAbility(2, cd, bd, dmg_mult); //
	a3->SetImages({ 816, 399, 48, 73 }, { 816, 521, 48, 73 }, { 1013, 2008, 48, 73 }, { 0,0,0,0 });

	dmg_mult = stats_node.child("ability4").attribute("mult").as_float();
	cd = stats_node.child("ability4").attribute("cd").as_float();
	bd = stats_node.child("ability4").attribute("bd").as_int();
	Ability* a4 = AddAbility(3, cd, bd, dmg_mult);
	a4->SetImages({ 864, 399, 48, 73 }, { 864, 521,48, 73 }, { 1061, 2008, 48, 73 }, { 0,0,0,0 });
	// -------------------------------------

	game_object->SetTexture(game_object->LoadAnimationsFromXML(doc, "animations"));

	draw_offset = restore_draw_offset = { 56,  48 };

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
		
		}
	}

	// Dies
	if (stats.life <= 0 && !to_delete && entity != nullptr)
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
	
	// ------------------------------------------------

	return ret;
}

bool Ganon::Draw(float dt)
{
	bool ret = true;

	LifeBar(iPoint(60, 5), iPoint(-25, -65));

	// Blit
	if (flip)
		App->view->LayerBlit(GetPos().y + 15, game_object->GetTexture(), { game_object->GetPos().x - draw_offset.x - 3, game_object->GetPos().y - draw_offset.y }, game_object->GetCurrentAnimationRect(dt), 0, -1.0f, true, SDL_FLIP_HORIZONTAL);
	else
		App->view->LayerBlit(GetPos().y + 15, game_object->GetTexture(), { game_object->GetPos().x - draw_offset.x, game_object->GetPos().y - draw_offset.y }, game_object->GetCurrentAnimationRect(dt), 0, -1.0f, true, SDL_FLIP_NONE);


	// -------------
	// End atacking (It's down the blit because of a reason)
	// -------------
	if (attacking)
	{
		bool reset = false;
		// Basic atack --------------------
		if (game_object->animator->IsCurrentAnimation("basic_attack_up") || game_object->animator->IsCurrentAnimation("basic_attack_up_2")
			|| game_object->animator->IsCurrentAnimation("basic_attack_down") || game_object->animator->IsCurrentAnimation("basic_attack_down_2")
			|| game_object->animator->IsCurrentAnimation("basic_attack_lateral") || game_object->animator->IsCurrentAnimation("basic_attack_lateral_2"))
		{
			if (game_object->animator->GetCurrentAnimation()->Finished())
			{
				reset = true;
				game_object->DeleteFixture(abilities.at(0)->fixture);
			}
		}

		if (reset)
		{
			game_object->animator->GetCurrentAnimation()->Reset();
			attacking = false;
			can_move = true;
			draw_offset = restore_draw_offset;
		}
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

		draw_offset = { 54,  60 };
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


		draw_offset = restore_draw_offset;
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

		draw_offset = { 56, 44 };
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

		draw_offset = { 56, 44 };
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

		draw_offset = { 54, 60 };
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

		draw_offset = restore_draw_offset;
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

		draw_offset = { 56, 44 };
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

		draw_offset = { 56, 44 };
	}
}

void Ganon::BasicAttackUp()
{
	if (!attacking)
	{
		if (GetTeam() == ANIMATIONS_TEAM)
			game_object->SetAnimation("basic_attack_up");
		else
			game_object->SetAnimation("basic_attack_up_2");

		draw_offset = { draw_offset.x, 58 };
		attacking = true;
		can_move = false;
		flip = false;
		GetAbility(0)->fixture = game_object->CreateCollisionSensor(iPoint(0, -45), 25, 85, fixture_type::f_t_attack);

		draw_offset = { 54, 86 };
	}
}

void Ganon::BasicAttackDown()
{
	if (!attacking)
	{
		if (GetTeam() == ANIMATIONS_TEAM)
			game_object->SetAnimation("basic_attack_down");
		else
			game_object->SetAnimation("basic_attack_down_2");

		draw_offset = { draw_offset.x, 58 };
		attacking = true;
		can_move = false;
		flip = false;
		GetAbility(0)->fixture = game_object->CreateCollisionSensor(iPoint(-2, 45), 25, 50, fixture_type::f_t_attack);

		draw_offset = { 54, 40 };
	}
}

void Ganon::BasicAttackLeft()
{
	if (!attacking)
	{
		if (GetTeam() == ANIMATIONS_TEAM)
			game_object->SetAnimation("basic_attack_lateral");
		else
			game_object->SetAnimation("basic_attack_lateral_2");

		draw_offset = { draw_offset.x, 58 };
		attacking = true;
		can_move = false;
		flip = true;
		GetAbility(0)->fixture = game_object->CreateCollisionSensor(iPoint(-45, 7), 60, 25, fixture_type::f_t_attack);

		draw_offset = { 70, 45 };
	}
}

void Ganon::BasicAttackRight()
{
	if (!attacking)
	{
		if (GetTeam() == ANIMATIONS_TEAM)
			game_object->SetAnimation("basic_attack_lateral");
		else
			game_object->SetAnimation("basic_attack_lateral_2");

		draw_offset = { draw_offset.x, 58 };
		attacking = true;
		can_move = false;
		flip = false;
		GetAbility(0)->fixture = game_object->CreateCollisionSensor(iPoint(43, 7), 60, 25, fixture_type::f_t_attack);

		draw_offset = { 54, 45 };
	}
}

void Ganon::ShowBasicAttackUp()
{
	int main_view = App->scene->main_scene->player_manager->GetEntityViewportIfIsPlayer(this);
	App->view->LayerDrawQuad({ game_object->GetPos().x - 12, game_object->GetPos().y - 90, 25, 70 }, 51, 153, 255, 100, true, blit_layer - 1, main_view, true);
}

void Ganon::ShowBasicAttackDown()
{
	int main_view = App->scene->main_scene->player_manager->GetEntityViewportIfIsPlayer(this);
	App->view->LayerDrawQuad({ game_object->GetPos().x - 14, game_object->GetPos().y + 25, 25, 55 }, 51, 153, 255, 100, true, blit_layer - 1, main_view, true);
}

void Ganon::ShowBasicAttackLeft()
{
	int main_view = App->scene->main_scene->player_manager->GetEntityViewportIfIsPlayer(this);
	App->view->LayerDrawQuad({ game_object->GetPos().x - 75 , game_object->GetPos().y - 4, 75, 25 }, 51, 153, 255, 100, true, blit_layer - 1, main_view, true);
}

void Ganon::ShowBasicAttackRight()
{
	int main_view = App->scene->main_scene->player_manager->GetEntityViewportIfIsPlayer(this);
	App->view->LayerDrawQuad({ game_object->GetPos().x - 0 , game_object->GetPos().y - 4, 75, 25 }, 51, 153, 255, 100, true, blit_layer - 1, main_view, true);
}

void Ganon::Ability1Up()
{
	GanonBat* s = (GanonBat*)App->spell->CreateSpell(spell_name::ganon_bat, game_object->GetPos(), this);
	s->Set(g_b_direction::g_b_up);
}

void Ganon::Ability1Down()
{
	GanonBat* s = (GanonBat*)App->spell->CreateSpell(spell_name::ganon_bat, game_object->GetPos(), this);
	s->Set(g_b_direction::g_b_down);
}

void Ganon::Ability1Left()
{
	GanonBat* s = (GanonBat*)App->spell->CreateSpell(spell_name::ganon_bat, game_object->GetPos(), this);
	s->Set(g_b_direction::g_b_left);
}

void Ganon::Ability1Right()
{
	GanonBat* s = (GanonBat*)App->spell->CreateSpell(spell_name::ganon_bat, game_object->GetPos(), this);
	s->Set(g_b_direction::g_b_right);
}

void Ganon::ShowAbility1Up()
{
	int main_view = App->scene->main_scene->player_manager->GetEntityViewportIfIsPlayer(this);
	App->view->LayerDrawQuad({ game_object->GetPos().x - 12, game_object->GetPos().y - 12 - ABILITY1_RANGE, 25, (int)(ABILITY1_RANGE) }, 51, 153, 255, 100, true, blit_layer - 1, main_view, true);
}

void Ganon::ShowAbility1Down()
{
	int main_view = App->scene->main_scene->player_manager->GetEntityViewportIfIsPlayer(this);
	App->view->LayerDrawQuad({ game_object->GetPos().x - 12, game_object->GetPos().y + 15, 25, (int)(ABILITY1_RANGE) }, 51, 153, 255, 100, true, blit_layer - 1, main_view, true);
}

void Ganon::ShowAbility1Left()
{
	int main_view = App->scene->main_scene->player_manager->GetEntityViewportIfIsPlayer(this);
	App->view->LayerDrawQuad({ game_object->GetPos().x - 12, game_object->GetPos().y + 12, (int)(-ABILITY1_RANGE), -25 }, 51, 153, 255, 100, true, blit_layer - 1, main_view, true);
}

void Ganon::ShowAbility1Right()
{
	int main_view = App->scene->main_scene->player_manager->GetEntityViewportIfIsPlayer(this);
	App->view->LayerDrawQuad({ game_object->GetPos().x + 12, game_object->GetPos().y + 12, (int)(ABILITY1_RANGE), -25 }, 51, 153, 255, 100, true, blit_layer - 1, main_view, true);
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
