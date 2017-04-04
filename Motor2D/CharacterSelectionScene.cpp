#include "CharacterSelectionScene.h"
#include "j1Console.h"
#include "j1Gui.h"
#include "j1Window.h"
#include "j1Input.h"
#include "p2Log.h"
#include "j1Scene.h"
#include "j1Viewports.h"
#include "j1Audio.h"

//#define CHARACTER_NUM 3
//
//CharacterSelectionScene::CharacterSelectionScene()
//{
//}
//
//CharacterSelectionScene::~CharacterSelectionScene()
//{
//}
//
//bool CharacterSelectionScene::Start()
//{
//	uint w, h; 
//	App->win->GetWindowSize(w, h); 
//
//	window = App->gui->UI_CreateWin(iPoint(0,0), w, h, 2, false); 
//
//	CreateScene(w,h); 
//
//	App->audio->PlayMusic("Audio/Music/guessing-game_house.ogg");
//
//	return true;
//}
//
//bool CharacterSelectionScene::PreUpdate()
//{
//
//	return true;
//}
//
//bool CharacterSelectionScene::Update(float dt)
//{
//
//	uint w, h;
//	App->win->GetWindowSize(w, h);
//	
//	for (int i = 0; i < 4; i++) 
//	{
//		if (App->input->GetControllerButton(App->scene->players[i].gamepad-1, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) == KEY_DOWN && player_ready[App->scene->players[i].viewport - 1] == false)
//		{
//	
//			MoveCard(App->scene->players[i].viewport - 1, "right");
//
//			int a = char_view[App->scene->players[i].viewport - 1].size()/2;
//
//			int j = 0; 
//
//			// Update images 
//			for (list<character_info>::iterator it = char_view[App->scene->players[i].viewport - 1].begin(); it != char_view[i].end(); it++)
//			{
//				switch ((*it).character)
//				{
//				case link:
//					if (j != a)
//					{
//						viewport[App->scene->players[i].viewport - 1].char_images.at(j++)->image = link_rects[0];
//					}
//					else
//					{
//						viewport[App->scene->players[i].viewport - 1].char_images.at(j++)->image = link_rects[1];
//					}
//
//
//					break;
//				case navi:
//					if (j != a)
//					{
//						viewport[App->scene->players[i].viewport - 1].char_images.at(j++)->image = navi_rects[0];
//					}
//					else
//					{
//						viewport[App->scene->players[i].viewport - 1].char_images.at(j++)->image = navi_rects[1];
//					}
//					break;
//
//				case ganon:
//					if (j != a)
//					{
//						viewport[App->scene->players[i].viewport - 1].char_images.at(j++)->image = ganon_rects[0];
//					}
//					else
//					{
//						viewport[App->scene->players[i].viewport - 1].char_images.at(j++)->image = ganon_rects[1];
//					}
//					break;
//				}
//			}
//		}
//
//		if (App->input->GetControllerButton(App->scene->players[i].gamepad - 1, SDL_CONTROLLER_BUTTON_LEFTSHOULDER) == KEY_DOWN && player_ready[App->scene->players[i].viewport - 1] == false)
//		{
//			MoveCard(i, "left");
//
//			int a = char_view[App->scene->players[i].viewport - 1].size() / 2;
//
//			int j = 0;
//
//			// Update images
//			for (list<character_info>::iterator it = char_view[App->scene->players[i].viewport - 1].begin(); it != char_view[i].end(); it++)
//			{
//				switch ((*it).character)
//				{
//				case link:
//					if (j != a)
//					{
//						viewport[App->scene->players[i].viewport - 1].char_images.at(j++)->image = link_rects[0];
//					}
//					else
//					{
//						viewport[App->scene->players[i].viewport - 1].char_images.at(j++)->image = link_rects[1];
//					}
//
//
//					break;
//				case navi:
//					if (j != a)
//					{
//						viewport[App->scene->players[i].viewport - 1].char_images.at(j++)->image = navi_rects[0];
//					}
//					else
//					{
//						viewport[App->scene->players[i].viewport - 1].char_images.at(j++)->image = navi_rects[1];
//					}
//					break;
//
//				case ganon:
//					if (j != a)
//					{
//						viewport[App->scene->players[i].viewport - 1].char_images.at(j++)->image = ganon_rects[0];
//					}
//					else
//					{
//						viewport[App->scene->players[i].viewport - 1].char_images.at(j++)->image = ganon_rects[1];
//					}
//					break;
//				}
//			}
//		}
//
//		int center_pos = char_view[App->scene->players[i].viewport - 1].size() / 2;
//		int count = 0;
//
//		for (list<character_info>::iterator it = char_view[App->scene->players[i].viewport - 1].begin(); it != char_view[App->scene->players[i].viewport - 1].end(); it++)
//		{
//			if (count++ == center_pos) 
//			{
//				viewport[App->scene->players[i].viewport - 1].name->SetText(it->name);
//			}
//		}
//
//		if (App->input->GetControllerButton(App->scene->players[i].gamepad - 1, SDL_CONTROLLER_BUTTON_Y) == KEY_DOWN)
//		{
//			int center_pos = char_view[App->scene->players[i].viewport - 1].size() / 2;
//			int count = 0;
//
//
//			// Update images
//		
//			for (list<character_info>::iterator it = char_view[App->scene->players[i].viewport - 1].begin(); it != char_view[App->scene->players[i].viewport - 1].end(); it++)
//			{
//				if (count++ == center_pos) {
//					switch (it->character)
//					{
//					case link:
//						EnableInfo(link, App->scene->players[i].viewport - 1, w, h);
//						
//
//						break;
//					case ganon:
//						EnableInfo(ganon, App->scene->players[i].viewport - 1, w, h);
//
//						break;
//
//					case navi:
//						EnableInfo(navi, App->scene->players[i].viewport - 1, w, h);
//
//						break;
//					}
//				}
//			}
//				
//		}
//		if (App->input->GetControllerButton(App->scene->players[i].gamepad - 1, SDL_CONTROLLER_BUTTON_Y) == KEY_UP)
//		{
//			for (list<info_window>::iterator it = info_container[App->scene->players[i].viewport - 1].begin(); it != info_container[App->scene->players[i].viewport - 1].end(); it++)
//			{
//		
//					switch (it->name)
//					{
//					case link:
//						DisableInfo(link, App->scene->players[i].viewport - 1);
//						break;
//					case ganon:
//						DisableInfo(ganon, App->scene->players[i].viewport - 1);
//						break;
//
//					case navi:
//						DisableInfo(navi, App->scene->players[i].viewport - 1);
//						break;
//					}
//				
//			}
//		
//		}
//
//		if (App->input->GetControllerButton(App->scene->players[i].gamepad - 1, SDL_CONTROLLER_BUTTON_START) == KEY_DOWN)
//		{
//			if(player_ready[i] != true)
//				viewport[App->scene->players[i].viewport - 1].ready_text->SetPos(iPoint(viewport[App->scene->players[i].viewport - 1].ready_text->GetPos().x + 70, viewport[App->scene->players[i].viewport - 1].ready_text->GetPos().y));
//
//			player_ready[App->scene->players[i].viewport - 1] = true;
//	
//			viewport[App->scene->players[i].viewport - 1].ready_text->SetText("READY");
//
//						
//		}
//
//		if (App->input->GetControllerButton(App->scene->players[i].gamepad - 1, SDL_CONTROLLER_BUTTON_B) == KEY_DOWN && player_ready[App->scene->players[i].viewport - 1] == true)
//		{
//			player_ready[App->scene->players[i].viewport - 1] = false;
//			viewport[App->scene->players[i].viewport - 1].ready_text->SetPos(iPoint(viewport[App->scene->players[i].viewport - 1].ready_text->GetPos().x - 70, viewport[App->scene->players[i].viewport - 1].ready_text->GetPos().y));
//			viewport[App->scene->players[i].viewport - 1].ready_text->SetText("Press START when ready");
//		}
//	}
//
//	DrawScreenSeparation();
//
//	return true;
//}
//
//bool CharacterSelectionScene::PostUpdate()
//{
//	change_scene = AllReady();
//
//	if (App->input->GetControllerButton(0, SDL_CONTROLLER_BUTTON_BACK) == KEY_DOWN)
//	{
//		change_scene = true;
//	}
//
//	if(change_scene)
//	{
//		for (int i = 0; i < 4; i++)
//		{
//			App->scene->players[i].character = link;
//		}
//		App->scene->ChangeScene((Scene*)App->scene->main_scene);
//	}
//
//
//	return true;
//}
//
//bool CharacterSelectionScene::CleanUp()
//{
//	if (App->scene->GetCurrentScene() != App->scene->charselect_screen)
//		App->gui->DeleteElement(window);
//
//	for (int i = 0; i < 4; i++)
//	{
//		viewport[i].Reset();
//		info_container[i].clear();
//		char_view[i].clear();
//		player_ready[i] = false;
//	}
//
//	for (int i = 0; i < 2; i++)
//	{
//		ganon_rects[i] = NULLRECT;
//		navi_rects[i] = NULLRECT;
//		link_rects[i] = NULLRECT;
//		backgrounds_rects[i] = NULLRECT;
//	}
//	
//	positions.clear();
//	change_scene = false;
//
//	//Stop Music
//	App->audio->StopMusic();
//
//	return true;
//}
//
//void CharacterSelectionScene::CreateScene(uint w, uint h)
//{
//	//Seting the position of the cards -------------------------------
//
//	// viewport 1 ---
//	positions.push_back(iPoint(w / 10, h / 10));
//	positions.push_back(iPoint(w / 10 + 120, h / 10 - 20));
//	positions.push_back(iPoint(w / 10 + 240, h / 10));
//
//	positions.push_back(iPoint(positions[0].x + 12, positions[0].y - 10));
//	positions.push_back(iPoint(positions[1].x + 20, positions[1].y ));
//	positions.push_back(iPoint(positions[2].x + 12, positions[2].y - 10));
//
//	positions.push_back(iPoint(w/4 - 120, 220));
//
//	positions.push_back(iPoint(positions[6].x + 90, positions[6].y + 10));
//	positions.push_back(iPoint(550, 300));
//	positions.push_back(iPoint(30, 30));
//	positions.push_back(iPoint(w/5 - 60, h/4 + 125));
//
//	//---
//	// viewport 2 ---
//
//	positions.push_back(iPoint(w/2 + 100, 100));
//	positions.push_back(iPoint(w / 2 + 230, 70));
//	positions.push_back(iPoint(w / 2 + 425, 100));
//
//	positions.push_back(iPoint(w / 2 + positions[0].x + 12, positions[0].y - 10));
//	positions.push_back(iPoint(w / 2 + positions[1].x + 20, positions[1].y));
//	positions.push_back(iPoint(w / 2 + positions[2].x + 12, positions[2].y - 10));
//
//	positions.push_back(iPoint(w / 2 + (w / 4 - 120), 220));
//
//	positions.push_back(iPoint(w / 2 + positions[6].x + 90, positions[6].y + 10));
//	positions.push_back(iPoint(w / 2 + 520, 300));
//	positions.push_back(iPoint(w / 2 + 30, 30));
//	positions.push_back(iPoint(w / 2 + positions[10].x, positions[10].y));
//
//	//---
//
//	//viewport 3 ---
//
//	positions.push_back(iPoint( 100, h / 2 + 100));
//	positions.push_back(iPoint( 230, h / 2 + 70));
//	positions.push_back(iPoint( 425, h / 2 + 100));
//
//	positions.push_back(iPoint( positions[0].x + 12, h / 2 + positions[0].y - 10));
//	positions.push_back(iPoint(positions[1].x + 20, h / 2 + positions[1].y));
//	positions.push_back(iPoint(positions[2].x + 12, h / 2 + positions[2].y - 10));
//
//	positions.push_back(iPoint( (w / 4 - 120), h / 2 + 220));
//
//	positions.push_back(iPoint( positions[6].x + 90, h / 2 + positions[6].y + 10));
//	positions.push_back(iPoint( 550, h / 2 + 300));
//	positions.push_back(iPoint(30, h / 2 + 30));
//	positions.push_back(iPoint(positions[10].x, h / 2 + positions[10].y));
//
//	// --
//
//	//viewport 4 ---
//
//	positions.push_back(iPoint(w/2 + 100, h / 2 + 100));
//	positions.push_back(iPoint(w / 2 + 230, h / 2 + 70));
//	positions.push_back(iPoint(w / 2 + 425, h / 2 + 100));
//
//	positions.push_back(iPoint(w / 2 + positions[0].x + 12, h / 2 + positions[0].y - 10));
//	positions.push_back(iPoint(w / 2 + positions[1].x + 20, h / 2 + positions[1].y ));
//	positions.push_back(iPoint(w / 2 + positions[2].x + 12, h / 2 + positions[2].y - 10));
//
//	positions.push_back(iPoint(w / 2 + (w / 4 - 120), h / 2 + 220));
//
//	positions.push_back(iPoint(w / 2 + positions[6].x + 90, h / 2 + positions[6].y + 10));
//	positions.push_back(iPoint(w / 2 + 520, h / 2 + 300));
//	positions.push_back(iPoint(w / 2 + 30, h / 2 + 30));
//	positions.push_back(iPoint(w / 2 + positions[10].x, h / 2 + positions[10].y));
//
//	// ---
//
//	// -----------------------------
//
//	backgrounds_rects[0] = SDL_Rect{ 410, 26, 108, 137 };
//	backgrounds_rects[1] = SDL_Rect{ 549, 4, 135, 179 };
//
//	ganon_rects[1] = SDL_Rect{ 116, 294, 116, 147 };
//	ganon_rects[0] = SDL_Rect{ 421, 445, 74, 90 };
//
//	navi_rects[1] = SDL_Rect{ 232, 294, 116, 147 };
//	navi_rects[0] = SDL_Rect{ 495, 445, 74, 90 };
//
//	link_rects[1] = SDL_Rect{ 0, 294, 116, 147 };
//	link_rects[0] = SDL_Rect{348, 445, 74, 90 };
//
//	player_ready[0] = false; 
//	player_ready[1] = false;
//	player_ready[2] = false;
//	player_ready[3] = false;
//
//	int pos_count = 3; 
//
//	for (int i = 0; i < 4; i++)
//	{
//		//Setting the characters image
//		viewport[i].char_images.push_back(window->CreateImage(positions[pos_count++], ganon_rects[0], false));
//
//		viewport[i].char_images.push_back(window->CreateImage(positions[pos_count++], link_rects[1], false));
//
//		viewport[i].char_images.push_back(window->CreateImage(positions[pos_count++], navi_rects[0], false));
//
//		//Setting name background
//		viewport[i].name_background = window->CreateImage(positions[pos_count++], { 128, 52, 220, 55 }, false);
//
//		//Setting the initial name 
//		viewport[i].name = window->CreateText(positions[pos_count++], App->font->game_font, 0, false, 255, 255, 255);
//
//		//Setting info button
//		viewport[i].info_button = window->CreateImage(positions[pos_count++], {324, 195, 95, 42}, false);
//
//		// Setting info of the characters
//		viewport[i].info_back = window->CreateImage(positions[pos_count++], { 656, 595, 470, 325 }, false);
//		viewport[i].info_back->enabled = false; 
//
//		// Setting ready text
//		viewport[i].ready_text = window->CreateText(positions[pos_count++], App->font->game_font, 0, false, 255, 255, 255);
//		viewport[i].ready_text->SetText("Press START when ready"); 
//
//		pos_count += 3;
//	}
//
//
//	for (int i = 0; i < 4; i++) 
//	{
//		character_info d;
//
//		d.character = ganon;
//		d.name = "GANON";
//
//		char_view[i].push_back(d);
//
//		d.character = link;
//		d.name = "LINK";
//
//		char_view[i].push_back(d);
//
//		d.character = navi;
//		d.name ="NAVI";
//
//		char_view[i].push_back(d);
//
//	}
//	
//
//	info_window win; 
//
//// Link info ----
//
//	win.name = link; 
//
//	win.habilites.push_back(window->CreateImage(iPoint(60, 70), { 284, 195, 40, 40 }, false));
//	win.habilites.push_back(window->CreateImage(iPoint(60, 120), { 284, 195, 40, 40 }, false));
//	win.habilites.push_back(window->CreateImage(iPoint(60, 200), { 284, 195, 40, 40 }, false));
//	win.habilites.push_back(window->CreateImage(iPoint(60, 270), { 284, 195, 40, 40 }, false));
//	
//	win.description.push_back(window->CreateText(iPoint(110, 70), App->font->game_font_12, 25, false, 255, 255, 255));
//	win.description.at(0)->SetText("Basic Attack    Frontal smash with the sword"); 
//	win.description.push_back(window->CreateText(iPoint(110, 120), App->font->game_font_12, 25, false, 255, 255, 255));
//	win.description.at(1)->SetText("Sword Swing    Link swings his sword around him and\n damage all nearby enemies");
//	win.description.push_back(window->CreateText(iPoint(110, 200), App->font->game_font_12, 25, false, 255, 255, 255));

