#ifndef _ITEM_H_
#define _ITEM_H_

#include "j1Render.h"

class Item
{
public:
	Item(const char* name, int power = 0, int hp = 0, int speed = 0, int price = 0, const char* description = nullptr, SDL_Rect image_rect = { 0,0,0,0 }, Item* upgrade = nullptr) :
		name(name), power(power), hp(hp), speed(speed), price(price), description(description), image_rect(image_rect), upgrade(upgrade)	{	}

private:

public:
	string		name;
	int			power = 0;
	int			hp = 0;
	int			speed = 0;
	int			price = 0;

	string		description;

	SDL_Rect	image_rect = NULLRECT;

	Item*		upgrade = nullptr;
	Item*		upgrade_from = nullptr;

private:

};

#endif // !_ITEM_H_

