#include "Navi.h"
#include "GameObject.h"
#include "j1App.h"
#include "CollisionFilters.h"
#include "Functions.h"
#include "j1Textures.h"
#include "p2Log.h"
#include "j1Viewports.h"
#include "j1Entity.h"
#include "j1Spell.h"
#include "PlayerManager.h"
#include "j1Pathfinding.h"
#include "j1Map.h"
#include "Quest_Manager.h"
#include "j1XMLLoader.h"
#include "NaviBasicAttack.h"
#include "j1Audio.h"

#define BASIC_ATTACK_RANGE 270

#define ABILITY1_RANGE 140

#define ABILITY2_RANGE 160
#define ABILITY2_MOVE_SAFE_OFFSET 10
#define ABILITY2_SPEED 350

#define ABILITY3_DURATION 6

Navi::Navi(iPoint pos)
{
	game_object = new GameObject(iPoint(pos.x, pos.y), iPoint(30, 40), App->cf->CATEGORY_PLAYER, App->cf->MASK_PLAYER, pbody_type::p_t_player, 0);

	game_object->CreateCollisionSensor(iPoint(0, 0), game_object->GetHitBoxSize().x, game_object->GetHitBoxSize().y, fixture_type::f_t_hit_box);
	game_object->CreateCollision(iPoint(2, 10), 13, fixture_type::f_t_collision_box);
	game_object->SetListener((j1Module*)App->entity);
	game_object->SetListener((j1Module*)App->spell);
	game_object->SetFixedRotation(true);
	game_object->pbody->body->SetBullet(true);

	pugi::xml_document doc;
	App->xml->LoadXML("navi.xml", doc);

	pugi::xml_document doc2;
	App->xml->LoadXML("abilities_rects.xml", doc2);
	pugi::xml_node hit_node = doc2.child("file").child("navi");

	// Loading Abilities ----------------
	pugi::xml_node stats_node = doc.child("file").child("stats");
	rupee_reward = stats_node.attribute("rupees").as_int();

	stats.life = stats.base_hp = stats.max_life = stats_node.attribute("hp").as_int();
	stats.base_power = stats.power = stats_node.attribute("power").as_int();
	stats.base_speed = stats.speed = stats.restore_speed = stats_node.attribute("speed").as_int();

	float dmg_mult = 0;
	float cd = stats_node.child("ability1").attribute("cd").as_float();
	float bd = stats_node.child("ability1").attribute("bd").as_float();
	
	Ability* a1 = AddAbility(0, cd, bd, dmg_mult, "navi_basic_attack");
	a1->SetImages({ 816, 351, 81, 48 }, { 816, 474, 81, 48 }, { 1013, 1960, 80, 48 }, { 978, 425, 32, 32 }, 
	{ hit_node.child("basicattack_rect").child("rect").attribute("x").as_int(0),
		hit_node.child("basicattack_rect").child("rect").attribute("y").as_int(0),
		hit_node.child("basicattack_rect").child("rect").attribute("w").as_int(0),
		hit_node.child("basicattack_rect").child("rect").attribute("h").as_int(0) });

	dmg_mult = stats_node.child("ability2").attribute("mult").as_float();
	cd = stats_node.child("ability2").attribute("cd").as_float();
	heal = stats_node.child("ability2").attribute("heal").as_float();
	ability1_duration = stats_node.child("ability2").attribute("duration").as_float();
	Ability* a2 = AddAbility(1, cd, 0, 0);
	a2->SetImages({ 896, 351, 80, 48 }, { 896, 474, 80, 48 }, { 1093, 1960, 80, 48 }, { 1050, 425, 32, 32 });

	dmg_mult = stats_node.child("ability3").attribute("mult").as_float();
	cd = stats_node.child("ability3").attribute("cd").as_float();
	bd = stats_node.child("ability3").attribute("bd").as_int();
	Ability* a3 = AddAbility(2, cd, bd, dmg_mult);
	a3->SetImages({ 816, 399, 48, 73 }, { 816, 522, 48, 73 }, { 1013, 2008, 48, 73 }, { 1014, 425, 32, 32 }, 
	{ hit_node.child("blink_rect").child("rect").attribute("x").as_int(0),
		hit_node.child("blink_rect").child("rect").attribute("y").as_int(0),
		hit_node.child("blink_rect").child("rect").attribute("w").as_int(0),
		hit_node.child("blink_rect").child("rect").attribute("h").as_int(0) });

	dmg_mult = stats_node.child("ability4").attribute("mult").as_float();
	cd = stats_node.child("ability4").attribute("cd").as_float();
	bd = stats_node.child("ability4").attribute("bd").as_int();
	Ability* a4 = AddAbility(3, cd, bd, dmg_mult);
	a4->SetImages({ 864, 399, 48, 73 }, { 864, 522, 48, 73 }, { 1061, 2008, 48, 73 }, { 1086, 425, 32, 32 });
	// -------------------------------------

	game_object->SetTexture(game_object->LoadAnimationsFromXML(doc, "animations"));

	draw_offset = restore_draw_offset = { 23, 26 };

	blit_layer = 2;

	name = "navi";

	hey_listen = App->audio->LoadFx("Audio/FX/Entities/Navi/OOT_Navi_HeyListen.wav");
}

