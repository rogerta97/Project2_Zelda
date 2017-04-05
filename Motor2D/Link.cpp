#include "Link.h"
#include "GameObject.h"
#include "Scene.h"
#include "j1App.h"
#include "j1Input.h"
#include "CollisionFilters.h"
#include "j1Render.h"
#include "j1Scene.h"
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

#define ABILITY3_MAX_RANGE 200
#define ABILITY3_GROW_SPEED 205.0f
#define ABILITY3_MOVE_SPEED 50
#define ABILITY3_MOVE_SAFE_OFFSET 15


Link::Link(iPoint pos)
{
	game_object = new GameObject(iPoint(pos.x, pos.y), iPoint(30,40), App->cf->CATEGORY_PLAYER, App->cf->MASK_PLAYER, pbody_type::p_t_player, 0);

	game_object->CreateCollisionSensor(iPoint(0, 0), game_object->GetHitBoxSize().x, game_object->GetHitBoxSize().y, fixture_type::f_t_hit_box);
	game_object->CreateCollision(iPoint(0, 15), 10, fixture_type::f_t_collision_box);
	game_object->SetListener((j1Module*)App->entity);
	game_object->SetFixedRotation(true);
	game_object->pbody->body->SetBullet(true);

	pugi::xml_document doc;
	App->xml->LoadXML("link.xml", doc);
	pugi::xml_node stats_node = doc.child("file").child("stats");
	rupee_reward = stats_node.attribute("rupees").as_int();

	stats.life = stats.base_hp = stats.max_life = stats_node.attribute("hp").as_int();
	stats.base_power = stats.power = stats_node.attribute("power").as_int();
	stats.base_speed = stats.speed = stats.restore_speed = stats_node.attribute("speed").as_int();

	float dmg_mult = stats_node.child("ability1").attribute("mult").as_float();
	float cd = stats_node.child("ability1").attribute("cd").as_float();
	int bd = stats_node.child("ability1").attribute("bd").as_int();
	Ability* a1 = AddAbility(0, cd, bd, dmg_mult);		           a1->SetImages({481, 0, 80, 48}, { 561, 0, 80, 48 }, { 481, 244, 80, 48 });

	dmg_mult = stats_node.child("ability2").attribute("mult").as_float();
	cd = stats_node.child("ability2").attribute("cd").as_float();
	bd = stats_node.child("ability2").attribute("bd").as_int();
	Ability* a2 = AddAbility(1, cd, bd, dmg_mult);			   a2->SetImages({ 481, 48, 80, 48 }, { 561, 48, 80, 48 }, { 481, 292, 80, 48 });

	dmg_mult = stats_node.child("ability3").attribute("mult").as_float();
	cd = stats_node.child("ability3").attribute("cd").as_float();
	bd = stats_node.child("ability3").attribute("bd").as_int();
	Ability* a3 = AddAbility(2, cd, bd, dmg_mult, "boomerang"); a3->SetImages({ 481, 96, 48, 73 }, { 529, 96, 48, 73 }, { 481, 341, 48, 73 }); // Name references to the Spell name

	dmg_mult = stats_node.child("ability4").attribute("mult").as_float();
	cd = stats_node.child("ability4").attribute("cd").as_float();
	bd = stats_node.child("ability4").attribute("bd").as_int();
	Ability* a4 = AddAbility(3, cd, bd, dmg_mult);			       a4->SetImages({ 481, 170, 48, 73 }, { 529, 170, 48, 73 }, { 529, 341, 48, 73 });

	game_object->SetTexture(game_object->LoadAnimationsFromXML(doc, "animations"));

	draw_offset = restore_draw_offset = { 16, 26 }; // 

	blit_layer = 2;

	name = "link";

}

Link::~Link()
{
}

bool Link::Start()
{
	bool ret = true;

	game_object->SetAnimation("idle_down");

	can_move = true;

	return ret;
}

bool Link::PreUpdate()
{
	bool ret = true;


	return ret;
}