//
//	int f = 0; 
//	while (f < 4) 
//	{
//		info_container[f++].push_back(win);
//
//	}
//	
//	win.habilites.clear(); 
//	win.description.clear(); 
//
//// ----
//
//// Ganon info ----
//
//	win.name = ganon;
//
//	win.habilites.push_back(window->CreateImage(iPoint(60, 70), { 284, 195, 40, 40 }, false));
//	win.habilites.push_back(window->CreateImage(iPoint(60, 120), { 284, 195, 40, 40 }, false));
//	win.habilites.push_back(window->CreateImage(iPoint(60, 200), { 284, 195, 40, 40 }, false));
//	win.habilites.push_back(window->CreateImage(iPoint(60, 270), { 284, 195, 40, 40 }, false));
//

//	while (f < 4)
//	{
//		info_container[f++].push_back(win);
//
//	}
//
//	win.habilites.clear();
//	win.description.clear();
//
//// ----
//
//// navi info ----
//
//	win.name = navi;
//
//	win.habilites.push_back(window->CreateImage(iPoint(60, 70), { 284, 195, 40, 40 }, false));
//	win.habilites.push_back(window->CreateImage(iPoint(60, 120), { 284, 195, 40, 40 }, false));
//	win.habilites.push_back(window->CreateImage(iPoint(60, 200), { 284, 195, 40, 40 }, false));
//	win.habilites.push_back(window->CreateImage(iPoint(60, 270), { 284, 195, 40, 40 }, false));

