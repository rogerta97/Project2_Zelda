#include "LogoScene.h"
#include "p2Log.h"
#include "j1Window.h"
#include "j1Textures.h"
#include "Scene.h"
#include "j1Scene.h"
#include "j1Input.h"
#include "j1Audio.h"
#include "j1XMLLoader.h"

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

	pugi::xml_document doc;

	App->xml->LoadXML("logo_scene.xml", doc);

	pugi::xml_node skip_node = doc.child("file").child("skip_img");
	pugi::xml_node logo_node = doc.child("file").child("logo");
	
	SDL_Rect skip_image = { skip_node.attribute("x").as_int(),skip_node.attribute("y").as_int(),skip_node.attribute("w").as_int(),skip_node.attribute("h").as_int() };

	win_size = App->win->GetWindowSize();

	window = App->gui->UI_CreateWin({ 0,0 }, win_size.x, win_size.y, 1, false);

	iPoint margin = { logo_node.attribute("x_margin").as_int(),logo_node.attribute("y_margin").as_int() };

	skip = window->CreateImage({ win_size.x - skip_image.w - margin.x,win_size.y - skip_image.h - margin.y }, skip_image);

	App->render->background = {255, 255, 255};

	int logo_size = logo_node.attribute("size").as_int();

	App->video->PlayVideo("logo.ogv", { win_size.x / 2 - logo_size/2,win_size.y / 2 - logo_size / 2,logo_size,logo_size });

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
