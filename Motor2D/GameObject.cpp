#include "GameObject.h"
#include "j1Physics.h"
#include "Animation.h"
#include "p2Defs.h"
#include "Functions.h" 
#include "CollisionFilters.h"

GameObject::GameObject(iPoint _pos, iPoint _size, int _cat, int _mask, pbody_type pb_type, float _gravity_scale,  float _density, float _friction) : gravity_scale(_gravity_scale), density(_density), friction(_friction), cat(_cat), mask(_mask)
{
	animator = new Animator();
	pbody = App->physics->CreateCircleSensor(_pos.x, _pos.y, 2, _density, _gravity_scale, 0, cat, mask);
	pbody->body->SetType(b2_dynamicBody);
	pbody->body->SetSleepingAllowed(false);
	pbody->type = pb_type;
	hitbox_size = _size;
}

GameObject::~GameObject()
{
}

iPoint GameObject::GetPos()
{
	iPoint ret = NULLPOINT;

	pbody->GetPosition(ret.x, ret.y);

	return ret;
}

fPoint GameObject::fGetPos()
{
	fPoint ret = NULLPOINT;

	pbody->fGetPosition(ret.x, ret.y);

	return ret;
}

float GameObject::GetRotation()
{
	return pbody->GetRotation();
}

void GameObject::SetPos(fPoint new_pos)
{
	pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(new_pos.x), PIXEL_TO_METERS(new_pos.y)), pbody->body->GetAngle());
}

void GameObject::SetRotation(float angle)
{
	pbody->body->SetTransform(b2Vec2(pbody->body->GetPosition().x, pbody->body->GetPosition().y), DEGTORAD * angle);
}

void GameObject::SetHitBoxSize(uint w, uint h)
{
	hitbox_size.x = w; 
	hitbox_size.y = h; 
}

void GameObject::SetFixedRotation(bool set)
{
	pbody->body->SetFixedRotation(set);
}

void GameObject::SetDynamic()
{
	pbody->body->SetType(b2_dynamicBody);
}

void GameObject::SetKinematic()
{
	pbody->body->SetType(b2_kinematicBody);
}

void GameObject::SetGravityScale(float _gravity_scale)
{
	pbody->body->SetGravityScale(_gravity_scale);
	gravity_scale = _gravity_scale;
}

void GameObject::SetListener(j1Module * scene)
{
	pbody->listeners.push_back(scene);
}

void GameObject::DeleteListener(j1Module * scene)
{
	for (vector<j1Module*>::iterator it = pbody->listeners.begin(); it != pbody->listeners.end();)
	{
		if (scene = *it)
			it = pbody->listeners.erase(it);
		else
			++it;
	}
}

void GameObject::SetCatMask(int cat, int mask)
{
	b2Filter data;
	data.categoryBits = cat;
	data.maskBits = mask;
	data.groupIndex = pbody->body->GetFixtureList()->GetFilterData().groupIndex;

	for (b2Fixture* f = pbody->body->GetFixtureList(); f; f = f->GetNext())
	{
		f->SetFilterData(data);
	}
}

void GameObject::AddAnimation(Animation* animation)
{
	animator->AddAnimation(animation);
}

SDL_Texture* GameObject::LoadAnimationsFromXML(pugi::xml_document& doc, char* node_name)
{
	return animator->LoadAnimationsFromXML(doc, node_name);
}

void GameObject::SetAnimation(const char * animation)
{
	animator->SetAnimation(animation);
}

SDL_Rect GameObject::GetCurrentAnimationRect(float dt)
{
	Animation* current = animator->GetCurrentAnimation();

	if (current != nullptr)
		return current->GetAnimationFrame(dt);

	return NULLRECT;
}

b2Fixture* GameObject::CreateCollision(iPoint offset, int width, int height, fixture_type type)
{
	return App->physics->AddRectangleToBody(pbody, offset.x, offset.y, width, height, type, density, 0, friction);
}

b2Fixture* GameObject::CreateCollision(iPoint offset, int rad, fixture_type type)
{
	return App->physics->AddCircleToBody(pbody, offset.x, offset.y, rad, type, density, 0, friction);
}

b2Fixture* GameObject::CreateCollision(iPoint offset, iPoint dim, fixture_type type)
{
	return App->physics->AddRectangleToBody(pbody, offset.x, offset.y, dim.x, dim.y, type, density, 0, friction);
}

b2Fixture* GameObject::CreateCollision(iPoint offset, int* data, int size, fixture_type type)
{
	return App->physics->AddChainBody(pbody, offset.x, offset.y, data, size, type, density, 0, friction);
}

b2Fixture* GameObject::CreateCollisionSensor(iPoint offset, int width, int height, fixture_type type)
{
	return App->physics->AddRectangleSensorToBody(pbody, offset.x, offset.y, width, height, type, density, 0, friction);
}

b2Fixture* GameObject::CreateCollisionSensor(iPoint offset, int rad, fixture_type type)
{
	return App->physics->AddCircleSensorToBody(pbody, offset.x, offset.y, rad, type, density, 0, friction);
}

void GameObject::DeleteFixture(b2Fixture * fixture)
{
	App->physics->DeleteFixture(fixture, pbody);
}

void GameObject::SetTexture(SDL_Texture * _texture)
{
	if (_texture != nullptr)
		texture = _texture;
}

SDL_Texture * GameObject::GetTexture()
{
	return texture;
}

iPoint GameObject::GetHitBoxSize() const
{
	return hitbox_size;
}

void GameObject::CleanUp()
{
	App->physics->DeleteBody(pbody);
	animator->CleanUp();
	RELEASE(animator);
}




