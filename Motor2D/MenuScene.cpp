#include "MenuScene.h"
#include "j1Input.h"
#include "MainScene.h"
#include "j1Viewports.h"
#include "j1Console.h"
#include "j1Scene.h"
#include "p2Log.h"
#include "j1Audio.h"
#include "j1XMLLoader.h"
#include "Animation.h"
#include "j1Window.h"
#include "Mapping.h"
#include "j1Scene.h"

#define FADE_SPEED 150


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

	SDL_Rect screen = {0, 0, App->win->GetWindowSize().x,  App->win->GetWindowSize().y};
	menu_window = App->gui->UI_CreateWin(iPoint(0, 0), screen.w, screen.h, 0, false);

	// Background
	background_image = App->tex->LoadTexture("gui/intro_background.png");
	background_pos = {0 , 0 };
	background_image_rect = {0, 0, 1994, 1359};

	// Main banner
	main_banner = new Animator();
	pugi::xml_document doc;
	App->xml->LoadXML("menu_scene.xml", doc);
	main_banner_texture = main_banner->LoadAnimationsFromXML(doc, "animations");
	main_banner->SetAnimation("idle");
	main_banner_pos = {(screen.w / 2) - (main_banner->GetCurrentAnimation()->GetActualFrame().w / 2), (screen.h/2) - 350};
	main_banner->SetAnimation("idle");

	// Start ---
	start_button = menu_window->CreateButton(iPoint(screen.w/2 - 110, (screen.h / 2) + 30), 223, 60, false);
	button_list.push_back(start_button);

	start_button->AddImage("idle", { 128, 52, 220, 55 });

	start_button->SetImage("idle");

	start_text = menu_window->CreateText(iPoint(start_button->rect.x + 60, (screen.h / 2) + 40), App->font->game_font);
	start_text->SetText("NEW GAME");
	start_text->click_through = true;

	// ---------

	// Options -
	options_button = menu_window->CreateButton(iPoint(screen.w/2 - 110, (screen.h / 2) + 100), 223, 60, false);
	button_list.push_back(options_button);

	options_button->AddImage("idle", { 128, 52, 220, 55 });
	options_button->SetImage("idle");

	options_text = menu_window->CreateText(iPoint(options_button->rect.x + 66, (screen.h / 2) + 110), App->font->game_font);
	options_text->SetText("OPTIONS");
	options_text->click_through = true;

	iPoint fx_button_pos = { screen.w / 2 - 110, screen.h - (screen.h / 2) + 70 }; 

	fx_button = menu_window->CreateButton(fx_button_pos, 223, 60, false);

	fx_button->AddImage("idle", { 128, 52, 220, 55 });
	fx_button->SetImage("idle");

	fx_button->enabled = false;

	fx_text = menu_window->CreateText(iPoint(fx_button_pos.x + 80, fx_button_pos.y + 10), App->font->game_font);
	fx_text->SetText("FX");
	fx_text->enabled = false;

	iPoint music_button_pos = { screen.w / 2 - 110, screen.h - (screen.h / 2) + 140 };

	music_button = menu_window->CreateButton(music_button_pos, 223, 60, false);

	music_button->AddImage("idle", { 128, 52, 220, 55 });
	music_button->SetImage("idle");

	music_button->enabled = false;

	music_text = menu_window->CreateText({ music_button_pos .x + 60, music_button_pos .y + 10}, App->font->game_font);
	music_text->SetText("MUSIC");
	music_text->enabled = false;

	iPoint remap_button_pos = { screen.w / 2 - 110, screen.h - (screen.h / 2) + 210 };

	remap_button = menu_window->CreateButton(remap_button_pos, 223, 60, false);

	remap_button->AddImage("idle", { 128, 52, 220, 55 });
	remap_button->SetImage("idle");

	remap_button->enabled = false;

	remap_text = menu_window->CreateText({ remap_button_pos.x + 60, remap_button_pos.y +10 }, App->font->game_font);
	remap_text->SetText("REMAPING");
	remap_text->enabled = false;

	//Check Box 

	options_checkbox = menu_window->CreateCheckBox(iPoint(0, 0), 44, 44, {404, 44, 44, 44}, { 404, 0, 44, 44 }, true);
	options_checkbox->AddBox(iPoint(fx_button->GetPos().x + fx_button->rect.w - options_checkbox->rect.w - 13, fx_button->GetPos().y + 5), 44, 44, "fx");
	options_checkbox->AddBox(iPoint(music_button->GetPos().x + music_button->rect.w - options_checkbox->rect.w - 13, music_button->GetPos().y + 5), 44, 44, "music");

	options_checkbox->SetBox(true, "fx"); 
	options_checkbox->SetBox(true, "music");

	options_checkbox->enabled = false;
	
	// ---------

	// Credits --
	credits_button = menu_window->CreateButton(iPoint(screen.w/2 - 110, (screen.h / 2) + 170), 223, 60, false);
	button_list.push_back(credits_button); 

	credits_button->AddImage("idle", { 128, 52, 220, 55 });

	credits_button->SetImage("idle");

	credits_text = menu_window->CreateText(iPoint(credits_button->rect.x + 68, (screen.h / 2) + 180), App->font->game_font);
	credits_text->SetText("CREDITS"); 
	credits_text->click_through = true; 
	// ---------

	// Quit ---
	quit_button = menu_window->CreateButton(iPoint(screen.w/2 - 110, (screen.h / 2) + 240), 223, 60, false);
	button_list.push_back(quit_button);

	quit_button->AddImage("idle", { 128, 52, 220, 55 });

	quit_button->SetImage("idle");

	quit_text = menu_window->CreateText(iPoint(quit_button->rect.x + 55, (screen.h / 2) + 250), App->font->game_font);
	quit_text->SetText("QUIT GAME");
	quit_text->click_through = true;
	// ---------

	button_list.push_back(fx_button);
	button_list.push_back(music_button);
	button_list.push_back(remap_button); 

	// Cursor --
	cursor_1 = menu_window->CreateImage(iPoint(button_list.at(current_button)->GetPos().x - 70, button_list.at(current_button)->GetPos().y + 2), {80, 52, 48, 48 }, false);
	cursor_2 = menu_window->CreateImage(iPoint(button_list.at(current_button)->GetPos().x + button_list.at(current_button)->rect.w + 5, button_list.at(current_button)->GetPos().y + 2), { 80, 52, 48, 48 }, false);

	cursors.push_back(cursor_1); 
	cursors.push_back(cursor_2);

	App->console->AddText("viewports.set 1", Input);
	App->view->camera1.x = 0;
	App->view->camera1.y = 0;

	//Music
	App->audio->DefaultVolume();
	App->audio->PlayMusic("Audio/Music/title.ogg");
	music_time.Start();

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
			if (current_button < 6)
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

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
	{
		quit_text->StartInterpolationAnimation(iPoint(quit_text->GetPos().x - 200, quit_text->GetPos().y), fPoint(1, 0), fPoint(0, 1), 5);
	}

	// Blit main banner
	if (App->scene->GetCurrentScene() == this)
	{
		App->render->Blit(background_image, background_pos.x, background_pos.y, &background_image_rect);
		App->render->DrawQuad({ 0, 0, App->win->GetWindowSize().x, App->win->GetWindowSize().y }, 255, 255, 255, -1.0f, fade_value, true);
		FadeOut();
		App->render->Blit(main_banner_texture, main_banner_pos.x, main_banner_pos.y, &main_banner->GetCurrentAnimation()->GetAnimationFrame(dt));
	}

	//Stop music ones it finish
	if (music_time.ReadSec() > 17)
	{
		App->audio->StopMusic();
	}

	return true;
}