bool Link::Update(float dt)
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

				if (TextCmp(spell->name.c_str(), "boomerang"))
					BoomerangEffects(entity, ability, spell);
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
						GetAbility(2)->cd_timer.SubstractTimeFromStart(7);
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
			}
			
			App->entity->AddRupeesIfPlayer(entity, rupee_reward);
		}
	}

	LifeBar(iPoint(60, 5), iPoint(-25, -40));

	// Ability3 movement
	if (ab3_dir != ability3_dir::a3_direction_null)
	{
		can_move = false;

		iPoint target = NULLPOINT;
		game_object->SetCatMask(App->cf->CATEGORY_NONCOLLISIONABLE, App->cf->MASK_NONCOLLISIONABLE);
		switch (ab3_dir)
		{
		case ability3_dir::a3_up:
			while(!App->pathfinding->IsWalkable(App->map->WorldToMap(ability3_point_up.x, ability3_point_up.y)) && !to_delete)
			{
				ability3_point_up = iPoint(ability3_point_up.x, ability3_point_up.y + 30);
				find = true;
			}
			target = ability3_point_up;
			if(find)
				target.y += ABILITY3_MOVE_SAFE_OFFSET;
			else
				target.y -= (ABILITY3_MOVE_SAFE_OFFSET * 2);
			break;
		case ability3_dir::a3_down:
			while (!App->pathfinding->IsWalkable(App->map->WorldToMap(ability3_point_down.x, ability3_point_down.y)) && !to_delete)
			{
				ability3_point_down = iPoint(ability3_point_down.x, ability3_point_down.y - 30);
				find = true;
			}
			target = ability3_point_down;

			if (find)
				target.y -= ABILITY3_MOVE_SAFE_OFFSET;
			else
				target.y += (ABILITY3_MOVE_SAFE_OFFSET * 2);
			break;
		case ability3_dir::a3_left:
			while (!App->pathfinding->IsWalkable(App->map->WorldToMap(ability3_point_left.x, ability3_point_left.y)) && !to_delete)
			{
				ability3_point_left = iPoint(ability3_point_left.x + 30, ability3_point_left.y);
				find = true;
			}
			target = ability3_point_left;
			if(find)
				target.x += ABILITY3_MOVE_SAFE_OFFSET;
			else
				target.x -= (ABILITY3_MOVE_SAFE_OFFSET * 2);

			break;
		case ability3_dir::a3_right:
			while (!App->pathfinding->IsWalkable(App->map->WorldToMap(ability3_point_right.x, ability3_point_right.y)) && !to_delete)
			{
				ability3_point_right = iPoint(ability3_point_right.x - 30, ability3_point_right.y);
				find = true;
			}
			App->view->LayerDrawCircle(target.x, target.y, 3, 255, 255, 255, 255, 99);
			target = ability3_point_right;
			if (find)
				target.x -= ABILITY3_MOVE_SAFE_OFFSET;
			else
				target.x += (ABILITY3_MOVE_SAFE_OFFSET*2);
			break;
				
		}

		App->view->LayerDrawCircle(target.x, target.y, 3, 255, 255, 255, 255, 99);
		float angle = AngleFromTwoPoints(GetPos().x, GetPos().y, target.x, target.y);
		MoveAngle(300, angle - 180);

		if (abs(DistanceFromTwoPoints(GetPos().x, GetPos().y, target.x, target.y)) < 20)
		{
			game_object->SetCatMask(App->cf->CATEGORY_PLAYER, App->cf->MASK_PLAYER);

			// Reset
			ab3_dir = ability3_dir::a3_direction_null;
			ability3_point_up = NULLPOINT;
			ability3_point_down = NULLPOINT;
			ability3_point_left = NULLPOINT;
			ability3_point_right = NULLPOINT;
			can_move = true;
			attacking = false;
			find = false;

			Ability1Up();
		}
	}

	return ret;
}