Navi::~Navi()
{
}

bool Navi::Start()
{
	bool ret = true;

	ability1_timer = App->AddGameplayTimer();
	ability3_timer = App->AddGameplayTimer();

	if (GetTeam() == ANIMATIONS_TEAM)
		game_object->SetAnimation("down");
	else
		game_object->SetAnimation("down_2");

	can_move = true;

	return ret;
}

bool Navi::PreUpdate()
{
	bool ret = true;


	return ret;
}

bool Navi::Update(float dt)
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

	ability1_ticks_time = ability1_duration / (4 + (stats.speed - stats.base_speed)*GetAbility(1)->damage_multiplicator);

	// Ability 1 --------------------
	if (ability1)
	{
		if (ability1_timer->ReadSec() < ability1_duration)
		{
			if (GetTeam() == ANIMATIONS_TEAM)
				App->view->LayerBlit(game_object->GetPos().y - 1, game_object->GetTexture(), { GetPos().x - 140, GetPos().y - 140 }, game_object->animator->GetAnimation("heal_area")->GetAnimationFrame(dt), 0);
			else
				App->view->LayerBlit(game_object->GetPos().y - 1, game_object->GetTexture(), { GetPos().x - 140, GetPos().y - 140 }, game_object->animator->GetAnimation("heal_area_2")->GetAnimationFrame(dt), 0);

			if (look_for_entities)
			{
				vector<Entity*> players = App->entity->FindEntitiesByBodyType(pbody_type::p_t_player);
				vector<Entity*> minions = App->entity->FindEntitiesByName("minion");

				for (int i = 0; i < players.size(); i++)
				{
					if (players.at(i)->GetTeam() == GetTeam())
					{
						heal_element he;
						he.timer = App->AddGameplayTimer();
						he.timer->SubstractTimeFromStart(ability1_ticks_time);
						he.entity = players.at(i);
						to_heal.push_back(he);
					}
				}

				for (int i = 0; i < minions.size(); i++)
				{
					if (minions.at(i)->GetTeam() == GetTeam())
					{
						heal_element he;
						he.timer = App->AddGameplayTimer();
						he.timer->SubstractTimeFromStart(ability1_ticks_time);
						he.entity = minions.at(i);
						to_heal.push_back(he);
					}
				}
				look_for_entities = false;
			}

			for(vector<heal_element>::iterator it = to_heal.begin(); it != to_heal.end();)
			{
				if (abs(DistanceFromTwoPoints(GetPos().x, GetPos().y, (*it).entity->GetPos().x, (*it).entity->GetPos().y)) <= ABILITY1_RANGE && (*it).timer->ReadSec() > ability1_ticks_time)
				{
					(*it).entity->Heal(heal);
					(*it).timer->Start();
				}
				else
					++it;
			}
		}
		else
		{
			ability1 = false;
			look_for_entities = true;

			for (vector<heal_element>::iterator it = to_heal.begin(); it != to_heal.end();)
			{
				(*it).CleanUp();
				it = to_heal.erase(it);
			}
		}
	}
	// Ability 2 --------------------
	if (attacking && ability2)
	{
		if (!point_found)
		{
			switch (ability2_dir)
			{
			case navi_ability2_dir::a2_down:
				while (!App->pathfinding->IsWalkable(App->map->WorldToMap(ability2_point.x, ability2_point.y)) && !to_delete)
				{
					ability2_point = iPoint(ability2_point.x, ability2_point.y - 30);
					find = true;
				}
				if (find)
					ability2_point.y -= ABILITY2_MOVE_SAFE_OFFSET;
				else
					ability2_point.y += (ABILITY2_MOVE_SAFE_OFFSET * 2);
				break;
			case navi_ability2_dir::a2_up:
				while (!App->pathfinding->IsWalkable(App->map->WorldToMap(ability2_point.x, ability2_point.y)) && !to_delete)
				{
					ability2_point = iPoint(ability2_point.x, ability2_point.y + 30);
					find = true;
				}
				if (find)
					ability2_point.y += ABILITY2_MOVE_SAFE_OFFSET;
				else
					ability2_point.y -= (ABILITY2_MOVE_SAFE_OFFSET * 2);
				break;
			case navi_ability2_dir::a2_left:
				while (!App->pathfinding->IsWalkable(App->map->WorldToMap(ability2_point.x, ability2_point.y)) && !to_delete)
				{
					ability2_point = iPoint(ability2_point.x + 30, ability2_point.y);
					find = true;
				}
				if (find)
					ability2_point.x += ABILITY2_MOVE_SAFE_OFFSET;
				else
					ability2_point.x -= (ABILITY2_MOVE_SAFE_OFFSET * 2);
				break;
			case navi_ability2_dir::a2_right:
				while (!App->pathfinding->IsWalkable(App->map->WorldToMap(ability2_point.x, ability2_point.y)) && !to_delete)
				{
					ability2_point = iPoint(ability2_point.x - 30, ability2_point.y);
					find = true;
				}
				if (find)
					ability2_point.x -= ABILITY2_MOVE_SAFE_OFFSET;
				else
					ability2_point.x += (ABILITY2_MOVE_SAFE_OFFSET * 2);
				break;
			}
			point_found = true;

			can_move = false;

			if (GetTeam() == ANIMATIONS_TEAM)
				game_object->SetAnimation("blink_movement");
			else
				game_object->SetAnimation("blink_movement_2");

			game_object->SetCatMask(App->cf->CATEGORY_NONCOLLISIONABLE, App->cf->MASK_NONCOLLISIONABLE);

		}

		if (point_found)
		{
			App->view->LayerDrawCircle(ability2_point.x, ability2_point.y, 3, 255, 255, 255, 255, 99);
			float angle = AngleFromTwoPoints(GetPos().x, GetPos().y, ability2_point.x, ability2_point.y) - 180;
			MoveAngle(ABILITY2_SPEED, angle);

			if (abs(DistanceFromTwoPoints(GetPos().x, GetPos().y, ability2_point.x, ability2_point.y)) < 20)
			{
				// Reset
				ability2_dir = navi_ability2_dir::a2_direction_null;
				attacking = false;
				ability2 = false;
				ability2_point = NULLPOINT;
				find = false;
				point_found = false;
				can_move = true;
				game_object->SetCatMask(App->cf->CATEGORY_PLAYER, App->cf->MASK_PLAYER);
			}
		}
	}
	
	return ret;
}

