#include "CharacterSelectionScene.h"
#include "j1Console.h"
#include "j1Gui.h"
#include "j1Window.h"
#include "j1Input.h"
#include "p2Log.h"
#include "j1Scene.h"
#include "j1Viewports.h"
#include "j1Audio.h"


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

	// Background images
	background_image = App->tex->LoadTexture("gui/intro_background.png");
	background_pos = { 0 , 0 };
	background_image_rect = { 0, 0, 1994, 1359 };

	// Create players and info
	link = new player_data();
	link->ability1_text = "    Basic Attack:\n\nFrontal smash with\n     the sword";
	link->ability2_text = "     Sword Swing:\n\nLink swings his sword\n       around him.";
	link->ability3_text = "    Boomerang:\n\nLink shoots his boo-\nmerang, catch it when\n    it comes back!";
	link->ability4_text = "         Charge:\n\n Link will run to target\n  location damaging.";

	link->ability1_rect_icon = { 978, 353, 32, 32 };
	link->ability2_rect_icon = { 1014, 353, 32, 32 };
	link->ability3_rect_icon = { 1050, 353, 32, 32 };
	link->ability4_rect_icon = { 1086, 353, 32, 32 };

	link->big_image = {0, 294, 116, 147}; 
	link->small_image = {348, 445, 74, 90};
	link->name = "LINK"; 
	link->entity = entity_name::link; 
	players_data.push_back(link);

	ganon = new player_data();
	ganon->ability1_text = "   Basic Attack:\n\nFrontal smash with\n     the trident.";
	ganon->ability2_text = "           Bat:\n\nGanon shoot a bat\n to target direction.";
	ganon->ability3_text = "    Flame Shield:\n\nGanon puts a shield\n     to himself. ";
	ganon->ability4_text = "      Evil Jump:\n\nGanaon jumps to a\ntarget position and\n  stuns enemies.";

	ganon->ability1_rect_icon = { 978, 497, 32, 32 };
	ganon->ability2_rect_icon = { 1014, 497, 32, 32 };
	ganon->ability3_rect_icon = { 1050, 497, 32, 32 };
	ganon->ability4_rect_icon = { 1086, 497, 32, 32 };

	ganon->big_image = { 116, 294, 116, 147 };
	ganon->small_image = { 422, 445, 75, 90 };
	ganon->name = "GANON";
	ganon->entity = entity_name::ganon;
	players_data.push_back(ganon);

	navi = new player_data();
	navi->ability1_text = "      Basic Attack:\n\nNavi shoots an energy\n ball in front of her."; 
	navi->ability2_text = "        Watch Out!:\n\n   Navi heals allies\n       around her.";
	navi->ability3_text = "         Blink:\n\nNavi uses her magic\n energy to teleport a\n   small distance!";
	navi->ability4_text = "      Hey Listen!:\n\ndisable enemy players\n view for 3  seconds.";

	navi->ability1_rect_icon = { 978, 425, 32, 32 };
	navi->ability2_rect_icon = { 1014, 425, 32, 32 };
	navi->ability3_rect_icon = { 1050, 425, 32, 32 };
	navi->ability4_rect_icon = { 1086, 425, 32, 32 };

	navi->big_image = { 232, 294, 116, 147 };
	navi->small_image = { 496, 445, 75, 90 };
	navi->name = "NAVI";
	navi->entity = entity_name::navi;
	players_data.push_back(navi);

	curr_player_data1 = players_data.at(0);
	curr_player_data2 = players_data.at(0);
	curr_player_data3 = players_data.at(0);
	curr_player_data4 = players_data.at(0);

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

	// Starting data
	for (int i = 0; i < viewports_data.size(); i++)
	{
		SetDataToViewport(players_data.at(0), i);
	}

	App->audio->PlayMusic("Audio/Music/guessing-game_house.ogg");

	return ret;
}

bool CharacterSelectionScene::CleanUp()
{
	// Free UI
	if (App->scene->GetCurrentScene() != App->scene->charselect_screen)
	{
		for (int i = 0; i < viewports_data.size(); i++)
		{
			App->gui->DeleteElement(viewports_data[i].window);
		}
	}

	// Free players
	for (int i = 0; i < players_data.size(); i++)
	{
		RELEASE(players_data.at(i));
	}

	viewports_data.clear();
	players_data.clear();

	// Stop Music
	App->audio->StopMusic();

	// Background image
	App->tex->UnLoadTexture(background_image);

	//Stop Music
	App->audio->StopMusic();

	all_ready = false;

	return true;
}