//	f = 0;
//	while (f < 4)
//	{
//		info_container[f++].push_back(win);
//
//	}
//
//// ----
//
//	f = 0;
//	while (f < 4)
//	{
//		for (list<info_window>::iterator it = info_container[f].begin(); it != info_container[f].end(); it++)
//		{
//			it->habilites.at(0)->enabled = false;
//			it->habilites.at(1)->enabled = false;
//			it->habilites.at(2)->enabled = false;
//			it->habilites.at(3)->enabled = false;
//
//			it->description.at(0)->enabled = false;
//			it->description.at(1)->enabled = false;
//			it->description.at(2)->enabled = false;
//			it->description.at(3)->enabled = false;
//		}
//		f++; 
//	}
//
//}
//
//void CharacterSelectionScene::MoveCard(int pad, const char * direction)
//{
//	
//	if (direction == "right") 
//	{
//		character_info first = char_view[pad].front();
//		char_view[pad].pop_front();
//		char_view[pad].push_back(first);
//
//	}
//	else if(direction == "left")
//	{
//		character_info last = char_view[pad].back();
//		char_view[pad].pop_back();
//		char_view[pad].push_front(last);
//	}
//}
//
//void CharacterSelectionScene::EnableInfo(entity_name character, int viewport_num, uint w, uint h)
//{
//
//	
//	iPoint start_point = iPoint(60, 70); 
//
//	for (list<info_window>::iterator it = info_container[viewport_num].begin(); it != info_container[viewport_num].end(); it++)
//	{
//
//		int vertical_spacing = 0; 
//
//		if (it->name == character) 
//		{
//
//			switch (viewport_num) {
//			case 0: 	
//				break;
//
//			case 1:
//				start_point.x += w / 2; 
//
//				for (int i = 0; i < 4; i++)
//				{
//					it->habilites.at(i)->SetPos(iPoint(start_point.x, start_point.y + vertical_spacing));
//					it->description.at(i)->SetPos(iPoint(start_point.x + 50, start_point.y + vertical_spacing));
//					vertical_spacing += 70; 
//				}
//				break;
//
//			case 2:
//
//				start_point.y += h / 2;
//
//				for (int i = 0; i < 4; i++)
//				{
//					it->habilites.at(i)->SetPos(iPoint(start_point.x, start_point.y + vertical_spacing));
//					it->description.at(i)->SetPos(iPoint(start_point.x + 50, start_point.y + vertical_spacing));
//					vertical_spacing += 70;
//				}
//				break;
//
//			case 3:
//
//				start_point.x += w / 2;
//				start_point.y += h / 2;
//
//				for (int i = 0; i < 4; i++)
//				{
//					it->habilites.at(i)->SetPos(iPoint(start_point.x, start_point.y + vertical_spacing));
//					it->description.at(i)->SetPos(iPoint(start_point.x + 50, start_point.y + vertical_spacing));
//					vertical_spacing += 70;
//				}
//				break;
//
//			}
//
//			it->habilites.at(0)->enabled = true;
//			it->habilites.at(1)->enabled = true;
//			it->habilites.at(2)->enabled = true;
//			it->habilites.at(3)->enabled = true;
//
//			it->description.at(0)->enabled = true;
//			it->description.at(1)->enabled = true;
//			it->description.at(2)->enabled = true;
//			it->description.at(3)->enabled = true;
//
//		}
//
//	};
//
//	viewport[viewport_num].info_back->enabled = true;
//	viewport[viewport_num].ready_text->enabled = false;
//}
//
//void CharacterSelectionScene::DisableInfo(entity_name character, int viewport_num)
//{
//	int i = 0; 
//	for (list<info_window>::iterator it = info_container[viewport_num].begin(); it != info_container[viewport_num].end(); it++)
//	{
//		if (it->name == character)
//		{
//			it->habilites.at(0)->enabled = false;
//			it->habilites.at(1)->enabled = false;
//			it->habilites.at(2)->enabled = false;
//			it->habilites.at(3)->enabled = false;
//
//			it->description.at(0)->enabled = false;
//			it->description.at(1)->enabled = false;
//			it->description.at(2)->enabled = false;
//			it->description.at(3)->enabled = false;
//
//			viewport[viewport_num].info_back->enabled = false;
//			viewport[viewport_num].ready_text->enabled = true;
//		}
//	};
//}
//
//bool CharacterSelectionScene::AllReady()
//{
//	bool ret = true;
//
//	for (int i = 0; i < 4; i++) 
//	{
//		if (player_ready[i] == false) 
//		{
//			ret = false;
//			break;
//		}		 
//	}
//	return ret;
//}
//
//void CharacterSelectionScene::DrawScreenSeparation()
//{
//	uint win_w, win_h;
//	App->win->GetWindowSize(win_w, win_h);
//
//	for (uint y = 0; y < win_h - 2; y += 2)
//	{
//		App->view->LayerBlit(1, App->gui->atlas, iPoint(win_w / 4 - 2, y), { 130,0,4,2 });
//	}
//
//	for (uint x = 0; x < win_w - 2; x += 2)
//	{
//		App->view->LayerBlit(1, App->gui->atlas, iPoint(x, win_h / 4 - 4), { 130,2,2,4 });
//	}
//
//	App->view->LayerBlit(1, App->gui->atlas, iPoint(win_w / 4 - 2, win_h / 4 - 6), { 130,0,6,6 });
//}

