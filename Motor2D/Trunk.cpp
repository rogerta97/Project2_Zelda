
#include "Trunk.h"
#include "GameObject.h"
#include "j1Viewports.h"
#include "j1FileSystem.h"

Trunk::Trunk(iPoint _pos)
{
	pugi::xml_document doc;
	App->LoadXML("trunk.xml", doc);

	pugi::xml_node file = doc.child("file");

	iPoint w_h(file.child("coords").child("rect").attribute("w").as_int(0), file.child("coords").child("rect").attribute("h").as_int(0));

	game_object = new GameObject(iPoint(_pos.x + 10, _pos.y), w_h , App->cf->CATEGORY_NONCOLLISIONABLE, App->cf->MASK_SCENERY, pbody_type::p_t_trunk, 0);

	game_object->SetTexture(game_object->LoadAnimationsFromXML(doc, "animations"));
	game_object->SetAnimation("main"); 
}


bool Trunk::Draw(float dt)
{
	App->view->LayerBlit(2200, game_object->GetTexture(), game_object->GetPos(), game_object->GetCurrentAnimationRect(dt));

	return true;
}



bool Trunk::CleanUp()
{
	return false;
}

Trunk::~Trunk()
{

}
