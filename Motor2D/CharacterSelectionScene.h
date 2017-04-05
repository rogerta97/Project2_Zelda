#pragma once
#ifndef _CHARACTERSELECTION_H_
#define _CHARACTERSELECTION_H_
//
#include "Scene.h"
#include "j1Entity.h"
#include "j1Gui.h"
#include "j1App.h"
#include "j1Viewports.h"

struct player_data
{
	player_data() {};
	~player_data() {};

	string      name;

	SDL_Rect    big_image = NULLRECT;
	SDL_Rect    small_image = NULLRECT;

	string      ability1_text;
	string      ability2_text;
	string      ability3_text;
	string      ability4_text;

	entity_name entity = entity_name::e_n_null;
};

struct viewport_data
{
	viewport_data() {};

	viewport_data(int viewport)
	{
		SDL_Rect view = App->view->GetViewportRect(1);

		iPoint big_image_pos = { view.w/2 - 50, view.h / 2 - 120 };
		iPoint small_image_left_pos = { big_image_pos.x - 120, big_image_pos.y + 30};
		iPoint small_image_right_pos = { big_image_pos.x + 116 + 120 - 74, big_image_pos.y + 30};

		iPoint button_info_pos = {view.w - 120, view.h - 65};
		SDL_Rect button_info_rect = { 324, 195, 95, 40 };

		iPoint background_info_image_pos = {view.w / 15, view.h / 15 };
		SDL_Rect background_info_image_rect = { 656, 595, 470, 325 };

		iPoint background_name_image_pos = {view.w / 2 - 100, view.h - ( view.h / 3)};
		SDL_Rect background_name_image_rect = { 128, 52,  217, 55 };

		iPoint text_name_pos = { background_name_image_pos.x + 85, background_name_image_pos.y + 10};

		iPoint abilities_info1_pos = { background_info_image_pos.x + 25, background_info_image_pos.y + 30 };
		iPoint abilities_info2_pos = { background_info_image_pos.x + 25, background_info_image_pos.y + 90 };
		iPoint abilities_info3_pos = { background_info_image_pos.x + 25, background_info_image_pos.y + 150 };
		iPoint abilities_info4_pos = { background_info_image_pos.x + 25, background_info_image_pos.y + 210 };

		iPoint ready_text_pos = { view.w/3 + 40, view.h - 80}; 

		window = App->gui->UI_CreateWin(iPoint(0, 0), view.w, view.h, 1, true, false, true);
		window->viewport = viewport;

		big_image = window->CreateImage(big_image_pos, { 0, 0, 0, 0 });
		small_image_left = window->CreateImage(small_image_left_pos, { 0, 0, 0, 0 });
		small_image_right = window->CreateImage(small_image_right_pos, { 0, 0, 0, 0 });

		button_info = window->CreateImage(button_info_pos, button_info_rect);

		background_name_image = window->CreateImage(background_name_image_pos, background_name_image_rect);
		text_name = window->CreateText(text_name_pos, App->font->game_font);

		ready_text = window->CreateText(ready_text_pos, App->font->game_font, 30);
		ready_text->SetText("Press START when ready");

		background_info_image = window->CreateImage(background_info_image_pos, background_info_image_rect);
		background_info_image->enabled = false;

		abilities_info1 = window->CreateText(abilities_info1_pos, App->font->game_font, 25);
		abilities_info1->enabled = false;
		abilities_info2 = window->CreateText(abilities_info2_pos, App->font->game_font, 25);
		abilities_info2->enabled = false;
		abilities_info3 = window->CreateText(abilities_info3_pos, App->font->game_font, 25);
		abilities_info3->enabled = false;
		abilities_info4 = window->CreateText(abilities_info4_pos, App->font->game_font, 25);
		abilities_info4->enabled = false;
	};

	~viewport_data() {};

	int viewport = 0;
	bool is_ready = false; 

	UI_Window* window = nullptr;
	UI_Image* big_image = nullptr;
	UI_Image* small_image_left = nullptr;
	UI_Image* small_image_right = nullptr;

	UI_Image* button_info = nullptr;
	UI_Image* background_info_image = nullptr;

	UI_Image* background_name_image = nullptr;
	UI_Text* text_name = nullptr;

	UI_Text* abilities_info1 = nullptr;
	UI_Text* abilities_info2 = nullptr;
	UI_Text* abilities_info3 = nullptr;
	UI_Text* abilities_info4 = nullptr;

	UI_Text* ready_text = nullptr; 

	bool SetViewportInfo(player_data* player); 
};

class CharacterSelectionScene : public Scene
{
public:
	CharacterSelectionScene();
	~CharacterSelectionScene();
	bool Start();
	bool Update(float dt);
	bool CleanUp();

private:
	player_data* MoveCharacterLeft(player_data* data, int viewport);
	player_data* MoveCharacterRight(player_data* data, int viewport);
	void SetDataToViewport(player_data* data, int viewport);
	int GetIndexByPlayerData(player_data* data);

	void DrawScreenSeparation();

public:

	player_data* link = nullptr;
	player_data* ganon = nullptr;
	player_data* navi = nullptr;

private:
	vector<viewport_data> viewports_data;
	vector<player_data*> players_data;

	player_data* curr_player_data1 = nullptr;
	player_data* curr_player_data2 = nullptr;
	player_data* curr_player_data3 = nullptr;
	player_data* curr_player_data4 = nullptr;
};

#endif
