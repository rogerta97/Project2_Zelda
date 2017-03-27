#include "Trees.h"
#include "GameObject.h"
#include "j1Viewports.h"
#include "j1FileSystem.h"

Tree::Tree(iPoint pos)
{
	pugi::xml_document doc;
	App->LoadXML("Tree.xml", doc);

	pugi::xml_node file = doc.child("file");

	iPoint w_h(file.child("rects").child("green").attribute("w").as_int(0), file.child("rects").child("green").attribute("h").as_int(0));

	game_object = new GameObject(iPoint(pos.x + 10, pos.y), w_h, App->cf->CATEGORY_NONCOLLISIONABLE, App->cf->MASK_SCENERY, pbody_type::p_t_tree, 0);

	game_object->SetTexture(App->tex->LoadTexture(file.child("texture").attribute("name").as_string()));
}

bool Tree::Draw(float dt)
{
	
	
	return true; 
}

bool Tree::CleanUp()
{
	return false;
}

Tree::~Tree()
{
}
