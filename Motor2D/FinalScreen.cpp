#include "FinalScreen.h"
#include "j1Console.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Window.h"
#include "j1Scene.h"
#include "j1Viewports.h"

bool FinalScreen::Start()
{
	uint w, h; 
	App->win->GetWindowSize(w,h);

	SDL_Rect screen = { 0,0,w,h };

	// Creating general elements ----

	window = App->gui->UI_CreateWin({0, 0}, screen.w, screen.h, 0, false);

	SDL_Rect background_title_rect = { 652, 526, 316, 69}; 
	iPoint background_title_rect_pos = { screen.w/2 - background_title_rect.w / 2, 20 };
	title_background = window->CreateImage(background_title_rect_pos, background_title_rect);

	title_text = window->CreateText({ background_title_rect_pos.x + 75, background_title_rect_pos.y + 13 }, App->font->game_font_20);
	title_text->SetText("GAME STATS");

	background_image = App->tex->LoadTexture("gui/intro_background.png");

	// ----

	//Creating player info 

	SDL_Rect placeholder_item = { 1321, 587, 45, 45 };
	SDL_Rect placeholder_character = { 1321, 587, 90,90 };

	// This need to be modified to adapt how we save characters data

	for (int i = 0; i < 4; i++)
	{
		final_screen_player_info new_info; 

		new_info.deaths = 0; 
		new_info.kills = 0;
		new_info.minions = 0;
		new_info.towers = 0;

		new_info.items[0] = placeholder_item;
		new_info.items[1] = placeholder_item;
		new_info.items[2] = placeholder_item;

		new_info.character_name = "link"; 

		player_info.push_back(new_info); 
	} 

	// ----


	// Creating player cards 

	int y_offset = 0; 
	int offset_increment = screen.h / 5; 

	SDL_Rect background_card_rect_team1 = { 1112, 208, 858, 122 };
	SDL_Rect background_card_rect_team2 = { 727, 364, 858, 122 };

	iPoint background_card_pos = { screen.w/2 - background_card_rect_team1.w/2, screen.h/6};

	iPoint kills_text_pos = { background_card_pos.x + 60, background_card_pos.y + 20 };
	iPoint deaths_text_pos = { background_card_pos.x + 190, background_card_pos.y + 20 };
	iPoint minions_text_pos = { background_card_pos.x + 340, background_card_pos.y + 20 };
	iPoint towers_text_pos = { background_card_pos.x + 490, background_card_pos.y + 20 };
	iPoint items_text_pos = { background_card_pos.x + 690, background_card_pos.y + 20 };

	iPoint item_1_image_pos = iPoint( items_text_pos.x - 62, items_text_pos.y + 35);
	iPoint item_2_image_pos = iPoint( items_text_pos.x + 7, items_text_pos.y + 35 );
	iPoint item_3_image_pos = iPoint( items_text_pos.x + 72, items_text_pos.y + 35);

	for (int i = 0; i < 4; i++)
	{
		final_screen_element new_card; 

		if(i < 2)
			new_card.rectangle_image = window->CreateImage({ background_card_pos.x, background_card_pos.y + y_offset }, background_card_rect_team1);
		else
			new_card.rectangle_image = window->CreateImage({ background_card_pos.x, background_card_pos.y + y_offset }, background_card_rect_team2);

		
		new_card.character_image = window->CreateImage({ background_card_pos.x - 130, background_card_pos.y + 15 + y_offset }, placeholder_character);

		new_card.kills_text = window->CreateText({ kills_text_pos.x, kills_text_pos.y + y_offset }, App->font->game_font);
		new_card.kills_text->SetText("KILLS");

		new_card.kills_num = window->CreateText({ kills_text_pos.x + 20, kills_text_pos.y + 35 + y_offset }, App->font->game_font_20);
		
		new_card.deaths_text = window->CreateText({ deaths_text_pos.x, deaths_text_pos.y + y_offset }, App->font->game_font);
		new_card.deaths_text->SetText("DEATHS");

		new_card.deaths_num = window->CreateText({ deaths_text_pos.x + 30, deaths_text_pos.y + 35 + y_offset }, App->font->game_font_20);
	
		new_card.minions_text = window->CreateText({ minions_text_pos.x, minions_text_pos.y + y_offset }, App->font->game_font);
		new_card.minions_text->SetText("MINIONS");

		new_card.minions_num = window->CreateText({ minions_text_pos.x + 35, minions_text_pos.y + 35 + y_offset }, App->font->game_font_20);
	
		new_card.towers_text = window->CreateText({ towers_text_pos.x, towers_text_pos.y + y_offset }, App->font->game_font);
		new_card.towers_text->SetText("TOWERS");

		new_card.towers_num = window->CreateText({ towers_text_pos.x + 35, towers_text_pos.y + 35 + y_offset }, App->font->game_font_20);
	
		new_card.items_text = window->CreateText({ items_text_pos.x, items_text_pos.y + y_offset }, App->font->game_font);
		new_card.items_text->SetText("ITEMS");
	
		new_card.item_images_1 = window->CreateImage({ item_1_image_pos.x, item_1_image_pos.y + y_offset }, placeholder_item);

		new_card.item_images_2 = window->CreateImage({ item_2_image_pos.x, item_2_image_pos.y + y_offset }, placeholder_item);

		new_card.item_images_3 = window->CreateImage({ item_3_image_pos.x, item_3_image_pos.y + y_offset }, placeholder_item);

		new_card.SetInfo(player_info[i]); 

		y_offset += offset_increment;
	}

	return false;
}

bool FinalScreen::Update(float dt)
{

	// Printing background
		App->view->LayerBlit(0, background_image, App->view->camera1, {0,0,1994, 1359}, 0,1);

		if (App->input->GetControllerButton(0, SDL_CONTROLLER_BUTTON_START) == KEY_DOWN)
			App->scene->ChangeScene((Scene*)App->scene->menu_scene); 

	return false;
}

bool FinalScreen::CleanUp()
{
	if(App->scene->GetCurrentScene() != App->scene->final_screen)
		App->gui->DeleteElement(window);

	return false;
}

void final_screen_element::SetInfo(final_screen_player_info player_info)
{

	SDL_Rect placeholder_character = { 1321, 587, 90, 90 };
	SDL_Rect placeholder_item = { 1321, 587, 45, 45 };

	string kills_num_str = to_string(player_info.kills); 
	string deaths_num_str = to_string(player_info.deaths);
	string minions_num_str = to_string(player_info.minions);
	string towers_num_str = to_string(player_info.towers);

	kills_num->SetText(kills_num_str);
	deaths_num->SetText(deaths_num_str);
	minions_num->SetText(minions_num_str);
	towers_num->SetText(towers_num_str);

	if (player_info.character_name == "link")
		character_image->image = placeholder_character;

	else if (player_info.character_name == "ganon")
		character_image->image = placeholder_character;

	else if (player_info.character_name == "navi")
		character_image->image = placeholder_character;

	item_images_1->image = player_info.items[0]; 
	item_images_2->image = player_info.items[1];
	item_images_3->image = player_info.items[2];
	
	
}
