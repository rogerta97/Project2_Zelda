#include "Minion.h"
#include "GameObject.h"
#include "j1Viewports.h"
#include "CollisionFilters.h"
#include "j1App.h"
#include "j1Map.h"
#include "j1Pathfinding.h"
#include "PlayerManager.h"
#include "j1Entity.h"
#include "Entity.h"
#include "p2Log.h"
#include "MinionManager.h"
#include "MainScene.h"
#include "Tower.h"
#include "TowerManager.h"
#include "Quest_Manager.h"
#include "j1XMLLoader.h"
#include "BaseManager.h"
#include <climits>

#define Half_Tile 16

Minion::Minion(iPoint pos)
{
	game_object = new GameObject(iPoint(pos.x, pos.y), iPoint(30,40), App->cf->CATEGORY_PLAYER, App->cf->MASK_PLAYER, pbody_type::p_t_npc, 0);

	game_object->CreateCollisionSensor(iPoint(0, 0), game_object->GetHitBoxSize().x, game_object->GetHitBoxSize().y, fixture_type::f_t_hit_box);
	game_object->CreateCollision(iPoint(0, 15), 7, fixture_type::f_t_collision_box);
	game_object->SetListener((j1Module*)App->entity);
	game_object->SetListener((j1Module*)App->spell);
	game_object->SetFixedRotation(true);

	pugi::xml_document doc;
	App->xml->LoadXML("minion.xml", doc);
	pugi::xml_node stats_node = doc.child("file").child("stats");
	rupee_reward = stats_node.attribute("rupees").as_int();

	stats.life = stats.base_hp = stats.max_life = stats_node.attribute("hp").as_int();
	stats.base_power = stats.power = stats_node.attribute("power").as_int();
	stats.base_speed = stats.speed = stats.restore_speed = stats_node.attribute("speed").as_int();
	tower_dmg_mult = stats_node.attribute("tower_dmg_mult").as_float();

	float dmg_mult = stats_node.child("ability1").attribute("mult").as_float();
	float cd = stats_node.child("ability1").attribute("cd").as_float();
	int bd = stats_node.child("ability1").attribute("bd").as_int();
	AddAbility(0, cd, bd, dmg_mult);
	
	game_object->SetTexture(game_object->LoadAnimationsFromXML(doc, "animations"));

	event_thrower = new EventThrower();

	name = "minion";
}

Minion::~Minion()
{

}

bool Minion::Start()
{
	bool ret = true;

	game_object->SetAnimation("idle_down");

	show_life_bar = true;

	return ret;
}

bool Minion::PreUpdate()
{
	bool ret = true;



	return ret;
}

bool Minion::Update(float dt)
{
	bool ret = true;

	if (to_delete)
		return true;

	speed = stats.speed*dt;

	if (!stuned)
	{
		switch (state)
		{
		case Minion_Idle:
			MinionIdle();
			break;
		case Minion_Move:
			MinionMove();
			break;
		case Minion_Attack:
			MinionAttack();
			break;
		default:
			break;
		}
	}
	else
		SetIdleAnim();
	
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
				if (spell->name == "t_attack")
				{
					DealDamage((((float)entity->stats.power * (float)spell->stats.damage_multiplicator) + (float)ability->damage)*tower_dmg_mult);
				}
				else
				{
					DealDamage(((float)entity->stats.power * (float)spell->stats.damage_multiplicator) + (float)ability->damage); // Spells control their own damage mutiplicator

					spell->Effects(entity, this, ability);
				}
			}
			else
				DealDamage((entity->stats.power * ability->damage_multiplicator) + ability->damage);
		}

		Die(entity);
	}

	return ret;
}

bool Minion::Draw(float dt)
{
	bool ret = true;

	LifeBar(iPoint(20, 3), iPoint(-10, -25));

	if (flip)
		App->view->LayerBlit(GetPos().y, game_object->GetTexture(), { game_object->GetPos().x - draw_offset.x - 45, game_object->GetPos().y - draw_offset.y - 35 }, game_object->GetCurrentAnimationRect(dt), 0, -1.0f, true, SDL_FLIP_HORIZONTAL);
	else
		App->view->LayerBlit(GetPos().y, game_object->GetTexture(), { game_object->GetPos().x - draw_offset.x - 17, game_object->GetPos().y - draw_offset.y - 35 }, game_object->GetCurrentAnimationRect(dt), 0, -1.0f, true, SDL_FLIP_NONE);

	return ret;
}