bool Navi::Draw(float dt)
{
	bool ret = true;

	LifeBar(iPoint(60, 5), iPoint(-29, -40));

	// Blit
	if (flip)
		App->view->LayerBlit(GetPos().y, game_object->GetTexture(), { game_object->GetPos().x - draw_offset.x - 3, game_object->GetPos().y - draw_offset.y }, game_object->GetCurrentAnimationRect(dt), 0, -1.0f, true, SDL_FLIP_HORIZONTAL);
	else
		App->view->LayerBlit(GetPos().y, game_object->GetTexture(), { game_object->GetPos().x - draw_offset.x, game_object->GetPos().y - draw_offset.y }, game_object->GetCurrentAnimationRect(dt), 0, -1.0f, true, SDL_FLIP_NONE);

	// Ability 3 ----------------------
	if (ability3)
	{
		if (ability3_timer->ReadSec() < ABILITY3_DURATION)
		{
			// Get enemy team
			int enemy_team = 0;
			if (GetTeam() == 1)
				enemy_team = 2;
			else
				enemy_team = 1;

			// Print hey listen
			vector<int> view = App->scene->main_scene->player_manager->GetTeamViewports(enemy_team);
			SDL_Rect rect = App->view->GetViewportRect(1);
			for (int i = 0; i < view.size(); i++)
			{
				App->view->LayerBlit(9999, game_object->GetTexture(), { (int)(rect.w*0.5f) - 180, (int)(rect.h*0.5f) - 150 }, game_object->animator->GetAnimation("ulti_letters")->GetAnimationFrame(dt), view.at(i), -1.0f, false);
			}

			// Invert controls
			vector<Entity*> enemy_p = App->scene->main_scene->player_manager->GetTeamPlayers(enemy_team);
			
			for (int i = 0; i < enemy_p.size(); i++)
			{
				Player* p = App->scene->main_scene->player_manager->GetPlayerFromBody(enemy_p.at(i)->game_object->pbody);
				if (p != nullptr && !p->is_dead && p->entity != nullptr)
					p->invert_controls = true;
			}
		}
		else
		{
			ability3 = false;

			// Get enemy team
			int enemy_team = 0;
			if (GetTeam() == 1)
				enemy_team = 2;
			else
				enemy_team = 1;

			// Des-invert controls
			vector<Entity*> enemy_p = App->scene->main_scene->player_manager->GetTeamPlayers(enemy_team);

			for (int i = 0; i < enemy_p.size(); i++)
			{
				Player* p = App->scene->main_scene->player_manager->GetPlayerFromBody(enemy_p.at(i)->game_object->pbody);
				if (p != nullptr)
					p->invert_controls = false;
			}
		}
	}
	// -------------------------------

	// -------------
	// End atacking (It's down the blit because of a reason)
	// -------------

	return ret;
}

