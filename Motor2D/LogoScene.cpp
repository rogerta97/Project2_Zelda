#include "LogoScene.h"
#include "p2Log.h"
#include "j1Window.h"
#include "j1Textures.h"
#include "Scene.h"
#include "j1Scene.h"
#include "j1Input.h"
#include "j1Audio.h"

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

	win_size = App->win->GetWindowSize();

	App->render->background = {255, 255, 255};

	App->video->PlayVideo("logo.ogv", { win_size.x / 2 - 154,win_size.y / 2 - 154,306,306 });

	return ret;
}

bool LogoScene::Update(float dt)
{
	bool ret = true;

	if (!App->video->IsPlaying())
	{
		if (logo_played == true)
		{
			App->scene->ChangeScene((Scene*)App->scene->menu_scene);
		}
		else
		{
			logo_played = true;
			App->video->PlayVideo("intro.ogv", { 0,0,win_size.x,win_size.y });
		}
	}

	return ret;
}

bool LogoScene::CleanUp()
{
	bool ret = true;

	LOG("Start LogoScene");

	App->render->background = { 0, 0, 0 };

	App->audio->RestartAudio();

	return ret;
}