bool Minion::PostUpdate()
{
	bool ret = true;



	return ret;
}

bool Minion::CleanUp()
{
	bool ret = true;

	RELEASE(event_thrower);

	return ret;
}

void Minion::MoveUp(float speed)
{
	game_object->SetPos({ game_object->fGetPos().x, game_object->fGetPos().y - speed });
}

void Minion::MoveDown(float speed)
{
	game_object->SetPos({ game_object->fGetPos().x, game_object->fGetPos().y + speed });
}

void Minion::MoveLeft(float speed)
{
	game_object->SetPos({ game_object->fGetPos().x - speed, game_object->fGetPos().y });
}

void Minion::MoveRight(float speed)
{
	game_object->SetPos({ game_object->fGetPos().x + speed, game_object->fGetPos().y });
}

void Minion::MoveUpRight(float speed)
{
	fPoint s(speed * cos(DEGTORAD * 45), speed * sin(DEGTORAD * 45));
	game_object->SetPos({ game_object->fGetPos().x + s.x, game_object->fGetPos().y - s.y });
}

void Minion::MoveDownRight(float speed)
{
	fPoint s(speed * cos(DEGTORAD * 45), speed * sin(DEGTORAD * 45));
	game_object->SetPos({ game_object->fGetPos().x + s.x, game_object->fGetPos().y + s.y });
}

void Minion::MoveUpLeft(float speed)
{
	fPoint s(speed * cos(DEGTORAD * 45), speed * sin(DEGTORAD * 45));
	game_object->SetPos({ game_object->fGetPos().x - s.x, game_object->fGetPos().y - s.y });
}

void Minion::MoveDownLeft(float speed)
{
	fPoint s(speed * cos(DEGTORAD * 45), speed * sin(DEGTORAD * 45));
	game_object->SetPos({ game_object->fGetPos().x - s.x, game_object->fGetPos().y + s.y });
}

void Minion::RunUp()
{
	game_object->SetAnimation("run_up");
	flip = false;
	anim_state = run_up;
}

void Minion::RunDown()
{
	game_object->SetAnimation("run_down");
	flip = false;
	anim_state = run_down;
}

void Minion::RunLeft()
{
	game_object->SetAnimation("run_lateral");
	flip = true;
	anim_state = run_left;
	draw_offset.x = 8;
}

void Minion::RunRight()
{
	game_object->SetAnimation("run_lateral");
	flip = false;
	anim_state = run_right;
	draw_offset.x = -8;
}

void Minion::IdleUp()
{
	game_object->SetAnimation("idle_up");
	flip = false;
	anim_state = idle_up;
}

void Minion::IdleDown()
{
	game_object->SetAnimation("idle_down");
	flip = false;
	anim_state = idle_down;
}

void Minion::IdleLeft()
{
	game_object->SetAnimation("idle_lateral");
	flip = true;
	anim_state = idle_left;
	draw_offset.x = 8;
}

void Minion::IdleRight()
{
	game_object->SetAnimation("idle_lateral");
	flip = false;
	anim_state = idle_right;
	draw_offset.x = -8;
}

void Minion::OnColl(PhysBody* bodyA, PhysBody * bodyB, b2Fixture * fixtureA, b2Fixture * fixtureB)
{
	switch (bodyA->type)
	{
	case pbody_type::p_t_npc:
		
		break;

	}
}

iPoint Minion::GetPos() const
{
	return game_object->GetPos();
}

void Minion::SetBasePath(std::list<iPoint>& path)
{
	for (std::list<iPoint>::iterator it = path.begin(); it != path.end(); it++)
	{
		base_path.push_back(*it);
	}
}

void Minion::Die(Entity * killed_by)
{
	if (stats.life <= 0 && !to_delete)
	{
		App->entity->AddRupeesIfPlayer(killed_by, rupee_reward);
		App->scene->main_scene->minion_manager->KillMinion(this);

		if (killed_by != nullptr && killed_by->is_player && killed_by != nullptr)
		{
			//Add kill to killer
			App->scene->players[App->scene->main_scene->player_manager->GetEntityViewportIfIsPlayer(killed_by) - 1].minions++;
		}
	}
}

