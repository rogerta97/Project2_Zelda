#include "MenuScene.h"
#include "j1Input.h"
#include "MainScene.h"
#include "j1Viewports.h"
#include "j1Console.h"
#include "j1Scene.h"
#include "p2Log.h"



MenuScene::MenuScene()
{
}

MenuScene::~MenuScene()
{
}

bool MenuScene::Start()
{
	bool ret = false;

	change_scene = false;

	SDL_Rect screen = App->view->GetViewportRect(1);
	menu_window = App->gui->UI_CreateWin(iPoint(0, 0), screen.w, screen.h, 0, false);

	// Triforce
//	triforce = menu_window->CreateImage(iPoint(50, 25), {34, 133, 115, 104}, false);

	// Start ---
	start_button = menu_window->CreateButton(iPoint(screen.w - 70, 150), 223, 60, false);
	button_list.push_back(start_button);

	start_button->AddImage("idle", { 128, 52, 220, 55 });

	start_button->SetImage("idle");

	start_text = menu_window->CreateText(iPoint(screen.w, 160), App->font->game_font);
	start_text->SetText("NEW GAME");
	start_text->click_through = true;

	// ---------

	// Options -
	options_button = menu_window->CreateButton(iPoint(screen.w - 70, 220), 223, 60, false);
	button_list.push_back(options_button);

	options_button->AddImage("idle", { 128, 52, 220, 55 });


	options_button->SetImage("idle");

	options_text = menu_window->CreateText(iPoint(screen.w, 230), App->font->game_font);
	options_text->SetText("OPTIONS");
	options_text->click_through = true;

	fx_button = menu_window->CreateButton(iPoint(screen.w - 70, 220), 223, 60, false);

	fx_button->AddImage("idle", { 128, 52, 220, 55 });


	fx_button->SetImage("idle");

	fx_button->enabled = false;

	fx_text = menu_window->CreateText(iPoint(screen.w, 230), App->font->game_font);
	fx_text->SetText("FX");
	fx_text->enabled = false;

	music_button = menu_window->CreateButton(iPoint(screen.w - 70, 290), 223, 60, false);

	music_button->AddImage("idle", { 128, 52, 220, 55 });


	music_button->SetImage("idle");

	music_button->enabled = false;

	music_text = menu_window->CreateText(iPoint(screen.w, 300), App->font->game_font);
	music_text->SetText("MUSIC");
	music_text->enabled = false;

	//Check Box 

	options_checkbox = menu_window->CreateCheckBox(iPoint(0, 0), 44, 44, {404, 44, 44, 44}, { 404, 0, 44, 44 }, true, false);
	options_checkbox->AddBox(iPoint(screen.w + 99, screen.h / 2 + 45), 44, 44, "fx");
	options_checkbox->AddBox(iPoint(screen.w + 99, screen.h / 2 + 115), 44, 44, "music");
	options_checkbox->enabled = false;
	
	// ---------

	// Credits --
	credits_button = menu_window->CreateButton(iPoint(screen.w - 70, 290), 223, 60, false);
	button_list.push_back(credits_button); 

	credits_button->AddImage("idle", { 128, 52, 220, 55 });

	credits_button->SetImage("idle");

	credits_text = menu_window->CreateText(iPoint(screen.w, 300), App->font->game_font);
	credits_text->SetText("CREDITS"); 
	credits_text->click_through = true; 
	// ---------

	// Quit ---
	quit_button = menu_window->CreateButton(iPoint(screen.w - 70, 360), 223, 60, false);
	button_list.push_back(quit_button);

	quit_button->AddImage("idle", { 128, 52, 220, 55 });

	quit_button->SetImage("idle");

	quit_text = menu_window->CreateText(iPoint(screen.w, 370), App->font->game_font);
	quit_text->SetText("QUIT GAME");
	quit_text->click_through = true;
	// ---------

	button_list.push_back(fx_button);
	button_list.push_back(music_button); 

	// Cursor --
	cursor_1 = menu_window->CreateImage(iPoint(button_list.at(current_button)->GetPos().x - 70, button_list.at(current_button)->GetPos().y + 2), {80, 52, 48, 48 }, false);
	cursor_2 = menu_window->CreateImage(iPoint(button_list.at(current_button)->GetPos().x + button_list.at(current_button)->rect.w + 5, button_list.at(current_button)->GetPos().y + 2), { 80, 52, 48, 48 }, false);

	cursors.push_back(cursor_1); 
	cursors.push_back(cursor_2);

	// Checkbox test
	UI_Check_Box* cb = menu_window->CreateCheckBox(iPoint(300, 300), 100, 100, {166, 5, 11, 9}, {181, 5, 11, 9 }, false, true);
	cb->AddBox(iPoint(320, 320), 20, 20, "test1");
	cb->AddBox(iPoint(320, 340), 20, 20, "test2");
	cb->AddBox(iPoint(320, 360), 20, 20, "test3");
	cb->AddBox(iPoint(320, 380), 20, 20, "test4");
	//cb->GetBox("test1");

	return true;
}

