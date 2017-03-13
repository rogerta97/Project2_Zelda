#include "TeamSelectScene.h"
#include "j1Gui.h"
#include "j1Window.h"
#include "j1Input.h"
#include "j1Scene.h"

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

	// Middle pads holes
	pad_middle1 = window->CreateImage(iPoint(win_w / 2 - 62, win_h / 2 - 200), { 536,128,123,109 });
	pad_middle2 = window->CreateImage(iPoint(win_w / 2 - 62, win_h / 2 - 100), { 536,128,123,109 });
	pad_middle3 = window->CreateImage(iPoint(win_w / 2 - 62, win_h/2), { 536,128,123,109 });
	pad_middle4 = window->CreateImage(iPoint(win_w / 2 - 62, win_h / 2 + 100), { 536,128,123,109 });
	middle_pads.push_back(pad_middle1);
	middle_pads.push_back(pad_middle2);
	middle_pads.push_back(pad_middle3);
	middle_pads.push_back(pad_middle4);

	// Views pads holes
	pad_view1 = window->CreateImage(iPoint(win_w / 4 - 62, win_h / 4-59), { 536,128,123,109 });
	pad_view2 = window->CreateImage(iPoint(3*win_w / 4 - 62, win_h / 4-59), { 536,128,123,109 });
	pad_view3 = window->CreateImage(iPoint(win_w / 4 - 62, 3*win_h / 4-59), { 536,128,123,109 });
	pad_view4 = window->CreateImage(iPoint(3*win_w / 4 - 62, 3*win_h / 4-59), { 536,128,123,109 });

	// Buttons images
	button_a = window->CreateImage(iPoint(win_w / 4 - 20, win_h / 4 - 40), { 656,72,40,40 });
	button_b = window->CreateImage(iPoint(3 * win_w / 4 - 20, win_h / 4 - 40), { 696,72,40,40 });
	button_x = window->CreateImage(iPoint(win_w / 4 - 20, 3 * win_h / 4 - 40), { 736,72,40,40 });
	button_y = window->CreateImage(iPoint(3 * win_w / 4 - 20, 3 * win_h / 4 - 40), { 776,72,40,40 });

	// Pads
	pad_1 = window->CreateImage(iPoint(win_w / 2 - 57, win_h / 2 - 200+5), { 424,132,112,81 });
	pad_2 = window->CreateImage(iPoint(win_w / 2 - 57, win_h / 2 - 100+5), { 424,132,112,81 });
	pad_3 = window->CreateImage(iPoint(win_w / 2 - 57, win_h / 2+5), { 424,132,112,81 });
	pad_4 = window->CreateImage(iPoint(win_w / 2 - 57, win_h / 2 + 100+5), { 424,132,112,81 });
	pads.push_back(pad_1);
	pads.push_back(pad_2);
	pads.push_back(pad_3);
	pads.push_back(pad_4);

	//TEAM text
	team_view1 = window->CreateText(iPoint(win_w / 4 - 30, 15), App->font->game_font);
	team_view2 = window->CreateText(iPoint(3 * win_w / 4 - 30, 15), App->font->game_font);
	team_view3 = window->CreateText(iPoint(win_w / 4 - 30, win_h/2 + 15), App->font->game_font);
	team_view4 = window->CreateText(iPoint(3 * win_w / 4 - 30, win_h / 2 + 15), App->font->game_font);
	team_view1->SetText(string("TEAM"));
	team_view2->SetText(string("TEAM"));
	team_view3->SetText(string("TEAM"));
	team_view4->SetText(string("TEAM"));

	//Team number
	team_num_view1 = window->CreateText(iPoint(win_w / 4 + 20, 15), App->font->game_font);
	team_num_view2 = window->CreateText(iPoint(3 * win_w / 4 + 20, 15), App->font->game_font);
	team_num_view3 = window->CreateText(iPoint(win_w / 4 + 20, win_h / 2 + 15), App->font->game_font);
	team_num_view4 = window->CreateText(iPoint(3 * win_w / 4 + 20, win_h / 2 + 15), App->font->game_font);
	team_num_view1->SetText(string("X"));
	team_num_view2->SetText(string("X"));
	team_num_view3->SetText(string("X"));
	team_num_view4->SetText(string("X"));
	team_nums.push_back(team_num_view1);
	team_nums.push_back(team_num_view2);
	team_nums.push_back(team_num_view3);
	team_nums.push_back(team_num_view4);

	return true;
}

bool TeamSelectScene::PreUpdate()
{
	return true;
}