bool CharacterSelectionScene::Update(float dt)
{
	bool ret = true;

	// Blit background
	if (App->scene->GetCurrentScene() == this)
	{
		App->view->LayerBlit(1, background_image, background_pos, background_image_rect);
	}

	DrawScreenSeparation();

	// Debug
	if (App->input->GetControllerButton(0, SDL_CONTROLLER_BUTTON_BACK) == KEY_DOWN)
	{
		for (int i = 0; i < 4; i++)
			App->scene->players[i].character = entity_name::ganon;

		App->scene->ChangeScene((Scene*)App->scene->transition_scene);
	}

	if (all_ready)
	{
		// Set characters when finished
		App->scene->players[0].character = curr_player_data1->entity;
		App->scene->players[1].character = curr_player_data2->entity;
		App->scene->players[2].character = curr_player_data3->entity;
		App->scene->players[3].character = curr_player_data4->entity;

		App->scene->ChangeScene((Scene*)App->scene->transition_scene);
	}

	for (int i = 0; i < viewports_data.size(); i++)
	{
		// Take curr

		player_data* curr = nullptr;

		// Pick current viewport data
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

		// Move Left
		if (App->input->GetControllerButton(App->scene->players[i].gamepad - 1, SDL_CONTROLLER_BUTTON_DPAD_LEFT) == KEY_DOWN && viewports_data[i].is_ready == false)
		{
			switch (i)
			{
			case 0:
				curr = curr_player_data1 = MoveCharacterLeft(curr, i);
				break;
			case 1:
				curr = curr_player_data2 = MoveCharacterLeft(curr, i);
				break;
			case 2:
				curr = curr_player_data3 = MoveCharacterLeft(curr, i);
				break;
			case 3:
				curr = curr_player_data4 = MoveCharacterLeft(curr, i);
				break;
			}
		}

		// Move right
		else if (App->input->GetControllerButton(App->scene->players[i].gamepad - 1, SDL_CONTROLLER_BUTTON_DPAD_RIGHT) == KEY_DOWN && viewports_data[i].is_ready == false)
		{
			switch (i)
			{
			case 0:
				curr = curr_player_data1 = MoveCharacterRight(curr, i);
				break;
			case 1:
				curr = curr_player_data2 = MoveCharacterRight(curr, i);
				break;
			case 2:
				curr = curr_player_data3 = MoveCharacterRight(curr, i);
				break;
			case 3:
				curr = curr_player_data4 = MoveCharacterRight(curr, i);
				break;
			}
		}

		// Show info
		if (App->input->GetControllerButton(App->scene->players[i].gamepad - 1, SDL_CONTROLLER_BUTTON_Y) == KEY_DOWN)
		{
			viewports_data[i].background_info_image->enabled = true;

			viewports_data[i].abilities_info1->enabled = true;
			viewports_data[i].abilities_info2->enabled = true;
			viewports_data[i].abilities_info3->enabled = true;
			viewports_data[i].abilities_info4->enabled = true;

			viewports_data[i].abilities_image1->enabled = true;
			viewports_data[i].abilities_image2->enabled = true;
			viewports_data[i].abilities_image3->enabled = true;
			viewports_data[i].abilities_image4->enabled = true;
		}
		else if (App->input->GetControllerButton(App->scene->players[i].gamepad - 1, SDL_CONTROLLER_BUTTON_Y) == KEY_UP)
		{
			viewports_data[i].background_info_image->enabled = false;

			viewports_data[i].abilities_info1->enabled = false;
			viewports_data[i].abilities_info2->enabled = false;
			viewports_data[i].abilities_info3->enabled = false;
			viewports_data[i].abilities_info4->enabled = false;

			viewports_data[i].abilities_image1->enabled = false;
			viewports_data[i].abilities_image2->enabled = false;
			viewports_data[i].abilities_image3->enabled = false;
			viewports_data[i].abilities_image4->enabled = false;

		}

		// Ready
		if (App->input->GetControllerButton(App->scene->players[i].gamepad - 1, SDL_CONTROLLER_BUTTON_START) == KEY_DOWN && viewports_data[i].is_ready == false)
		{
			viewports_data[i].is_ready = true;

			viewports_data[i].ready_text->SetPos(viewports_data[i].ready_text_pos);
			viewports_data[i].ready_text->SetText("READY!");

		}

		if (App->input->GetControllerButton(App->scene->players[i].gamepad - 1, SDL_CONTROLLER_BUTTON_B) == KEY_DOWN && viewports_data[i].is_ready == true)
		{
			viewports_data[i].is_ready = false;

			viewports_data[i].ready_text->SetPos(viewports_data[i].press_start_text_pos);
			viewports_data[i].ready_text->SetText("Press START when ready!");
		}

		if (viewports_data[0].is_ready && viewports_data[1].is_ready && viewports_data[2].is_ready && viewports_data[3].is_ready)
		{
			all_ready = true;
		}


		// Check if all ready
		int counter = 0;
		for (int i = 0; i < viewports_data.size(); i++)
		{
			if (viewports_data[i].is_ready)
				counter++;
		}

		if (counter == viewports_data.size())
			all_ready = true;

	}

	return ret;
	
}



