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
	uint w, h; 
	App->win->GetWindowSize(w, h); 

	window = App->gui->UI_CreateWin(iPoint(0,0), w, h, 2, false); 

	CreateScene(w,h); 

	return true;
}

bool CharacterSelectionScene::PreUpdate()
{

	return true;
}

bool CharacterSelectionScene::Update(float dt)
{



	return true;
}

bool CharacterSelectionScene::PostUpdate()
{
	return true;
}

bool CharacterSelectionScene::CleanUp()
{
	if (App->scene->GetCurrentScene() != App->scene->charselect_screen)
		App->gui->DeleteElement(window);

	return true;
}



void CharacterSelectionScene::CreateScene(uint w, uint h)
{
	//Seting the position of the cards -------------------------------

	// viewport 1 ---
	positions.push_back(iPoint(100, 100));
	positions.push_back(iPoint(230, 70));
	positions.push_back(iPoint(425, 100));

	positions.push_back(iPoint(positions[0].x + 12, positions[0].y + 10));
	positions.push_back(iPoint(positions[1].x + 20, positions[1].y + 20));
	positions.push_back(iPoint(positions[2].x + 12, positions[2].y + 10));

	positions.push_back(iPoint(w/4 - 120, 220));

	positions.push_back(iPoint(positions[6].x + 90, positions[6].y + 10));
	positions.push_back(iPoint(400, 300));

	//---
	// viewport 2 ---

	positions.push_back(iPoint(w/2 + 100, 100));
	positions.push_back(iPoint(w / 2 + 230, 70));
	positions.push_back(iPoint(w / 2 + 425, 100));

	positions.push_back(iPoint(w / 2 + positions[0].x + 12, positions[0].y + 10));
	positions.push_back(iPoint(w / 2 + positions[1].x + 20, positions[1].y + 20));
	positions.push_back(iPoint(w / 2 + positions[2].x + 12, positions[2].y + 10));

	positions.push_back(iPoint(w / 2 + (w / 4 - 120), 220));

	positions.push_back(iPoint(w / 2 + positions[6].x + 90, positions[6].y + 10));
	positions.push_back(iPoint(w / 2 + 400, 300));

	//---

	//viewport 3 ---

	positions.push_back(iPoint( 100, h / 2 + 100));
	positions.push_back(iPoint( 230, h / 2 + 70));
	positions.push_back(iPoint( 425, h / 2 + 100));

	positions.push_back(iPoint( positions[0].x + 12, h / 2 + positions[0].y + 10));
	positions.push_back(iPoint( positions[1].x + 20, h / 2 + positions[1].y + 20));
	positions.push_back(iPoint( positions[2].x + 12, h / 2 + positions[2].y + 10));

	positions.push_back(iPoint( (w / 4 - 120), h / 2 + 220));

	positions.push_back(iPoint( positions[6].x + 90, h / 2 + positions[6].y + 10));
	positions.push_back(iPoint( 400, h / 2 + 300));

	// --

	//viewport 4 ---

	positions.push_back(iPoint(w/2 + 100, h / 2 + 100));
	positions.push_back(iPoint(w / 2 + 230, h / 2 + 70));
	positions.push_back(iPoint(w / 2 + 425, h / 2 + 100));

	positions.push_back(iPoint(w / 2 + positions[0].x + 12, h / 2 + positions[0].y + 10));
	positions.push_back(iPoint(w / 2 + positions[1].x + 20, h / 2 + positions[1].y + 20));
	positions.push_back(iPoint(w / 2 + positions[2].x + 12, h / 2 + positions[2].y + 10));

	positions.push_back(iPoint(w / 2 + (w / 4 - 120), h / 2 + 220));

	positions.push_back(iPoint(w / 2 + positions[6].x + 90, h / 2 + positions[6].y + 10));
	positions.push_back(iPoint(w / 2 + 400, h / 2 + 300));

	// ---

	// -----------------------------

	backgrounds_rects[0] = SDL_Rect{ 410, 26, 108, 137 };
	backgrounds_rects[1] = SDL_Rect{ 549, 4, 135, 179 };

	ganon_rects[1] = SDL_Rect{ 714, 18, 98, 128 };
	ganon_rects[0] = SDL_Rect{ 733, 170, 55, 75 };

	navi_rects[1] = SDL_Rect{ 836, 19, 97, 125 };
	navi_rects[0] = SDL_Rect{ 860, 168, 55, 80 };

	link_rects[1] = SDL_Rect{ 972, 20, 97, 125 };
	link_rects[0] = SDL_Rect{ 996, 169, 55, 80 };

	int pos_count = 0; 

	for (int i = 0; i < 4; i++) {

		//Setting the background card image
		viewport[i].back_images.push_back(window->CreateImage(positions[pos_count++], backgrounds_rects[0], false));

		viewport[i].back_images.push_back(window->CreateImage(positions[pos_count++], backgrounds_rects[1], false));

		viewport[i].back_images.push_back(window->CreateImage(positions[pos_count++], backgrounds_rects[0], false));

		//Setting the characters image
		viewport[i].char_images.push_back(window->CreateImage(positions[pos_count++], ganon_rects[0], false));

		viewport[i].char_images.push_back(window->CreateImage(positions[pos_count++], navi_rects[1], false));

		viewport[i].char_images.push_back(window->CreateImage(positions[pos_count++], link_rects[0], false));

		//Setting name background
		viewport[i].name_background = window->CreateImage(positions[pos_count++], {128, 52, 220, 55}, false);

		//Setting the initial name 
		viewport[i].name = window->CreateText(positions[pos_count++], App->font->game_font, 0, false, 255, 255, 255);
		viewport[i].name->SetText("link");

		//Setting info button
		viewport[i].character_info = window->CreateButton(positions[pos_count++], 200, 50, false);
		viewport[i].character_info->AddImage("idle", {128, 52, 220, 55});
		viewport[i].character_info->SetImage("idle"); 
	}
}