void Minion::MinionIdle()
{
	CheckState();

	SetIdleAnim();
}

void Minion::MinionMove()
{
	CheckState();
	draw_offset.SetToZero();

	iPoint map_pos = App->map->WorldToMap(GetPos().x, GetPos().y);

	switch (move_state)
	{
	case Move_FollowBasePath:
	{
		if (base_path_index < base_path.size()-1)
		{
			if (map_pos == base_path.at(base_path_index))
				base_path_index++;
		}
		else
		{
			state = Minion_Idle;
			break;
		}

		iPoint target_pos = App->map->MapToWorld(base_path.at(base_path_index).x, base_path.at(base_path_index).y);
		target_pos.y += Half_Tile;
		target_pos.x += Half_Tile;

		Move(target_pos.x - GetPos().x, target_pos.y - GetPos().y);

		break; 
	}
	case Move_AproachTarget:
	{
		if (target_path_index < target_path.size()-1)
		{
			if (map_pos == target_path.at(target_path_index))
				target_path_index++;
		}
		else
		{
			move_state = Move_FollowBasePath;
			break;
		}

		iPoint target_pos = App->map->MapToWorld(target_path.at(target_path_index).x, target_path.at(target_path_index).y);
		target_pos.y += Half_Tile;
		target_pos.x += Half_Tile;

		Move(target_pos.x - GetPos().x, target_pos.y - GetPos().y);

		break;
	}
	case Move_ReturnToPath:
	{
		if (target_path_index < target_path.size()-1)
		{
			if (map_pos == target_path.at(target_path_index))
				target_path_index++;
		}
		else {
			move_state = Move_FollowBasePath;
			break;
		}

		iPoint target_pos = App->map->MapToWorld(target_path.at(target_path_index).x, target_path.at(target_path_index).y);
		target_pos.y += Half_Tile;
		target_pos.x += Half_Tile;

		Move(target_pos.x - GetPos().x, target_pos.y - GetPos().y);

		break;
	}
	default:
		break;
	}

}

void Minion::MinionAttack()
{
	CheckState();

	//FaceTarget();

	if (abilities.at(0)->CdCompleted())
	{
		Attack();
		abilities.at(0)->CdReset();
	}
}

void Minion::CheckState()
{
	if (target != nullptr)
	{
		if (target->to_delete == true)
		{
			target = nullptr;
		}
	}

	switch (state)
	{
	case Minion_Idle:
		if (base_path_index < base_path.size()-1)
		{
			state = Minion_Move;
			move_state = Move_FollowBasePath;
		}
		else {
			if (LookForTarget())
			{
				PathToTarget(); 
				state = Minion_Move;
			}
		}
		break;
	case Minion_Move:
	{
		switch (move_state)
		{
		case Move_FollowBasePath:
			if (LookForTarget())
				PathToTarget();
			else if (base_path_index == base_path.size()-1)
				App->scene->main_scene->minion_manager->KillMinion(this);
			break;
		case Move_AproachTarget:
			if (target == nullptr)
			{
				target_path_index = 0;
				target = nullptr;
				move_state = Move_ReturnToPath;
				PathToBasePath();
			}
			else
			{
				if (GetPos().DistanceTo(target->GetPos()) < attack_range - attack_range / 4) // Improve
					state = Minion_Attack;
				else
				{
					if (game_object->GetPos().DistanceTo(target->GetPos()) < vision_range && GetPos().DistanceTo(App->map->MapToWorld(base_path.at(base_path_index).x, base_path.at(base_path_index).y)) < vision_range)
					{
						if (target != nullptr && App->map->WorldToMap(target->GetPos().x, target->GetPos().y) != *target_path.end())
						{
							PathToTarget();
						}
					}
					else
					{
						target_path_index = 0;
						target = nullptr;
						move_state = Move_ReturnToPath;
						CheckNearestPathTile();
						PathToBasePath();
					}
				}
			}
			break;
		case Move_ReturnToPath:
			if (base_path_index < base_path.size()) 
			{
				iPoint map_pos = App->map->WorldToMap(GetPos().x, GetPos().y);
				if (map_pos.DistanceTo(base_path.at(base_path_index)) < 2)
					move_state = Move_FollowBasePath;
			}
			break;
		default:
			break;
		}
		break; 
	}
	case Minion_Attack:
		if (target == nullptr)
		{
			target_path_index = 0;
			move_state = Move_ReturnToPath;
			state = Minion_Move;
			PathToBasePath();
			break;
		}
		if (game_object->animator->IsCurrentAnimation("basic_attack_up") || game_object->animator->IsCurrentAnimation("basic_attack_down")
			|| game_object->animator->IsCurrentAnimation("basic_attack_left") || game_object->animator->IsCurrentAnimation("basic_attack_right"))
		{
			if (game_object->animator->GetCurrentAnimation()->Finished())
			{
				if (abilities.at(0)->fixture != nullptr)
				{
					game_object->DeleteFixture(abilities.at(0)->fixture);
					abilities.at(0)->fixture = nullptr;
				}

				if (GetPos().DistanceTo(target->GetPos()) > attack_range + abs(attack_pos_offset.x))
				{
					state = Minion_Move;
					move_state = Move_AproachTarget;
				}
				game_object->animator->GetCurrentAnimation()->Reset();
				draw_offset.SetToZero();
				SetIdleAnim();
			}
		}
		else if (target->to_delete == true)
		{
			state = Minion_Move;
			move_state = Move_ReturnToPath;
			PathToBasePath();
		}
		break;
	default:
		break;
	}
}

