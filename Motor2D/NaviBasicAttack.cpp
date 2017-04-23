#include "NaviBasicAttack.h"
#include "GameObject.h"
#include "j1Viewports.h"
#include "p2Log.h"
#include "Entity.h"
#include "j1XMLLoader.h"

#define ROTATION_SPEED 300
#define DESTRUCTION_TIME 0.9f
#define SPEED 300
#define DEAD_ANIMATION_TIME 0.3f

NaviBasicAttack::NaviBasicAttack(iPoint pos)
{
	game_object = new GameObject(iPoint(pos.x, pos.y), iPoint(20, 20), App->cf->CATEGORY_ABILITIES, App->cf->MASK_ABILITIES, pbody_type::p_t_navi_basic_attack, 0);
	hit_box = game_object->CreateCollisionSensor(iPoint(0, 0), game_object->GetHitBoxSize().x, game_object->GetHitBoxSize().y, fixture_type::f_t_attack);
	game_object->SetListener((j1Module*)App->entity);
	game_object->SetListener((j1Module*)App->spell);
	game_object->SetFixedRotation(true);
	game_object->pbody->body->SetBullet(true);

	pugi::xml_document doc;
	App->xml->LoadXML("navibasicattack.xml", doc);
	game_object->SetTexture(game_object->LoadAnimationsFromXML(doc, "animations"));

	App->xml->LoadXML("navi.xml", doc);
	pugi::xml_node stats_node = doc.child("file").child("stats").child("basic_attack_spell");
	stats.damage_multiplicator = stats_node.attribute("mult").as_float(0.0f);

	draw_offset = restore_draw_offset = { 25, 25 };

	name = "navi_basic_attack";

	timer.Start();

	starting_pos = pos;
}

NaviBasicAttack::~NaviBasicAttack()
{
}

bool NaviBasicAttack::Start()
{
	bool ret = true;

	die = false;
	game_object->SetAnimation("basic_projectile");

	return ret;
}

bool NaviBasicAttack::PreUpdate()
{
	bool ret = true;



	return ret;
}

bool NaviBasicAttack::Update(float dt)
{
	bool ret = true;

	if (!die)
	{
		float speed = dt * SPEED;

		// Move
		switch (dir)
		{
		case n_b_a_direction::n_b_a_up:
			game_object->SetPos({ game_object->fGetPos().x, game_object->fGetPos().y - speed });
			break;
		case n_b_a_direction::n_b_a_down:
			game_object->SetPos({ game_object->fGetPos().x, game_object->fGetPos().y + speed });
			break;
		case n_b_a_direction::n_b_a_left:
			game_object->SetPos({ game_object->fGetPos().x - speed, game_object->fGetPos().y });
			break;
		case n_b_a_direction::n_b_a_right:
			game_object->SetPos({ game_object->fGetPos().x + speed, game_object->fGetPos().y });
			break;
		default:
			break;
		}
	}

	// Delete if life time over
	if (!die && timer.ReadSec() > DESTRUCTION_TIME)
	{
		die = true;
		timer.Start();
		game_object->SetAnimation("destroy");
		draw_offset = { 15, 15 };
		game_object->SetCatMask(App->cf->CATEGORY_NONCOLLISIONABLE, App->cf->MASK_NONCOLLISIONABLE);
	}

	// Delete entity if dead animation time over
	if(die && timer.ReadSec() > DEAD_ANIMATION_TIME)
		App->spell->DeleteSpell(this);

	return ret;
}

bool NaviBasicAttack::Draw(float dt)
{
	bool ret = true;

	rotation += ROTATION_SPEED*dt;

	App->view->LayerBlit(game_object->GetPos().y, game_object->GetTexture(), { game_object->GetPos().x - draw_offset.x - 3, game_object->GetPos().y - draw_offset.y }, game_object->GetCurrentAnimationRect(dt), 0, -1.0f, true, SDL_FLIP_HORIZONTAL, rotation);

	return ret;
}

bool NaviBasicAttack::PostUpdate()
{
	bool ret = true;



	return ret;
}

bool NaviBasicAttack::CleanUp()
{
	bool ret = true;

	return ret;
}

void NaviBasicAttack::CleanSpell()
{
}

void NaviBasicAttack::OnCollEnter(PhysBody * bodyA, PhysBody * bodyB, b2Fixture * fixtureA, b2Fixture * fixtureB)
{
	// Delete if hits another enemy entity
	if (fixtureB->type == fixture_type::f_t_hit_box && bodyB != game_object->pbody)
	{
		Entity* e = App->entity->FindEntityByBody(bodyB);

		if (e != nullptr && e->GetTeam() != owner->GetTeam())
		{
			die = true;
			timer.Start();
			game_object->SetAnimation("destroy");
			draw_offset = { 15, 15 };
			game_object->SetCatMask(App->cf->CATEGORY_NONCOLLISIONABLE, App->cf->MASK_NONCOLLISIONABLE);
		}
	}
}

void NaviBasicAttack::Set(n_b_a_direction _dir)
{
	dir = _dir;
}

void NaviBasicAttack::Effects(Entity * entity, Ability * ability)
{

}
