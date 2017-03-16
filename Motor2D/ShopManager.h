#ifndef _SHOPMANAGER_H_
#define _SHOPMANAGER_H_

#include <vector>
#include  "Item.h"

class UI_Image;
class UI_Text;
class UI_Window;

struct item_info
{
	Item*		item = nullptr;
	UI_Image*	item_image = nullptr;
};

struct shop
{
	UI_Image*			background = nullptr;
	vector<item_info>	items;

	UI_Text*			power = nullptr;
	UI_Text*			hp = nullptr;
	UI_Text*			speed = nullptr;
	UI_Text*			upgrade = nullptr;
	UI_Text*			upgrade_from = nullptr;
	UI_Text*			item_text = nullptr;
	UI_Image*			rupee = nullptr;
	UI_Text*			price = nullptr;

	UI_Text*			power_num = nullptr;
	UI_Text*			hp_num = nullptr;
	UI_Text*			speed_num = nullptr;

	UI_Image*			upgrade_item = nullptr;
	UI_Image*			upgrade_from_item = nullptr;

	UI_Text*			shop_title = nullptr;

	UI_Image*			selector = nullptr;
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

