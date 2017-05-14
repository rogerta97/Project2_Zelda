#include "Trees.h"
#include "GameObject.h"
#include "j1Viewports.h"
#include "j1FileSystem.h"
#include "j1XMLLoader.h"
#include "j1Map.h"
#include "j1Pathfinding.h"

Tree::Tree(iPoint pos)
{
	pugi::xml_document doc;
	App->xml->LoadXML("Trees.xml", doc);

	pugi::xml_node file = doc.child("file");

	iPoint w_h(file.child("rects").child("green").attribute("w").as_int(0), file.child("rects").child("green").attribute("h").as_int(0));

	game_object = new GameObject(iPoint(pos.x + 128/2, pos.y + 80), w_h, App->cf->CATEGORY_PAYLOAD_AREA, App->cf->MASK_SCENERY, pbody_type::p_t_tree, 0);

	game_object->SetTexture(game_object->LoadAnimationsFromXML(doc, "animations"));
	game_object->SetAnimation("green");

	int Tree_entity[22] = {
		10, 111,
		22, 120,
		18, 142,
		44, 138,
		64, 162,
		82, 133,
		111, 141,
		105, 119,
		122, 108,
		112, 85,
		15, 85
	};

	game_object->CreateCollision(iPoint(- 128/2 , - 80), Tree_entity, 22, fixture_type::f_t_aesthetics);

	game_object->SetFixedRotation(true); 
	game_object->SetKinematic(); 

	uchar* matrix = new uchar[20];
	uchar data[20] = {  1,1,1,1,
						0,0,0,0,
						0,0,0,0,
						0,0,0,0,
						0,0,0,0};

	memcpy_s(matrix, 20, data, 20);

	iPoint map_pos = App->map->WorldToMap(GetPos().x - 128/2, GetPos().y - 80);
	App->pathfinding->ChangeWalkability(matrix, map_pos.x, map_pos.y, 4, 5);

	RELEASE_ARRAY(matrix);

	name = "tree";
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

iPoint Tree::GetPos() const
{
	return game_object->GetPos();
}

Tree::~Tree()
{
}