CharacterSelectionScene::CharacterSelectionScene()
{
}

CharacterSelectionScene::~CharacterSelectionScene()
{
}

bool CharacterSelectionScene::Start()
{
	bool ret = true;

	App->console->AddText("viewports.set 4", Input); 

	// Create players and info
	link = new player_data();
	link->ability1_text = "Basic Attack    Frontal smash with the sword";
	link->ability2_text = "Sword Swing    Link swings his sword around him and\n damage all nearby enemies"; 
	link->ability3_text = "Boomerang    Link shoots his boomerang, catch it\n when it comes back!";
	link->ability4_text = "Charge    Link will run to target location ignoring\n collisions and damaging all enemies he touches.";
	link->big_image = {0, 294, 116, 147}; 
	link->small_image = {348, 445, 74, 90};
	link->name = "link"; 
	link->entity = entity_name::link; 

	ganon = new player_data();
	ganon->ability1_text = "Basic Attack    Frontal smash with the trident";
	ganon->ability2_text = "Bat    Ganon shoot a bat to target direction";
	ganon->ability3_text = "Flame Shield     Ganon puts a shield to himself and\n summon 3 fire orbs that orbit around him. ";
	ganon->ability4_text = "Evil Jump    Ganaon jumps to a target position and\n stuns all enemies on the area.";
	ganon->big_image = { 116, 294, 116, 147 };
	ganon->small_image = { 422, 445, 75, 90 };
	ganon->name = "ganon";
	ganon->entity = entity_name::ganon;

	navi = new player_data();
	navi->ability1_text = "Basic Attack    Navi shoots an energy ball in front\n of her that hits the first enemy hit";
	navi->ability2_text = "Watch Out!    Navi heals allies around her. Range is\n displayed as a circle with Navi as centre.";
	navi->ability3_text = "Blink     Navi uses her magic energy to teleport a\n small distance in front of her.!";
	navi->ability4_text = "Hey Listen!    disable enemy players view for 3\n seconds";
	navi->big_image = { 232, 294, 116, 147 };
	navi->small_image = { 496, 445, 75, 90 };
	navi->name = "navi";
	navi->entity = entity_name::navi;

	curr_player_data1 = link;
	curr_player_data2 = ganon;
	curr_player_data3 = navi;
	curr_player_data4 = link;

	// ----

	// Creating viewports

	viewport_data data_view_1(1); 
	viewport_data data_view_2(2);
	viewport_data data_view_3(3);
	viewport_data data_view_4(4);

	viewports_data.push_back(data_view_1);
	viewports_data.push_back(data_view_2);
	viewports_data.push_back(data_view_3);
	viewports_data.push_back(data_view_4);

	// ----

	

	return ret;
}

