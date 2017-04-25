#include "Mapping.h"
#include "j1Input.h"
#include "SDL/include/SDL.h"

Mapping::Mapping()
{
	mapping = new key_mapping[m_k_null];
	
	//Set initial keys ids.
	mapping[m_k_ability1].key_id = SDL_CONTROLLER_BUTTON_RIGHTSHOULDER;

	mapping[m_k_ability2].key_id = RIGHT_TRIGGER;
	mapping[m_k_ability2].is_button = false;

	mapping[m_k_ability3].key_id = SDL_CONTROLLER_BUTTON_LEFTSHOULDER;

	mapping[m_k_ability4].key_id = LEFT_TRIGGER;
	mapping[m_k_ability4].is_button = false;

	mapping[m_k_back].key_id = SDL_CONTROLLER_BUTTON_B;

	mapping[m_k_confirm].key_id = SDL_CONTROLLER_BUTTON_A;

	mapping[m_k_minimap].key_id = SDL_CONTROLLER_BUTTON_Y;

	mapping[m_k_shop].key_id = SDL_CONTROLLER_BUTTON_X;
}

Mapping::~Mapping()
{
} 

void Mapping::MapKey(map_keys function, int key_id)
{
}

void Mapping::GetKey(map_keys function)
{
}
