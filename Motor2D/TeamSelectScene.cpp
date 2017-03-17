#include "TeamSelectScene.h"
#include "j1Gui.h"
#include "j1Window.h"
#include "j1Input.h"
#include "j1Scene.h"
#include "j1Viewports.h"

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
	pad_middle1 = window->CreateImage(iPoint(win_w / 2 - 62, win_h / 2 - 200), { 280,107,124,88 });
	pad_middle2 = window->CreateImage(iPoint(win_w / 2 - 62, win_h / 2 - 100), { 280,107,124,88 });
	pad_middle3 = window->CreateImage(iPoint(win_w / 2 - 62, win_h/2), { 280,107,124,88 });
	pad_middle4 = window->CreateImage(iPoint(win_w / 2 - 62, win_h / 2 + 100), { 280,107,124,88 });
	middle_pads.push_back(pad_middle1);
	middle_pads.push_back(pad_middle2);
	middle_pads.push_back(pad_middle3);
	middle_pads.push_back(pad_middle4);

	// Views pads holes
	pad_view1 = window->CreateImage(iPoint(win_w / 4 - 62, win_h / 4-59), { 280,107,124,88 });
	pad_view2 = window->CreateImage(iPoint(3*win_w / 4 - 62, win_h / 4-59), { 280,107,124,88 });
	pad_view3 = window->CreateImage(iPoint(win_w / 4 - 62, 3*win_h / 4-59), { 280,107,124,88 });
	pad_view4 = window->CreateImage(iPoint(3*win_w / 4 - 62, 3*win_h / 4-59), { 280,107,124,88 });

	// Buttons images
	button_a = window->CreateImage(iPoint(win_w / 4 - 20, win_h / 4 - 40), { 164,195,40,40 });
	button_b = window->CreateImage(iPoint(3 * win_w / 4 - 20, win_h / 4 - 40), { 204,195,40,40 });
	button_x = window->CreateImage(iPoint(win_w / 4 - 20, 3 * win_h / 4 - 40), { 244,195,40,40 });
	button_y = window->CreateImage(iPoint(3 * win_w / 4 - 20, 3 * win_h / 4 - 40), { 284,195,40,40 });

	// Pads
	pad_1 = window->CreateImage(iPoint(win_w / 2 - 58, win_h / 2 - 200+4), { 164,111,116,80 });
	pad_2 = window->CreateImage(iPoint(win_w / 2 - 58, win_h / 2 - 100+4), { 164,111,116,80 });
	pad_3 = window->CreateImage(iPoint(win_w / 2 - 58, win_h / 2+4), { 164,111,116,80 });
	pad_4 = window->CreateImage(iPoint(win_w / 2 - 58, win_h / 2 + 100+4), { 164,111,116,80 });
	pads.push_back(pad_1);
	pads.push_back(pad_2);
	pads.push_back(pad_3);
	pads.push_back(pad_4);

	// RB Button
	rb_view1 = window->CreateImage(iPoint(win_w / 4 - 58, win_h / 4 - 90), { 164,259,48,24 });
	rb_view2 = window->CreateImage(iPoint(3*win_w / 4 - 58, win_h / 4 - 90), { 164,259,48,24 });
	rb_view3 = window->CreateImage(iPoint(win_w / 4 - 58, 3*win_h / 4 - 90), { 164,259,48,24 });
	rb_view4 = window->CreateImage(iPoint(3*win_w / 4 - 58, 3*win_h / 4 - 90), { 164,259,48,24 });

	// LB Button
	lb_view1 = window->CreateImage(iPoint(win_w / 4 + 5, win_h / 4 - 90), { 212,259,48,24 });
	lb_view2 = window->CreateImage(iPoint(3 * win_w / 4 + 5, win_h / 4 - 90), { 212,259,48,24 });
	lb_view3 = window->CreateImage(iPoint(win_w / 4 + 5, 3 * win_h / 4 - 90), { 212,259,48,24 });
	lb_view4 = window->CreateImage(iPoint(3 * win_w / 4 + 5, 3 * win_h / 4 - 90), { 212,259,48,24 });

	// TEAM1 Buttons
	team1_view1 = window->CreateImage(iPoint(win_w / 4 - 58, win_h / 4 - 90), { 164,235,48,24 });
	team1_view2 = window->CreateImage(iPoint(3 * win_w / 4 - 58, win_h / 4 - 90), { 164,235,48,24 });
	team1_view3 = window->CreateImage(iPoint(win_w / 4 - 58, 3 * win_h / 4 - 90), { 164,235,48,24 });
	team1_view4 = window->CreateImage(iPoint(3 * win_w / 4 - 58, 3 * win_h / 4 - 90), { 164,235,48,24 });
	team1_view1->enabled = false;
	team1_view2->enabled = false;
	team1_view3->enabled = false;
	team1_view4->enabled = false;

	// TEAM2 Buttons
	team2_view1 = window->CreateImage(iPoint(win_w / 4 + 5, win_h / 4 - 90), { 212,235,48,24 });
	team2_view2 = window->CreateImage(iPoint(3 * win_w / 4 + 5, win_h / 4 - 90), { 212,235,48,24 });
	team2_view3 = window->CreateImage(iPoint(win_w / 4 + 5, 3 * win_h / 4 - 90), { 212,235,48,24 });
	team2_view4 = window->CreateImage(iPoint(3 * win_w / 4 + 5, 3 * win_h / 4 - 90), { 212,235,48,24 });
	team2_view1->enabled = false;
	team2_view2->enabled = false;
	team2_view3->enabled = false;
	team2_view4->enabled = false;

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

	//READY text
	ready_text1 = window->CreateText(iPoint(win_w / 4 - 25, win_h / 4 + 50), App->font->game_font);
	ready_text2 = window->CreateText(iPoint(3 * win_w / 4 - 25, win_h / 4 + 50), App->font->game_font);
	ready_text3 = window->CreateText(iPoint(win_w / 4 - 25, 3 * win_h / 4 + 50), App->font->game_font);
	ready_text4 = window->CreateText(iPoint(3 * win_w / 4 - 25, 3 * win_h / 4 + 50), App->font->game_font);
	ready_text1->SetText(string("READY"));
	ready_text2->SetText(string("READY"));
	ready_text3->SetText(string("READY"));
	ready_text4->SetText(string("READY"));
	ready_text1->enabled = false;
	ready_text2->enabled = false;
	ready_text3->enabled = false;
	ready_text4->enabled = false;
	ready_texts.push_back(ready_text1);
	ready_texts.push_back(ready_text2);
	ready_texts.push_back(ready_text3);
	ready_texts.push_back(ready_text4);

	//Set Ready texts
	set_ready_text1 = window->CreateText(iPoint(win_w / 4 - 105, win_h / 4 + 50), App->font->game_font);
	set_ready_text2 = window->CreateText(iPoint(3 * win_w / 4 - 105, win_h / 4 + 50), App->font->game_font);
	set_ready_text3 = window->CreateText(iPoint(win_w / 4 - 105, 3 * win_h / 4 + 50), App->font->game_font);
	set_ready_text4 = window->CreateText(iPoint(3 * win_w / 4 - 105, 3 * win_h / 4 + 50), App->font->game_font);
	set_ready_text1->SetText(string("Press START when Ready"));
	set_ready_text2->SetText(string("Press START when Ready"));
	set_ready_text3->SetText(string("Press START when Ready"));
	set_ready_text4->SetText(string("Press START when Ready"));
	set_ready_text1->enabled = false;
	set_ready_text2->enabled = false;
	set_ready_text3->enabled = false;
	set_ready_text4->enabled = false;
	set_ready_texts.push_back(set_ready_text1);
	set_ready_texts.push_back(set_ready_text2);
	set_ready_texts.push_back(set_ready_text3);
	set_ready_texts.push_back(set_ready_text4);

	return true;
}

