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
