#include "FinalScreen.h"
#include "j1Console.h"
#include "j1Textures.h"
#include "j1Viewports.h"

bool FinalScreen::Start()
{
	
	SDL_Rect screen = App->view->GetViewportRect(1);

	// Creating general elements ----

	window = App->gui->UI_CreateWin({0, 0}, screen.w, screen.h, 0, false);

	SDL_Rect background_title_rect = { 652, 526, 316, 69}; 
	iPoint background_title_rect_pos = { screen.w - background_title_rect.w / 2, 20 };
	title_background = window->CreateImage(background_title_rect_pos, background_title_rect);

	title_text = window->CreateText({ background_title_rect_pos.x + 75, background_title_rect_pos.y + 13 }, App->font->game_font_20);
	title_text->SetText("GAME STATS");

	background_image = App->tex->LoadTexture("gui/intro_background.png");

	// ----

	// Creating player cards 

	int y_offset = 0; 

	SDL_Rect background_card_rect = {727, 364, 858, 122}; 
	iPoint background_card_pos = { screen.w - background_card_rect.w/2, 115};

	iPoint kills_text_pos = { background_card_pos.x + 60, background_card_pos.y + 20 };
	iPoint deaths_text_pos = { background_card_pos.x + 190, background_card_pos.y + 20 };
	iPoint minions_text_pos = { background_card_pos.x + 340, background_card_pos.y + 20 };
	iPoint towers_text_pos = { background_card_pos.x + 490, background_card_pos.y + 20 };
	iPoint items_text_pos = { background_card_pos.x + 690, background_card_pos.y + 20 };

	for (int i = 0; i < 4; i++)
	{
		final_screen_element new_card; 

		new_card.rectangle_image = window->CreateImage({ background_card_pos.x, background_card_pos.y + y_offset}, background_card_rect);

		new_card.kills_text = window->CreateText({ kills_text_pos.x, kills_text_pos.y + y_offset }, App->font->game_font);
		new_card.kills_text->SetText("KILLS");

		new_card.deaths_text = window->CreateText({ deaths_text_pos.x, deaths_text_pos.y + y_offset }, App->font->game_font);
		new_card.deaths_text->SetText("DEATHS");

		new_card.minions_text = window->CreateText({ minions_text_pos.x, minions_text_pos.y + y_offset }, App->font->game_font);
		new_card.minions_text->SetText("MINIONS");

		new_card.towers_text = window->CreateText({ towers_text_pos.x, towers_text_pos.y + y_offset }, App->font->game_font);
		new_card.towers_text->SetText("TOWERS");

		new_card.items_text = window->CreateText({ items_text_pos.x, items_text_pos.y + y_offset }, App->font->game_font);
		new_card.items_text->SetText("ITEMS");

		y_offset += 150;
	}

	return false;
}

bool FinalScreen::Update()
{


	// Printing background
		App->render->Blit(background_image, 0,0);

	return false;
}

bool FinalScreen::CleanUp()
{
	return false;
}

void final_screen_element::SetInfo(final_screen_player_info player_info)
{



}