bool CharacterSelectionScene::Update(float dt)
{
	bool ret = true;

	bool change_cene = false;

	if (App->input->GetControllerButton(0, SDL_CONTROLLER_BUTTON_BACK) == KEY_DOWN)
	{
		change_scene = true;
	}
	
	if(change_scene)
	{

		for (int i = 0; i < 4; i++) {
			App->scene->players[i].character = entity_name::link; 
		}
		
		/*App->scene->players[0].character = curr_player_data1->entity;
		App->scene->players[1].character = curr_player_data2->entity;
		App->scene->players[2].character = curr_player_data3->entity;
		App->scene->players[3].character = curr_player_data4->entity;*/
		
		App->scene->ChangeScene((Scene*)App->scene->main_scene);
	}

	for (int i = 0; i < viewports_data.size(); i++)
	{
		// Take curr

		player_data* curr = nullptr;

		switch (i)
		{
		case 0:
			curr = curr_player_data1;
			break;
		case 1:
			curr = curr_player_data2;
			break;
		case 2:
			curr = curr_player_data3;
			break;
		case 3:
			curr = curr_player_data4;
			break;
		}

		if (App->input->GetControllerButton(App->scene->players[i].gamepad - 1, SDL_CONTROLLER_BUTTON_LEFTSHOULDER) == KEY_DOWN && viewports_data[i].is_ready == false)
		{
			switch (i)
			{
			case 0:
				curr_player_data1 = MoveCharacter("left", curr->entity);
				break;
			case 1:
				curr_player_data2 = MoveCharacter("left", curr->entity);
				break;
			case 2:
				curr_player_data3 = MoveCharacter("left", curr->entity);
				break;
			case 3:
				curr_player_data4 = MoveCharacter("left", curr->entity);
				break;
			}
			
		}

		if (App->input->GetControllerButton(App->scene->players[i].gamepad - 1, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) == KEY_DOWN && viewports_data[i].is_ready == false)
		{
			switch (i)
			{
			case 0:
				curr_player_data1 = MoveCharacter("right", curr->entity);
				break;
			case 1:
				curr_player_data2 = MoveCharacter("right", curr->entity);
				break;
			case 2:
				curr_player_data3 = MoveCharacter("right", curr->entity);
				break;
			case 3:
				curr_player_data4 = MoveCharacter("right", curr->entity);
				break;
			}

		}

		if (App->input->GetControllerButton(App->scene->players[i].gamepad - 1, SDL_CONTROLLER_BUTTON_Y) == KEY_DOWN)
		{
			viewports_data[i].background_info_image->enabled = true; 

			viewports_data[i].abilities_info1->enabled = true; 
			viewports_data[i].abilities_info2->enabled = true;
			viewports_data[i].abilities_info3->enabled = true;
			viewports_data[i].abilities_info4->enabled = true;
		}

		if (App->input->GetControllerButton(App->scene->players[i].gamepad - 1, SDL_CONTROLLER_BUTTON_Y) == KEY_UP)
		{
			viewports_data[i].background_info_image->enabled = false;

			viewports_data[i].abilities_info1->enabled = false;
			viewports_data[i].abilities_info2->enabled = false;
			viewports_data[i].abilities_info3->enabled = false;
			viewports_data[i].abilities_info4->enabled = false;
		}

		if (App->input->GetControllerButton(App->scene->players[i].gamepad - 1, SDL_CONTROLLER_BUTTON_START) == KEY_DOWN && viewports_data[i].is_ready == false) 
		{
			viewports_data[i].is_ready = true; 
			viewports_data[i].ready_text->SetPos(iPoint(viewports_data[i].ready_text->GetPos().x + 15, viewports_data[i].ready_text->GetPos().y));
		}

		if (App->input->GetControllerButton(App->scene->players[i].gamepad - 1, SDL_CONTROLLER_BUTTON_B) == KEY_DOWN && viewports_data[i].is_ready == true)
		{
			viewports_data[i].is_ready = false;
			viewports_data[i].ready_text->SetPos(iPoint(viewports_data[i].ready_text->GetPos().x - 15, viewports_data[i].ready_text->GetPos().y));
		}

		viewports_data[i].SetViewportInfo(curr);

		if (viewports_data[0].is_ready == true && viewports_data[1].is_ready == true && viewports_data[2].is_ready == true && viewports_data[3].is_ready == true)
			change_cene = true; 
	
	}

	return ret;
}

