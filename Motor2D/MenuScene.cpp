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

	triforce = menu_window->CreateImage(iPoint(50, 25), {34, 133, 115, 104}, false);

	start_button = menu_window->CreateButton(iPoint(screen.w - 70, 150), 223, 60, false);

	start_button->AddImage("idle", { 657, 7, 223, 57 }); 
	start_button->AddImage("above", { 428, 65, 219, 57 });
	start_button->AddImage("clicked", { 428, 6, 220, 57 });

	start_button->SetImage("idle"); 

	button_list.push_back(start_button); 

	start_text = menu_window->CreateText(iPoint(screen.w, 165), App->font->default_15, 15);
	start_text->SetText("NEW GAME");
	start_text->click_through = true;

	options_button = menu_window->CreateButton(iPoint(screen.w - 70, 220), 223, 60, false);

	options_button->AddImage("idle", { 657, 7, 223, 57 });
	options_button->AddImage("above", { 428, 65, 219, 57 });
	options_button->AddImage("clicked", { 428, 6, 220, 57 });

	options_button->SetImage("clicked");

	button_list.push_back(options_button);

	options_text = menu_window->CreateText(iPoint(screen.w, 235), App->font->default_15, 15);
	options_text->SetText("OPTIONS");
	options_text->click_through = true;

	quit_button = menu_window->CreateButton(iPoint(screen.w - 70, 290), 223, 60, false);

	quit_button->AddImage("idle", { 657, 7, 223, 57 });
	quit_button->AddImage("above", { 428, 65, 219, 57 });
	quit_button->AddImage("clicked", { 428, 6, 220, 57 });

	quit_button->SetImage("above");

	button_list.push_back(quit_button);

	quit_text = menu_window->CreateText(iPoint(screen.w, 305), App->font->default_15, 15);
	quit_text->SetText("QUIT GAME");
	quit_text->click_through = true;


	return false;
}

bool MenuScene::PreUpdate()
{
	return false;
}

bool MenuScene::Update(float dt)
{

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		App->scene->ChangeScene(App->scene->main_scene); 
	}

	for (list<UI_Button*>::iterator it = button_list.begin(); it != button_list.end(); it++) 
	{
		if ((*it)->MouseEnter())
			(*it)->SetImage("above");

		else if ((*it)->MouseOut())
			(*it)->SetImage("idle");

		if ((*it)->MouseClickEnterLeft())
			(*it)->SetImage("clicked");

		if ((*it)->MouseClickOutLeft())
			(*it)->SetImage("above");
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

	App->gui->DeleteElement(menu_window); 
	
	return false;
}

void MenuScene::OnColl(PhysBody * bodyA, PhysBody * bodyB, b2Fixture * fixtureA, b2Fixture * fixtureB)
{

}

void MenuScene::OnCommand(std::list<std::string>& tokens)
{
}