bool Navi::PostUpdate()
{
	bool ret = true;



	return ret;
}

bool Navi::CleanUp()
{
	bool ret = true;

	App->DeleteGameplayTimer(ability1_timer);
	App->DeleteGameplayTimer(ability3_timer);

	if (!to_heal.empty())
	{
		for (vector<heal_element>::iterator it = to_heal.begin(); it != to_heal.end();)
		{
			(*it).CleanUp();
			it = to_heal.erase(it);
		}
	}

	return ret;
}

void Navi::MoveAngle(float speed, float angle)
{
	float sp = speed * App->GetDT();
	fPoint s(sp * cos(DEGTORAD *angle), sp * sin(DEGTORAD * angle));
	game_object->SetPos({ game_object->fGetPos().x - s.x, game_object->fGetPos().y - s.y });
}

void Navi::MoveUp(float speed)
{
	if (can_move)
	{
		game_object->SetPos({ game_object->fGetPos().x, game_object->fGetPos().y - speed });
	}
}

void Navi::MoveDown(float speed)
{
	if (can_move)
		game_object->SetPos({ game_object->fGetPos().x, game_object->fGetPos().y + speed });
}

void Navi::MoveLeft(float speed)
{
	if (can_move)
		game_object->SetPos({ game_object->fGetPos().x - speed, game_object->fGetPos().y });
}

void Navi::MoveRight(float speed)
{
	if (can_move)
		game_object->SetPos({ game_object->fGetPos().x + speed, game_object->fGetPos().y });
}

