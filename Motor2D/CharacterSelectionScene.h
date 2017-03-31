#pragma once
#ifndef _CHARACTERSELECTION_H_
#define _CHARACTERSELECTION_H_

#include "Scene.h"
#include "j1Entity.h"
#include <vector>

class UI_Image;
class UI_Text;
class UI_Window;

struct info_window
{
	entity_name			name = e_n_null; 
	vector<UI_Image*>	habilites; 
	vector<UI_Text*>	description;

	void Reset() { name = e_n_null; habilites.clear(); description.clear(); }
};

struct char_select_view
{	
	vector<UI_Image*>   char_images;

	UI_Image*			name_background = nullptr; 
	UI_Text*			name = nullptr; 
	UI_Image*			info_button = nullptr; 
	UI_Image*			info_back = nullptr;
	UI_Text*			ready_text = false;

	void Reset() { char_images.clear(); name_background = nullptr; name = nullptr; info_button = nullptr; info_back = nullptr; ready_text = false;}
};

struct character_info
{
	entity_name	 character = e_n_null; 
	string		 name; 
	void Reset() { character = e_n_null; name = ""; }
};


class CharacterSelectionScene : public Scene
{
public:
	CharacterSelectionScene();

	~CharacterSelectionScene();

	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	void CreateScene(uint w, uint h); 
	void MoveCard(int pad, const char* direction); 
	void EnableInfo(entity_name character, int viewport_num, uint w, uint h);
	void DisableInfo(entity_name character, int viewport_num);
	bool AllReady();

	UI_Window*		window = nullptr; 

private:
	void DrawScreenSeparation();

private:
	int					 player_viewports[4];
	char_select_view	 viewport[4]; 

	list<info_window>	 info_container[4]; 

	list<character_info> char_view[4]; 


	// Ganon
	SDL_Rect			 ganon_rects[2];

	// Navi
	SDL_Rect			 navi_rects[2];

	// Link
	SDL_Rect			 link_rects[2];

	// Background
	SDL_Rect			 backgrounds_rects[2];

	vector<iPoint>		 positions;

	bool				 player_ready[4] = {false, false, false, false};
	bool				 change_scene = false; 



public:

private:



};


#endif