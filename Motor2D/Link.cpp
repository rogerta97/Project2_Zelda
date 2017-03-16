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


Link::Link(iPoint pos)
{
	game_object = new GameObject(iPoint(pos.x, pos.y), iPoint(30,40), App->cf->CATEGORY_PLAYER, App->cf->MASK_PLAYER, pbody_type::p_t_link, 0);

	game_object->CreateCollision(iPoint(0, 0), game_object->GetHitBoxSize().x, game_object->GetHitBoxSize().y, fixture_type::f_t_hit_box);
	game_object->SetListener((j1Module*)App->entity);
	game_object->SetFixedRotation(true);

	AddAbility(0, 10, 1, 2);
	AddAbility(1, 15, 1, 2);
	AddAbility(2, 10, 1, 2, "boomerang"); // Name references to the Spell name
	AddAbility(3, 10, 1, 2);

	pugi::xml_document doc;
	App->LoadXML("link.xml", doc);
	game_object->SetTexture(game_object->LoadAnimationsFromXML(doc, "animations"));
	App->UnloadXML(doc);

	draw_offset = restore_draw_offset = { 16, 26 }; // 

	blit_layer = 2;
}

Link::~Link()
{
}

bool Link::Start()
{
	bool ret = true;

	game_object->SetAnimation("idle_down");

	can_move = true;
	stats.speed = stats.restore_speed = 160;

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

	App->view->CenterCamera(camera, game_object->GetPos().x, game_object->GetPos().y);

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
				DealDamage(ability->damage * (spell->stats.damage_multiplicator - 1));

				if (spell->stats.slow_duration > 0)
					Slow(spell->stats.slow_multiplicator, spell->stats.slow_duration);
			}
		}

		// Friendly attacks
		else
		{
			if (spell != nullptr && TextCmp(spell->name.c_str(), "boomerang"))
			{
				if (spell->owner == this)
				{
					if(spell->can_delete)
						App->spell->DeleteSpell(spell);
				}
			}
		}
	}

	LifeBar(iPoint(60, 5), iPoint(-25, -40));

	return ret;
}

bool Link::Draw(float dt)
{
	bool ret = true;
	
	// Blit
	if(flip)
		App->view->LayerBlit(blit_layer, game_object->GetTexture(), { game_object->GetPos().x - draw_offset.x - 3, game_object->GetPos().y - draw_offset.y}, game_object->GetCurrentAnimationRect(dt), 0, -1.0f, true, SDL_FLIP_HORIZONTAL);
	else
		App->view->LayerBlit(blit_layer, game_object->GetTexture(), { game_object->GetPos().x - draw_offset.x, game_object->GetPos().y - draw_offset.y}, game_object->GetCurrentAnimationRect(dt), 0, -1.0f, true, SDL_FLIP_NONE);


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
	int main_view = App->entity->player_manager->GetEntityViewportIfIsPlayer(this);
	App->view->LayerDrawQuad({ game_object->GetPos().x - 13, game_object->GetPos().y - 55, 10, 35 }, 51, 153, 255, 100, true, blit_layer - 1, main_view, true);
}

void Link::ShowBasicAttackDown()
{
	int main_view = App->entity->player_manager->GetEntityViewportIfIsPlayer(this);
	App->view->LayerDrawQuad({ game_object->GetPos().x + 5, game_object->GetPos().y + 20, 10, 35 }, 51, 153, 255, 100, true, blit_layer - 1, main_view, true);
}

void Link::ShowBasicAttackLeft()
{
	int main_view = App->entity->player_manager->GetEntityViewportIfIsPlayer(this);
	App->view->LayerDrawQuad({ game_object->GetPos().x -50 , game_object->GetPos().y - 4, 35, 10 }, 51, 153, 255, 100, true, blit_layer - 1, main_view, true);
}

void Link::ShowBasicAttackRight()
{
	int main_view = App->entity->player_manager->GetEntityViewportIfIsPlayer(this);
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
	int main_view = App->entity->player_manager->GetEntityViewportIfIsPlayer(this);
	App->view->LayerDrawCircle( game_object->GetPos().x, game_object->GetPos().y, 50, 51, 153, 255, 100, blit_layer - 1, main_view, true);
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
	int main_view = App->entity->player_manager->GetEntityViewportIfIsPlayer(this);
	App->view->LayerDrawQuad({ game_object->GetPos().x - 12, game_object->GetPos().y - 85, 25, (int)(BOOMERANG_RANGE * 0.5) }, 51, 153, 255, 100, true, blit_layer - 1, main_view, true);
	App->view->LayerDrawQuad({ game_object->GetPos().x - 12, game_object->GetPos().y - 85 - (int)(BOOMERANG_RANGE * 0.5), 25, (int)(BOOMERANG_RANGE * 0.5) }, 201, 153, 255, 100, true, blit_layer - 1, main_view, true);
}

void Link::ShowAbility2Down()
{
	int main_view = App->entity->player_manager->GetEntityViewportIfIsPlayer(this);
	App->view->LayerDrawQuad({ game_object->GetPos().x - 12, game_object->GetPos().y + 15, 25, (int)(BOOMERANG_RANGE * 0.5) }, 51, 153, 255, 100, true, blit_layer - 1, main_view, true);
	App->view->LayerDrawQuad({ game_object->GetPos().x - 12, game_object->GetPos().y + 15 + (int)(BOOMERANG_RANGE * 0.5), 25, (int)(BOOMERANG_RANGE * 0.5) }, 201, 153, 255, 100, true, blit_layer - 1, main_view, true);

}

void Link::ShowAbility2Left()
{

	int main_view = App->entity->player_manager->GetEntityViewportIfIsPlayer(this);
	App->view->LayerDrawQuad({ game_object->GetPos().x - 75, game_object->GetPos().y + 12, (int)(BOOMERANG_RANGE * 0.5), -25}, 51, 153, 255, 100, true, blit_layer - 1, main_view, true);
	App->view->LayerDrawQuad({ game_object->GetPos().x - 75 - (int)(BOOMERANG_RANGE * 0.5), game_object->GetPos().y + 12, (int)(BOOMERANG_RANGE * 0.5), -25 }, 201, 153, 255, 100, true, blit_layer - 1, main_view, true);
}

void Link::ShowAbility2Right()
{
	int main_view = App->entity->player_manager->GetEntityViewportIfIsPlayer(this);
	App->view->LayerDrawQuad({ game_object->GetPos().x + 12, game_object->GetPos().y + 12, (int)(BOOMERANG_RANGE * 0.5), -25 }, 51, 153, 255, 100, true, blit_layer - 1, main_view, true);
	App->view->LayerDrawQuad({ game_object->GetPos().x + 12 + (int)(BOOMERANG_RANGE * 0.5), game_object->GetPos().y + 12, (int)(BOOMERANG_RANGE * 0.5), -25 }, 201, 153, 255, 100, true, blit_layer - 1, main_view, true);
}

void Link::Ability3Up()
{
}

void Link::Ability3Down()
{
}

void Link::Ability3Left()
{
}

void Link::Ability3Right()
{
}

void Link::ShowAbility3Up()
{
}

void Link::ShowAbility3Down()
{
}

void Link::ShowAbility3Left()
{
}

void Link::ShowAbility3Right()
{
}

void Link::OnColl(PhysBody* bodyA, PhysBody * bodyB, b2Fixture * fixtureA, b2Fixture * fixtureB)
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
