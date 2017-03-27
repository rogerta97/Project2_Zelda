#include "Trees.h"
#include "GameObject.h"
#include "j1Viewports.h"
#include "j1FileSystem.h"

Tree::Tree(iPoint pos)
{
	pugi::xml_document doc;
	App->LoadXML("Trees.xml", doc);

	pugi::xml_node file = doc.child("file");

	iPoint w_h(file.child("rects").child("green").attribute("w").as_int(0), file.child("rects").child("green").attribute("h").as_int(0));

	game_object = new GameObject(iPoint(pos.x + 128/2, pos.y + 80), w_h, App->cf->CATEGORY_NONCOLLISIONABLE, App->cf->MASK_SCENERY, pbody_type::p_t_tree, 0);

	game_object->SetTexture(game_object->LoadAnimationsFromXML(doc, "animations"));
	game_object->SetAnimation("green");
}

bool Tree::Draw(float dt)
{
	App->view->LayerBlit(game_object->fGetPos().y, game_object->GetTexture(), iPoint(game_object->GetPos().x - 128/2, game_object->GetPos().y - 80), game_object->GetCurrentAnimationRect(dt));
	
	return true; 
}

bool Tree::CleanUp()
{
	return false;
}

void Tree::SetTreeColor(const char * color)
{
		game_object->SetAnimation(color);
	
}

Tree::~Tree()
{
}