#include "j1XMLLoader.h"
#include "Bushes.h"
#include "GameObject.h"
#include "j1Viewports.h"
#include "PlayerManager.h"
#include "j1FileSystem.h"

Bush::Bush(iPoint _pos)
{
	pugi::xml_document doc;
	App->xml->LoadXML("Bush.xml", doc);

	pugi::xml_node file = doc.child("file");

	iPoint w_h(file.child("coords").child("rect").attribute("w").as_int(0), file.child("coords").child("rect").attribute("h").as_int(0));

	game_object = new GameObject(iPoint(_pos.x, _pos.y), w_h, App->cf->CATEGORY_SCENERY, App->cf->MASK_SCENERY, pbody_type::p_t_bush, 0);

	game_object->SetTexture(game_object->LoadAnimationsFromXML(doc, "animations"));
	game_object->SetListener(App->entity); 

	name = "bush";
}

bool Bush::Draw(float dt)
{
	int offset = 0; 

	if (is_middle == false)
		offset = 5; 

	App->view->LayerBlit(game_object->fGetPos().y, game_object->GetTexture(), iPoint(game_object->GetPos().x, game_object->GetPos().y + offset), game_object->GetCurrentAnimationRect(dt));
	
	return true;
}

void Bush::SetBushColor(const char * color)
{
	game_object->SetAnimation(color);

	if (color == "green_half" || color == "purple_half")
		game_object->CreateCollisionSensor(iPoint(16, 10), 32, 18, fixture_type::f_t_aesthetics); 
}



void Bush::SetMiddle(bush_color type)
{
	if (type == green_bush || type == purple_bush)
		is_middle = false;

	else
		is_middle = true;

}

void Bush::OnCollEnter(PhysBody * bodyA, PhysBody * bodyB, b2Fixture * fixtureA, b2Fixture * fixtureB)
{
	if (bodyA == game_object->pbody && fixtureA->type == fixture_type::f_t_aesthetics) 
	{
		if (fixtureB->type == fixture_type::f_t_hit_box)
		{
			if(game_object->animator->IsCurrentAnimation("green_half"))
			{
				game_object->SetAnimation("green_move"); 
			}
			else if(game_object->animator->IsCurrentAnimation("purple_half"))
			{
				game_object->SetAnimation("purple_move");
			}
		}
	}
		
}

void Bush::OnCollOut(PhysBody * bodyA, PhysBody * bodyB, b2Fixture * fixtureA, b2Fixture * fixtureB)
{
	if (bodyA == game_object->pbody && fixtureA->type == fixture_type::f_t_aesthetics)
	{
		if (fixtureB->type == fixture_type::f_t_hit_box)
		{
			if (game_object->animator->IsCurrentAnimation("green_move"))
			{
				game_object->SetAnimation("green_half");
			}
			else if (game_object->animator->IsCurrentAnimation("purple_move"))
			{
				game_object->SetAnimation("purple_half");
			}
		}
	}

}

bool Bush::CleanUp()
{
	return true;
}

Bush::~Bush()
{
}
