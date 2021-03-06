#ifndef _SHOPMANAGER_H_
#define _SHOPMANAGER_H_

#include <vector>
#include  "Item.h"

class UI_Image;
class UI_Text;
class UI_Window;
class Player;
class Animator;

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
	UI_Text*			item_name = nullptr;
	UI_Text*			price = nullptr;

	UI_Text*			power_num = nullptr;
	UI_Text*			hp_num = nullptr;
	UI_Text*			speed_num = nullptr;

	UI_Image*			upgrade_item = nullptr;
	UI_Image*			upgrade_from_item = nullptr;

	UI_Image*			player_items[3] = { nullptr,nullptr,nullptr };

	UI_Image*			selector = nullptr;
	int					selected_item = 0;

	UI_Image*			buy_icon = nullptr;
	bool				item_selected = false;

	bool				active = false;

	UI_Image*			shop_icon = nullptr;

	Animator*	     	shop_icon_anim = nullptr;
}; 

class ShopManager
{
public:
	ShopManager();
	~ShopManager();

	bool Start();
	bool Update();
	bool CleanUp();

	SDL_Rect GetPlayerItem(int player_index, int item_index);
	void UpdatePlayerItems(int view, Player* player);
	bool IsActive(int viewport);

private:
	void ChangeShopState(int view, Player* player = nullptr);
	void UpdateItemInfo(int view, Player* player = nullptr);
	
public:
	UI_Window*		shop_window = nullptr;
	iPoint			team_shop[2] = { NULLPOINT,NULLPOINT };

private:
	shop*			shops[4] = { nullptr,nullptr,nullptr };
};

#endif // !_SHOPMANAGER_H_