bool TeamSelectScene::Update(float dt)
{
	for (int i = 0; i < 4; i++)
	{
		// VIEW 1 - Button A
		if (App->input->GetControllerButton(i, SDL_CONTROLLER_BUTTON_A) == KEY_DOWN)
		{
			if (positioned[0] == false)
			{
				button_a->SetPos({ pads[i]->GetPos().x + 36,pads[i]->GetPos().y + 15 });
				pads[i]->SetPos({ pad_view1->GetPos().x + 5,pad_view1->GetPos().y + 4 });
				positioned[0] = true;
				App->scene->players[i].viewport = 1;
			}
			else if (App->scene->players[i].viewport == 1)
			{
				positioned[0] = false;
				App->scene->players[i].viewport = 5;
				button_a->SetPos({ pads[i]->GetPos().x + 36,pads[i]->GetPos().y + 15 });
				pads[i]->SetPos({ middle_pads[i]->GetPos().x + 5,middle_pads[i]->GetPos().y + 4 });
				team_num_view1->SetText(string("X"));
				App->scene->players[i].team = 3;
			}
		}

		// VIEW 2 - Button B
		if (App->input->GetControllerButton(i, SDL_CONTROLLER_BUTTON_B) == KEY_DOWN)
		{
			if (positioned[1] == false)
			{
				button_b->SetPos({ pads[i]->GetPos().x + 36,pads[i]->GetPos().y + 15 });
				pads[i]->SetPos({ pad_view2->GetPos().x + 5,pad_view2->GetPos().y + 4 });
				positioned[1] = true;
				App->scene->players[i].viewport = 2;
			}
			else if (App->scene->players[i].viewport == 2)
			{
				positioned[1] = false;
				App->scene->players[i].viewport = 5;
				button_b->SetPos({ pads[i]->GetPos().x + 36,pads[i]->GetPos().y + 15 });
				pads[i]->SetPos({ middle_pads[i]->GetPos().x + 5,middle_pads[i]->GetPos().y + 4 });
				team_num_view2->SetText(string("X"));
				App->scene->players[i].team = 3;
			}
		}

		// VIEW 3 - Button X
		if (App->input->GetControllerButton(i, SDL_CONTROLLER_BUTTON_X) == KEY_DOWN)
		{
			if (positioned[2] == false)
			{
				button_x->SetPos({ pads[i]->GetPos().x + 36,pads[i]->GetPos().y + 15 });
				pads[i]->SetPos({ pad_view3->GetPos().x + 5,pad_view3->GetPos().y + 4 });
				positioned[2] = true;
				App->scene->players[i].viewport = 3;
			}
			else if (App->scene->players[i].viewport == 3)
			{
				positioned[2] = false;
				App->scene->players[i].viewport = 5;
				button_x->SetPos({ pads[i]->GetPos().x + 36,pads[i]->GetPos().y + 15 });
				pads[i]->SetPos({ middle_pads[i]->GetPos().x + 5,middle_pads[i]->GetPos().y + 4 });
				team_num_view3->SetText(string("X"));
				App->scene->players[i].team = 3;
			}
		}

		// VIEW 4 - Button Y
		if (App->input->GetControllerButton(i, SDL_CONTROLLER_BUTTON_Y) == KEY_DOWN)
		{
			if (positioned[3] == false)
			{
				button_y->SetPos({ pads[i]->GetPos().x + 36,pads[i]->GetPos().y + 15 });
				pads[i]->SetPos({ pad_view4->GetPos().x + 5,pad_view4->GetPos().y + 4 });
				positioned[3] = true;
				App->scene->players[i].viewport = 4;
			}
			else if (App->scene->players[i].viewport == 4)
			{
				positioned[3] = false;
				App->scene->players[i].viewport = 5;
				button_y->SetPos({ pads[i]->GetPos().x + 36,pads[i]->GetPos().y + 15 });
				pads[i]->SetPos({ middle_pads[i]->GetPos().x + 5,middle_pads[i]->GetPos().y + 4 });
				team_num_view4->SetText(string("X"));
				App->scene->players[i].team = 3;
			}
		}

		//set team to positioned gamepad
		if (App->scene->players[i].viewport != 5)
		{
			if (App->input->GetControllerButton(i, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) == KEY_DOWN)
			{
				if (positioned[App->scene->players[i].viewport-1] == true && team2_members < 2)
				{
					team_nums[App->scene->players[i].viewport-1]->SetText(string("2"));
					App->scene->players[i].team = 2;
				}
			}

			if (App->input->GetControllerButton(i, SDL_CONTROLLER_BUTTON_LEFTSHOULDER) == KEY_DOWN)
			{
				if (positioned[App->scene->players[i].viewport-1] == true && team1_members < 2)
				{
					team_nums[App->scene->players[i].viewport-1]->SetText(string("1"));
					App->scene->players[i].team = 1;
				}
			}
		}
	}

	return true;
}

bool TeamSelectScene::PostUpdate()
{
	team1_members = 0;
	team2_members = 0;

	for (int i = 0; i < 4; i++)
	{
		if (App->scene->players[i].team == 1)
			team1_members++;
		if (App->scene->players[i].team == 2)
			team2_members++;
	}

	return true;
}

bool TeamSelectScene::CleanUp()
{
	team_nums.clear();
	pads.clear();
	middle_pads.clear();

	App->gui->DeleteElement(window);

	return true;
}
