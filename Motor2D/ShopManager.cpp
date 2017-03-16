#include "ShopManager.h"
#include "j1Window.h"
#include "j1Gui.h"

ShopManager::ShopManager()
{
}

ShopManager::~ShopManager()
{
}

bool ShopManager::Start()
{
	// Create shops for each viewport
	for (int i = 0; i < 4; i++)
	{
		shops[i] = new shop();
	}
	// -----

	uint win_w, win_h;
	App->win->GetWindowSize(win_w, win_h);

	//Load shop_config.xml
	pugi::xml_document shop_config;
	pugi::xml_node shop_node;

	App->LoadXML("Shop_config.xml", shop_config);
	shop_node = shop_config.child("shop");
	// -----

	//Create window
	shop_window = App->gui->UI_CreateWin(iPoint(0, 0), win_w, win_h, 0, false);
	// -----

	//Set shop background images to each viewport
	SDL_Rect rect = { shop_node.child("background").attribute("x").as_int(),shop_node.child("background").attribute("y").as_int(),shop_node.child("background").attribute("w").as_int(),shop_node.child("background").attribute("h").as_int() };

	for (int i = 0; i < 4; i++)
	{
		int x = (win_w / 4 - rect.w / 2) + (i % 2)*win_w / 2;
		int y = (win_h / 4 - rect.h / 2) + (i / 2)*win_h / 2;
		shops[i]->background = shop_window->CreateImage(iPoint(x, y), rect);
	}
	// -----

	//Load all items
	pugi::xml_document items_doc;
	pugi::xml_node file_node;

	App->LoadXML("Items.xml", items_doc);
	file_node = items_doc.child("file");

	int item_num = 0;

	for (pugi::xml_node item = file_node.child("item"); item != NULL; item = item.next_sibling("item"))
	{
		item_info new_item;

		int power = item.child("power").attribute("value").as_int(0);
		int hp = item.child("hp").attribute("value").as_int(0);
		int speed = item.child("speed").attribute("value").as_int(0);
		int price = item.child("price").attribute("rupees").as_int(0);
		int upgrade = item.child("upgrade").attribute("id").as_int(-1);

		rect = { item.child("rect").attribute("x").as_int(),item.child("rect").attribute("y").as_int(),item.child("rect").attribute("w").as_int(),item.child("rect").attribute("h").as_int() };

		Item* upgrade_item = nullptr;

		if (upgrade >= 0)
		{
			upgrade_item = shops[0]->items.at(upgrade).item;
		}

		new_item.item = new Item(item.child("name").child_value(), power, hp, speed, price, item.child("description").child_value(), rect, upgrade_item);
		for (int i = 0; i < 4; i++)
		{
			int x = (win_w / 4 - 180) + (i % 2)*win_w / 2 + (item_num % 2) * 45;
			int y = (win_h / 4 - 90) + (i / 2)*win_h / 2 + (item_num / 2) * 45;
			new_item.item_image = shop_window->CreateImage(iPoint(x, y), rect);
			shops[i]->items.push_back(new_item);
		}
		item_num++;
	}


	item_num = 0;
	for (pugi::xml_node item = file_node.child("item"); item != NULL; item = item.next_sibling("item"))
	{
		int upgrade_from = item.child("upgrade_from").attribute("id").as_int(-1);

		if (upgrade_from >= 0)
		{
			for (int i = 0; i < 4; i++)
			{
				shops[i]->items[item_num].item->upgrade_from = shops[i]->items[upgrade_from].item;
			}
		}
		item_num++;
	}
	// -----

	// Set Selector 
	rect = { shop_node.child("selector").attribute("x").as_int(),shop_node.child("selector").attribute("y").as_int(),shop_node.child("selector").attribute("w").as_int(),shop_node.child("selector").attribute("h").as_int() };

	for (int i = 0; i < 4; i++)
	{
		shops[i]->selector = shop_window->CreateImage(shops[i]->items.begin()->item_image->GetPos(), rect);
	}

	// Set info
	for (int i = 0; i < 4; i++)
	{

	}

	return true;
}

bool ShopManager::Update()
{
	return true;
}

bool ShopManager::CleanUp()
{
	App->gui->DeleteElement(shop_window);

	//Release items
	for (std::vector<item_info>::iterator it = shops[0]->items.begin(); it != shops[0]->items.end(); it++)
	{
		RELEASE(it->item);
	}

	//Release shops memory
	for (int i = 0; i < 4; i++)
	{
		RELEASE(shops[i]);
	}
	return true;
}