void Minion::SetTargetPath(const std::list<iPoint>* path)
{
	for (std::list<iPoint>::const_iterator it = path->begin(); it != path->end(); it++)
	{
		target_path.push_back(*it);
	}
}

void Minion::PathToTarget()
{
	if (App->pathfinding->CreatePath(App->map->WorldToMap(GetPos().x, GetPos().y), App->map->WorldToMap(target->GetPos().x + attack_pos_offset.x, target->GetPos().y + attack_pos_offset.y)) > 0)
	{
		target_path.clear();
		SetTargetPath(App->pathfinding->GetLastPath());
		target_path_index = 0;
		move_state = Move_AproachTarget;
	}
	else 
	{
		PathToBasePath();
	}
}

void Minion::PathToBasePath()
{
	if (App->pathfinding->CreatePath(App->map->WorldToMap(GetPos().x, GetPos().y), base_path.at(base_path_index)) > 0)
	{
		target_path.clear();
		SetTargetPath(App->pathfinding->GetLastPath());
		target_path_index = 0;
		move_state = Move_ReturnToPath;
	}
	else
	{
		move_state = Move_FollowBasePath;
	}
}

bool Minion::LookForTarget()
{
	bool ret = false;

	attack_pos_offset.SetToZero();

	//Chack for enemy minion
	std::list<Minion*> minions;
	if (GetTeam() == 1)
		minions = App->scene->main_scene->minion_manager->GetMinionList(2);
	else
		minions = App->scene->main_scene->minion_manager->GetMinionList(1);

	for (std::list<Minion*>::iterator it = minions.begin(); it != minions.end(); it++)
	{
		if (GetPos().DistanceTo((*it)->GetPos()) < vision_range)
		{
			target = *it;
			ret = true;
			break;
		}
	}

	// Check for turrets
	if (ret == false)
	{
		std::list<Tower*> towers;
		if (GetTeam() == 1)
			towers = App->scene->main_scene->tower_manager->GetTowerList(2);
		else
			towers = App->scene->main_scene->tower_manager->GetTowerList(1);

		for (std::list<Tower*>::iterator it = towers.begin(); it != towers.end(); it++)
		{
			if (GetPos().DistanceTo((*it)->GetPos()) < vision_range)
			{
				target = *it;
				switch (GetTeam())
				{
				case 1:
					attack_pos_offset.x = -40;
					break;
				case 2:
					attack_pos_offset.x = 40;
					break;
				}
				ret = true;
				break;
			}
		}
	}

	//Check for Players
	if (ret == false)
	{
		std::vector<Entity*> players;
		if (GetTeam() == 1)
			players = App->scene->main_scene->player_manager->GetTeamPlayers(2);
		else
			players = App->scene->main_scene->player_manager->GetTeamPlayers(1);

		for (std::vector<Entity*>::iterator it = players.begin(); it != players.end(); it++)
		{
			if (GetPos().DistanceTo((*it)->GetPos()) < vision_range)
			{
				target = *it;
				ret = true;
				break;
			}
		}
	}
	return ret;
}