bool Link::Draw(float dt)
{
	bool ret = true;
	
	// Blit
	if(flip)
		App->view->LayerBlit(GetPos().y, game_object->GetTexture(), { game_object->GetPos().x - draw_offset.x - 3, game_object->GetPos().y - draw_offset.y}, game_object->GetCurrentAnimationRect(dt), 0, -1.0f, true, SDL_FLIP_HORIZONTAL);
	else
		App->view->LayerBlit(GetPos().y, game_object->GetTexture(), { game_object->GetPos().x - draw_offset.x, game_object->GetPos().y - draw_offset.y}, game_object->GetCurrentAnimationRect(dt), 0, -1.0f, true, SDL_FLIP_NONE);


	// -------------
	// End atacking (It's down the blit because of a reason)
	// -------------
	if (attacking)
	{
		bool reset = false;
		// Basic atack --------------------
		if (game_object->animator->IsCurrentAnimation("basic_attack_up") || game_object->animator->IsCurrentAnimation("basic_attack_down") 
			|| game_object->animator->IsCurrentAnimation("basic_attack_left") || game_object->animator->IsCurrentAnimation("basic_attack_right"))
		{
			if (game_object->animator->GetCurrentAnimation()->Finished())
			{
				reset = true;
				game_object->DeleteFixture(abilities.at(0)->fixture);
			}
		}

		if (game_object->animator->IsCurrentAnimation("spin_attack"))
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

bool Link::PostUpdate()
{
	bool ret = true;



	return ret;
}

bool Link::CleanUp()
{
	bool ret = true;

	return ret;
}

void Link::MoveAngle(float speed, float angle)
{
	float sp = speed * App->GetDT();
	fPoint s(sp * cos(DEGTORAD *angle), sp * sin(DEGTORAD * angle));
	game_object->SetPos({ game_object->fGetPos().x - s.x, game_object->fGetPos().y - s.y });
}

void Link::MoveUp(float speed)
{
	if (can_move)
	{
		game_object->SetPos({ game_object->fGetPos().x, game_object->fGetPos().y - speed });
	}
}

void Link::MoveDown(float speed)
{
	if (can_move)
		game_object->SetPos({ game_object->fGetPos().x, game_object->fGetPos().y + speed });
}

void Link::MoveLeft(float speed)
{
	if (can_move)
		game_object->SetPos({ game_object->fGetPos().x - speed, game_object->fGetPos().y });
}

void Link::MoveRight(float speed)
{
	if (can_move)
		game_object->SetPos({ game_object->fGetPos().x + speed, game_object->fGetPos().y });
}

void Link::MoveUpRight(float speed)
{
	if (can_move)
	{
		fPoint s(speed * cos(DEGTORAD*45), speed * sin(DEGTORAD * 45));
		game_object->SetPos({ game_object->fGetPos().x + s.x, game_object->fGetPos().y - s.y });
	}
}

void Link::MoveDownRight(float speed)
{
	if (can_move)
	{
		fPoint s(speed * cos(DEGTORAD * 45), speed * sin(DEGTORAD * 45));
		game_object->SetPos({ game_object->fGetPos().x + s.x, game_object->fGetPos().y + s.y });
	}
}

void Link::MoveUpLeft(float speed)
{
	if (can_move)
	{
		fPoint s(speed * cos(DEGTORAD * 45), speed * sin(DEGTORAD * 45));
		game_object->SetPos({ game_object->fGetPos().x - s.x, game_object->fGetPos().y - s.y });
	}
}

void Link::MoveDownLeft(float speed)
{
	if (can_move)
	{
		fPoint s(speed * cos(DEGTORAD * 45), speed * sin(DEGTORAD * 45));
		game_object->SetPos({ game_object->fGetPos().x - s.x, game_object->fGetPos().y + s.y });
	}
}

void Link::RunUp()
{
	if (can_move)
	{
		game_object->SetAnimation("run_up");
		flip = false;
	}
}

void Link::RunDown()
{
	if (can_move)
	{
		game_object->SetAnimation("run_down");
		flip = false;
	}
}

void Link::RunLeft()
{
	if (can_move)
	{
		game_object->SetAnimation("run_lateral");
		flip = true;
	}
}

void Link::RunRight()
{
	if (can_move)
	{
		game_object->SetAnimation("run_lateral");
		flip = false;
	}
}

void Link::IdleUp()
{
	if (can_move)
	{
		game_object->SetAnimation("idle_up");
		flip = false;
	}
}

void Link::IdleDown()
{
	if (can_move)
	{
		game_object->SetAnimation("idle_down");
		flip = false;
	}
}

void Link::IdleLeft()
{
	if (can_move)
	{
		game_object->SetAnimation("idle_lateral");
		flip = true;
	}
}

void Link::IdleRight()
{
	if (can_move)
	{
		game_object->SetAnimation("idle_lateral");
		flip = false;
	}
}

void Link::BasicAttackUp()
{
	if (!attacking)
	{
		game_object->SetAnimation("basic_attack_up");
		draw_offset = { draw_offset.x, 58 };
		attacking = true;
		can_move = false;
		flip = false;
		GetAbility(0)->fixture = game_object->CreateCollisionSensor(iPoint(-8, -35), 10, 40, fixture_type::f_t_attack);
	}
}

void Link::BasicAttackDown()
{
	if (!attacking)
	{
		game_object->SetAnimation("basic_attack_down");
		draw_offset = { 22, 24 };
		attacking = true;
		can_move = false;
		flip = false;
		GetAbility(0)->fixture = game_object->CreateCollisionSensor(iPoint(10, 35), 10, 40, fixture_type::f_t_attack);
	}
}

void Link::BasicAttackLeft()
{
	if (!attacking)
	{
		game_object->SetAnimation("basic_attack_left");
		draw_offset = { 48, draw_offset.y };
		attacking = true;
		can_move = false;
		flip = true;
		GetAbility(0)->fixture = game_object->CreateCollisionSensor(iPoint(-30, 0), 40, 10, fixture_type::f_t_attack);
	}
}

void Link::BasicAttackRight()
{
	if (!attacking)
	{
		game_object->SetAnimation("basic_attack_right");
		attacking = true;
		can_move = false;
		flip = false;
		GetAbility(0)->fixture = game_object->CreateCollisionSensor(iPoint(35, 0), 40, 10, fixture_type::f_t_attack);
	}
}

void Link::ShowBasicAttackUp()
{
	int main_view = App->scene->main_scene->player_manager->GetEntityViewportIfIsPlayer(this);
	App->view->LayerDrawQuad({ game_object->GetPos().x - 13, game_object->GetPos().y - 55, 10, 35 }, 51, 153, 255, 100, true, blit_layer - 1, main_view, true);
}

void Link::ShowBasicAttackDown()
{
	int main_view = App->scene->main_scene->player_manager->GetEntityViewportIfIsPlayer(this);
	App->view->LayerDrawQuad({ game_object->GetPos().x + 5, game_object->GetPos().y + 20, 10, 35 }, 51, 153, 255, 100, true, blit_layer - 1, main_view, true);
}

void Link::ShowBasicAttackLeft()
{
	int main_view = App->scene->main_scene->player_manager->GetEntityViewportIfIsPlayer(this);
	App->view->LayerDrawQuad({ game_object->GetPos().x -50 , game_object->GetPos().y - 4, 35, 10 }, 51, 153, 255, 100, true, blit_layer - 1, main_view, true);
}

void Link::ShowBasicAttackRight()
{
	int main_view = App->scene->main_scene->player_manager->GetEntityViewportIfIsPlayer(this);
	App->view->LayerDrawQuad({ game_object->GetPos().x + 20, game_object->GetPos().y - 4, 35, 10 }, 51, 153, 255, 100, true, blit_layer - 1, main_view, true);
}

void Link::Ability1Up()
{
	if (!attacking)
	{
		draw_offset = { 44, 60 };
		game_object->SetAnimation("spin_attack");
		attacking = true;
		can_move = false;
		flip = false;
		GetAbility(1)->fixture = game_object->CreateCollisionSensor(iPoint(0, 0), 50, fixture_type::f_t_attack);
	}
}

void Link::Ability1Down()
{
	Ability1Up();
}

void Link::Ability1Left()
{
	Ability1Up();
}

void Link::Ability1Right()
{
	Ability1Up();
}

void Link::ShowAbility1Up()
{
	int main_view = App->scene->main_scene->player_manager->GetEntityViewportIfIsPlayer(this);
	App->view->LayerDrawCircle(game_object->GetPos().x, game_object->GetPos().y, 50, 51, 153, 255, 100, blit_layer - 1, main_view, false, true);
}

void Link::ShowAbility1Down()
{
	ShowAbility1Up();
}

void Link::ShowAbility1Left()
{
	ShowAbility1Up();
}

void Link::ShowAbility1Right()
{
	ShowAbility1Up();
}

void Link::Ability2Up()
{
	Boomerang* s = (Boomerang*)App->spell->CreateSpell(spell_name::boomerang, game_object->GetPos(), this);
	s->Set(direction::up);
}

void Link::Ability2Down()
{
	Boomerang* s = (Boomerang*)App->spell->CreateSpell(spell_name::boomerang, game_object->GetPos(), this);
	s->Set(direction::down);
}

void Link::Ability2Left()
{
	Boomerang* s = (Boomerang*)App->spell->CreateSpell(spell_name::boomerang, game_object->GetPos(), this);
	s->Set(direction::left);
}

void Link::Ability2Right()
{
	Boomerang* s = (Boomerang*)App->spell->CreateSpell(spell_name::boomerang, game_object->GetPos(), this);
	s->Set(direction::right);
}

void Link::ShowAbility2Up()
{
	int main_view = App->scene->main_scene->player_manager->GetEntityViewportIfIsPlayer(this);
	App->view->LayerDrawQuad({ game_object->GetPos().x - 12, game_object->GetPos().y - 85, 25, (int)(BOOMERANG_RANGE * 0.5) }, 51, 153, 255, 100, true, blit_layer - 1, main_view, true);
	App->view->LayerDrawQuad({ game_object->GetPos().x - 12, game_object->GetPos().y - 85 - (int)(BOOMERANG_RANGE * 0.5), 25, (int)(BOOMERANG_RANGE * 0.5) }, 201, 153, 255, 100, true, blit_layer - 1, main_view, true);
}

void Link::ShowAbility2Down()
{
	int main_view = App->scene->main_scene->player_manager->GetEntityViewportIfIsPlayer(this);
	App->view->LayerDrawQuad({ game_object->GetPos().x - 12, game_object->GetPos().y + 15, 25, (int)(BOOMERANG_RANGE * 0.5) }, 51, 153, 255, 100, true, blit_layer - 1, main_view, true);
	App->view->LayerDrawQuad({ game_object->GetPos().x - 12, game_object->GetPos().y + 15 + (int)(BOOMERANG_RANGE * 0.5), 25, (int)(BOOMERANG_RANGE * 0.5) }, 201, 153, 255, 100, true, blit_layer - 1, main_view, true);

}

void Link::ShowAbility2Left()
{
	int main_view = App->scene->main_scene->player_manager->GetEntityViewportIfIsPlayer(this);
	App->view->LayerDrawQuad({ game_object->GetPos().x - 75, game_object->GetPos().y + 12, (int)(BOOMERANG_RANGE * 0.5), -25}, 51, 153, 255, 100, true, blit_layer - 1, main_view, true);
	App->view->LayerDrawQuad({ game_object->GetPos().x - 75 - (int)(BOOMERANG_RANGE * 0.5), game_object->GetPos().y + 12, (int)(BOOMERANG_RANGE * 0.5), -25 }, 201, 153, 255, 100, true, blit_layer - 1, main_view, true);
}

void Link::ShowAbility2Right()
{
	int main_view = App->scene->main_scene->player_manager->GetEntityViewportIfIsPlayer(this);
	App->view->LayerDrawQuad({ game_object->GetPos().x + 12, game_object->GetPos().y + 12, (int)(BOOMERANG_RANGE * 0.5), -25 }, 51, 153, 255, 100, true, blit_layer - 1, main_view, true);
	App->view->LayerDrawQuad({ game_object->GetPos().x + 12 + (int)(BOOMERANG_RANGE * 0.5), game_object->GetPos().y + 12, (int)(BOOMERANG_RANGE * 0.5), -25 }, 201, 153, 255, 100, true, blit_layer - 1, main_view, true);
}

void Link::Ability3Up()
{
	ab3_dir = ability3_dir::a3_up;
	can_move = false;
	attacking = true;
	ability3_range = 0;
	DeleteAbility3Test();
}

void Link::Ability3Down()
{
	ab3_dir = ability3_dir::a3_down;
	can_move = false;
	attacking = true;
	ability3_range = 0;
	DeleteAbility3Test();
}

void Link::Ability3Left()
{
	ab3_dir = ability3_dir::a3_left;
	can_move = false;
	attacking = true;
	ability3_range = 0;
	DeleteAbility3Test();
}

void Link::Ability3Right()
{
	ab3_dir = ability3_dir::a3_right;
	can_move = false;
	attacking = true;
	ability3_range = 0;
	DeleteAbility3Test();
}

void Link::ShowAbility3Up()
{
	if(ability3_range<=ABILITY3_MAX_RANGE)
		ability3_range += ABILITY3_GROW_SPEED * App->GetDT();

	int main_view = App->scene->main_scene->player_manager->GetEntityViewportIfIsPlayer(this);
	App->view->LayerDrawQuad({ game_object->GetPos().x - 17, game_object->GetPos().y, 35, -(int)ability3_range }, 51, 153, 255, 100, true, blit_layer - 1, main_view, true);

	CreateAbility3Test();
}

void Link::ShowAbility3Down()
{
	if (ability3_range <= ABILITY3_MAX_RANGE)
		ability3_range += ABILITY3_GROW_SPEED * App->GetDT();

	int main_view = App->scene->main_scene->player_manager->GetEntityViewportIfIsPlayer(this);
	App->view->LayerDrawQuad({ game_object->GetPos().x - 17, game_object->GetPos().y, 35, (int)ability3_range }, 51, 153, 255, 100, true, blit_layer - 1, main_view, true);

	CreateAbility3Test();
}

void Link::ShowAbility3Left()
{
	if (ability3_range <= ABILITY3_MAX_RANGE)
		ability3_range += ABILITY3_GROW_SPEED * App->GetDT();

	int main_view = App->scene->main_scene->player_manager->GetEntityViewportIfIsPlayer(this);
	App->view->LayerDrawQuad({ game_object->GetPos().x - 10, game_object->GetPos().y - 18, -(int)ability3_range, 35 }, 51, 153, 255, 100, true, blit_layer - 1, main_view, true);

	CreateAbility3Test();
}

void Link::ShowAbility3Right()
{
	if (ability3_range <= ABILITY3_MAX_RANGE)
		ability3_range += ABILITY3_GROW_SPEED * App->GetDT();

	int main_view = App->scene->main_scene->player_manager->GetEntityViewportIfIsPlayer(this);
	App->view->LayerDrawQuad({ game_object->GetPos().x + 10, game_object->GetPos().y - 18, (int)ability3_range, 35 }, 51, 153, 255, 100, true, blit_layer - 1, main_view, true);

	CreateAbility3Test();
}

void Link::OnColl(PhysBody* bodyA, PhysBody * bodyB, b2Fixture * fixtureA, b2Fixture * fixtureB)
{

}

void Link::OnCollEnter(PhysBody * bodyA, PhysBody * bodyB, b2Fixture * fixtureA, b2Fixture * fixtureB)
{

}

void Link::OnCollOut(PhysBody * bodyA, PhysBody * bodyB, b2Fixture * fixtureA, b2Fixture * fixtureB)
{

}

void Link::SetCamera(int id)
{
	if (id > 0 && id < 5)
	{
		camera = id;
	}
}


iPoint Link::GetPos() const
{
	return game_object->GetPos();
}

void Link::CreateAbility3Test()
{
	ability3_point_up = iPoint(GetPos().x, GetPos().y - ability3_range);

	ability3_point_down = iPoint(GetPos().x, GetPos().y + ability3_range);

	ability3_point_left = iPoint(GetPos().x - ability3_range, GetPos().y);

	ability3_point_right = iPoint(GetPos().x + ability3_range, GetPos().y);
}

void Link::DeleteAbility3Test()
{
	get_up = true;
	get_down = true;
	get_left = true;
	get_right = true;
	
}
