#include "TeamSelectScene.h"
#include "j1Gui.h"
#include "j1Window.h"

TeamSelectScene::TeamSelectScene()
{
}

TeamSelectScene::~TeamSelectScene()
{
}

bool TeamSelectScene::Start()
{
	uint win_w, win_h;

	App->win->GetWindowSize(win_w, win_h);

	window = App->gui->UI_CreateWin(iPoint(0, 0), win_w, win_h, 2, false);

	pad_middle1 = window->CreateImage(iPoint(win_w / 2 - 62, win_h / 2 - 200), { 536,128,123,109 });
	pad_middle2 = window->CreateImage(iPoint(win_w / 2 - 62, win_h / 2 - 100), { 536,128,123,109 });
	pad_middle3 = window->CreateImage(iPoint(win_w / 2 - 62, win_h/2), { 536,128,123,109 });
	pad_middle4 = window->CreateImage(iPoint(win_w / 2 - 62, win_h / 2 + 100), { 536,128,123,109 });

	pad_view1 = window->CreateImage(iPoint(win_w / 4 - 62, win_h / 4-59), { 536,128,123,109 });
	pad_view2 = window->CreateImage(iPoint(3*win_w / 4 - 62, win_h / 4-59), { 536,128,123,109 });
	pad_view3 = window->CreateImage(iPoint(win_w / 4 - 62, 3*win_h / 4-59), { 536,128,123,109 });
	pad_view4 = window->CreateImage(iPoint(3*win_w / 4 - 62, 3*win_h / 4-59), { 536,128,123,109 });

	button_a = window->CreateImage(iPoint(win_w / 4 - 20, win_h / 4 - 40), { 656,72,40,40 });
	button_b = window->CreateImage(iPoint(3 * win_w / 4 - 20, win_h / 4 - 40), { 696,72,40,40 });
	button_x = window->CreateImage(iPoint(win_w / 4 - 20, 3 * win_h / 4 - 40), { 736,72,40,40 });
	button_y = window->CreateImage(iPoint(3 * win_w / 4 - 20, 3 * win_h / 4 - 40), { 776,72,40,40 });

	pad_1 = window->CreateImage(iPoint(win_w / 2 - 57, win_h / 2 - 200+5), { 424,132,112,81 });
	pad_2 = window->CreateImage(iPoint(win_w / 2 - 57, win_h / 2 - 100+5), { 424,132,112,81 });
	pad_3 = window->CreateImage(iPoint(win_w / 2 - 57, win_h / 2+5), { 424,132,112,81 });
	pad_4 = window->CreateImage(iPoint(win_w / 2 - 57, win_h / 2 + 100+5), { 424,132,112,81 });

	return true;
}

bool TeamSelectScene::PreUpdate()
{
	return true;
}

bool TeamSelectScene::Update(float dt)
{
	return true;
}

bool TeamSelectScene::PostUpdate()
{
	return true;
}

bool TeamSelectScene::CleanUp()
{
	return true;
}
