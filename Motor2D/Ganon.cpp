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
#include "j1Input.h"

#define ABILITY1_RANGE 260

#define ABILITY2_TIME 7
#define ABILITY2_RADIOUS 90
#define ABILITY2_TOTATION_SPEED 300

#define CAMERA_SPEED 400
#define ABILITY3_RANGE 300
#define ABILITY3_ATACK_EFFECT 100
#define ABILITY3_MOVE_SPEED 800

Ganon::Ganon(iPoint pos)
{
	game_object = new GameObject(iPoint(pos.x, pos.y), iPoint(30, 40), App->cf->CATEGORY_PLAYER, App->cf->MASK_PLAYER, pbody_type::p_t_player, 0);
	game_object->SetHitBoxSize(80, 80); 

	game_object->CreateCollisionSensor(iPoint(0,0), game_object->GetHitBoxSize().x, game_object->GetHitBoxSize().y, fixture_type::f_t_hit_box);

	game_object->CreateCollision(iPoint(0, 10), 18, fixture_type::f_t_collision_box);
	game_object->SetListener((j1Module*)App->entity);
	game_object->SetListener((j1Module*)App->spell);
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
	a1->SetImages({ 816, 351, 81, 48 }, { 816, 351, 81, 48 }, { 1013, 1960, 80, 48 }, { 978, 497, 32, 32 });

	dmg_mult = stats_node.child("ability2").attribute("mult").as_float();
	cd = stats_node.child("ability2").attribute("cd").as_float();
	bd = stats_node.child("ability2").attribute("bd").as_int();
	Ability* a2 = AddAbility(1, cd, bd, dmg_mult, "ganon_bat");
	a2->SetImages({ 896, 351, 80, 48 }, { 896, 351, 80, 48 }, { 1093, 1960, 80, 48 }, { 1014, 497, 32, 32 });

	shield = stats_node.child("ability3").attribute("shield").as_int();
	dmg_mult = stats_node.child("ability3").attribute("mult").as_float();
	cd = stats_node.child("ability3").attribute("cd").as_float();
	bd = stats_node.child("ability3").attribute("bd").as_int();
	Ability* a3 = AddAbility(2, cd, bd, dmg_mult); //
	a3->SetImages({ 816, 399, 48, 73 }, { 816, 399, 48, 73 }, { 1013, 2008, 48, 73 }, { 1050, 497, 32, 32 });

	dmg_mult = stats_node.child("ability4").attribute("mult").as_float();
	cd = stats_node.child("ability4").attribute("cd").as_float();
	bd = stats_node.child("ability4").attribute("bd").as_int();
	Ability* a4 = AddAbility(3, cd, bd, dmg_mult);
	a4->SetImages({ 864, 399, 48, 73 }, { 864, 399, 48, 73 }, { 1061, 2008, 48, 73 }, { 1086, 497, 32, 32 });
	// -------------------------------------

	game_object->SetTexture(game_object->LoadAnimationsFromXML(doc, "animations"));

	draw_offset = restore_draw_offset = { 56,  48 };

	blit_layer = 2;

	ability2_timer = App->AddGameplayTimer();
	look_for_target_timer = App->AddGameplayTimer();

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

	if(!look_for_target)
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
				DealDamage(((float)entity->stats.power * (float)spell->stats.damage_multiplicator) + (float)ability->damage); // Spells control their own damage mutiplicator

				spell->Effects(entity, this, ability);
			}
			else
				DealDamage(((float)entity->stats.power * (float)ability->damage_multiplicator) + (float)ability->damage);

			// Base travel cancel
			Player* p = App->scene->main_scene->player_manager->GetPlayerFromBody(game_object->pbody);
			if (p != nullptr)
				p->base_travel = false;
			// -------------

			Die(entity);
		}

		// Friendly attacks
		else
		{
		
		}
	}
	// ------------------------------------------------

	// Ability2 update -----
	if (ability2)
	{
		if (ability2_timer->ReadSec() > ABILITY2_TIME)
		{
			ability2 = false;
			ClearAbility2Balls();
			stats.shield = 0;
		}

		if (!balls.empty())
		{
			for (int i = 0; i < balls.size(); i++)
			{
				balls.at(i).angle += ABILITY2_TOTATION_SPEED*dt;
				fPoint pos = { GetPos().x + (ABILITY2_RADIOUS*cos(DEGTORAD*balls.at(i).angle)), GetPos().y + (ABILITY2_RADIOUS*sin(DEGTORAD*balls.at(i).angle)) };
				balls.at(i).game_object->SetPos(pos);
			}
		}
	}
	// ---------------------

	// Ability3 update -----
	if (ability3)
	{
		// Choose target
		if (look_for_target)
		{
			MoveCamera();

			target = DrawTarget();

		}
		else
		{
			// Recalculate target to fit the map and the range
			if (target_found = true)
			{
				int distance = abs(DistanceFromTwoPoints(GetPos().x, GetPos().y, target.x, target.y));

				if (distance > ABILITY3_RANGE)
				{
					int angle = AngleFromTwoPoints(GetPos().x, GetPos().y, target.x, target.y) - 180;
					int substract = distance - ABILITY3_RANGE;

					target.x += (int)(substract * cos(angle*DEGTORAD));
					target.y += (int)(substract * sin(angle*DEGTORAD));
				}

				while (!App->pathfinding->IsWalkable(App->map->WorldToMap(target.x, target.y)))
				{
					int angle = AngleFromTwoPoints(GetPos().x, GetPos().y, target.x, target.y) - 180;
					int substract = 10;
					target.x += (int)(substract * cos(angle*DEGTORAD));
					target.y += (int)(substract * sin(angle*DEGTORAD));
				}

				target_found = false;
			}

			// Move ganon to target
			if (abs(DistanceFromTwoPoints(GetPos().x, GetPos().y, target.x, target.y)) > 7)
			{
				float angle = AngleFromTwoPoints(GetPos().x, GetPos().y, target.x, target.y) - 180;
				MoveAngle(ABILITY3_MOVE_SPEED, angle);
				game_object->SetCatMask(App->cf->CATEGORY_NONCOLLISIONABLE, App->cf->MASK_NONCOLLISIONABLE);
			}
			// Finish ability, deal damage
			else
			{
				ability3 = false;
				game_object->SetCatMask(App->cf->CATEGORY_PLAYER, App->cf->MASK_PLAYER);
				GetAbility(3)->fixture = game_object->CreateCollisionSensor(iPoint(0, 0), ABILITY3_ATACK_EFFECT, fixture_type::f_t_attack);
			}
		}
	}
	// ---------------------

	return ret;
}