void Navi::MoveUpRight(float speed)
{
	if (can_move)
	{
		fPoint s(speed * cos(DEGTORAD * 45), speed * sin(DEGTORAD * 45));
		game_object->SetPos({ game_object->fGetPos().x + s.x, game_object->fGetPos().y - s.y });
	}
}

void Navi::MoveDownRight(float speed)
{
	if (can_move)
	{
		fPoint s(speed * cos(DEGTORAD * 45), speed * sin(DEGTORAD * 45));
		game_object->SetPos({ game_object->fGetPos().x + s.x, game_object->fGetPos().y + s.y });
	}
}

void Navi::MoveUpLeft(float speed)
{
	if (can_move)
	{
		fPoint s(speed * cos(DEGTORAD * 45), speed * sin(DEGTORAD * 45));
		game_object->SetPos({ game_object->fGetPos().x - s.x, game_object->fGetPos().y - s.y });
	}
}

void Navi::MoveDownLeft(float speed)
{
	if (can_move)
	{
		fPoint s(speed * cos(DEGTORAD * 45), speed * sin(DEGTORAD * 45));
		game_object->SetPos({ game_object->fGetPos().x - s.x, game_object->fGetPos().y + s.y });
	}
}

void Navi::RunUp()
{
	if (can_move)
	{
		if (ability3)
		{
			if (GetTeam() == ANIMATIONS_TEAM)
				game_object->SetAnimation("blink_up");
			else
				game_object->SetAnimation("blink_up_2");
		}
		else
		{
			if (GetTeam() == ANIMATIONS_TEAM)
				game_object->SetAnimation("up");
			else
				game_object->SetAnimation("up_2");
		}
		flip = false;
	}
}

void Navi::RunDown()
{
	if (can_move)
	{
		if (ability3)
		{
			if (GetTeam() == ANIMATIONS_TEAM)
				game_object->SetAnimation("blink_down");
			else
				game_object->SetAnimation("blink_down_2");
		}
		else
		{
			if (GetTeam() == ANIMATIONS_TEAM)
				game_object->SetAnimation("down");
			else
				game_object->SetAnimation("down_2");
		}
		flip = false;
	}
}

void Navi::RunLeft()
{
	if (can_move)
	{
		if (ability3)
		{
			if (GetTeam() == ANIMATIONS_TEAM)
				game_object->SetAnimation("blink_side");
			else
				game_object->SetAnimation("blink_side_2");
		}
		else
		{
			if (GetTeam() == ANIMATIONS_TEAM)
				game_object->SetAnimation("lateral");
			else
				game_object->SetAnimation("lateral_2");
		}
		flip = true;
	}
}

void Navi::RunRight()
{
	if (can_move)
	{
		if (ability3)
		{
			if (GetTeam() == ANIMATIONS_TEAM)
				game_object->SetAnimation("blink_side");
			else
				game_object->SetAnimation("blink_side_2");
		}
		else
		{
			if (GetTeam() == ANIMATIONS_TEAM)
				game_object->SetAnimation("lateral");
			else
				game_object->SetAnimation("lateral_2");
		}
		flip = false;
	}
}

void Navi::IdleUp()
{
	if (can_move)
	{
		RunUp();
	}
}

void Navi::IdleDown()
{
	if (can_move)
	{
		RunDown();
	}
}

void Navi::IdleLeft()
{
	if (can_move)
	{
		RunLeft();
	}
}

void Navi::IdleRight()
{
	if (can_move)
	{
		RunRight();
	}
}

void Navi::BasicAttackUp()
{
	if (!attacking)
	{
		NaviBasicAttack* ba = (NaviBasicAttack*)App->spell->CreateSpell(navi_basic_attack, GetPos(), this);
		ba->Set(n_b_a_direction::n_b_a_up);
	}
}

void Navi::BasicAttackDown()
{
	if (!attacking)
	{
		NaviBasicAttack* ba = (NaviBasicAttack*)App->spell->CreateSpell(navi_basic_attack, GetPos(), this);
		ba->Set(n_b_a_direction::n_b_a_down);
	}
}

