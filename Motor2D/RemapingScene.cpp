#include "RemapingScene.h"
#include "j1Window.h"
#include "j1Input.h"
#include "j1Textures.h"

#define SPACING_BETWEEN_BUTTONS 80
#define COLUMNS_SEPARATIONS 300

RemapingScene::RemapingScene()
{
}

RemapingScene::~RemapingScene()
{
}

bool RemapingScene::Start()
{
	
	SDL_Rect viewport_rect = App->view->GetViewportRect(1); 

	uint w, h;
	App->win->GetWindowSize(w, h); 

	window = App->gui->UI_CreateWin({0,0} ,w ,h ,0,false);

	SDL_Rect placeholder_trigger = { 490, 2274, 48, 24 }; 

	SDL_Rect A_button_rect = { 164, 195, 40, 40 }; 
	SDL_Rect B_button_rect = { 204, 195, 40, 40 };
	SDL_Rect X_button_rect = { 244, 195, 40, 40 };
	SDL_Rect Y_button_rect = { 284, 195, 40, 40 };

	SDL_Rect RB_trigger_rect = {212, 259, 48, 24}; 
	SDL_Rect LB_trigger_rect = {164, 259, 48, 24};
	SDL_Rect RT_trigger_rect = placeholder_trigger;
	SDL_Rect LT_trigger_rect = placeholder_trigger;

	SDL_Rect cursor_rect = { 80, 52, 48, 48 };

	SDL_Rect rectangle_background_rect = {490, 2272, 150, 45}; 	
	iPoint confirm_background_pos = { viewport_rect.w/ 8, viewport_rect.h/ 6};
	iPoint back_background_pos = { confirm_background_pos.x, confirm_background_pos.y + SPACING_BETWEEN_BUTTONS };
	iPoint minimap_background_pos = { back_background_pos.x, back_background_pos.y + SPACING_BETWEEN_BUTTONS };
	iPoint shop_background_pos = { minimap_background_pos.x, minimap_background_pos.y + SPACING_BETWEEN_BUTTONS };
	iPoint a1_background_pos = { confirm_background_pos.x + COLUMNS_SEPARATIONS, confirm_background_pos.y};
	iPoint a2_background_pos = { a1_background_pos.x, a1_background_pos.y + SPACING_BETWEEN_BUTTONS };
	iPoint a3_background_pos = { a2_background_pos.x, a2_background_pos.y + SPACING_BETWEEN_BUTTONS };
	iPoint a4_background_pos = { a3_background_pos.x, a3_background_pos.y + SPACING_BETWEEN_BUTTONS };

	int offset_x = 0;
	int offset_y = 0;

	for (int i = 0; i < 4; i++)
	{
		switch (i)
		{
			case 0:
			{
				offset_x = 0;
				offset_y = 0;		
			}
				break;				
		case 1: 
			{
				offset_x = viewport_rect.w;
				offset_y = 0;
			}
				break;
		case 2:
			{
				offset_x = 0;
				offset_y = viewport_rect.h; 
			}
				break; 
		case 3:
			{
				offset_x = viewport_rect.w;
				offset_y = viewport_rect.h;			
			}
				break;
		}

		remapping_ui[i].confirm_background = window->CreateImage({ confirm_background_pos.x + offset_x, confirm_background_pos.y + offset_y }, rectangle_background_rect);
		remapping_ui[i].back_background = window->CreateImage({ back_background_pos.x + offset_x, back_background_pos.y + offset_y }, rectangle_background_rect);
		remapping_ui[i].minimap_background = window->CreateImage({ minimap_background_pos.x + offset_x, minimap_background_pos.y + offset_y }, rectangle_background_rect);
		remapping_ui[i].shop_background = window->CreateImage({ shop_background_pos.x + offset_x, shop_background_pos.y + offset_y }, rectangle_background_rect);
		remapping_ui[i].a1_background = window->CreateImage({ a1_background_pos.x + offset_x, a1_background_pos.y + offset_y }, rectangle_background_rect);
		remapping_ui[i].a2_background = window->CreateImage({ a2_background_pos.x + offset_x, a2_background_pos.y + offset_y }, rectangle_background_rect);
		remapping_ui[i].a3_background = window->CreateImage({ a3_background_pos.x + offset_x, a3_background_pos.y + offset_y }, rectangle_background_rect);
		remapping_ui[i].a4_background = window->CreateImage({ a4_background_pos.x + offset_x, a4_background_pos.y + offset_y }, rectangle_background_rect);
		remapping_ui[i].cursor = window->CreateImage({ GetCursorPosFromCurrentState(remapping_ui[i].current_state).x + offset_x, GetCursorPosFromCurrentState(remapping_ui[i].current_state).y + offset_y }, cursor_rect);
	}

	background_image = App->tex->LoadTexture("gui/intro_background.png");

	return false;
}

bool RemapingScene::Update(float dt)
{

	for (int i = 0; i < 4; i++)
	{
		if (App->input->GetControllerButton(i, SDL_CONTROLLER_BUTTON_DPAD_UP) == KEY_DOWN)
		{
			if (remapping_ui[i].current_state != r_s_confirm)
			{
				int curr = remapping_ui[i].current_state;
				curr--; 

				remapping_ui[i].current_state = static_cast<remaping_state> (curr);

				remapping_ui[i].cursor->SetPos(GetCursorPosFromCurrentState(remapping_ui[i].current_state));
			}
				
		}

		if (App->input->GetControllerButton(i, SDL_CONTROLLER_BUTTON_DPAD_DOWN) == KEY_DOWN)
		{
			if (remapping_ui[i].current_state != r_s_a4)
			{
				int curr = remapping_ui[i].current_state;
				curr++;

				remapping_ui[i].current_state = static_cast<remaping_state> (curr);

				remapping_ui[i].cursor->SetPos(GetCursorPosFromCurrentState(remapping_ui[i].current_state));
			}

		}
	}

	App->view->LayerBlit(0, background_image, App->view->camera1, { 0,0,1994, 1359 }, 0, 1);

	return false;
}

bool RemapingScene::CleanUp()
{
	return false;
}

iPoint RemapingScene::GetCursorPosFromCurrentState(remaping_state curr_state)
{
	iPoint ret = NULLPOINT; 

	switch (curr_state) 
	{

	case r_s_confirm:
		ret = {20, 60}; 
			break; 

	case r_s_back:
		ret = { 20, 140 };
		break;

	case r_s_minimap:
		ret = { 20, 220 };
		break;

	case r_s_shop:
		ret = { 20, 300 };
		break;

	case r_s_a1:
		ret = { 320, 60 };
		break;

	case r_s_a2:
		ret = { 320, 140 };
		break;

	case r_s_a3:
		ret = { 320, 220 };
		break;

	case r_s_a4:
		ret = { 320, 300};
		break;


	}

	return ret;
}
