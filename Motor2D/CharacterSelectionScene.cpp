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

	// Create players and info
	link = new player_data();
	link->ability1_text = "Basic Attack: Frontal smash with the\nsword";
	link->ability2_text = "Sword Swing: Link swings his sword\naround him and damage all nearby\nenemies"; 
	link->ability3_text = "Boomerang: Link shoots his boomerang,\ncatch it when it comes back!";
	link->ability4_text = "Charge: Link will run to target location\nignoring collisions and damaging\nall enemies he touches.";
	link->big_image = {0, 294, 116, 147}; 
	link->small_image = {348, 445, 74, 90};
	link->name = "LINK"; 
	link->entity = entity_name::link; 
	players_data.push_back(link);

	ganon = new player_data();
	ganon->ability1_text = "Basic Attack: Frontal smash with the trident";
	ganon->ability2_text = "Bat: Ganon shoot a bat to target direction";
	ganon->ability3_text = "Flame Shield: Ganon puts a shield to himself\nand summon 3 fire orbs that orbit around him. ";
	ganon->ability4_text = "Evil Jump: Ganaon jumps to a target position\nand stuns all enemies on the area.";
	ganon->big_image = { 116, 294, 116, 147 };
	ganon->small_image = { 422, 445, 75, 90 };
	ganon->name = "GANON";
	ganon->entity = entity_name::ganon;
	players_data.push_back(ganon);

	navi = new player_data();
	navi->ability1_text = "Basic Attack: Navi shoots an energy ball in\nfront of her that hits the first enemy hit";
	navi->ability2_text = "Watch Out!:  Navi heals allies around her. Range\nis displayed as a circle with Navi as centre.";
	navi->ability3_text = "Blink: Navi uses her magic energy to teleport\na small distance in front of her.!";
	navi->ability4_text = "Hey Listen!: disable enemy players view for 3\n seconds";
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

	return ret;
}

bool CharacterSelectionScene::Update(float dt)
{
	bool ret = true;

	bool change_cene = false;

	DrawScreenSeparation();

	// Debug
	if (App->input->GetControllerButton(0, SDL_CONTROLLER_BUTTON_BACK) == KEY_DOWN)
	{
		for (int i = 0; i < 4; i++) 
			App->scene->players[i].character = entity_name::link;
		
		App->scene->ChangeScene((Scene*)App->scene->main_scene);
	}
	
	if(change_scene)
	{
		// Set characters when finished
		App->scene->players[0].character = curr_player_data1->entity;
		App->scene->players[1].character = curr_player_data2->entity;
		App->scene->players[2].character = curr_player_data3->entity;
		App->scene->players[3].character = curr_player_data4->entity;
		
		App->scene->ChangeScene((Scene*)App->scene->main_scene);
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
		if (App->input->GetControllerButton(App->scene->players[i].gamepad - 1, SDL_CONTROLLER_BUTTON_LEFTSHOULDER) == KEY_DOWN && viewports_data[i].is_ready == false)
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
		else if (App->input->GetControllerButton(App->scene->players[i].gamepad - 1, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) == KEY_DOWN && viewports_data[i].is_ready == false)
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
		}
		else if (App->input->GetControllerButton(App->scene->players[i].gamepad - 1, SDL_CONTROLLER_BUTTON_Y) == KEY_UP)
		{
			viewports_data[i].background_info_image->enabled = false;

			viewports_data[i].abilities_info1->enabled = false;
			viewports_data[i].abilities_info2->enabled = false;
			viewports_data[i].abilities_info3->enabled = false;
			viewports_data[i].abilities_info4->enabled = false;
		}

		// Ready
		if (App->input->GetControllerButton(App->scene->players[i].gamepad - 1, SDL_CONTROLLER_BUTTON_START) == KEY_DOWN && viewports_data[i].is_ready == false)
		{
			viewports_data[i].is_ready = true;
		
			viewports_data[i].ready_text->SetPos(iPoint(viewports_data[i].ready_text->GetPos().x + 80, viewports_data[i].ready_text->GetPos().y));
			viewports_data[i].ready_text->SetText("READY!");
							
		}

		if (App->input->GetControllerButton(App->scene->players[i].gamepad - 1, SDL_CONTROLLER_BUTTON_B) == KEY_DOWN && viewports_data[i].is_ready == true)
		{
			viewports_data[i].is_ready = false;

			viewports_data[i].ready_text->SetPos(iPoint(viewports_data[i].ready_text->GetPos().x - 80, viewports_data[i].ready_text->GetPos().y));
			viewports_data[i].ready_text->SetText("Press START when ready!");
		}

		if (viewports_data[0].is_ready == true && viewports_data[1].is_ready == true && viewports_data[2].is_ready == true && viewports_data[3].is_ready == true)
			change_cene = true; 
	
	}

	// Check if all ready
	int counter = 0;
	for (int i = 0; i < viewports_data.size(); i++)
	{
		if (viewports_data[i].is_ready)
			counter++;
	}

	if (counter == viewports_data.size())
		change_cene = true;

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
	
	//Stop Music
	App->audio->StopMusic();

	change_scene = false;
	
	return true;
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
