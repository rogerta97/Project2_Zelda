#pragma once
#ifndef _CHARACTERSELECTION_H_
#define _CHARACTERSELECTION_H_

#include "Scene.h"
#include "j1Entity.h"
#include <vector>

class UI_Image;
class UI_Text;
class UI_Window;




struct char_select_view
{	
	vector<UI_Image*>   char_images;
	vector<UI_Image*>	back_images; 
	UI_Image*			name_background = nullptr; 
	UI_Text*			name = nullptr; 
	UI_Button*			character_info = nullptr; 
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

	UI_Window* window = nullptr; 


private:

	char_select_view viewport[4]; 

	// Ganon
	SDL_Rect		ganon_rects[2];

	// Navi
	SDL_Rect		navi_rects[2];

	// Link
	SDL_Rect		link_rects[2];

	// Background
	SDL_Rect		backgrounds_rects[2];

	vector<iPoint>			positions;

public:

private:



};


#endif