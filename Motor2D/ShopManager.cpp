#include "ShopManager.h"
#include "j1Window.h"
#include "j1Gui.h"
#include "j1Entity.h"
#include "PlayerManager.h"
#include "j1Input.h"
#include "j1App.h"
#include "j1Scene.h"
#include "j1Map.h"
#include "j1XMLLoader.h"
#include "Animation.h"

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

	App->xml->LoadXML("Shop_config.xml", shop_config);
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

	App->xml->LoadXML("Items.xml", items_doc);
	file_node = items_doc.child("file");

	int item_num = 0;

	for (pugi::xml_node item = file_node.child("item"); item != NULL; item = item.next_sibling("item"))
	{
		item_info new_item;

		int power = item.child("power").attribute("value").as_int(0);
		int hp = item.child("hp").attribute("value").as_int(0);
		int speed = item.child("speed").attribute("value").as_int(0);
		int price = item.child("price").attribute("rupees").as_int(0);

		rect = { item.child("rect").attribute("x").as_int(),item.child("rect").attribute("y").as_int(),item.child("rect").attribute("w").as_int(),item.child("rect").attribute("h").as_int() };

		Item* upgrade_item = nullptr;

		new_item.item = new Item(item.child("name").child_value(), power, hp, speed, price, item.child("description").child_value(), rect, upgrade_item);
		for (int i = 0; i < 4; i++)
		{
			int x = (win_w / 4 - 180) + (i % 2)*win_w / 2 + (item_num % 2) * 45;
			int y = (win_h / 4 - 120) + (i / 2)*win_h / 2 + (item_num / 2) * 45;
			new_item.item_image = shop_window->CreateImage(iPoint(x, y), rect);
			shops[i]->items.push_back(new_item);
		}
		item_num++;
	}


	item_num = 0;
	for (pugi::xml_node item = file_node.child("item"); item != NULL; item = item.next_sibling("item"))
	{
		int upgrade_from = item.child("upgrade_from").attribute("id").as_int(-1);
		int upgrade = item.child("upgrade").attribute("id").as_int(-1);

		if (upgrade_from >= 0)
		{
			for (int i = 0; i < 4; i++)
			{
				shops[i]->items[item_num].item->upgrade_from = shops[i]->items[upgrade_from].item;
			}
		}


		if (upgrade >= 0)
		{
			for (int i = 0; i < 4; i++)
			{
				shops[i]->items[item_num].item->upgrade = shops[i]->items[upgrade].item;
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
	// -----

	rect = { shop_node.child("buy_icon").attribute("x").as_int(),shop_node.child("buy_icon").attribute("y").as_int(),shop_node.child("buy_icon").attribute("w").as_int(),shop_node.child("buy_icon").attribute("h").as_int() };

	// Set buy icon and price
	for (int i = 0; i < 4; i++)
	{
		int x = (win_w / 4 + 50) + (i % 2)*win_w / 2;
		int y = (win_h / 4 + 60) + (i / 2)*win_h / 2;

		shops[i]->buy_icon = shop_window->CreateImage(iPoint(x, y), rect);

		y += 20;
		x += 10;
		
		shops[i]->price = shop_window->CreateText(iPoint(x, y), App->font->game_font_small);
		string price = std::to_string(shops[i]->items[0].item->price);
		shops[i]->price->SetText(price);
	}
	// -----

	// Set Upgrade and Upgrade from images
	for (int i = 0; i < 4; i++)
	{
		int x = (win_w / 4 - 10) + (i % 2)*win_w / 2;
		int y = (win_h / 4) + (i / 2)*win_h / 2;

		shops[i]->upgrade_from_item = shop_window->CreateImage(iPoint(x, y), NULLRECT);

		x += 145;

		shops[i]->upgrade_item = shop_window->CreateImage(iPoint(x, y), NULLRECT);
	}
	// -----

	// Set Items name and description text
	for (int i = 0; i < 4; i++)
	{
		int x = (win_w / 4 - 30) + (i % 2)*win_w / 2;
		int y = (win_h / 4 - 130) + (i / 2)*win_h / 2;

		shops[i]->item_name = shop_window->CreateText(iPoint(x, y), App->font->game_font);
		shops[i]->item_name->SetText(shops[i]->items[0].item->name);

		y += 35;

		shops[i]->item_text = shop_window->CreateText(iPoint(x, y), App->font->game_font_small);
		shops[i]->item_text->SetText(shops[i]->items[0].item->description);
	}
	// -----

	// Set stats texts
	for (int i = 0; i < 4; i++)
	{
		int x = (win_w / 4 - 30) + (i % 2)*win_w / 2;
		int y = (win_h / 4 - 60) + (i / 2)*win_h / 2;

		shops[i]->power = shop_window->CreateText(iPoint(x, y), App->font->game_font_small);
		shops[i]->power->SetText("Power");

		x += 85;

		shops[i]->hp = shop_window->CreateText(iPoint(x, y), App->font->game_font_small);
		shops[i]->hp->SetText("HP");

		x += 75;

		shops[i]->speed = shop_window->CreateText(iPoint(x, y), App->font->game_font_small);
		shops[i]->speed->SetText("Speed");
	}
	// -----

	//Set stat numbers text
	for (int i = 0; i < 4; i++)
	{
		int x = (win_w / 4 + 14) + (i % 2)*win_w / 2;
		int y = (win_h / 4 - 60) + (i / 2)*win_h / 2;

		shops[i]->power_num = shop_window->CreateText(iPoint(x, y), App->font->game_font_small);
		string p = std::to_string(shops[i]->items[0].item->power);
		shops[i]->power_num->SetText(p);

		x += 65;

		shops[i]->hp_num = shop_window->CreateText(iPoint(x, y), App->font->game_font_small);
		string hp = std::to_string(shops[i]->items[0].item->hp);
		shops[i]->hp_num->SetText(hp);

		x += 99;

		shops[i]->speed_num = shop_window->CreateText(iPoint(x, y), App->font->game_font_small);
		string s = std::to_string(shops[i]->items[0].item->speed);
		shops[i]->speed_num->SetText(s);
	}
	// -----

	// Set Update text
	for (int i = 0; i < 4; i++)
	{
		int x = (win_w / 4 - 30) + (i % 2)*win_w / 2;
		int y = (win_h / 4 - 30) + (i / 2)*win_h / 2;

		shops[i]->upgrade_from = shop_window->CreateText(iPoint(x, y), App->font->game_font_small);
		shops[i]->upgrade_from->SetText("Upgrade from");
		shops[i]->upgrade_from->enabled = false;

		x += 160;

		shops[i]->upgrade = shop_window->CreateText(iPoint(x, y), App->font->game_font_small);
		shops[i]->upgrade->SetText("Upgrade");
		shops[i]->upgrade->enabled = false;
	}
	// -----

	//Player items images
	for (int i = 0; i < 4; i++)
	{
		int x = (win_w / 4 - 205) + (i % 2)*win_w / 2;
		int y = (win_h / 4 + 100) + (i / 2)*win_h / 2;

		for (int j = 0; j < 3; j++)
		{
			shops[i]->player_items[j] = shop_window->CreateImage(iPoint(x, y), { 0,0,0,0 });
			shops[i]->player_items[j]->enabled = false;
			x += 45;
		}
	}

	//Get shop positions
	team_shop[0] = App->map->GetShopPosition(1);
	team_shop[1] = App->map->GetShopPosition(2);

	//Shop icon
	for (int i = 0; i < 4; i++)
	{
		int x = (win_w / 4 - 16);
		int y = (win_h / 4 - win_h / 6) + (i / 2)*win_h / 2;

		shops[i]->shop_icon = shop_window->CreateImage(iPoint(x, y), { 0,0,0,0 });
	}

	shop_window->SetEnabledAndChilds(false);
	shop_window->enabled = true;

	shop_icon_anim = new Animator();
	shop_icon_anim->LoadAnimationsFromXML(shop_config, "animations");

	return true;
}

bool ShopManager::Update()
{
	for (std::vector<Player*>::iterator it = App->scene->main_scene->player_manager->players.begin(); it != App->scene->main_scene->player_manager->players.end(); it++)
	{
		if ((*it)->is_dead)
			return true;

		if (team_shop[(*it)->entity->GetTeam() - 1].DistanceTo((*it)->entity->GetPos()) < 200 && !shops[(*it)->viewport - 1]->active)
		{
			shops[(*it)->viewport - 1]->shop_icon->enabled = true;
			shops[(*it)->viewport - 1]->shop_icon->image = shop_icon_anim->GetCurrentAnimation()->GetAnimationFrame(App->GetDT());
		}
		else
		{
			if(shops[(*it)->viewport - 1]->shop_icon->enabled == true)
				shops[(*it)->viewport - 1]->shop_icon->enabled = false;
		}

		if (App->input->GetControllerButton((*it)->controller_index, SDL_CONTROLLER_BUTTON_X) == KEY_DOWN && team_shop[(*it)->entity->GetTeam() - 1].DistanceTo((*it)->entity->GetPos()) < 200)
		{
			ChangeShopState((*it)->viewport - 1);
		}

		if(shops[(*it)->viewport - 1]->active)
		{
			if (App->input->GetControllerButton((*it)->controller_index, SDL_CONTROLLER_BUTTON_DPAD_DOWN) == KEY_DOWN && shops[(*it)->viewport - 1]->selected_item < shops[(*it)->viewport - 1]->items.size() - 2 && !shops[(*it)->viewport - 1]->item_selected)
			{
				shops[(*it)->viewport - 1]->selected_item += 2;
				UpdateItemInfo((*it)->viewport - 1);
			}

			if (App->input->GetControllerButton((*it)->controller_index, SDL_CONTROLLER_BUTTON_DPAD_UP) == KEY_DOWN && shops[(*it)->viewport - 1]->selected_item > 1 && !shops[(*it)->viewport - 1]->item_selected)
			{
				shops[(*it)->viewport - 1]->selected_item -= 2;
				UpdateItemInfo((*it)->viewport - 1);
			}

			if (App->input->GetControllerButton((*it)->controller_index, SDL_CONTROLLER_BUTTON_DPAD_RIGHT) == KEY_DOWN&& shops[(*it)->viewport - 1]->selected_item < shops[(*it)->viewport - 1]->items.size() - 1 && !shops[(*it)->viewport - 1]->item_selected)
			{
				shops[(*it)->viewport - 1]->selected_item += 1;
				UpdateItemInfo((*it)->viewport - 1);
			}

			if (App->input->GetControllerButton((*it)->controller_index, SDL_CONTROLLER_BUTTON_DPAD_LEFT) == KEY_DOWN && shops[(*it)->viewport - 1]->selected_item > 0 && !shops[(*it)->viewport - 1]->item_selected)
			{
				shops[(*it)->viewport - 1]->selected_item -= 1;
				UpdateItemInfo((*it)->viewport - 1);
			}

			if (App->input->GetControllerButton((*it)->controller_index, SDL_CONTROLLER_BUTTON_A) == KEY_DOWN)
			{
				if (shops[(*it)->viewport - 1]->item_selected)
				{
					int final_price = shops[(*it)->viewport - 1]->items[shops[(*it)->viewport - 1]->selected_item].item->price;

					bool can_buy = false;

					//Check if there is a free item slot
					for (int i = 0; i < 3; i++)
					{
						if ((*it)->items[i] == nullptr)
						{
							can_buy = true;
							break;
						}
					}

					//Check if the item is an upgrade 
					if (shops[(*it)->viewport - 1]->items[shops[(*it)->viewport - 1]->selected_item].item->upgrade_from != nullptr)
					{
						for (int i = 0; i < 3; i++)
						{
							if ((*it)->items[i] == shops[(*it)->viewport - 1]->items[shops[(*it)->viewport - 1]->selected_item].item->upgrade_from)
							{
								final_price -= (*it)->items[i]->price;
								can_buy = true;
								break;
							}
						}
					}

					if (can_buy && (*it)->rupees >= final_price)
					{
						(*it)->BuyItem(shops[(*it)->viewport - 1]->items[shops[(*it)->viewport - 1]->selected_item].item, final_price);
						UpdatePlayerItems((*it)->viewport - 1, *it);
					}
					
					shops[(*it)->viewport - 1]->selected_item = 0;

					UpdateItemInfo((*it)->viewport - 1);

					shops[(*it)->viewport - 1]->item_selected = false;

				}
				else
				{
					shops[(*it)->viewport - 1]->item_selected = true;
					shops[(*it)->viewport - 1]->selector->SetPos(shops[(*it)->viewport - 1]->buy_icon->GetPos());
				}
			}

			if (App->input->GetControllerButton((*it)->controller_index, SDL_CONTROLLER_BUTTON_B) == KEY_DOWN)
			{
				if (shops[(*it)->viewport - 1]->item_selected)
				{
					shops[(*it)->viewport - 1]->item_selected = false;
					shops[(*it)->viewport - 1]->selector->SetPos(shops[(*it)->viewport - 1]->items[shops[(*it)->viewport - 1]->selected_item].item_image->GetPos());
				}
			}
		}

	}

	return true;
}

bool ShopManager::CleanUp()
{
	// Free UI
	if (App->scene->GetCurrentScene() != (Scene*)App->scene->main_scene)
		App->gui->DeleteElement(shop_window);
	// -------

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

void ShopManager::ChangeShopState(int view)
{
	shops[view]->background->enabled = !shops[view]->background->enabled;
	shops[view]->item_name->enabled = !shops[view]->item_name->enabled;
	shops[view]->item_text->enabled = !shops[view]->item_text->enabled;
	shops[view]->selector->enabled = !shops[view]->selector->enabled;
	shops[view]->upgrade_from_item->enabled = !shops[view]->upgrade_from_item->enabled;
	shops[view]->upgrade_item->enabled = !shops[view]->upgrade_item->enabled;
	shops[view]->price->enabled = !shops[view]->price->enabled;
	shops[view]->buy_icon->enabled = !shops[view]->buy_icon->enabled;
	shops[view]->power->enabled = !shops[view]->power->enabled;
	shops[view]->hp->enabled = !shops[view]->hp->enabled;
	shops[view]->speed->enabled = !shops[view]->speed->enabled;
	shops[view]->power_num->enabled = !shops[view]->power_num->enabled;
	shops[view]->hp_num->enabled = !shops[view]->hp_num->enabled;
	shops[view]->speed_num->enabled = !shops[view]->speed_num->enabled;

	for (std::vector<item_info>::iterator it = shops[view]->items.begin(); it != shops[view]->items.end(); it++)
	{
		it->item_image->enabled = !it->item_image->enabled;
	}

	for (int i = 0; i < 3; i++)
	{
		shops[view]->player_items[i]->enabled = !shops[view]->player_items[i]->enabled;
	}

	if (shops[view]->upgrade->enabled)
		shops[view]->upgrade->enabled = !shops[view]->upgrade->enabled;

	if (shops[view]->upgrade_from->enabled)
		shops[view]->upgrade_from->enabled = !shops[view]->upgrade_from->enabled;

	if (shops[view]->upgrade_item->enabled)
		shops[view]->upgrade_item->enabled = !shops[view]->upgrade_item->enabled;

	if (shops[view]->upgrade_from_item->enabled)
		shops[view]->upgrade_from_item->enabled = !shops[view]->upgrade_from_item->enabled;

	shops[view]->selected_item = 0;
	
	UpdateItemInfo(view);

	shops[view]->active = !shops[view]->active;
	shops[view]->item_selected = false;

	if (shops[view]->background->enabled == true)

		App->scene->main_scene->player_manager->DisableInput(view + 1);
	else
		App->scene->main_scene->player_manager->AllowInput(view + 1);
}

void ShopManager::UpdateItemInfo(int view)
{
	string text;
	text = std::to_string(shops[view]->items[shops[view]->selected_item].item->power);
	shops[view]->power_num->SetText(text);

	text = std::to_string(shops[view]->items[shops[view]->selected_item].item->hp);
	shops[view]->hp_num->SetText(text);

	text = std::to_string(shops[view]->items[shops[view]->selected_item].item->speed);
	shops[view]->speed_num->SetText(text);

	shops[view]->item_name->SetText(shops[view]->items[shops[view]->selected_item].item->name);

	shops[view]->item_text->SetText(shops[view]->items[shops[view]->selected_item].item->description);

	text = std::to_string(shops[view]->items[shops[view]->selected_item].item->price);
	shops[view]->price->SetText(text);

	if (shops[view]->items[shops[view]->selected_item].item->upgrade == nullptr)
	{
		shops[view]->upgrade->enabled = false;
		shops[view]->upgrade_item->enabled = false;
	}
	else
	{
		shops[view]->upgrade->enabled = true;
		shops[view]->upgrade_item->enabled = true;
		shops[view]->upgrade_item->image = shops[view]->items[shops[view]->selected_item].item->upgrade->image_rect;
	}

	if (shops[view]->items[shops[view]->selected_item].item->upgrade_from == nullptr)
	{
		shops[view]->upgrade_from->enabled = false;
		shops[view]->upgrade_from_item->enabled = false;
	}
	else
	{
		shops[view]->upgrade_from->enabled = true;
		shops[view]->upgrade_from_item->enabled = true;
		shops[view]->upgrade_from_item->image = shops[view]->items[shops[view]->selected_item].item->upgrade_from->image_rect;
	}

	shops[view]->selector->SetPos(shops[view]->items[shops[view]->selected_item].item_image->GetPos());

}

void ShopManager::UpdatePlayerItems(int view, Player * player)
{
	for (int i = 0; i < 3; i++)
	{
		if (player->items[i] == nullptr)
			break;

		shops[view]->player_items[i]->image = player->items[i]->image_rect;
	}
}