void Minion::Move(int delta_x, int delta_y)
{
	if (delta_x > 0)
	{
		if (delta_y > 0)
		{
			MoveDownRight(speed);
			RunRight();
		}
		else if (delta_y < 0)
		{
			MoveUpRight(speed);
			RunRight();
		}
		else {
			MoveRight(speed);
			RunRight();
		}
	}
	else if (delta_x < 0)
	{
		if (delta_y > 0)
		{
			MoveDownLeft(speed);
			RunLeft();
		}
		else if (delta_y < 0)
		{
			MoveUpLeft(speed);
			RunLeft();
		}
		else {
			MoveLeft(speed);
			RunLeft();
		}
	}
	else
	{
		if (delta_y > 0)
		{
			MoveDown(speed);
			RunDown();
		}
		else if (delta_y < 0)
		{
			MoveUp(speed);
			RunUp();
		}
	}
}

void Minion::BasicAttackUp()
{
	game_object->animator->SetAnimation("basic_attack_up");
	anim_state = basic_atack_up;
	GetAbility(0)->fixture = game_object->CreateCollisionSensor(iPoint(10, -25), 5, 10, fixture_type::f_t_attack);
}

void Minion::BasicAttackDown()
{
	game_object->animator->SetAnimation("basic_attack_down");
	anim_state = basic_atack_down;
	GetAbility(0)->fixture = game_object->CreateCollisionSensor(iPoint(-10, 25), 5, 10, fixture_type::f_t_attack);
	draw_offset.y = 10;
}

void Minion::BasicAttackLeft()
{
	game_object->animator->SetAnimation("basic_attack_left");
	anim_state = basic_atack_left;
	GetAbility(0)->fixture = game_object->CreateCollisionSensor(iPoint(-23, 11), 15, 5, fixture_type::f_t_attack);
	draw_offset.x = 8;
}

void Minion::BasicAttackRight()
{
	game_object->animator->SetAnimation("basic_attack_right");
	anim_state = basic_atack_right;
	GetAbility(0)->fixture = game_object->CreateCollisionSensor(iPoint(23, 11), 15, 5, fixture_type::f_t_attack);
	draw_offset.x = -8;
}

void Minion::FaceTarget()
{
	iPoint delta;
	iPoint pos = GetPos();
	iPoint target_pos = target->GetPos();

	delta.x = pos.x - target_pos.x;
	delta.y = pos.y - target_pos.y;

	iPoint hitbox_diff = target->game_object->GetHitBoxSize();
	hitbox_diff.x += game_object->GetHitBoxSize().x;
	hitbox_diff.y += game_object->GetHitBoxSize().y;

	hitbox_diff.x /= 2;
	hitbox_diff.y /= 2;

	delta -= hitbox_diff;

	
	
}

void Minion::Attack()
{
	switch (anim_state)
	{
	case run_up:
	case idle_up:
	case basic_atack_up:
		BasicAttackUp();
		break;
	case run_left:
	case idle_left:
	case basic_atack_left:
		BasicAttackLeft();
		break;
	case run_down:
	case idle_down:
	case basic_atack_down:
		BasicAttackDown();
		break;
	case run_right:
	case idle_right:
	case basic_atack_right:
		BasicAttackRight();
		break;
	default:
		break;
	}
}

void Minion::SetIdleAnim()
{
	switch (anim_state)
	{
	case run_up:
	case idle_up:
	case basic_atack_up:
		IdleUp();
		break;
	case run_left:
	case idle_left:
	case basic_atack_left:
		IdleLeft();
		break;
	case run_down:
	case idle_down:
	case basic_atack_down:
		IdleDown();
		break;
	case run_right:
	case idle_right:
	case basic_atack_right:
		IdleRight();
		break;
	default:
		break;
	}
}

void Minion::CheckNearestPathTile()
{
	int distance = INT_MAX, i = 0;
	iPoint map_pos = App->map->WorldToMap(GetPos().x, GetPos().y);
	for (; i < base_path.size(); ++i)
	{
		if (abs(base_path[i].x - map_pos.x) < distance)
			distance = abs(base_path[i].x - map_pos.x);
		else
			break;
	}

	base_path_index = i;
}
