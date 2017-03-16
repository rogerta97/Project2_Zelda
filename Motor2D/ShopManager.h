#ifndef _SHOPMANAGER_H_
#define _SHOPMANAGER_H_

#include <vector>
#include  "Item.h"

class UI_Image;
class UI_Text;
class UI_Window;

struct item_info
{
	Item*		item;
	UI_Image*	item_image;
};

struct shop
{
	UI_Image*			background;
	vector<item_info>	items;

	UI_Text*			power;
	UI_Text*			hp;
	UI_Text*			speed;
	UI_Text*			upgrade;

	UI_Text*			power_num;
	UI_Text*			hp_num;
	UI_Text*			speed_num;

	UI_Image*			upgrade_item;
};

class ShopManager
{
public:
	ShopManager();
	~ShopManager();

	bool Start();

	bool Update();

	bool CleanUp();

private:

public:

private:
	UI_Window*		shop_window = nullptr;

	shop*			shops[4] = { nullptr,nullptr,nullptr };


};

#endif // !_SHOPMANAGER_H_

