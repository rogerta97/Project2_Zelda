#include "MenuScene.h"
#include "j1Input.h"
#include "MainScene.h"
#include "j1Viewports.h"
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
	triforce = menu_window->CreateImage(iPoint(50, 25), {34, 133, 115, 104}, false);

	// Start ---
	start_button = menu_window->CreateButton(iPoint(screen.w - 70, 150), 223, 60, false);
	button_list.push_back(start_button);

	start_button->AddImage("idle", { 657, 7, 223, 57 }); 
	start_button->AddImage("above", { 428, 65, 219, 57 });
	start_button->AddImage("clicked", { 428, 6, 220, 57 });

	start_button->SetImage("idle"); 

	start_text = menu_window->CreateText(iPoint(screen.w, 165), App->font->default_15, 15);
	start_text->SetText("NEW GAME");
	start_text->click_through = true;
	// ---------

	// Options -
	options_button = menu_window->CreateButton(iPoint(screen.w - 70, 220), 223, 60, false);
	button_list.push_back(options_button);

	options_button->AddImage("idle", { 657, 7, 223, 57 });
	options_button->AddImage("above", { 428, 65, 219, 57 });
	options_button->AddImage("clicked", { 428, 6, 220, 57 });

	options_button->SetImage("clicked");

	options_text = menu_window->CreateText(iPoint(screen.w, 235), App->font->default_15, 15);
	options_text->SetText("OPTIONS");
	options_text->click_through = true;
	// ---------

	// Quit ---
	quit_button = menu_window->CreateButton(iPoint(screen.w - 70, 290), 223, 60, false);
	button_list.push_back(quit_button);

	quit_button->AddImage("idle", { 657, 7, 223, 57 });
	quit_button->AddImage("above", { 428, 65, 219, 57 });
	quit_button->AddImage("clicked", { 428, 6, 220, 57 });

	quit_button->SetImage("above");

	quit_text = menu_window->CreateText(iPoint(screen.w, 305), App->font->default_15, 15);
	quit_text->SetText("QUIT GAME");
	quit_text->click_through = true;
	// ---------

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

	for(int i = 0; i<button_list.size(); i++)
	{
		if (button_list.at(i)->MouseEnter())
			button_list.at(i)->SetImage("above");

		else if (button_list.at(i)->MouseOut())
			button_list.at(i)->SetImage("idle");

		if (button_list.at(i)->MouseClickEnterLeft())
			button_list.at(i)->SetImage("clicked");

		if (button_list.at(i)->MouseClickOutLeft())
			button_list.at(i)->SetImage("above");
	}

	if(start_button->MouseClickEnterLeft())
		App->scene->ChangeScene(App->scene->main_scene);

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
