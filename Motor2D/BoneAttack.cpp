#include "BoneAttack.h"
#include "GameObject.h"
#include "j1Viewports.h"
#include "Functions.h"
#include "p2Log.h"
#include "Entity.h"
#include "j1XMLLoader.h"

#define DESTRUCTION_TIME 1.5f
#define SPEED 230

BoneAttack::BoneAttack(iPoint pos)
{
	game_object = new GameObject(iPoint(pos.x, pos.y), iPoint(20, 20), App->cf->CATEGORY_ABILITIES, App->cf->MASK_ABILITIES, pbody_type::p_t_boomerang, 0);
	hit_box = game_object->CreateCollisionSensor(iPoint(0, 0), game_object->GetHitBoxSize().x, game_object->GetHitBoxSize().y, fixture_type::f_t_attack);
	game_object->SetListener((j1Module*)App->entity);
	game_object->SetFixedRotation(true);
	game_object->pbody->body->SetBullet(true);

	pugi::xml_document doc;
	App->xml->LoadXML("boneattack.xml", doc);
	game_object->SetTexture(game_object->LoadAnimationsFromXML(doc, "animations"));

	draw_offset = restore_draw_offset = { 7, 9 };

	name = "bone";

	timer.Start();

	starting_pos = pos;
}

BoneAttack::~BoneAttack()
{
}

bool BoneAttack::Start()
{
	bool ret = true;

	game_object->SetAnimation("spin");


	return ret;
}

bool BoneAttack::PreUpdate()
{
	bool ret = true;



	return ret;
}

bool BoneAttack::Update(float dt)
{
	bool ret = true;

	int speed = SPEED*dt;

	game_object->SetPos({ game_object->fGetPos().x + (speed * cos(DEGTORAD * angle)), game_object->fGetPos().y + (speed * sin(DEGTORAD * angle))});
		
	
	if (timer.ReadSec() > DESTRUCTION_TIME)
		App->spell->DeleteSpell(this);

	return ret;
}

bool BoneAttack::Draw(float dt)
{
	bool ret = true;

	App->view->LayerBlit(1, game_object->GetTexture(), { game_object->GetPos().x - draw_offset.x - 3, game_object->GetPos().y - draw_offset.y }, game_object->GetCurrentAnimationRect(dt), 0, -1.0f, true, SDL_FLIP_HORIZONTAL);

	return ret;
}

bool BoneAttack::PostUpdate()
{
	bool ret = true;


	return ret;
}

bool BoneAttack::CleanUp()
{
	bool ret = true;

	return ret;
}

void BoneAttack::SetAngle(int _angle)
{
	angle = _angle;
}

void BoneAttack::CleanSpell()
{
}

void BoneAttack::OnColl(PhysBody * bodyA, PhysBody * bodyB, b2Fixture * fixtureA, b2Fixture * fixtureB)
{

}