bool CharacterSelectionScene::CleanUp()
{
	for (int i = 0; i < 4; i++)
	{
		if (App->scene->GetCurrentScene() != App->scene->charselect_screen)
			App->gui->DeleteElement(viewports_data[i].window);
	}
		
		viewports_data.clear(); 
			
		change_scene = false;
	
		//Stop Music
		App->audio->StopMusic();
	
		return true;
}

player_data* CharacterSelectionScene::MoveCharacter(const char * direction, entity_name middle)
{
	switch (middle) 
	{
	case entity_name::link: 
		if (direction == "left")
			return ganon; 
		else
			return navi;
		break; 

	case entity_name::ganon:
		if (direction == "left")
			return navi;
		else 
			return link;
		break;

	case entity_name::navi:
		if (direction == "left")
			return link;
		else
			return ganon;
		break;
	}

}


bool viewport_data::SetViewportInfo(player_data * player)
{

	 big_image->image = player->big_image;

	 if (player->entity == entity_name::link) {
		 small_image_left->image = App->scene->charselect_screen->ganon->small_image;
		 small_image_right->image = App->scene->charselect_screen->navi->small_image;
	 }
	 else if(player->entity == entity_name::ganon)
	 {
		 small_image_left->image = App->scene->charselect_screen->navi->small_image;
		 small_image_right->image = App->scene->charselect_screen->link->small_image;
	 }
	 else
	 {
		 small_image_left->image = App->scene->charselect_screen->link->small_image;
		 small_image_right->image = App->scene->charselect_screen->ganon->small_image;
	 }

	 text_name->SetText(player->name);

	 abilities_info1->SetText(player->ability1_text);
	 abilities_info2->SetText(player->ability2_text);
	 abilities_info3->SetText(player->ability3_text);
	 abilities_info4->SetText(player->ability4_text);

	 if (is_ready == false)
		 ready_text->SetText("Press START when ready");

	 else {
		 ready_text->SetText("READY");
	 }
		

	return false;
}
