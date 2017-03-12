#include "MenuScene.h"
#include "j1Input.h"
#include "MainScene.h"
#include "j1Viewports.h"
#include "j1Console.h"
#include "j1Scene.h"
#include "p2Log.h"

enum button_action {
	START,
	OPTIONS,
	CREDITS,
	QUIT
};

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
	triforce = menu_window->CreateImage(iPoint(50, 25), {34, 133, 115, 104}, false);

	// Start ---
	start_button = menu_window->CreateButton(iPoint(screen.w - 70, 150), 223, 60, false);
	button_list.push_back(start_button);

	start_button->AddImage("idle", { 657, 7, 223, 57 }); 
	start_button->AddImage("above", { 428, 65, 219, 57 });
	start_button->AddImage("clicked", { 428, 6, 220, 57 });

	start_button->SetImage("idle"); 

	start_text = menu_window->CreateText(iPoint(screen.w, 160), App->font->game_font);
	start_text->SetText("NEW GAME");
	start_text->click_through = true;

	// ---------

	// Options -
	options_button = menu_window->CreateButton(iPoint(screen.w - 70, 220), 223, 60, false);
	button_list.push_back(options_button);

	options_button->AddImage("idle", { 657, 7, 223, 57 });
	options_button->AddImage("above", { 428, 65, 219, 57 });
	options_button->AddImage("clicked", { 428, 6, 220, 57 });

	options_button->SetImage("idle");

	options_text = menu_window->CreateText(iPoint(screen.w, 230), App->font->game_font);
	options_text->SetText("OPTIONS");
	options_text->click_through = true;

	// ---------

	// Credits --
	credits_button = menu_window->CreateButton(iPoint(screen.w - 70, 290), 223, 60, false);
	button_list.push_back(credits_button); 

	credits_button->AddImage("idle", { 657, 7, 223, 57 });
	credits_button->AddImage("above", { 428, 65, 219, 57 });
	credits_button->AddImage("clicked", { 428, 6, 220, 57 });

	credits_button->SetImage("idle");

	credits_text = menu_window->CreateText(iPoint(screen.w, 300), App->font->game_font);
	credits_text->SetText("CREDITS"); 
	credits_text->click_through = true; 
	// ---------

	// Quit ---
	quit_button = menu_window->CreateButton(iPoint(screen.w - 70, 360), 223, 60, false);
	button_list.push_back(quit_button);

	quit_button->AddImage("idle", { 657, 7, 223, 57 });
	quit_button->AddImage("above", { 428, 65, 219, 57 });
	quit_button->AddImage("clicked", { 428, 6, 220, 57 });

	quit_button->SetImage("idle");

	quit_text = menu_window->CreateText(iPoint(screen.w, 370), App->font->game_font);
	quit_text->SetText("QUIT GAME");
	quit_text->click_through = true;

	// ---------
	current_button = start_button; 

	// Cursor --
	cursor_1 = menu_window->CreateImage(iPoint(current_button->GetPos().x - 70, current_button->GetPos().y + 2), { 36, 57, 57, 52 }, false);
	cursor_2 = menu_window->CreateImage(iPoint(current_button->GetPos().x + current_button->rect.w + 5, current_button->GetPos().y + 2), { 36, 57, 57, 52 }, false);

	cursors.push_back(cursor_1); 
	cursors.push_back(cursor_2);

	return false;
}

bool MenuScene::PreUpdate()
{
	return false;
}

bool MenuScene::Update(float dt)
{

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) 
	{
		App->scene->ChangeScene(App->scene->main_scene); 
	}

	// Getting current position of the mouse
	int cursor_pos = -1; 
	for (vector<UI_Button*>::iterator it = button_list.begin(); it != button_list.end(); it++) 
	{
		cursor_pos++;
		if ((*it) == current_button)
			break; 
	}

	if (App->input->GetControllerButton(0, SDL_CONTROLLER_BUTTON_DPAD_DOWN) == KEY_DOWN && cursor_pos < 3) 
	{
		current_button = button_list.at(++cursor_pos); 
	}

	if (App->input->GetControllerButton(0, SDL_CONTROLLER_BUTTON_DPAD_UP) == KEY_DOWN && cursor_pos > 0) 
	{
		current_button = button_list.at(--cursor_pos);
	}

	if (App->input->GetControllerButton(0, SDL_CONTROLLER_BUTTON_A) == KEY_DOWN) 
	{

		switch (cursor_pos)
		{
		case START:
			App->scene->ChangeScene(App->scene->main_scene);
			break;

		case OPTIONS:
			
			break;

		case CREDITS:
			
			break;

		case QUIT:
			App->console->AddText("quit", Input); 
			break; 
		}
	}

	cursors.at(0)->SetPos(iPoint(current_button->GetPos().x - 70, current_button->GetPos().y + 2));
	cursors.at(1)->SetPos(iPoint(current_button->GetPos().x + current_button->rect.w + 5, current_button->GetPos().y + 2));

	return false;
}

bool MenuScene::PostUpdate()
{
	return false;
}

bool MenuScene::CleanUp()
{
	// Free UI
	if(App->scene->GetCurrentScene() != App->scene->menu_scene)
		App->gui->DeleteElement(menu_window);
	// -------

	button_list.clear();
	
	return false;
}

void MenuScene::OnColl(PhysBody * bodyA, PhysBody * bodyB, b2Fixture * fixtureA, b2Fixture * fixtureB)
{

}

void MenuScene::OnCommand(std::list<std::string>& tokens)
{
}
