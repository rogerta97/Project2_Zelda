#include "Spell.h"
#include "GameObject.h"

void Spell::CleanSpell()
{
	game_object->CleanUp();
	RELEASE(game_object);
}