bool MenuScene::PreUpdate()
{
	return true;
}

bool MenuScene::Update(float dt)
{
	if (App->input->GetControllerButton(0, SDL_CONTROLLER_BUTTON_DPAD_DOWN) == KEY_DOWN)
	{
		if (is_options) 
		{
			if (current_button < 5)
			{
				int current_button_int = current_button;
				current_button_int++;

				current_button = static_cast<button_action> (current_button_int);
			}
		}
		else 
		{
			if (current_button < 3) {
				int current_button_int = current_button;
				current_button_int++;

				current_button = static_cast<button_action> (current_button_int);
			}	
		}
	}

	if (App->input->GetControllerButton(0, SDL_CONTROLLER_BUTTON_DPAD_UP) == KEY_DOWN && current_button > 0)
	{
		if (is_options)
		{
			if (current_button > 4)
			{
				int current_button_int = current_button;
				current_button_int--;

				current_button = static_cast<button_action> (current_button_int);
			}
		}
		else
		{
			if (current_button > 0) {
				int current_button_int = current_button;
				current_button_int--;

				current_button = static_cast<button_action> (current_button_int);
			}

		}
	
	}

	cursors.at(0)->SetPos(iPoint(button_list.at(current_button)->GetPos().x - 70, button_list.at(current_button)->GetPos().y + 2));
	cursors.at(1)->SetPos(iPoint(button_list.at(current_button)->GetPos().x + button_list.at(current_button)->rect.w + 5, button_list.at(current_button)->GetPos().y + 2));
	
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		App->scene->ChangeScene(App->scene->main_scene);
	}

	return true;
}

bool MenuScene::PostUpdate()
{
	if (App->input->GetControllerButton(0, SDL_CONTROLLER_BUTTON_A) == KEY_DOWN)
	{

		switch (current_button)
		{
		case START:
			App->scene->ChangeScene((Scene*)App->scene->team_select);
			break;

		case OPTIONS:
			is_options = true; 
			GoOptions();
			break;

		case CREDITS:

			break;

		case QUIT:
			App->console->AddText("quit", Input);
			break;

		case FX:
			options_checkbox->SetBox(!options_checkbox->GetBox("fx"), "fx");
			break; 

		case MUSIC:
			options_checkbox->SetBox(!options_checkbox->GetBox("music"), "music");
			break; 

		}
	}


	if (App->input->GetControllerButton(0, SDL_CONTROLLER_BUTTON_B) == KEY_DOWN && is_options == true)
	{
		is_options = false; 
		GoMenu(); 
	}

	return true;
}

bool MenuScene::CleanUp()
{
	// Free UI
	if(App->scene->GetCurrentScene() != App->scene->menu_scene)
		App->gui->DeleteElement(menu_window);
	// -------

	button_list.clear();
	
	return true;
}

void MenuScene::OnColl(PhysBody * bodyA, PhysBody * bodyB, b2Fixture * fixtureA, b2Fixture * fixtureB)
{

}

void MenuScene::OnCommand(std::list<std::string>& tokens)
{
}

void MenuScene::GoOptions()
{
	for (int i = 0; i < 4; i++) {
		button_list.at(i)->enabled = false; 
	}

	start_text->enabled = false; 
	options_text->enabled = false; 
	credits_text->enabled = false; 
	quit_text->enabled = false; 

	fx_button->enabled = true; 
	music_button->enabled = true; 
	fx_text->enabled = true; 
	music_text->enabled = true; 
	options_checkbox->enabled = true; 

	current_button = FX; 
}

void MenuScene::GoMenu()
{
	for (int i = 0; i < 4; i++) {
		button_list.at(i)->enabled = true;
	}

	start_text->enabled = true;
	options_text->enabled = true;
	credits_text->enabled = true;
	quit_text->enabled = true;

	fx_button->enabled = false;
	music_button->enabled = false;
	fx_text->enabled = false;
	music_text->enabled = false;
	options_checkbox->enabled = false;

	current_button = START;
}
