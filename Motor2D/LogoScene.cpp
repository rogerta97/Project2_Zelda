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

	App->video->PlayVideo("catwithsound.ogv", { 10,10,500,500 });

	return ret;
}

bool LogoScene::Update(float dt)
{
	bool ret = true;


	if (!App->video->IsPlaying())
		App->scene->ChangeScene((Scene*)App->scene->menu_scene);

	return ret;
}

bool LogoScene::CleanUp()
{
	bool ret = true;

	LOG("Start LogoScene");

	App->render->background = { 0, 0, 0 };

	return ret;
}
