#include "TransitionScene.h"
#include "j1Textures.h"
#include "Mapping.h"
#include "j1Input.h"
#include "j1Window.h"
#include "j1Scene.h"

TransitionScene::TransitionScene()
{
}

TransitionScene::~TransitionScene()
{
}

bool TransitionScene::Start()
{
	App->win->GetWindowSize(w, h);

	// Background
	background_image = App->tex->LoadTexture("gui/intro_background.png");
	background_pos = { 0 , 0 };
	background_image_rect = { 0, 0, 1994, 1359 };

	window = App->gui->UI_CreateWin({0,0}, w, h, 0, false);

	SDL_Rect background_text_rect = {81, 1345, 230, 55}; 

	background_text_image = window->CreateImage({(int)w/2 - background_text_rect.w/2, (int)h - 70}, background_text_rect);
	continue_text = window->CreateText({ (int)background_text_image->GetPos().x + 15, (int)background_text_image->GetPos().y + 13 }, App->font->game_font_25);
	continue_text->SetText("CONFIRM TO CONTINUE");

	screen = App->tex->LoadTexture("textures/controllers1.png"); 

	return true;
}

bool TransitionScene::Update(float dt)
{
	bool ret = true;

	App->render->Blit(background_image, background_pos.x, background_pos.y, &background_image_rect);

	App->render->Blit(screen, w/2 - 532, h/ 2 - 380);

	key_mapping a1 = App->scene->players[0].mapping->GetMapping(m_k_confirm);

	if(App->input->GetControllerButton(0, a1.key_id) == KEY_DOWN)
		App->scene->ChangeScene((Scene*)App->scene->main_scene);

	return ret; 
}

bool TransitionScene::CleanUp()
{
	bool ret = true;

	if (App->scene->GetCurrentScene() != App->scene->transition_scene)
		App->gui->DeleteElement(window); 

	App->tex->UnLoadTexture(screen);
	App->tex->UnLoadTexture(background_image);

	return ret;
}

void TransitionScene::FadeIn()
{
}

void TransitionScene::FadeOut()
{
}
