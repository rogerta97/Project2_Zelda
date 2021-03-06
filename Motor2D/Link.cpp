#include "Link.h"
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
#include "j1Audio.h"

#define ABILITY3_MAX_RANGE 200
#define ABILITY3_GROW_SPEED 205.0f
#define ABILITY3_MOVE_SPEED 50
#define ABILITY3_MOVE_SAFE_OFFSET 15

Link::Link(iPoint pos)
{
	game_object = new GameObject(iPoint(pos.x, pos.y), iPoint(30,40), App->cf->CATEGORY_PLAYER, App->cf->MASK_PLAYER, pbody_type::p_t_player, 0);

	game_object->CreateCollisionSensor(iPoint(0, 0), game_object->GetHitBoxSize().x, game_object->GetHitBoxSize().y, fixture_type::f_t_hit_box);
	link_collision = game_object->CreateCollision(iPoint(2, 10), 13, fixture_type::f_t_collision_box);
	game_object->SetListener((j1Module*)App->entity);
	game_object->SetListener((j1Module*)App->spell);
	game_object->SetFixedRotation(true);
	game_object->pbody->body->SetBullet(true);

	pugi::xml_document doc;
	App->xml->LoadXML("link.xml", doc);

	pugi::xml_document doc2;
	App->xml->LoadXML("abilities_rects.xml", doc2);
	pugi::xml_node hit_node = doc2.child("file").child("link");

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
	a1->SetImages({ 816, 351, 81, 48 }, { 816, 474, 81, 48 }, { 1013, 1960, 80, 48 }, { 978, 353, 32, 32 }, 
	{ hit_node.child("basicattack_rect").child("rect").attribute("x").as_int(0), 
		hit_node.child("basicattack_rect").child("rect").attribute("y").as_int(0),
		hit_node.child("basicattack_rect").child("rect").attribute("w").as_int(0), 
		hit_node.child("basicattack_rect").child("rect").attribute("h").as_int(0) });

	dmg_mult = stats_node.child("ability2").attribute("mult").as_float();
	cd = stats_node.child("ability2").attribute("cd").as_float();
	bd = stats_node.child("ability2").attribute("bd").as_int();
	Ability* a2 = AddAbility(1, cd, bd, dmg_mult);
	a2->SetImages({ 896, 351, 80, 48 }, { 896, 474, 80, 48 }, { 1093, 1960, 80, 48 }, { 1014, 353, 32, 32 }, 
	{ hit_node.child("boomerang_rect").child("rect").attribute("x").as_int(0),
		hit_node.child("boomerang_rect").child("rect").attribute("y").as_int(0),
		hit_node.child("boomerang_rect").child("rect").attribute("w").as_int(0),
		hit_node.child("boomerang_rect").child("rect").attribute("h").as_int(0) });

	dmg_mult = stats_node.child("ability3").attribute("mult").as_float();
	cd = stats_node.child("ability3").attribute("cd").as_float();
	bd = stats_node.child("ability3").attribute("bd").as_int();
	Ability* a3 = AddAbility(2, cd, bd, dmg_mult, "boomerang"); // Name references to the Spell name
	a3->SetImages({ 816, 399, 48, 73 }, { 816, 522, 48, 73 }, { 1013, 2008, 48, 73 }, { 1050, 353, 32, 32 }, 
	{ hit_node.child("spinattack_rect").child("rect").attribute("x").as_int(0),
		hit_node.child("spinattack_rect").child("rect").attribute("y").as_int(0),
		hit_node.child("spinattack_rect").child("rect").attribute("w").as_int(0),
		hit_node.child("spinattack_rect").child("rect").attribute("h").as_int(0) });

	dmg_mult = stats_node.child("ability4").attribute("mult").as_float();
	cd = stats_node.child("ability4").attribute("cd").as_float();
	bd = stats_node.child("ability4").attribute("bd").as_int();
	Ability* a4 = AddAbility(3, cd, bd, dmg_mult);
	a4->SetImages({ 864, 399, 48, 73 }, { 864, 522, 48, 73 }, { 1061, 2008, 48, 73 }, { 1086, 353, 32, 32 }, 
	{ hit_node.child("ulti_rect").child("rect").attribute("x").as_int(0),
		hit_node.child("ulti_rect").child("rect").attribute("y").as_int(0),
		hit_node.child("ulti_rect").child("rect").attribute("w").as_int(0),
		hit_node.child("ulti_rect").child("rect").attribute("h").as_int(0) });
	// -------------------------------------

	game_object->SetTexture(game_object->LoadAnimationsFromXML(doc, "animations"));

	draw_offset = restore_draw_offset = { 16, 26 }; 

	blit_layer = 2;

	name = "link";

	link_ultimate = App->audio->LoadFx("Audio/FX/Entities/Link/OOT_Link_StrongAttack.wav");
}

