#include "Eyes.h"
#include "GameObject.h"
#include "j1Entity.h"
#include "Scene.h"
#include "j1Scene.h"
#include "MainScene.h"
#include "j1Viewports.h"

Eyes::Eyes(iPoint pos)
{
	game_object = new GameObject(iPoint(pos.x, pos.y), iPoint(121, 145), App->cf->CATEGORY_NONCOLLISIONABLE, App->cf->MASK_NONCOLLISIONABLE, pbody_type::p_t_npc, 0);

	pugi::xml_document doc;
	App->LoadXML("eyes.xml", doc);
	game_object->SetTexture(game_object->LoadAnimationsFromXML(doc, "animations"));
	App->UnloadXML(doc);
}

Eyes::~Eyes()
{
}

bool Eyes::Start()
{
	game_object->SetAnimation("idle");
	return true;
}

bool Eyes::Draw(float dt)
{
	App->view->LayerBlit(3, game_object->GetTexture(), { game_object->GetPos().x, game_object->GetPos().y }, game_object->GetCurrentAnimationRect(dt), 0, -1.0f, true, SDL_FLIP_NONE);
	return true;
}