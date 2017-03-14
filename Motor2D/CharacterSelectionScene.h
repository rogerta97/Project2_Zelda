#pragma once
#ifndef _CHARACTERSELECTION_H_
#define _CHARACTERSELECTION_H_

#include "Scene.h"
#include "j1Entity.h"
#include <vector>

class UI_Image;
class UI_Text;
class UI_Window;



struct card 
{
	entity_name	 entit_name = e_n_null;

	UI_Image*	 image = nullptr;
	UI_Image*	 background = nullptr;	
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

	void AddCard(card* new_card); 

	UI_Window* window = nullptr; 


private:

	//first position is the big one and second is the little one

	// Ganon
	SDL_Rect		ganon_rects[2];

	// Navi
	SDL_Rect		navi_rects[2];

	// Link
	SDL_Rect		link_rects[2];

	// Background
	SDL_Rect		backgrounds_rects[2];

	list<card*>		list_cards; 

	// Position of the cards respectively 
	iPoint			positions[3]; 

public:

private:



};


#endif