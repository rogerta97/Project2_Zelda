#include "CharacterSelectionScene.h"
#include "j1Gui.h"
#include "j1Window.h"
#include "j1Input.h"
#include "p2Log.h"
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

	uint w, h;
	App->win->GetWindowSize(w, h);
	
	for (int i = 0; i < 4; i++) 
	{
		if (App->input->GetControllerButton(i, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) == KEY_DOWN) 
		{
	
			MoveCard(i, "right");

			int a = char_view[i].size()/2;

			int j = 0; 

			// Update images 
			for (list<character_info>::iterator it = char_view[i].begin(); it != char_view[i].end(); it++)
			{
				switch ((*it).character)
				{
				case link:
					if (j != a)
					{
						viewport[i].char_images.at(j++)->image = link_rects[0];
					}
					else
					{
						viewport[i].char_images.at(j++)->image = link_rects[1];
					}


					break;
				case navi:
					if (j != a)
					{
						viewport[i].char_images.at(j++)->image = navi_rects[0];
					}
					else
					{
						viewport[i].char_images.at(j++)->image = navi_rects[1];
					}
					break;

				case ganon:
					if (j != a)
					{
						viewport[i].char_images.at(j++)->image = ganon_rects[0];
					}
					else
					{
						viewport[i].char_images.at(j++)->image = ganon_rects[1];
					}
					break;
				}
			}
		}

		if (App->input->GetControllerButton(i, SDL_CONTROLLER_BUTTON_LEFTSHOULDER) == KEY_DOWN)
		{
			MoveCard(i, "left");

			int a = char_view[i].size() / 2;

			int j = 0;

			// Update images
			for (list<character_info>::iterator it = char_view[i].begin(); it != char_view[i].end(); it++)
			{
				switch ((*it).character)
				{
				case link:
					if (j != a)
					{
						viewport[i].char_images.at(j++)->image = link_rects[0];
					}
					else
					{
						viewport[i].char_images.at(j++)->image = link_rects[1];
					}


					break;
				case navi:
					if (j != a)
					{
						viewport[i].char_images.at(j++)->image = navi_rects[0];
					}
					else
					{
						viewport[i].char_images.at(j++)->image = navi_rects[1];
					}
					break;

				case ganon:
					if (j != a)
					{
						viewport[i].char_images.at(j++)->image = ganon_rects[0];
					}
					else
					{
						viewport[i].char_images.at(j++)->image = ganon_rects[1];
					}
					break;
				}
			}
		}

		int center_pos = char_view[i].size() / 2;
		int count = 0;

		for (list<character_info>::iterator it = char_view[i].begin(); it != char_view[i].end(); it++)
		{
			if (count++ == center_pos) {
				viewport[i].name->SetText(it->name);
			}
		}

		if (App->input->GetControllerButton(i, SDL_CONTROLLER_BUTTON_Y) == KEY_DOWN)
		{
			int center_pos = char_view[i].size() / 2;
			int count = 0;


			// Update images
		
			for (list<character_info>::iterator it = char_view[i].begin(); it != char_view[i].end(); it++)
			{
				if (count++ == center_pos) {
					switch (it->character)
					{
					case link:
						EnableInfo(link, i, w, h);
						

						break;
					case ganon:
						EnableInfo(ganon, i, w, h);

						break;

					case navi:
						EnableInfo(navi, i, w, h);

						break;
					}
				}
			}
				
		}
		if (App->input->GetControllerButton(i, SDL_CONTROLLER_BUTTON_Y) == KEY_UP)
		{
			for (list<info_window>::iterator it = info_container[i].begin(); it != info_container[i].end(); it++)
			{
		
					switch (it->name)
					{
					case link:
						DisableInfo(link, i);
						break;
					case ganon:
						DisableInfo(ganon, i);
						break;

					case navi:
						DisableInfo(navi, i);
						break;
					}
				
			}
		
		}

		if (App->input->GetControllerButton(i, SDL_CONTROLLER_BUTTON_START) == KEY_DOWN) 
		{
			if(player_ready[i] != true)
				viewport[i].ready_text->SetPos(iPoint(viewport[i].ready_text->GetPos().x + 70, viewport[i].ready_text->GetPos().y));

			player_ready[i] = true;
	
			viewport[i].ready_text->SetText("READY"); 
						
		}

		if (App->input->GetControllerButton(i, SDL_CONTROLLER_BUTTON_B) == KEY_DOWN) 
		{
			player_ready[i] = false;
			viewport[i].ready_text->SetPos(iPoint(viewport[i].ready_text->GetPos().x - 70, viewport[i].ready_text->GetPos().y));
			viewport[i].ready_text->SetText("Press START when ready");
		}
	}

	return true;
}