void Navi::BasicAttackLeft()
{
	if (!attacking)
	{
		NaviBasicAttack* ba = (NaviBasicAttack*)App->spell->CreateSpell(navi_basic_attack, GetPos(), this);
		ba->Set(n_b_a_direction::n_b_a_left);
	}
}

void Navi::BasicAttackRight()
{
	if (!attacking)
	{
		NaviBasicAttack* ba = (NaviBasicAttack*)App->spell->CreateSpell(navi_basic_attack, GetPos(), this);
		ba->Set(n_b_a_direction::n_b_a_right);
	}
}

void Navi::ShowBasicAttackUp()
{
	int main_view = App->scene->main_scene->player_manager->GetEntityViewportIfIsPlayer(this);
	App->view->LayerBlit(GetPos().y - 1, GetAbility(0)->hitbox_texture, { game_object->GetPos().x - 133, game_object->GetPos().y - 153 }, GetAbility(0)->hitbox_image, main_view, -1.0f, true, SDL_FLIP_NONE, -90);
}

void Navi::ShowBasicAttackDown()
{
	int main_view = App->scene->main_scene->player_manager->GetEntityViewportIfIsPlayer(this);
	App->view->LayerBlit(GetPos().y - 1, GetAbility(0)->hitbox_texture, { game_object->GetPos().x - 136, game_object->GetPos().y + 110 }, GetAbility(0)->hitbox_image, main_view, -1.0f, true, SDL_FLIP_NONE, 90);
}

void Navi::ShowBasicAttackLeft()
{
	int main_view = App->scene->main_scene->player_manager->GetEntityViewportIfIsPlayer(this);
	App->view->LayerBlit(GetPos().y - 1, GetAbility(0)->hitbox_texture, { game_object->GetPos().x - 286, game_object->GetPos().y - 5 }, GetAbility(0)->hitbox_image, main_view, -1.0f, true, SDL_FLIP_NONE, 180);
}

void Navi::ShowBasicAttackRight()
{
	int main_view = App->scene->main_scene->player_manager->GetEntityViewportIfIsPlayer(this);
	App->view->LayerBlit(GetPos().y - 1, GetAbility(0)->hitbox_texture, { game_object->GetPos().x + 16, game_object->GetPos().y - 5 }, GetAbility(0)->hitbox_image, main_view, -1.0f, true, SDL_FLIP_NONE, 0);
}

void Navi::Ability1Up()
{
	ability1 = true;
	ability1_timer->Start();
}

void Navi::Ability1Down()
{
	Ability1Up();
}

void Navi::Ability1Left()
{
	Ability1Up();
}

void Navi::Ability1Right()
{
	Ability1Up();
}

void Navi::ShowAbility1Up()
{
	
}

void Navi::ShowAbility1Down()
{

}

void Navi::ShowAbility1Left()
{

}

void Navi::ShowAbility1Right()
{

}

void Navi::Ability2Up()
{
	if (!attacking)
	{
		ability2 = true;
		attacking = true;
		ability2_dir = navi_ability2_dir::a2_up;
		ability2_point = { GetPos().x, GetPos().y - ABILITY2_RANGE };
	}
}

void Navi::Ability2Down()
{
	if (!attacking)
	{
		ability2 = true;
		attacking = true;
		ability2_dir = navi_ability2_dir::a2_down;
		ability2_point = { GetPos().x, GetPos().y + ABILITY2_RANGE };
	}
}

void Navi::Ability2Left()
{
	if (!attacking)
	{
		ability2 = true;
		attacking = true;
		ability2_dir = navi_ability2_dir::a2_left;
		ability2_point = { GetPos().x - ABILITY2_RANGE , GetPos().y};
	}
}

void Navi::Ability2Right()
{
	if (!attacking)
	{
		ability2 = true;
		attacking = true;
		ability2_dir = navi_ability2_dir::a2_right;
		ability2_point = { GetPos().x + ABILITY2_RANGE , GetPos().y };
	}
}

