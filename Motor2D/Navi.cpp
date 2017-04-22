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

#define BASIC_ATTACK_RANGE 100
#define ABILITY1_RANGE 100

#define ABILITY1_DURATION 5
#define ABILITY3_DURATION 6

Navi::Navi(iPoint pos)
{
	game_object = new GameObject(iPoint(pos.x, pos.y), iPoint(30, 40), App->cf->CATEGORY_PLAYER, App->cf->MASK_PLAYER, pbody_type::p_t_player, 0);

	game_object->CreateCollisionSensor(iPoint(0, 0), game_object->GetHitBoxSize().x, game_object->GetHitBoxSize().y, fixture_type::f_t_hit_box);
	game_object->CreateCollision(iPoint(2, 10), 13, fixture_type::f_t_collision_box);
	game_object->SetListener((j1Module*)App->entity);
	game_object->SetFixedRotation(true);
	game_object->pbody->body->SetBullet(true);

	pugi::xml_document doc;
	App->xml->LoadXML("navi.xml", doc);

	// Loading Abilities ----------------
	pugi::xml_node stats_node = doc.child("file").child("stats");
	rupee_reward = stats_node.attribute("rupees").as_int();

	stats.life = stats.base_hp = stats.max_life = stats_node.attribute("hp").as_int();
	stats.base_power = stats.power = stats_node.attribute("power").as_int();
	stats.base_speed = stats.speed = stats.restore_speed = stats_node.attribute("speed").as_int();

	float dmg_mult = stats_node.child("ability1").attribute("mult").as_float();
	float cd = stats_node.child("ability1").attribute("cd").as_float();
	int bd = stats_node.child("ability1").attribute("bd").as_int();
	Ability* a1 = AddAbility(0, cd, bd, dmg_mult, "navi_basic_attack");
	a1->SetImages({ 481, 0, 80, 48 }, { 561, 0, 80, 48 }, { 481, 244, 80, 48 });

	dmg_mult = stats_node.child("ability2").attribute("mult").as_float();
	cd = stats_node.child("ability2").attribute("cd").as_float();
	bd = stats_node.child("ability2").attribute("bd").as_int();
	Ability* a2 = AddAbility(1, cd, bd, dmg_mult);
	a2->SetImages({ 481, 48, 80, 48 }, { 561, 48, 80, 48 }, { 481, 292, 80, 48 });

	dmg_mult = stats_node.child("ability3").attribute("mult").as_float();
	cd = stats_node.child("ability3").attribute("cd").as_float();
	bd = stats_node.child("ability3").attribute("bd").as_int();
	Ability* a3 = AddAbility(2, cd, bd, dmg_mult);
	a3->SetImages({ 481, 96, 48, 73 }, { 529, 96, 48, 73 }, { 481, 341, 48, 73 });

	dmg_mult = stats_node.child("ability4").attribute("mult").as_float();
	cd = stats_node.child("ability4").attribute("cd").as_float();
	bd = stats_node.child("ability4").attribute("bd").as_int();
	Ability* a4 = AddAbility(3, cd, bd, dmg_mult);
	a4->SetImages({ 481, 170, 48, 73 }, { 529, 170, 48, 73 }, { 529, 341, 48, 73 });
	// -------------------------------------

	game_object->SetTexture(game_object->LoadAnimationsFromXML(doc, "animations"));

	draw_offset = restore_draw_offset = { 23, 26 };

	blit_layer = 2;

	name = "navi";
}

Navi::~Navi()
{
}

bool Navi::Start()
{
	bool ret = true;

	game_object->SetAnimation("down");

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
				DealDamage((entity->stats.power * spell->stats.damage_multiplicator) + ability->damage); // Spells control their own damage mutiplicator

				spell->Effects(entity, ability);
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

	LifeBar(iPoint(60, 5), iPoint(-29, -40));

	// Basic atack --------------------
	if (ability1)
	{
		if (ability1_timer.ReadSec() < ABILITY1_DURATION)
		{
			App->view->LayerBlit(game_object->GetPos().y - 1, game_object->GetTexture(), { GetPos().x - 140, GetPos().y - 140 }, game_object->animator->GetAnimation("heal_area")->GetAnimationFrame(dt), 0);
		}
		else
		{
			ability1 = false;
		}
	}
	// Ability 3 ----------------------
	if (ability3)
	{
		if (ability3_timer.ReadSec() < ABILITY3_DURATION)
		{
			// Get enemy team
			int enemy_team = 0;
			if (GetTeam() == 1)
				enemy_team = 2;
			else
				enemy_team = 1;

			vector<int> view = App->scene->main_scene->player_manager->GetTeamViewports(enemy_team);
			SDL_Rect rect = App->view->GetViewportRect(1);
			for (int i = 0; i < view.size(); i++)
			{
				App->view->LayerBlit(9999, game_object->GetTexture(), { (int)(rect.w*0.5f) - 180, (int)(rect.h*0.5f) - 150 }, game_object->animator->GetAnimation("ulti_letters")->GetAnimationFrame(dt), view.at(i), -1.0f, false);
			}
		}
		else
		{
			ability3 = false;
		}
	}
	// -------------------------------

	return ret;
}

