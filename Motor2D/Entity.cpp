#include "Entity.h"
#include "j1Viewports.h"
#include "GameObject.h"
#include "p2Log.h"

#define LIFE_BAR_WIDTH 60;
#define LIFE_BAR_HEIGHT 10;
#define LIFE_BAR_COLOR_1 {30, 30, 30, 255}

bool Entity::GotHit(Entity *& entity, Ability *& ability)
{
	if (hit)
	{
		entity = hit_by;
		ability = hit_ability;
		hit_by = nullptr;
		hit_ability = nullptr;
		hit = false;

		return true;
	}
	return false;
}

void Entity::LifeBar(iPoint size, iPoint offset)
{
	if (show_life_bar && game_object != nullptr)
	{
		SDL_Rect rect = { game_object->GetPos().x + offset.x, game_object->GetPos().y + offset.y, size.x, size.y };

		App->view->LayerDrawQuad(rect, 30, 30, 30, 255, true, 10, 0, true);
	}
}