Link::~Link()
{
}

bool Link::Start()
{
	bool ret = true;

	if(GetTeam()==1)
		game_object->SetAnimation("idle_down");
	else
		game_object->SetAnimation("idle_down_2");

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
				DealDamage((int)(((float)entity->stats.power * spell->stats.damage_multiplicator) + ability->damage)); // Spells control their own damage mutiplicator

				spell->Effects(entity, this, ability);
			}
			else
				DealDamage((int)(((float)entity->stats.power * ability->damage_multiplicator) + ability->damage));

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

		Die(entity);
	}

	// Ability3 movement ------------------------------
	if (ab3_dir != link_ability3_dir::a3_direction_null)
	{
		if (GetTeam() == ANIMATIONS_TEAM)
			game_object->SetAnimation("ultimate_attack");
		else
			game_object->SetAnimation("ultimate_attack_2");

		can_move = false;

		iPoint target = NULLPOINT;

		// Disable collisions
		link_collision->SetSensor(true);

		// Check if there is collisions and find a nice spot
		switch (ab3_dir)
		{
		case link_ability3_dir::a3_up:
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
		case link_ability3_dir::a3_down:
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
		case link_ability3_dir::a3_left:
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
		case link_ability3_dir::a3_right:
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

		// Move to target
		App->view->LayerDrawCircle(target.x, target.y, 3, 255, 255, 255, 255, 99);
		float angle = AngleFromTwoPoints(GetPos().x, GetPos().y, target.x, target.y);
		MoveAngle(300, angle - 180);

		if (abs(DistanceFromTwoPoints(GetPos().x, GetPos().y, target.x, target.y)) < 20)
		{
			// Reset
			draw_offset = restore_draw_offset;
			game_object->DeleteFixture(GetAbility(3)->fixture);
			ab3_dir = link_ability3_dir::a3_direction_null;
			ability3_point_up = NULLPOINT;
			ability3_point_down = NULLPOINT;
			ability3_point_left = NULLPOINT;
			ability3_point_right = NULLPOINT;
			can_move = true;
			attacking = false;
			find = false;
			link_collision->SetSensor(false);
			invulnerable = false;
			Ability1Up();
		}
	}
	// ------------------------------------------------

	return ret;
}

bool Link::Draw(float dt)
{
	bool ret = true;

	LifeBar(iPoint(60, 5), iPoint(-25, -40));

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
		if (GetTeam() == ANIMATIONS_TEAM)
			game_object->SetAnimation("run_up");
		else
			game_object->SetAnimation("run_up_2");
		flip = false;
	}
}

void Link::RunDown()
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

void Link::RunLeft()
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

void Link::RunRight()
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

void Link::IdleUp()
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

void Link::IdleDown()
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

void Link::IdleLeft()
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

void Link::IdleRight()
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

void Link::BasicAttackUp()
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
		GetAbility(0)->fixture = game_object->CreateCollisionSensor(iPoint(-8, -35), 15, 50, fixture_type::f_t_attack);
	}
}

void Link::BasicAttackDown()
{
	if (!attacking)
	{
		if (GetTeam() == ANIMATIONS_TEAM)
			game_object->SetAnimation("basic_attack_down");
		else
			game_object->SetAnimation("basic_attack_down_2");

		draw_offset = { 22, 24 };
		attacking = true;
		can_move = false;
		flip = false;
		GetAbility(0)->fixture = game_object->CreateCollisionSensor(iPoint(10, 35), 15, 50, fixture_type::f_t_attack);
	}
}

void Link::BasicAttackLeft()
{
	if (!attacking)
	{
		if (GetTeam() == ANIMATIONS_TEAM)
			game_object->SetAnimation("basic_attack_left");
		else
			game_object->SetAnimation("basic_attack_left_2");
		draw_offset = { 48, draw_offset.y };
		attacking = true;
		can_move = false;
		flip = true;
		GetAbility(0)->fixture = game_object->CreateCollisionSensor(iPoint(-30, 0), 50, 15, fixture_type::f_t_attack);
	}
}

void Link::BasicAttackRight()
{
	if (!attacking)
	{
		if (GetTeam() == ANIMATIONS_TEAM)
			game_object->SetAnimation("basic_attack_right");
		else
			game_object->SetAnimation("basic_attack_right_2");

		attacking = true;
		can_move = false;
		flip = false;
		GetAbility(0)->fixture = game_object->CreateCollisionSensor(iPoint(35, 0), 50, 15, fixture_type::f_t_attack);
	}
}

