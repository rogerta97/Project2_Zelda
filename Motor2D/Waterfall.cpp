#include "Waterfall.h"
#include "j1XMLLoader.h"
#include "GameObject.h"
#include "j1Viewports.h"

Waterfall::Waterfall(iPoint pos)
{
	game_object = new GameObject(iPoint(pos.x, pos.y), iPoint(30, 40), App->cf->CATEGORY_PLAYER, App->cf->MASK_PLAYER, pbody_type::p_t_npc, 0);

	pugi::xml_document doc;
	App->xml->LoadXML("waterfall.xml", doc);

	game_object->SetTexture(game_object->LoadAnimationsFromXML(doc, "animations"));
	game_object->SetAnimation("idle");

	name = "waterfall";
}

Waterfall::~Waterfall()
{
}

bool Waterfall::Draw(float dt)
{
	App->view->LayerBlit(4, game_object->GetTexture(), { game_object->GetPos().x, game_object->GetPos().y }, game_object->GetCurrentAnimationRect(dt), 0, -1.0f, true, SDL_FLIP_NONE);
	return true;
}