bool CharacterSelectionScene::PostUpdate()
{

	if (App->input->GetControllerButton(0, SDL_CONTROLLER_BUTTON_BACK) == KEY_DOWN) 
	{
		App->scene->ChangeScene((Scene*)App->scene->main_scene);
	}

	change_scene = AllReady();

	if(change_scene)
	{
		App->scene->ChangeScene((Scene*)App->scene->main_scene);
	}


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
	positions.push_back(iPoint(550, 300));
	positions.push_back(iPoint(30, 30));
	positions.push_back(iPoint(w/5 - 60, h/4 + 125));

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
	positions.push_back(iPoint(w / 2 + 550, 300));
	positions.push_back(iPoint(w / 2 + 30, 30));
	positions.push_back(iPoint(w / 2 + positions[10].x, positions[10].y));

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
	positions.push_back(iPoint( 550, h / 2 + 300));
	positions.push_back(iPoint(30, h / 2 + 30));
	positions.push_back(iPoint(positions[10].x, h / 2 + positions[10].y));

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
	positions.push_back(iPoint(w / 2 + 550, h / 2 + 300));
	positions.push_back(iPoint(w / 2 + 30, h / 2 + 30));
	positions.push_back(iPoint(w / 2 + positions[10].x, h / 2 + positions[10].y));

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

	player_ready[0] = false; 
	player_ready[1] = false;
	player_ready[2] = false;
	player_ready[3] = false;

	int pos_count = 0; 

	for (int i = 0; i < 4; i++) {

		//Setting the background card image
		viewport[i].back_images.push_back(window->CreateImage(positions[pos_count++], backgrounds_rects[0], false));

		viewport[i].back_images.push_back(window->CreateImage(positions[pos_count++], backgrounds_rects[1], false));

		viewport[i].back_images.push_back(window->CreateImage(positions[pos_count++], backgrounds_rects[0], false));

		//Setting the characters image
		viewport[i].char_images.push_back(window->CreateImage(positions[pos_count++], ganon_rects[0], false));

		viewport[i].char_images.push_back(window->CreateImage(positions[pos_count++], link_rects[1], false));

		viewport[i].char_images.push_back(window->CreateImage(positions[pos_count++], navi_rects[0], false));

		//Setting name background
		viewport[i].name_background = window->CreateImage(positions[pos_count++], { 128, 52, 220, 55 }, false);

		//Setting the initial name 
		viewport[i].name = window->CreateText(positions[pos_count++], App->font->game_font, 0, false, 255, 255, 255);

		//Setting info button
		viewport[i].info_button = window->CreateImage(positions[pos_count++], {284, 195, 40, 40}, false);

		// Setting info of the characters
		viewport[i].info_back = window->CreateImage(positions[pos_count++], { 10, 295, 484, 327 }, false);
		viewport[i].info_back->enabled = false; 

		// Setting ready text
		viewport[i].ready_text = window->CreateText(positions[pos_count++], App->font->game_font, 0, false, 255, 255, 255);
		viewport[i].ready_text->SetText("Press START when ready"); 
	}


	for (int i = 0; i < 4; i++) 
	{
		character_info d;

		d.character = ganon;
		d.name = "GANON";

		char_view[i].push_back(d);

		d.character = link;
		d.name = "LINK";

		char_view[i].push_back(d);

		d.character = navi;
		d.name ="NAVI";

		char_view[i].push_back(d);

	}
	

	info_window win; 

// Link info ----

	win.name = link; 

	win.habilites.push_back(window->CreateImage(iPoint(60, 70), { 284, 195, 40, 40 }, false));
	win.habilites.push_back(window->CreateImage(iPoint(60, 120), { 284, 195, 40, 40 }, false));
	win.habilites.push_back(window->CreateImage(iPoint(60, 200), { 284, 195, 40, 40 }, false));
	win.habilites.push_back(window->CreateImage(iPoint(60, 270), { 284, 195, 40, 40 }, false));
	
	win.description.push_back(window->CreateText(iPoint(110, 70), App->font->game_font_12, 25, false, 255, 255, 255));
	win.description.at(0)->SetText("Basic Attack    Frontal smash with the sword"); 
	win.description.push_back(window->CreateText(iPoint(110, 120), App->font->game_font_12, 25, false, 255, 255, 255));
	win.description.at(1)->SetText("Sword Swing    Link swings his sword around him and\n damage all nearby enemies");
	win.description.push_back(window->CreateText(iPoint(110, 200), App->font->game_font_12, 25, false, 255, 255, 255));
	win.description.at(2)->SetText("Boomerang    Link shoots his boomerang, catch it\n when it comes back!");
	win.description.push_back(window->CreateText(iPoint(110, 270), App->font->game_font_12, 25, false, 255, 255, 255));
	win.description.at(3)->SetText("Charge    Link will run to target location ignoring\n collisions and damaging all enemies he touches.");

	int f = 0; 
	while (f < 4) 
	{
		info_container[f++].push_back(win);

	}
	
	win.habilites.clear(); 
	win.description.clear(); 

// ----

// Ganon info ----

	win.name = ganon;

	win.habilites.push_back(window->CreateImage(iPoint(60, 70), { 284, 195, 40, 40 }, false));
	win.habilites.push_back(window->CreateImage(iPoint(60, 120), { 284, 195, 40, 40 }, false));
	win.habilites.push_back(window->CreateImage(iPoint(60, 200), { 284, 195, 40, 40 }, false));
	win.habilites.push_back(window->CreateImage(iPoint(60, 270), { 284, 195, 40, 40 }, false));

	win.description.push_back(window->CreateText(iPoint(110, 70), App->font->game_font_12, 25, false, 255, 255, 255));
	win.description.at(0)->SetText("Basic Attack    Frontal smash with the trident");
	win.description.push_back(window->CreateText(iPoint(110, 130), App->font->game_font_12, 25, false, 255, 255, 255));
	win.description.at(1)->SetText("Bat    Ganon shoot a bat to target direction");
	win.description.push_back(window->CreateText(iPoint(110, 200), App->font->game_font_12, 25, false, 255, 255, 255));
	win.description.at(2)->SetText("Flame Shield     Ganon puts a shield to himself and\n summon 3 fire orbs that orbit around him. ");
	win.description.push_back(window->CreateText(iPoint(110, 270), App->font->game_font_12, 25, false, 255, 255, 255));
	win.description.at(3)->SetText("Evil Jump    Ganaon jumps to a target position and\n stuns all enemies on the area.");

	f = 0;
	while (f < 4)
	{
		info_container[f++].push_back(win);

	}

	win.habilites.clear();
	win.description.clear();

// ----

// navi info ----

	win.name = navi;

	win.habilites.push_back(window->CreateImage(iPoint(60, 70), { 284, 195, 40, 40 }, false));
	win.habilites.push_back(window->CreateImage(iPoint(60, 120), { 284, 195, 40, 40 }, false));
	win.habilites.push_back(window->CreateImage(iPoint(60, 200), { 284, 195, 40, 40 }, false));
	win.habilites.push_back(window->CreateImage(iPoint(60, 270), { 284, 195, 40, 40 }, false));

	win.description.push_back(window->CreateText(iPoint(110, 70), App->font->game_font_12, 25, false, 255, 255, 255));
	win.description.at(0)->SetText("Basic Attack    Navi shoots an energy ball in front\n of her that hits the first enemy hit");
	win.description.push_back(window->CreateText(iPoint(110, 130), App->font->game_font_12, 25, false, 255, 255, 255));
	win.description.at(1)->SetText("Watch Out!    Navi heals allies around her. Range is\n displayed as a circle with Navi as centre.");
	win.description.push_back(window->CreateText(iPoint(110, 200), App->font->game_font_12, 25, false, 255, 255, 255));
	win.description.at(2)->SetText("Blink     Navi uses her magic energy to teleport a\n small distance in front of her.");
	win.description.push_back(window->CreateText(iPoint(110, 270), App->font->game_font_12, 25, false, 255, 255, 255));
	win.description.at(3)->SetText("Hey Listen!    disable enemy players view for 3\n seconds");

	f = 0;
	while (f < 4)
	{
		info_container[f++].push_back(win);

	}

// ----

	f = 0;
	while (f < 4)
	{
		for (list<info_window>::iterator it = info_container[f].begin(); it != info_container[f].end(); it++)
		{
			it->habilites.at(0)->enabled = false;
			it->habilites.at(1)->enabled = false;
			it->habilites.at(2)->enabled = false;
			it->habilites.at(3)->enabled = false;

			it->description.at(0)->enabled = false;
			it->description.at(1)->enabled = false;
			it->description.at(2)->enabled = false;
			it->description.at(3)->enabled = false;
		}
		f++; 
	}

}