bool TeamSelectScene::PreUpdate()
{
	return true;
}

bool TeamSelectScene::Update(float dt)
{
	DrawScreenSeparation();

	for (int i = 0; i < 4; i++)
	{
		// VIEW 1 - Button A
		if (App->input->GetControllerButton(i, SDL_CONTROLLER_BUTTON_A) == KEY_DOWN)
		{
			if (positioned[0] == false && App->scene->players[i].viewport == 5)
			{
				button_a->SetPos({ pads[i]->GetPos().x + 36,pads[i]->GetPos().y + 15 });
				pads[i]->SetPos({ pad_view1->GetPos().x + 4,pad_view1->GetPos().y + 4 });
				positioned[0] = true;
				App->scene->players[i].viewport = 1;
				set_ready_text1->enabled = true;
				team1_view1->enabled = true;
				team2_view1->enabled = true;
			}
			else if (App->scene->players[i].viewport == 1)
			{
				positioned[0] = false;
				App->scene->players[i].viewport = 5;
				button_a->SetPos({ pads[i]->GetPos().x + 36,pads[i]->GetPos().y + 15 });
				pads[i]->SetPos({ middle_pads[i]->GetPos().x + 4,middle_pads[i]->GetPos().y + 4 });
				team_num_view1->SetText(string("X"));
				App->scene->players[i].team = 3;
				set_ready_text1->enabled = false;
				ready_text1->enabled = false;
				team1_view1->enabled = false;
				team2_view1->enabled = false;
			}
		}

		// VIEW 2 - Button B
		if (App->input->GetControllerButton(i, SDL_CONTROLLER_BUTTON_B) == KEY_DOWN)
		{
			if (positioned[1] == false && App->scene->players[i].viewport == 5)
			{
				button_b->SetPos({ pads[i]->GetPos().x + 36,pads[i]->GetPos().y + 15 });
				pads[i]->SetPos({ pad_view2->GetPos().x + 4,pad_view2->GetPos().y + 4 });
				positioned[1] = true;
				App->scene->players[i].viewport = 2;
				set_ready_text2->enabled = true;
				team1_view2->enabled = true;
				team2_view2->enabled = true;
			}
			else if (App->scene->players[i].viewport == 2)
			{
				positioned[1] = false;
				App->scene->players[i].viewport = 5;
				button_b->SetPos({ pads[i]->GetPos().x + 36,pads[i]->GetPos().y + 15 });
				pads[i]->SetPos({ middle_pads[i]->GetPos().x + 4,middle_pads[i]->GetPos().y + 4 });
				team_num_view2->SetText(string("X"));
				App->scene->players[i].team = 3;
				set_ready_text2->enabled = false;
				ready_text2->enabled = false;
				team1_view2->enabled = false;
				team2_view2->enabled = false;
			}
		}

		// VIEW 3 - Button X
		if (App->input->GetControllerButton(i, SDL_CONTROLLER_BUTTON_X) == KEY_DOWN)
		{
			if (positioned[2] == false && App->scene->players[i].viewport == 5)
			{
				button_x->SetPos({ pads[i]->GetPos().x + 36,pads[i]->GetPos().y + 15 });
				pads[i]->SetPos({ pad_view3->GetPos().x + 4,pad_view3->GetPos().y + 4 });
				positioned[2] = true;
				App->scene->players[i].viewport = 3;
				set_ready_text3->enabled = true;
				team1_view3->enabled = true;
				team2_view3->enabled = true;
			}
			else if (App->scene->players[i].viewport == 3)
			{
				positioned[2] = false;
				App->scene->players[i].viewport = 5;
				button_x->SetPos({ pads[i]->GetPos().x + 36,pads[i]->GetPos().y + 15 });
				pads[i]->SetPos({ middle_pads[i]->GetPos().x + 4,middle_pads[i]->GetPos().y + 4 });
				team_num_view3->SetText(string("X"));
				App->scene->players[i].team = 3;
				set_ready_text3->enabled = false;
				ready_text3->enabled = false;
				team1_view3->enabled = false;
				team2_view3->enabled = false;
			}
		}

		// VIEW 4 - Button Y
		if (App->input->GetControllerButton(i, SDL_CONTROLLER_BUTTON_Y) == KEY_DOWN)
		{
			if (positioned[3] == false && App->scene->players[i].viewport == 5)
			{
				button_y->SetPos({ pads[i]->GetPos().x + 36,pads[i]->GetPos().y + 15 });
				pads[i]->SetPos({ pad_view4->GetPos().x + 4,pad_view4->GetPos().y + 4 });
				positioned[3] = true;
				App->scene->players[i].viewport = 4;
				set_ready_text4->enabled = true;
				team1_view4->enabled = true;
				team2_view4->enabled = true;
			}
			else if (App->scene->players[i].viewport == 4)
			{
				positioned[3] = false;
				App->scene->players[i].viewport = 5;
				button_y->SetPos({ pads[i]->GetPos().x + 36,pads[i]->GetPos().y + 15 });
				pads[i]->SetPos({ middle_pads[i]->GetPos().x + 4,middle_pads[i]->GetPos().y + 4 });
				team_num_view4->SetText(string("X"));
				App->scene->players[i].team = 3;
				set_ready_text4->enabled = false;
				ready_text4->enabled = false;
				team1_view4->enabled = false;
				team2_view4->enabled = false;
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

		if (App->scene->players[i].viewport != 5 && App->scene->players[i].team != 3)
		{
			if (App->input->GetControllerButton(i, SDL_CONTROLLER_BUTTON_START) == KEY_DOWN)
			{
				ready[i] = true;
				ready_texts[App->scene->players[i].viewport - 1]->enabled = true;
				set_ready_texts[App->scene->players[i].viewport-1]->enabled = false;
			}
		}
		else
		{
			ready[i] = false;
		}
	}

	return true;
}

bool TeamSelectScene::PostUpdate()
{
	team1_members = 0;
	team2_members = 0;

	bool all_ready = true;

	for (int i = 0; i < 4; i++)
	{
		if (App->scene->players[i].team == 1)
			team1_members++;
		if (App->scene->players[i].team == 2)
			team2_members++;

		if (all_ready)
			all_ready = ready[i];
	}

	if (all_ready)
		App->scene->ChangeScene((Scene*)App->scene->main_scene);

	//FOR TESTING
	if(App->input->GetControllerButton(0,SDL_CONTROLLER_BUTTON_BACK) == KEY_DOWN)
		App->scene->ChangeScene((Scene*)App->scene->charselect_screen);



	return true;
}

bool TeamSelectScene::CleanUp()
{
	// Free UI
	if (App->scene->GetCurrentScene() != App->scene->team_select)
		App->gui->DeleteElement(window);
	// -------

	return true;
}

void TeamSelectScene::DrawScreenSeparation()
{
	uint win_w, win_h;
	App->win->GetWindowSize(win_w, win_h);

	for (uint y = 0; y < win_h - 2; y += 2)
	{
		App->view->LayerBlit(1, App->gui->atlas, iPoint(win_w/4-2, y), { 130,0,4,2 });
	}

	for (uint x = 0; x < win_w - 2; x += 2)
	{
		App->view->LayerBlit(1, App->gui->atlas, iPoint(x, win_h/4-4), { 130,2,2,4 });
	}

	App->view->LayerBlit(1, App->gui->atlas, iPoint(win_w / 4 - 2, win_h / 4 - 6), { 130,0,6,6 });

}