bool MenuScene::PostUpdate()
{
	//Testing remapping
	int accept_key_id = -1;
	if (App->scene->players[0].mapping->GetKey(m_k_confirm, &accept_key_id))
	{
		if (App->input->GetControllerButton(0, accept_key_id) == KEY_DOWN)
		{

			switch (current_button)
			{
			case START:
				App->scene->ChangeScene((Scene*)App->scene->charselect_screen);
				return true;
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

			case REMAP: 
				App->scene->ChangeScene((Scene*)App->scene->remaping_scene);
				break; 

			}
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
	cursors.clear();

	pos = NULLPOINT;

	current_button = START;
	is_options = false;
	
	//Stop Music
	App->audio->StopMusic();

	// Main banner
	main_banner->CleanUp();
	RELEASE(main_banner);
	App->tex->UnLoadTexture(main_banner_texture);	

	// Background image
	App->tex->UnLoadTexture(background_image);
	fade_value = 255.0f;

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

	remap_button->enabled = true; 
	remap_text->enabled = true; 

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

	remap_button->enabled = false;
	remap_text->enabled = false;

	current_button = START;
}

void MenuScene::FadeOut()
{
	fade_value -= FADE_SPEED*App->GetDT();
	if (fade_value < 0)
		fade_value = 0;
}