void CharacterSelectionScene::MoveCard(int pad, const char * direction)
{
	
	if (direction == "right") 
	{
		character_info first = char_view[pad].front();
		char_view[pad].pop_front();
		char_view[pad].push_back(first);

	}
	else if(direction == "left")
	{
		character_info last = char_view[pad].back();
		char_view[pad].pop_back();
		char_view[pad].push_front(last);
	}
}

void CharacterSelectionScene::EnableInfo(entity_name character, int viewport_num, uint w, uint h)
{

	
	iPoint start_point = iPoint(60, 70); 

	for (list<info_window>::iterator it = info_container[viewport_num].begin(); it != info_container[viewport_num].end(); it++)
	{

		int vertical_spacing = 0; 

		if (it->name == character) 
		{

			switch (viewport_num) {
			case 0: 	
				break;

			case 1:
				start_point.x += w / 2; 

				for (int i = 0; i < 4; i++)
				{
					it->habilites.at(i)->SetPos(iPoint(start_point.x, start_point.y + vertical_spacing));
					it->description.at(i)->SetPos(iPoint(start_point.x + 50, start_point.y + vertical_spacing));
					vertical_spacing += 70; 
				}
				break;

			case 2:

				start_point.y += h / 2;

				for (int i = 0; i < 4; i++)
				{
					it->habilites.at(i)->SetPos(iPoint(start_point.x, start_point.y + vertical_spacing));
					it->description.at(i)->SetPos(iPoint(start_point.x + 50, start_point.y + vertical_spacing));
					vertical_spacing += 70;
				}
				break;

			case 3:

				start_point.x += w / 2;
				start_point.y += h / 2;

				for (int i = 0; i < 4; i++)
				{
					it->habilites.at(i)->SetPos(iPoint(start_point.x, start_point.y + vertical_spacing));
					it->description.at(i)->SetPos(iPoint(start_point.x + 50, start_point.y + vertical_spacing));
					vertical_spacing += 70;
				}
				break;

			}

			it->habilites.at(0)->enabled = true;
			it->habilites.at(1)->enabled = true;
			it->habilites.at(2)->enabled = true;
			it->habilites.at(3)->enabled = true;

			it->description.at(0)->enabled = true;
			it->description.at(1)->enabled = true;
			it->description.at(2)->enabled = true;
			it->description.at(3)->enabled = true;

		}

	};

	viewport[viewport_num].info_back->enabled = true;
	viewport[viewport_num].ready_text->enabled = false;
}

void CharacterSelectionScene::DisableInfo(entity_name character, int viewport_num)
{
	int i = 0; 
	for (list<info_window>::iterator it = info_container[viewport_num].begin(); it != info_container[viewport_num].end(); it++)
	{
		if (it->name == character)
		{
			it->habilites.at(0)->enabled = false;
			it->habilites.at(1)->enabled = false;
			it->habilites.at(2)->enabled = false;
			it->habilites.at(3)->enabled = false;

			it->description.at(0)->enabled = false;
			it->description.at(1)->enabled = false;
			it->description.at(2)->enabled = false;
			it->description.at(3)->enabled = false;

			viewport[viewport_num].info_back->enabled = false;
			viewport[viewport_num].ready_text->enabled = true;
		}
			
		
	
	};
}

bool CharacterSelectionScene::AllReady()
{
	bool ret = true;

	for (int i = 0; i < 4; i++) 
	{
		if (player_ready[i] == false) 
		{
			ret = false;
			break;
		}		 
	}
	return ret;
}
