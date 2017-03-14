#include "CharacterSelectionScene.h"
#include "j1Gui.h"
#include "j1Window.h"
#include "j1Input.h"
#include "j1Scene.h"

#define CHARACTER_NUM 3

CharacterSelectionScene::CharacterSelectionScene()
{
}

CharacterSelectionScene::~CharacterSelectionScene()
{
}

bool CharacterSelectionScene::Start()
{

	positions[0] = iPoint(0, 0);
	positions[1] = iPoint(0, 50);
	positions[2] = iPoint(0, 200);

	backgrounds_rects[0] = SDL_Rect{410, 26, 216, 261};
	backgrounds_rects[1] = SDL_Rect{ 807, 28, 124, 165};

	ganon_rects[0] = SDL_Rect{ 410, 26, 216, 261 };
	ganon_rects[1] = SDL_Rect{ 807, 28, 124, 165 };

	navi_rects[0] = SDL_Rect{ 410, 26, 216, 261 };
	navi_rects[1] = SDL_Rect{ 807, 28, 124, 165 };

	link_rects[0] = SDL_Rect{ 410, 26, 216, 261 };
	link_rects[1] = SDL_Rect{ 807, 28, 124, 165 };

	uint w, h; 
	App->win->GetWindowSize(w, h); 

	App->gui->UI_CreateWin(iPoint(0,0), w, h, 2, false); 

	card* ganon_card = new card; 
	ganon_card->entit_name = ganon;

	card* navi_card = new card;
	navi_card->entit_name = navi;

	card* link_card = new card;
	navi_card->entit_name = link;

	list_cards.push_back(ganon_card);
	list_cards.push_back(navi_card);
	list_cards.push_back(link_card);

	return true;
}

bool CharacterSelectionScene::PreUpdate()
{




	return true;
}

bool CharacterSelectionScene::Update(float dt)
{
	int i = 0; 

	for (list<card*>::iterator it = list_cards.begin(); it != list_cards.end() ; it++) 
	{
		if (i != 1)
		{
			(*it)->background = window->CreateImage(positions[i], backgrounds_rects[1], false);
		}
		else {
			(*it)->background = window->CreateImage(positions[i], backgrounds_rects[0], false);
		}

		switch ((*it)->entit_name)
		{

	
		case ganon:
		

			break;

		case link:
			break;

		case navi: 
			break; 
		}

		i++; 
	}


	return true;
}

bool CharacterSelectionScene::PostUpdate()
{
	return true;
}

bool CharacterSelectionScene::CleanUp()
{
	return true;
}

void CharacterSelectionScene::AddCard(card* new_card)
{
	list_cards.push_back(new_card); 
}
