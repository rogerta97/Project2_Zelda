#include "LogoScene.h"
#include "p2Log.h"
#include "j1Window.h"
#include "j1Textures.h"
#include "Scene.h"
#include "j1Scene.h"
#include "j1Input.h"

#define LOGO_APPEAR 1
#define LOGO_OUT 5.5f
#define SCREEN_TIME 8
#define FADE_SPEED 150

LogoScene::LogoScene()
{
}

LogoScene::~LogoScene()
{
}

bool LogoScene::Start()
{
	bool ret = true;

	LOG("Start LogoScene");

	App->render->background = {255, 255, 255};

	// Logo properties
	logo = App->tex->LoadTexture("textures/catpad_logo.png");
	logo_rect = {0, 0, 306, 306 };

	win_size =  App->win->GetWindowSize();
	logo_pos = { (win_size.x/2) - (logo_rect.w /2), (win_size.y / 2) - (logo_rect.h/2) };
	// --------------

	timer.Start();

	return ret;
}

bool LogoScene::Update(float dt)
{
	bool ret = true;

	if (timer.ReadSec() > LOGO_APPEAR && timer.ReadSec() < LOGO_OUT)
		FadeOut();
	else
		FadeIn();

	App->render->Blit(logo, logo_pos.x, logo_pos.y, &logo_rect);
	App->render->DrawQuad({ 0, 0, win_size.x,  win_size.y }, 255, 255, 255, -1.0f, fade_value, true);

	if (timer.ReadSec() > SCREEN_TIME)
		App->scene->ChangeScene((Scene*)App->scene->menu_scene);

	if (App->input->GetControllerButton(0, SDL_CONTROLLER_BUTTON_START) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		App->scene->ChangeScene((Scene*)App->scene->menu_scene);
	}

	return ret;
}

bool LogoScene::CleanUp()
{
	bool ret = true;

	LOG("Start LogoScene");

	App->tex->UnLoadTexture(logo);
	App->render->background = { 0, 0, 0 };

	return ret;
}

void LogoScene::FadeIn()
{
	fade_value += (FADE_SPEED*1.5f)*App->GetDT();
	if (fade_value > 255)
		fade_value = 255;
}

void LogoScene::FadeOut()
{
	fade_value -= FADE_SPEED*App->GetDT();
	if (fade_value < 0)
		fade_value = 0;
}