player_data* CharacterSelectionScene::MoveCharacterLeft(player_data * data, int viewport)
{
	player_data* ret = nullptr;

	int main_index = GetIndexByPlayerData(data);

	// Move left
	main_index--;
	if (main_index < 0)
		main_index = players_data.size() - 1;

	ret = players_data.at(main_index);

	SetDataToViewport(ret, viewport);

	return ret;
}

player_data* CharacterSelectionScene::MoveCharacterRight(player_data * data, int viewport)
{
	player_data* ret = nullptr;

	int main_index = GetIndexByPlayerData(data);

	// Move right
	main_index++;
	if (main_index > players_data.size() - 1)
		main_index = 0;

	ret = players_data.at(main_index);

	SetDataToViewport(ret, viewport);

	return ret;
}

void CharacterSelectionScene::SetDataToViewport(player_data * data, int viewport)
{
	if (viewport >= 0 && viewport < viewports_data.size())
	{
		int main_index = GetIndexByPlayerData(data);

		int left_index = main_index - 1;
		if (left_index < 0)
			left_index = players_data.size() - 1;

		int right_index = main_index + 1;
		if (right_index > players_data.size()-1)
			right_index = 0;

		// Player name
		viewports_data.at(viewport).text_name->SetText(players_data.at(main_index)->name);

		// Abilities text
		viewports_data.at(viewport).abilities_info1->SetText(players_data.at(main_index)->ability1_text);
		viewports_data.at(viewport).abilities_info2->SetText(players_data.at(main_index)->ability2_text);
		viewports_data.at(viewport).abilities_info3->SetText(players_data.at(main_index)->ability3_text);
		viewports_data.at(viewport).abilities_info4->SetText(players_data.at(main_index)->ability4_text);

		viewports_data.at(viewport).abilities_image1->image = data->ability1_rect_icon; 
		viewports_data.at(viewport).abilities_image2->image = data->ability2_rect_icon;
		viewports_data.at(viewport).abilities_image3->image = data->ability3_rect_icon;
		viewports_data.at(viewport).abilities_image4->image = data->ability4_rect_icon;

		// Card Images
		viewports_data.at(viewport).big_image->ChangeImage(players_data.at(main_index)->big_image);
		viewports_data.at(viewport).small_image_left->ChangeImage(players_data.at(left_index)->small_image);
		viewports_data.at(viewport).small_image_right->ChangeImage(players_data.at(right_index)->small_image);
	}
}

int CharacterSelectionScene::GetIndexByPlayerData(player_data * data)
{
	int ret = 0;

	for (int i = 0; players_data.size(); i++)
	{
		if (data == players_data.at(i))
		{
			ret = i;
			break;
		}
	}
	return ret;
}

void CharacterSelectionScene::DrawScreenSeparation()
{
	uint win_w, win_h;
	App->win->GetWindowSize(win_w, win_h);

	for (uint y = 0; y < win_h - 2; y += 2)
	{
		SDL_Rect rect = { 130,0,4,2 };
		App->render->Blit(App->gui->atlas, win_w / 2 - 2, y, &rect);
	}

	for (uint x = 0; x < win_w - 2; x += 2)
	{
		SDL_Rect rect2 = { 130,2,2,4 };
		App->render->Blit(App->gui->atlas, x, win_h / 2 - 4, &rect2);
	}

	SDL_Rect rect3 = { 130,0,6,6 };
	App->render->Blit(App->gui->atlas, win_w / 2 - 2, win_h / 2 - 6, &rect3);
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