bool Navi::Draw(float dt)
{
	bool ret = true;

	// Blit
	if (flip)
		App->view->LayerBlit(GetPos().y, game_object->GetTexture(), { game_object->GetPos().x - draw_offset.x - 3, game_object->GetPos().y - draw_offset.y }, game_object->GetCurrentAnimationRect(dt), 0, -1.0f, true, SDL_FLIP_HORIZONTAL);
	else
		App->view->LayerBlit(GetPos().y, game_object->GetTexture(), { game_object->GetPos().x - draw_offset.x, game_object->GetPos().y - draw_offset.y }, game_object->GetCurrentAnimationRect(dt), 0, -1.0f, true, SDL_FLIP_NONE);


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
		game_object->SetAnimation("up");
		flip = false;
	}
}

void Navi::RunDown()
{
	if (can_move)
	{
		game_object->SetAnimation("down");
		flip = false;
	}
}

void Navi::RunLeft()
{
	if (can_move)
	{
		game_object->SetAnimation("lateral");
		flip = true;
	}
}

void Navi::RunRight()
{
	if (can_move)
	{
		game_object->SetAnimation("lateral");
		flip = false;
	}
}

void Navi::IdleUp()
{
	if (can_move)
	{
		game_object->SetAnimation("up");
		flip = false;
	}
}

void Navi::IdleDown()
{
	if (can_move)
	{
		game_object->SetAnimation("down");
		flip = false;
	}
}

void Navi::IdleLeft()
{
	if (can_move)
	{
		game_object->SetAnimation("lateral");
		flip = true;
	}
}

void Navi::IdleRight()
{
	if (can_move)
	{
		game_object->SetAnimation("lateral");
		flip = false;
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
	App->view->LayerDrawQuad({ game_object->GetPos().x - 12, game_object->GetPos().y - 85, 25, (int)(BASIC_ATTACK_RANGE) }, 51, 153, 255, 100, true, blit_layer - 1, main_view, true);
}

void Navi::ShowBasicAttackDown()
{
	int main_view = App->scene->main_scene->player_manager->GetEntityViewportIfIsPlayer(this);
	App->view->LayerDrawQuad({ game_object->GetPos().x - 12, game_object->GetPos().y + 15, 25, (int)(BASIC_ATTACK_RANGE) }, 51, 153, 255, 100, true, blit_layer - 1, main_view, true);
}

void Navi::ShowBasicAttackLeft()
{
	int main_view = App->scene->main_scene->player_manager->GetEntityViewportIfIsPlayer(this);
	App->view->LayerDrawQuad({ game_object->GetPos().x + 12, game_object->GetPos().y + 12, (int)(-BASIC_ATTACK_RANGE), -25 }, 51, 153, 255, 100, true, blit_layer - 1, main_view, true);
}

void Navi::ShowBasicAttackRight()
{
	int main_view = App->scene->main_scene->player_manager->GetEntityViewportIfIsPlayer(this);
	App->view->LayerDrawQuad({ game_object->GetPos().x + 12, game_object->GetPos().y + 12, (int)(BASIC_ATTACK_RANGE), -25 }, 51, 153, 255, 100, true, blit_layer - 1, main_view, true);
}

void Navi::Ability1Up()
{
	ability1 = true;
	ability1_timer.Start();
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
	int main_view = App->scene->main_scene->player_manager->GetEntityViewportIfIsPlayer(this);
	App->view->LayerDrawCircle(game_object->GetPos().x, game_object->GetPos().y, ABILITY1_RANGE, 51, 153, 255, 100, blit_layer - 1, main_view, false, true);
}

void Navi::ShowAbility1Down()
{
	ShowAbility1Up();
}

void Navi::ShowAbility1Left()
{
	ShowAbility1Up();
}

void Navi::ShowAbility1Right()
{
	ShowAbility1Up();
}

void Navi::Ability2Up()
{

}

void Navi::Ability2Down()
{

}

void Navi::Ability2Left()
{

}

void Navi::Ability2Right()
{

}

void Navi::ShowAbility2Up()
{
	
}

void Navi::ShowAbility2Down()
{
	
}

void Navi::ShowAbility2Left()
{
	
}

void Navi::ShowAbility2Right()
{
	
}

void Navi::Ability3Up()
{
	ability3 = true;
	ability3_timer.Start();
}

void Navi::Ability3Down()
{
	Ability3Up();
}

void Navi::Ability3Left()
{
	Ability3Up();
}

void Navi::Ability3Right()
{
	Ability3Up();
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