void Link::ShowBasicAttackUp()
{
	int main_view = App->scene->main_scene->player_manager->GetEntityViewportIfIsPlayer(this);
	App->view->LayerBlit(GetPos().y - 1, GetAbility(0)->hitbox_texture, { game_object->GetPos().x - 30, game_object->GetPos().y - 30 }, GetAbility(0)->hitbox_image, main_view, -1.0f, true, SDL_FLIP_NONE, -90);
}

void Link::ShowBasicAttackDown()
{
	int main_view = App->scene->main_scene->player_manager->GetEntityViewportIfIsPlayer(this);
	App->view->LayerBlit(GetPos().y - 1, GetAbility(0)->hitbox_texture, { game_object->GetPos().x - 18, game_object->GetPos().y + 20 }, GetAbility(0)->hitbox_image, main_view, -1.0f, true, SDL_FLIP_NONE, 90);
}

void Link::ShowBasicAttackLeft()
{
	int main_view = App->scene->main_scene->player_manager->GetEntityViewportIfIsPlayer(this);
	App->view->LayerBlit(GetPos().y - 1, GetAbility(0)->hitbox_texture, { game_object->GetPos().x - 53, game_object->GetPos().y + 0 }, GetAbility(0)->hitbox_image, main_view, -1.0f, true, SDL_FLIP_NONE, 180);
}

void Link::ShowBasicAttackRight()
{
	int main_view = App->scene->main_scene->player_manager->GetEntityViewportIfIsPlayer(this);
	App->view->LayerBlit(GetPos().y - 1, GetAbility(0)->hitbox_texture, { game_object->GetPos().x + 5, game_object->GetPos().y + 0 }, GetAbility(0)->hitbox_image, main_view, -1.0f, true, SDL_FLIP_NONE, 0);
}

