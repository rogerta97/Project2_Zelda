#pragma once
#ifndef _CHARACTERSELECTION_H_
#define _CHARACTERSELECTION_H_
//
#include "Scene.h"
#include "j1Entity.h"
#include "j1Gui.h"
#include "j1App.h"
#include "j1Viewports.h"

//#include "j1Entity.h"
//#include <vector>
//
//class UI_Image;
//class UI_Text;
//class UI_Window;
//
//struct info_window
//{
//	entity_name			name = e_n_null; 
//	vector<UI_Image*>	habilites; 
//	vector<UI_Text*>	description;
//
//	void Reset() { name = e_n_null; habilites.clear(); description.clear(); }
//};
//
//struct char_select_view
//{	
//	vector<UI_Image*>   char_images;
//
//	UI_Image*			name_background = nullptr; 
//	UI_Text*			name = nullptr; 
//	UI_Image*			info_button = nullptr; 
//	UI_Image*			info_back = nullptr;
//	UI_Text*			ready_text = false;
//
//	void Reset() { char_images.clear(); name_background = nullptr; name = nullptr; info_button = nullptr; info_back = nullptr; ready_text = false;}
//};
//
//struct character_info
//{
//	entity_name	 character = e_n_null; 
//	string		 name; 
//	void Reset() { character = e_n_null; name = ""; }
//};
//
//
//class CharacterSelectionScene : public Scene
//{
//public:
//	CharacterSelectionScene();
//
//	~CharacterSelectionScene();
//
//	bool Start();
//	bool PreUpdate();
//	bool Update(float dt);
//	bool PostUpdate();
//	bool CleanUp();
//
//	void CreateScene(uint w, uint h); 
//	void MoveCard(int pad, const char* direction); 
//	void EnableInfo(entity_name character, int viewport_num, uint w, uint h);
//	void DisableInfo(entity_name character, int viewport_num);
//	bool AllReady();
//
//	UI_Window*		window = nullptr; 
//
//private:
//	void DrawScreenSeparation();
//
//private:
//	int					 player_viewports[4];
//	char_select_view	 viewport[4]; 
//
//	list<info_window>	 info_container[4]; 
//
//	list<character_info> char_view[4]; 
//
//
//	// Ganon
//	SDL_Rect			 ganon_rects[2];
//
//	// Navi
//	SDL_Rect			 navi_rects[2];
//
//	// Link
//	SDL_Rect			 link_rects[2];
//
//	// Background
//	SDL_Rect			 backgrounds_rects[2];
//
//	vector<iPoint>		 positions;
//
//	bool				 player_ready[4] = {false, false, false, false};
//	bool				 change_scene = false; 
//
//	uint				 win_w = 0; 
//	uint				 win_h = 0; 
//
//public:
//
//private:
//
//
//
//};
//
//

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
		iPoint big_image_pos = { (int)(view.w*0.5), (int)(view.h*0.5) };
		iPoint small_image_left_pos = {};
		iPoint small_image_right_pos = {};

		iPoint button_info_pos = {};
		iPoint background_info_image_pos = {};

		iPoint background_name_image_pos = {};
		iPoint text_name_pos = {};

		iPoint abilities_info1 = {};
		iPoint abilities_info2 = {};
		iPoint abilities_info3 = {};
		iPoint abilities_info4 = {};

		window = App->gui->UI_CreateWin(iPoint(0, 0), view.w, view.h, 1, true, false, true);
		window->viewport = viewport;

		big_image = window->CreateImage(big_image_pos, { 0, 0, 0, 0 });
		small_image_left = window->CreateImage(small_image_left_pos, { 0, 0, 0, 0 });
		small_image_right = window->CreateImage(small_image_right_pos, { 0, 0, 0, 0 });
		button_info = window->CreateImage(button_info_pos, { 0, 0, 0, 0 });

	};
	~viewport_data() {};

	int viewport = 0;
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
public:
private:
	vector<viewport_data> viewports_data;
	vector<player_data*>  players_data;

	player_data* link = nullptr;

	player_data* curr_player_data1 = nullptr;
	player_data* curr_player_data2 = nullptr;
	player_data* curr_player_data3 = nullptr;
	player_data* curr_player_data4 = nullptr;
};

#endif