bool Ganon::Draw(float dt)
{
	bool ret = true;

	LifeBar(iPoint(60, 5), iPoint(-30, -65), stats.shield);

	// Blit
	if (flip)
		App->view->LayerBlit(GetPos().y + 15, game_object->GetTexture(), { game_object->GetPos().x - draw_offset.x - 3, game_object->GetPos().y - draw_offset.y }, game_object->GetCurrentAnimationRect(dt), 0, -1.0f, true, SDL_FLIP_HORIZONTAL);
	else
		App->view->LayerBlit(GetPos().y + 15, game_object->GetTexture(), { game_object->GetPos().x - draw_offset.x, game_object->GetPos().y - draw_offset.y }, game_object->GetCurrentAnimationRect(dt), 0, -1.0f, true, SDL_FLIP_NONE);


	// Ability2 balls
	if(!balls.empty())
	{
		for (int i = 0; i < balls.size(); i++)
		{
			ball curr_ball = balls.at(i);
			if (GetTeam() == ANIMATIONS_TEAM)
				App->view->LayerBlit(curr_ball.game_object->GetPos().y, game_object->GetTexture(), { curr_ball.game_object->GetPos().x - 25, curr_ball.game_object->GetPos().y - 15 }, game_object->animator->GetAnimation("shield_balls")->GetAnimationFrame(dt), 0, -1.0f, true, SDL_FLIP_HORIZONTAL);
			else
				App->view->LayerBlit(curr_ball.game_object->GetPos().y, game_object->GetTexture(), { curr_ball.game_object->GetPos().x - 25, curr_ball.game_object->GetPos().y - 15 }, game_object->animator->GetAnimation("shield_balls_2")->GetAnimationFrame(dt), 0, -1.0f, true, SDL_FLIP_HORIZONTAL);
		}
	}

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

		if (game_object->animator->IsCurrentAnimation("ultimate_attack") || game_object->animator->IsCurrentAnimation("ultimate_attack_2"))
		{
			if (game_object->animator->GetCurrentAnimation()->Finished() && !ability3)
			{
				reset = true;
				game_object->DeleteFixture(GetAbility(3)->fixture);
				IdleDown();
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

	ClearAbility2Balls();

	App->DeleteGameplayTimer(ability2_timer);
	App->DeleteGameplayTimer(look_for_target_timer);

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

void Ganon::Ability2Up()
{
	if (!attacking)
	{
		stats.shield = shield;
		CreateAbility2Balls();
		ability2 = true;
		ability2_timer->Start();
	}
}

void Ganon::Ability2Down()
{
	Ability2Up();
}

void Ganon::Ability2Left()
{
	Ability2Up();
}

void Ganon::Ability2Right()
{
	Ability2Up();
}

void Ganon::ShowAbility2Up()
{
	Ability2Up();
}

void Ganon::ShowAbility2Down()
{
}

void Ganon::ShowAbility2Left()
{
}

void Ganon::ShowAbility2Right()
{
}

void Ganon::Ability3Up()
{
	if (!attacking)
	{
		attacking = true;
		can_move = false;

		if (GetTeam() == ANIMATIONS_TEAM)
			game_object->SetAnimation("ultimate_attack");
		else
			game_object->SetAnimation("ultimate_attack_2");

		draw_offset = { 68, 115 };

		look_for_target = false;

		target_found = true;
	}
}

void Ganon::Ability3Down()
{
	Ability3Up();
}

void Ganon::Ability3Left()
{
	Ability3Up();
}

void Ganon::Ability3Right()
{
	Ability3Up();
}

void Ganon::ShowAbility3Up()
{
	if (!attacking)
	{
		look_for_target = true;
		can_move = false;
		ability3 = true;

		if (GetTeam() == ANIMATIONS_TEAM)
			game_object->SetAnimation("look_for_target");
		else
			game_object->SetAnimation("look_for_target_2");

		draw_offset = { 68, 115 };

		int main_view = App->scene->main_scene->player_manager->GetEntityViewportIfIsPlayer(this);
		App->view->LayerDrawCircle(game_object->GetPos().x, game_object->GetPos().y, ABILITY3_RANGE, 255, 255, 255, 255, 1, main_view);
	}
}

void Ganon::ShowAbility3Down()
{
	ShowAbility3Up();
}

void Ganon::ShowAbility3Left()
{
	ShowAbility3Up();
}

void Ganon::ShowAbility3Right()
{
	ShowAbility3Up();
}

void Ganon::OnCollEnter(PhysBody * bodyA, PhysBody * bodyB, b2Fixture * fixtureA, b2Fixture * fixtureB)
{
	// Balls hitting
	if (!balls.empty())
	{
		for (int i = 0; i < balls.size(); i++)
		{
			if (bodyA == balls.at(i).game_object->pbody)
			{
				Entity* e = App->entity->FindEntityByBody(bodyB);

				if (e != nullptr && e != this && e->GetTeam() != GetTeam() && !to_delete)
				{
					e->DealDamage((stats.power * GetAbility(2)->damage_multiplicator) + (GetAbility(2)->damage));
					e->Die(this);
					DeleteAbility2BallByPbody(bodyA);
					stats.shield += shield;
				}

				break;
			}
		}
	}

	if (game_object->animator->IsCurrentAnimation("ultimate_attack") || game_object->animator->IsCurrentAnimation("ultimate_attack_2"))
	{
		Entity* e = App->entity->FindEntityByBody(bodyB);

		if (bodyA == game_object->pbody && e != nullptr && e != this && e->GetTeam() != GetTeam() && !to_delete && !e->to_delete)
		{
			e->Stun(1.0f);
		}
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

void Ganon::Die(Entity * killed_by)
{
	// Dies
	if (stats.life <= 0 && !to_delete && killed_by != nullptr)
	{
		if (killed_by->is_player)
		{
			// Update quests
			App->scene->main_scene->quest_manager->DeathQuestEvent(killed_by, this);

			//Add kill to killer
			App->scene->players[App->scene->main_scene->player_manager->GetEntityViewportIfIsPlayer(killed_by) - 1].kills++;
		}

		App->entity->AddRupeesIfPlayer(killed_by, rupee_reward);
		App->scene->players[App->scene->main_scene->player_manager->GetEntityViewportIfIsPlayer(this) - 1].deaths++;
	}

}

void Ganon::CreateAbility2Balls()
{
	if (!balls.empty())
		return;

	ball b;

	for (int i = 1; i <= 3; i++)
	{
		int angle = i * 120;
		iPoint pos = { GetPos().x + (int)(ABILITY2_RADIOUS*cos(DEGTORAD*angle)), GetPos().y + (int)(ABILITY2_RADIOUS*sin(DEGTORAD*angle)) };

		GameObject* g_o = new GameObject(iPoint(pos.x, pos.y), iPoint(20, 20), App->cf->CATEGORY_ABILITIES, App->cf->MASK_ABILITIES, pbody_type::p_t_ganon_bat, 0);
		g_o->CreateCollisionSensor(iPoint(0, +10), g_o->GetHitBoxSize().x, g_o->GetHitBoxSize().y, fixture_type::f_t_attack);
		g_o->SetListener((j1Module*)App->entity);
		g_o->SetListener((j1Module*)App->spell);
		g_o->SetFixedRotation(true);
		g_o->pbody->body->SetBullet(true);

		b.game_object = g_o;
		b.angle = angle;

		balls.push_back(b);
	}
}

void Ganon::DeleteAbility2BallByPbody(PhysBody * body)
{
	for (vector<ball>::iterator it = balls.begin(); it != balls.end();)
	{
		if ((*it).game_object->pbody == body)
		{
			(*it).game_object->CleanUp();
			RELEASE((*it).game_object);
			it = balls.erase(it);
		}
		else
			++it;
	}
}

void Ganon::ClearAbility2Balls()
{
	if (!balls.empty())
	{
		for (vector<ball>::iterator it = balls.begin(); it != balls.end();)
		{
			(*it).game_object->CleanUp();
			RELEASE((*it).game_object);
			it = balls.erase(it);
		}
	}
}

void Ganon::MoveCamera()
{
	float speed = CAMERA_SPEED*App->GetDT();

	Player* curr_player = App->scene->main_scene->player_manager->GetPlayerFromBody(game_object->pbody);

	if (App->input->GetControllerJoystickMove(curr_player->controller_index, LEFTJOY_LEFT) > 12000 && App->input->GetControllerJoystickMove(curr_player->controller_index, LEFTJOY_UP) > 12000)
	{
		//iPoint new_pos = { App->view->GetCameraPos(curr_player->viewport).x + speed*cos(45 * DEGTORAD),  App->view->GetCameraPos(curr_player->viewport).y + speed*sin(45 * DEGTORAD) };
		App->view->MoveCamera(curr_player->viewport, speed*cos(45 * DEGTORAD), speed*sin(45 * DEGTORAD));
	}
	else if (App->input->GetControllerJoystickMove(curr_player->controller_index, LEFTJOY_RIGHT) > 12000 && App->input->GetControllerJoystickMove(curr_player->controller_index, LEFTJOY_UP) > 12000)
	{
		App->view->MoveCamera(curr_player->viewport, -speed*cos(45 * DEGTORAD), speed*sin(45 * DEGTORAD));
	}
	else if (App->input->GetControllerJoystickMove(curr_player->controller_index, LEFTJOY_LEFT) > 12000 && App->input->GetControllerJoystickMove(curr_player->controller_index, LEFTJOY_DOWN) > 12000)
	{
		App->view->MoveCamera(curr_player->viewport, speed*cos(45 * DEGTORAD), -speed*sin(45 * DEGTORAD));
	}
	else if (App->input->GetControllerJoystickMove(curr_player->controller_index, LEFTJOY_RIGHT) > 12000 && App->input->GetControllerJoystickMove(curr_player->controller_index, LEFTJOY_DOWN) > 12000)
	{
		App->view->MoveCamera(curr_player->viewport, -speed*cos(45 * DEGTORAD), -speed*sin(45 * DEGTORAD));
	}

	// Normal moves
	else if (App->input->GetControllerJoystickMove(curr_player->controller_index, LEFTJOY_LEFT) > 12000)
	{
		App->view->MoveCamera(curr_player->viewport, speed, 0);
	}
	else if (App->input->GetControllerJoystickMove(curr_player->controller_index, LEFTJOY_RIGHT) > 12000)
	{
		App->view->MoveCamera(curr_player->viewport, -speed, 0);
	}
	else if (App->input->GetControllerJoystickMove(curr_player->controller_index, LEFTJOY_UP) > 6000)
	{
		App->view->MoveCamera(curr_player->viewport, 0, speed);
	}
	else if (App->input->GetControllerJoystickMove(curr_player->controller_index, LEFTJOY_DOWN) > 6000)
	{
		App->view->MoveCamera(curr_player->viewport, 0, -speed);
	}
}

iPoint Ganon::DrawTarget()
{
	iPoint ret = NULLPOINT;

	SDL_Rect view = App->view->GetViewportRect(1);
	int main_view = App->scene->main_scene->player_manager->GetEntityViewportIfIsPlayer(this);
	iPoint camera = App->view->GetCameraPos(main_view);
	App->view->LayerDrawCircle(-camera.x + (view.w/2), -camera.y + (view.h / 2), 20, 255, 255, 255, 255, 1, main_view);
	App->view->LayerDrawCircle(-camera.x + (view.w / 2), -camera.y + (view.h / 2), ABILITY3_ATACK_EFFECT, 255, 255, 255, 255, 1, main_view);

	ret = { -camera.x + (view.w / 2), -camera.y + (view.h / 2) };

	return ret;
}