void Link::Ability1Up()
{
	if (!attacking)
	{
		draw_offset = { 44, 60 };
		if (GetTeam() == ANIMATIONS_TEAM)
			game_object->SetAnimation("spin_attack");
		else
			game_object->SetAnimation("spin_attack_2");
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
	App->view->LayerBlit(GetPos().y - 1, GetAbility(2)->hitbox_texture, { game_object->GetPos().x - 50, game_object->GetPos().y - 50 }, GetAbility(2)->hitbox_image, main_view, -1.0f, true, SDL_FLIP_NONE, -90);
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
	App->view->LayerBlit(GetPos().y - 1, GetAbility(1)->hitbox_texture, { game_object->GetPos().x - 83, game_object->GetPos().y - 100 }, GetAbility(1)->hitbox_image, main_view, -1.0f, true, SDL_FLIP_NONE, -90);
}

void Link::ShowAbility2Down()
{
	int main_view = App->scene->main_scene->player_manager->GetEntityViewportIfIsPlayer(this);
	App->view->LayerBlit(GetPos().y - 1, GetAbility(1)->hitbox_texture, { game_object->GetPos().x - 86, game_object->GetPos().y + 85 }, GetAbility(1)->hitbox_image, main_view, -1.0f, true, SDL_FLIP_NONE, 90);
}

void Link::ShowAbility2Left()
{
	int main_view = App->scene->main_scene->player_manager->GetEntityViewportIfIsPlayer(this);
	App->view->LayerBlit(GetPos().y - 1, GetAbility(1)->hitbox_texture, { game_object->GetPos().x - 176, game_object->GetPos().y - 14 }, GetAbility(1)->hitbox_image, main_view, -1.0f, true, SDL_FLIP_NONE, 180);
}

void Link::ShowAbility2Right()
{
	int main_view = App->scene->main_scene->player_manager->GetEntityViewportIfIsPlayer(this);
	App->view->LayerBlit(GetPos().y - 1, GetAbility(1)->hitbox_texture, { game_object->GetPos().x + 16, game_object->GetPos().y - 12 }, GetAbility(1)->hitbox_image, main_view, -1.0f, true, SDL_FLIP_NONE, 0);
}

void Link::Ability3Up()
{
	draw_offset = { 44, 60 };
	ab3_dir = link_ability3_dir::a3_up;
	can_move = false;
	attacking = true;
	ability3_range = 0;
	invulnerable = true;
	GetAbility(3)->fixture = game_object->CreateCollisionSensor(iPoint(0, 0), 50, fixture_type::f_t_attack);
	DeleteAbility3Test();

	App->audio->PlayFx(link_ultimate, 0);
}

void Link::Ability3Down()
{
	draw_offset = { 44, 60 };
	ab3_dir = link_ability3_dir::a3_down;
	can_move = false;
	attacking = true;
	ability3_range = 0;
	invulnerable = true;
	GetAbility(3)->fixture = game_object->CreateCollisionSensor(iPoint(0, 0), 50, fixture_type::f_t_attack);
	DeleteAbility3Test();

	App->audio->PlayFx(link_ultimate, 0);
}

void Link::Ability3Left()
{
	draw_offset = { 44, 60 };
	ab3_dir = link_ability3_dir::a3_left;
	can_move = false;
	attacking = true;
	ability3_range = 0;
	invulnerable = true;
	GetAbility(3)->fixture = game_object->CreateCollisionSensor(iPoint(0, 0), 50, fixture_type::f_t_attack);
	DeleteAbility3Test();

	App->audio->PlayFx(link_ultimate, 0);
}

void Link::Ability3Right()
{
	draw_offset = { 44, 60 };
	ab3_dir = link_ability3_dir::a3_right;
	can_move = false;
	attacking = true;
	ability3_range = 0;
	invulnerable = true;
	GetAbility(3)->fixture = game_object->CreateCollisionSensor(iPoint(0, 0), 50, fixture_type::f_t_attack);
	DeleteAbility3Test();

	App->audio->PlayFx(link_ultimate, 0);
}

void Link::ShowAbility3Up()
{
	if(ability3_range<=ABILITY3_MAX_RANGE)
		ability3_range += ABILITY3_GROW_SPEED * App->GetDT();

	int main_view = App->scene->main_scene->player_manager->GetEntityViewportIfIsPlayer(this);

	SDL_Rect rect = { GetAbility(3)->hitbox_image.x + GetAbility(3)->hitbox_image.w - ability3_range,  GetAbility(3)->hitbox_image.y, ability3_range, GetAbility(3)->hitbox_image.h };
	App->view->LayerBlit(GetPos().y - 1, GetAbility(3)->hitbox_texture, { game_object->GetPos().x - 032, game_object->GetPos().y - 15 }, rect, main_view, -1.0f, true, SDL_FLIP_NONE, -90, 0, -10);

	CreateAbility3Test();
}

void Link::ShowAbility3Down()
{
	if (ability3_range <= ABILITY3_MAX_RANGE)
		ability3_range += ABILITY3_GROW_SPEED * App->GetDT();

	int main_view = App->scene->main_scene->player_manager->GetEntityViewportIfIsPlayer(this);
	SDL_Rect rect = { GetAbility(3)->hitbox_image.x + GetAbility(3)->hitbox_image.w - ability3_range,  GetAbility(3)->hitbox_image.y, ability3_range, GetAbility(3)->hitbox_image.h };
	App->view->LayerBlit(GetPos().y - 1, GetAbility(3)->hitbox_texture, { game_object->GetPos().x + 26, game_object->GetPos().y + 30 }, rect, main_view, -1.0f, true, SDL_FLIP_NONE, 90, 0, -10);

	CreateAbility3Test();
}

void Link::ShowAbility3Left()
{
	if (ability3_range <= ABILITY3_MAX_RANGE)
		ability3_range += ABILITY3_GROW_SPEED * App->GetDT();

	int main_view = App->scene->main_scene->player_manager->GetEntityViewportIfIsPlayer(this);
	SDL_Rect rect = { GetAbility(3)->hitbox_image.x + GetAbility(3)->hitbox_image.w - ability3_range,  GetAbility(3)->hitbox_image.y, ability3_range, GetAbility(3)->hitbox_image.h };
	App->view->LayerBlit(GetPos().y - 1, GetAbility(3)->hitbox_texture, { game_object->GetPos().x - 15, game_object->GetPos().y + 35 }, rect, main_view, -1.0f, true, SDL_FLIP_NONE, 180, 0, -10);
	
	CreateAbility3Test();
}

void Link::ShowAbility3Right()
{
	if (ability3_range <= ABILITY3_MAX_RANGE)
		ability3_range += ABILITY3_GROW_SPEED * App->GetDT();

	int main_view = App->scene->main_scene->player_manager->GetEntityViewportIfIsPlayer(this);
	SDL_Rect rect = { GetAbility(3)->hitbox_image.x + GetAbility(3)->hitbox_image.w - ability3_range,  GetAbility(3)->hitbox_image.y, ability3_range, GetAbility(3)->hitbox_image.h };
	App->view->LayerBlit(GetPos().y - 1, GetAbility(3)->hitbox_texture, { game_object->GetPos().x + 15, game_object->GetPos().y - 18 }, rect, main_view, -1.0f, true, SDL_FLIP_NONE, 0, 0, -10);

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

void Link::Die(Entity * killed_by)
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
