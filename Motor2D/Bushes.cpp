
#include "Bushes.h"
#include "GameObject.h"
#include "j1Viewports.h"
#include "PlayerManager.h"
#include "j1FileSystem.h"

Bush::Bush(iPoint _pos)
{
	pugi::xml_document doc;
	App->LoadXML("Bush.xml", doc);

	pugi::xml_node file = doc.child("file");

	iPoint w_h(file.child("coords").child("rect").attribute("w").as_int(0), file.child("coords").child("rect").attribute("h").as_int(0));

	game_object = new GameObject(iPoint(_pos.x + 10, _pos.y), w_h, App->cf->CATEGORY_NONCOLLISIONABLE, App->cf->MASK_SCENERY, pbody_type::p_t_bush, 0);

	game_object->SetTexture(game_object->LoadAnimationsFromXML(doc, "animations"));
	game_object->SetAnimation("green");
}


bool Bush::Draw(float dt)
{

		App->view->LayerBlit(game_object->fGetPos().y, game_object->GetTexture(), iPoint(game_object->GetPos().x - 10, game_object->GetPos().y + 13), game_object->GetCurrentAnimationRect(dt));

		//App->view->LayerBlit(1, game_object->GetTexture(), iPoint(game_object->GetPos().x - 10, game_object->GetPos().y + 13), game_object->GetCurrentAnimationRect(dt));

	return false;
}

void Bush::SetBushColor(const char * color)
{
	game_object->SetAnimation(color);
}

bool Bush::IsInside()
{
	

	return false;
}

bool Bush::CleanUp()
{

	return false;
}

Bush::~Bush()
{

}