void Navi::ShowAbility2Up()
{
	int main_view = App->scene->main_scene->player_manager->GetEntityViewportIfIsPlayer(this);
	App->view->LayerBlit(GetPos().y - 1, GetAbility(2)->hitbox_texture, { game_object->GetPos().x - 16, game_object->GetPos().y - 10 - ABILITY2_RANGE }, GetAbility(2)->hitbox_image, main_view, -1.0f, true, SDL_FLIP_NONE, 0);
}

void Navi::ShowAbility2Down()
{
	int main_view = App->scene->main_scene->player_manager->GetEntityViewportIfIsPlayer(this);
	App->view->LayerBlit(GetPos().y - 1, GetAbility(2)->hitbox_texture, { game_object->GetPos().x - 16, game_object->GetPos().y - 17 + ABILITY2_RANGE }, GetAbility(2)->hitbox_image, main_view, -1.0f, true, SDL_FLIP_NONE, 180);
}

void Navi::ShowAbility2Left()
{
	int main_view = App->scene->main_scene->player_manager->GetEntityViewportIfIsPlayer(this);
	App->view->LayerBlit(GetPos().y - 1, GetAbility(2)->hitbox_texture, { game_object->GetPos().x - 16 - ABILITY2_RANGE, game_object->GetPos().y - 16 }, GetAbility(2)->hitbox_image, main_view, -1.0f, true, SDL_FLIP_NONE, -90);
}

void Navi::ShowAbility2Right()
{
	int main_view = App->scene->main_scene->player_manager->GetEntityViewportIfIsPlayer(this);
	App->view->LayerBlit(GetPos().y - 1, GetAbility(2)->hitbox_texture, { game_object->GetPos().x - 16 + ABILITY2_RANGE, game_object->GetPos().y - 16 }, GetAbility(2)->hitbox_image, main_view, -1.0f, true, SDL_FLIP_NONE, 90);
}

void Navi::Ability3Up()
{
	ability3 = true;
	ability3_timer->Start();
}

void Navi::Ability3Down()
{
	Ability3Up();
	App->audio->PlayFx(hey_listen, 0);
}

void Navi::Ability3Left()
{
	Ability3Up();
	App->audio->PlayFx(hey_listen, 0);
}

void Navi::Ability3Right()
{
	Ability3Up();
	App->audio->PlayFx(hey_listen, 0);
}

void Navi::ShowAbility3Up()
{

}

void Navi::ShowAbility3Down()
{

}

void Navi::ShowAbility3Left()
{

}

void Navi::ShowAbility3Right()
{

}

void Navi::OnColl(PhysBody* bodyA, PhysBody * bodyB, b2Fixture * fixtureA, b2Fixture * fixtureB)
{

}

void Navi::OnCollEnter(PhysBody * bodyA, PhysBody * bodyB, b2Fixture * fixtureA, b2Fixture * fixtureB)
{

}

void Navi::OnCollOut(PhysBody * bodyA, PhysBody * bodyB, b2Fixture * fixtureA, b2Fixture * fixtureB)
{

}

void Navi::ListenEv(int type, EventThrower * origin, int id)
{
	Event* curr_event = nullptr;

	if (type = static_cast<int>(event_type::e_t_death))
	{
		curr_event = origin->GetEvent(id);

		if (curr_event == nullptr)
			return;

		if (!to_heal.empty())
		{
			for (vector<heal_element>::iterator it = to_heal.begin(); it != to_heal.end();)
			{
				if ((*it).entity == curr_event->event_data.entity)
				{
					(*it).CleanUp();
					it = to_heal.erase(it);
					break;
				}
				else
					++it;
			}
		}
	}

}

void Navi::SetCamera(int id)
{
	if (id > 0 && id < 5)
	{
		camera = id;
	}
}

iPoint Navi::GetPos() const
{
	return game_object->GetPos();
}

void Navi::Die(Entity * killed_by)
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
