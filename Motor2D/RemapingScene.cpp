#include "RemapingScene.h"
#include "j1Window.h"
#include "j1Input.h"
#include "Mapping.h"
#include "j1Scene.h"
#include "j1Textures.h"

#define SPACING_BETWEEN_BUTTONS 70
#define COLUMNS_SEPARATIONS 350

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
	SDL_Rect RT_trigger_rect = {308, 259, 48, 24};
	SDL_Rect LT_trigger_rect = {260, 259, 48, 23};

	SDL_Rect cursor_rect = {80,52, 48,48 };
	SDL_Rect cursor_selector_rect = { 928, 2060, 47, 48}; 
	SDL_Rect button_suport_rect = {858, 1959, 70, 184}; 

	SDL_Rect up_left_pick = { 928, 2003, 40, 28 };


	SDL_Rect rectangle_background_rect = { 0, 2368, 136, 50};

	int block_w = (cursor_rect.w * 2) + 10 + rectangle_background_rect.w*2 + (COLUMNS_SEPARATIONS - rectangle_background_rect.w - 100);
	int block_h = rectangle_background_rect.h * 4 + ((SPACING_BETWEEN_BUTTONS - rectangle_background_rect.h) * 4);

	iPoint confirm_background_pos = { viewport_rect.w/ 2 - block_w/2, viewport_rect.h/2 - block_h/2};
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

		// Creating the background of the buttons

		remapping_ui[i].confirm_background = window->CreateImage({ confirm_background_pos.x + offset_x, confirm_background_pos.y + offset_y }, rectangle_background_rect);
		remapping_ui[i].back_background = window->CreateImage({ back_background_pos.x + offset_x, back_background_pos.y + offset_y }, rectangle_background_rect);
		remapping_ui[i].minimap_background = window->CreateImage({ minimap_background_pos.x + offset_x, minimap_background_pos.y + offset_y }, rectangle_background_rect);
		remapping_ui[i].shop_background = window->CreateImage({ shop_background_pos.x + offset_x, shop_background_pos.y + offset_y }, rectangle_background_rect);
		remapping_ui[i].a1_background = window->CreateImage({ a1_background_pos.x + offset_x, a1_background_pos.y + offset_y }, rectangle_background_rect);
		remapping_ui[i].a2_background = window->CreateImage({ a2_background_pos.x + offset_x, a2_background_pos.y + offset_y }, rectangle_background_rect);
		remapping_ui[i].a3_background = window->CreateImage({ a3_background_pos.x + offset_x, a3_background_pos.y + offset_y }, rectangle_background_rect);
		remapping_ui[i].a4_background = window->CreateImage({ a4_background_pos.x + offset_x, a4_background_pos.y + offset_y }, rectangle_background_rect);
		remapping_ui[i].button_support = window->CreateImage({ 0,0 }, button_suport_rect);
		remapping_ui[i].button_support->enabled = false;
		remapping_ui[i].cursor = window->CreateImage({ GetCursorPosFromCurrentState(remapping_ui[i].current_state).x + offset_x, GetCursorPosFromCurrentState(remapping_ui[i].current_state).y + offset_y }, cursor_rect);
		remapping_ui[i].button_selector_cursor = window->CreateImage( {0,0}, cursor_selector_rect);
		remapping_ui[i].button_selector_cursor->enabled = false; 
		remapping_ui[i].support_pick = window->CreateImage({0,0}, up_left_pick);
		remapping_ui[i].support_pick->enabled = false; 

		// ----

		// Setting text to the buttons 

		remapping_ui[i].confirm_text = window->CreateText({ confirm_background_pos.x + 25 + offset_x, confirm_background_pos.y + 8 + offset_y}, App->font->game_font);
		remapping_ui[i].confirm_text->SetText("CONFIRM");
		remapping_ui[i].back_text = window->CreateText({ back_background_pos.x + 43 + offset_x, back_background_pos.y + 8 + offset_y }, App->font->game_font);
		remapping_ui[i].back_text->SetText("BACK");
		remapping_ui[i].minimap_text = window->CreateText({ minimap_background_pos.x + 26 + offset_x, minimap_background_pos.y + 8 + offset_y }, App->font->game_font);
		remapping_ui[i].minimap_text->SetText("MINIMAP");
		remapping_ui[i].shop_text = window->CreateText({ shop_background_pos.x + 43 + offset_x, shop_background_pos.y + 8 + offset_y }, App->font->game_font);
		remapping_ui[i].shop_text->SetText("SHOP");
		remapping_ui[i].a1_text = window->CreateText({ a1_background_pos.x + 40 + offset_x, a1_background_pos.y + 8 + offset_y }, App->font->game_font);
		remapping_ui[i].a1_text->SetText("HAB. 1");							 
		remapping_ui[i].a2_text = window->CreateText({ a2_background_pos.x + 40 + offset_x, a2_background_pos.y + 8 + offset_y }, App->font->game_font);
		remapping_ui[i].a2_text->SetText("HAB. 2");							  									  
		remapping_ui[i].a3_text = window->CreateText({ a3_background_pos.x + 40 + offset_x, a3_background_pos.y + 8 + offset_y }, App->font->game_font);
		remapping_ui[i].a3_text->SetText("HAB. 3");							  									  
		remapping_ui[i].a4_text = window->CreateText({ a4_background_pos.x + 40 + offset_x, a4_background_pos.y + 8 + offset_y }, App->font->game_font);
		remapping_ui[i].a4_text->SetText("HAB. 4");

		// ----

		// Setting rects of the buttons

		remapping_ui[i].a = window->CreateImage( {0,0}, A_button_rect);
		remapping_ui[i].a->enabled = false; 
		remapping_ui[i].x = window->CreateImage({ 0,0 }, X_button_rect);
		remapping_ui[i].x->enabled = false;
		remapping_ui[i].y = window->CreateImage({ 0,0 }, Y_button_rect);
		remapping_ui[i].y->enabled = false;
		remapping_ui[i].b = window->CreateImage({ 0,0 }, B_button_rect);
		remapping_ui[i].b->enabled = false;

		remapping_ui[i].rb = window->CreateImage({ 0,0 }, RB_trigger_rect);
		remapping_ui[i].rb->enabled = false;
		remapping_ui[i].rt = window->CreateImage({ 0,0 }, RT_trigger_rect);
		remapping_ui[i].rt->enabled = false;
		remapping_ui[i].lb = window->CreateImage({ 0,0 }, LB_trigger_rect);
		remapping_ui[i].lb->enabled = false;
		remapping_ui[i].lt = window->CreateImage({ 0,0 }, LT_trigger_rect);
		remapping_ui[i].lt->enabled = false;

		// ----

		// Setting current keys of the players 

		remapping_ui[i].confirm_key = window->CreateImage({ confirm_background_pos.x + rectangle_background_rect.w + 5 + offset_x, confirm_background_pos.y + offset_y }, {0,0,0,0});
		remapping_ui[i].back_key = window->CreateImage({ back_background_pos.x + rectangle_background_rect.w + 5 + offset_x, back_background_pos.y + offset_y }, { 0,0,0,0 });
		remapping_ui[i].minimap_key = window->CreateImage({ minimap_background_pos.x + rectangle_background_rect.w + 5 + offset_x, minimap_background_pos.y + offset_y }, { 0,0,0,0 });
		remapping_ui[i].shop_key = window->CreateImage({ shop_background_pos.x + rectangle_background_rect.w + 5 + offset_x, shop_background_pos.y + offset_y }, { 0,0,0,0 });
		remapping_ui[i].a1_key = window->CreateImage({ a1_background_pos.x - 55 + offset_x, a1_background_pos.y + offset_y }, { 0,0,0,0 });
		remapping_ui[i].a2_key = window->CreateImage({ a2_background_pos.x - 55 + offset_x, a2_background_pos.y + offset_y }, { 0,0,0,0 });
		remapping_ui[i].a3_key = window->CreateImage({ a3_background_pos.x - 55 + offset_x, a3_background_pos.y + offset_y }, { 0,0,0,0 });
		remapping_ui[i].a4_key = window->CreateImage({ a4_background_pos.x - 55 + offset_x, a4_background_pos.y + offset_y}, { 0,0,0,0 });

		UpdateKeys(i); 
		
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
			if (inside)
			{
				MoveSelectorCursor(true, i);
				remapping_ui[i].curr_inside_pos--;
			}

			else
			{
				if (remapping_ui[i].current_state != r_s_confirm)
				{
					int curr = remapping_ui[i].current_state;
					curr--;

					remapping_ui[i].current_state = static_cast<remaping_state> (curr);

					remapping_ui[i].cursor->SetPos(GetCursorPosFromCurrentState(remapping_ui[i].current_state));
				}
			}
				
		}

		if (App->input->GetControllerButton(i, SDL_CONTROLLER_BUTTON_DPAD_DOWN) == KEY_DOWN)
		{
			if (inside)
			{
				MoveSelectorCursor(false, i); 
				remapping_ui[i].curr_inside_pos++; 
			}

			else
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

		if (App->input->GetControllerButton(i, SDL_CONTROLLER_BUTTON_DPAD_LEFT) == KEY_DOWN && inside == false)
		{
			if (remapping_ui[i].current_state <= r_s_a4 && remapping_ui[i].current_state > r_s_shop)
			{
				int curr = remapping_ui[i].current_state;
				curr -= 4; 

				remapping_ui[i].current_state = static_cast<remaping_state> (curr);

				remapping_ui[i].cursor->SetPos(GetCursorPosFromCurrentState(remapping_ui[i].current_state));
			}

		}

		if (App->input->GetControllerButton(i, SDL_CONTROLLER_BUTTON_DPAD_RIGHT) == KEY_DOWN && inside == false)
		{
			if (remapping_ui[i].current_state <= r_s_shop && remapping_ui[i].current_state > 0)
			{
				int curr = remapping_ui[i].current_state;
				curr += 4;

				remapping_ui[i].current_state = static_cast<remaping_state> (curr);

				remapping_ui[i].cursor->SetPos(GetCursorPosFromCurrentState(remapping_ui[i].current_state));
			}

		}

		if (App->input->GetControllerButton(i, SDL_CONTROLLER_BUTTON_X) == KEY_DOWN)
		{
			if(!inside)
				App->scene->ChangeScene((Scene*)App->scene->menu_scene);
		
		}

		if (App->input->GetControllerButton(i, SDL_CONTROLLER_BUTTON_B) == KEY_DOWN)
		{
			if (inside)
				QuitOption(i);
		}

		if (App->input->GetControllerButton(i, SDL_CONTROLLER_BUTTON_A) == KEY_DOWN)
		{
			if(!inside)
				EnterOption(i);
			else
			{
				RemapKey(remapping_ui[i].current_state, remapping_ui[i].sdl_code[remapping_ui[i].curr_inside_pos].key_id, remapping_ui[i].sdl_code[remapping_ui[i].curr_inside_pos].is_button, i);
				UpdateKeys(i); 
				QuitOption(i); 
			}
		}

	}

	App->view->LayerBlit(0, background_image, App->view->camera1, { 0,0,1994, 1359 }, 0, 1);

	return false;
}

bool RemapingScene::CleanUp()
{
	if (App->scene->GetCurrentScene() != App->scene->remaping_scene)
		App->gui->DeleteElement(window);

	return false;
}

iPoint RemapingScene::GetCursorPosFromCurrentState(remaping_state curr_state)
{
	iPoint ret = NULLPOINT; 

	switch (curr_state) 
	{

	case r_s_confirm:
		ret = { remapping_ui[0].confirm_background->GetPos().x - 50, remapping_ui[0].confirm_background->GetPos().y };
			break; 

	case r_s_back:
		ret = { remapping_ui[0].back_background->GetPos().x - 50, remapping_ui[0].back_background->GetPos().y };
		break;

	case r_s_minimap:
		ret = { remapping_ui[0].minimap_background->GetPos().x - 50, remapping_ui[0].minimap_background->GetPos().y  };
		break;

	case r_s_shop:
		ret = { remapping_ui[0].shop_background->GetPos().x - 50, remapping_ui[0].shop_background->GetPos().y  };
		break;

	case r_s_a1:
		ret = { remapping_ui[0].a1_background->GetPos().x + remapping_ui[0].confirm_background->image.w + 10, remapping_ui[0].a1_background->GetPos().y };
		break;

	case r_s_a2:
		ret = { remapping_ui[0].a2_background->GetPos().x + remapping_ui[0].confirm_background->image.w + 10, remapping_ui[0].a2_background->GetPos().y };
		break;

	case r_s_a3:
		ret = { remapping_ui[0].a3_background->GetPos().x + remapping_ui[0].confirm_background->image.w + 10, remapping_ui[0].a3_background->GetPos().y };
		break;

	case r_s_a4:
		ret = { remapping_ui[0].a4_background->GetPos().x + remapping_ui[0].confirm_background->image.w + 10, remapping_ui[0].a4_background->GetPos().y  };
		break;


	}

	return ret;
}

void RemapingScene::UpdateKeys(int i)
{
	key_mapping confirm = App->scene->players[i].mapping->GetMapping(m_k_confirm); 

	switch (confirm.key_id)
	{
	case SDL_CONTROLLER_BUTTON_A: 
		remapping_ui[i].confirm_key->image = remapping_ui[i].a->image; 
		break; 

	case SDL_CONTROLLER_BUTTON_B:
		remapping_ui[i].confirm_key->image = remapping_ui[i].b->image;
		break;

	case SDL_CONTROLLER_BUTTON_X:
		remapping_ui[i].confirm_key->image = remapping_ui[i].x->image;
		break;

	case SDL_CONTROLLER_BUTTON_Y:
		remapping_ui[i].confirm_key->image = remapping_ui[i].y->image;
		break;
	}

	key_mapping back = App->scene->players[i].mapping->GetMapping(m_k_back);

	switch (back.key_id)
	{
	case SDL_CONTROLLER_BUTTON_A:
		remapping_ui[i].back_key->image = remapping_ui[i].a->image;
		break;

	case SDL_CONTROLLER_BUTTON_B:
		remapping_ui[i].back_key->image = remapping_ui[i].b->image;
		break;

	case SDL_CONTROLLER_BUTTON_X:
		remapping_ui[i].back_key->image = remapping_ui[i].x->image;
		break;

	case SDL_CONTROLLER_BUTTON_Y:
		remapping_ui[i].back_key->image = remapping_ui[i].y->image;
		break;
	}

	key_mapping shop = App->scene->players[i].mapping->GetMapping(m_k_shop);

	switch (shop.key_id)
	{
	case SDL_CONTROLLER_BUTTON_A:
		remapping_ui[i].shop_key->image = remapping_ui[i].a->image;
		break;

	case SDL_CONTROLLER_BUTTON_B:
		remapping_ui[i].shop_key->image = remapping_ui[i].b->image;
		break;

	case SDL_CONTROLLER_BUTTON_X:
		remapping_ui[i].shop_key->image = remapping_ui[i].x->image;
		break;

	case SDL_CONTROLLER_BUTTON_Y:
		remapping_ui[i].shop_key->image = remapping_ui[i].y->image;
		break;
	}

	key_mapping minimap = App->scene->players[i].mapping->GetMapping(m_k_minimap);

	switch (minimap.key_id)
	{
	case SDL_CONTROLLER_BUTTON_A:
		remapping_ui[i].minimap_key->image = remapping_ui[i].a->image;
		break;

	case SDL_CONTROLLER_BUTTON_B:
		remapping_ui[i].minimap_key->image = remapping_ui[i].b->image;
		break;

	case SDL_CONTROLLER_BUTTON_X:
		remapping_ui[i].minimap_key->image = remapping_ui[i].x->image;
		break;

	case SDL_CONTROLLER_BUTTON_Y:
		remapping_ui[i].minimap_key->image = remapping_ui[i].y->image;
		break;
	}

	key_mapping a1 = App->scene->players[i].mapping->GetMapping(m_k_ability1);

	if (a1.is_button)
	{
		switch (a1.key_id)
		{
		case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
			remapping_ui[i].a1_key->image = remapping_ui[i].rb->image;
			break;

		case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
			remapping_ui[i].a1_key->image = remapping_ui[i].lb->image;
			break;
		}
	}
	else
	{
		switch (a1.key_id)
		{
		case RIGHT_TRIGGER:
			remapping_ui[i].a1_key->image = remapping_ui[i].rt->image;
			break;

		case LEFT_TRIGGER:
			remapping_ui[i].a1_key->image = remapping_ui[i].lt->image;
			break;
		}
	}

	key_mapping a2 = App->scene->players[i].mapping->GetMapping(m_k_ability2);

	if (a2.is_button)
	{
		switch (a2.key_id)
		{
		case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
			remapping_ui[i].a2_key->image = remapping_ui[i].rb->image;
			break;

		case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
			remapping_ui[i].a2_key->image = remapping_ui[i].lb->image;
			break;
		}
	}
	else
	{
		switch (a2.key_id)
		{
		case RIGHT_TRIGGER:
			remapping_ui[i].a2_key->image = remapping_ui[i].rt->image;
			break;

		case LEFT_TRIGGER:
			remapping_ui[i].a2_key->image = remapping_ui[i].lt->image;
			break;
		}
	}

	key_mapping a3 = App->scene->players[i].mapping->GetMapping(m_k_ability3);

	if (a3.is_button)
	{
		switch (a3.key_id)
		{
		case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
			remapping_ui[i].a3_key->image = remapping_ui[i].rb->image;
			break;

		case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
			remapping_ui[i].a3_key->image = remapping_ui[i].lb->image;
			break;
		}
	}
	else
	{
		switch (a3.key_id)
		{
		case RIGHT_TRIGGER:
			remapping_ui[i].a3_key->image = remapping_ui[i].rt->image;
			break;

		case LEFT_TRIGGER:
			remapping_ui[i].a3_key->image = remapping_ui[i].lt->image;
			break;
		}
	}

	key_mapping a4 = App->scene->players[i].mapping->GetMapping(m_k_ability4);

	if (a4.is_button)
	{
		switch (a4.key_id)
		{
		case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
			remapping_ui[i].a4_key->image = remapping_ui[i].rb->image;
			break;

		case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
			remapping_ui[i].a4_key->image = remapping_ui[i].lb->image;
			break;
		}
	}
	else
	{
		switch (a4.key_id)
		{
		case RIGHT_TRIGGER:
			remapping_ui[i].a4_key->image = remapping_ui[i].rt->image;
			break;

		case LEFT_TRIGGER:
			remapping_ui[i].a4_key->image = remapping_ui[i].lt->image;
			break;
		}
	}





}

void RemapingScene::EnterOption(int i)
{
	remapping_ui[i].prev_state = remapping_ui[i].current_state; 
	remapping_ui[i].curr_inside_pos = 0; 
	OpenFreeButtonList(i);
	inside = true; 
	
}

void RemapingScene::QuitOption(int i)
{
	remapping_ui[i].current_state = remapping_ui[i].prev_state; 
	
	remapping_ui[i].button_support->enabled = false; 

	remapping_ui[i].a->enabled = false; 
	remapping_ui[i].b->enabled = false;
	remapping_ui[i].x->enabled = false;
	remapping_ui[i].y->enabled = false;
	remapping_ui[i].rb->enabled = false;
	remapping_ui[i].rt->enabled = false;
	remapping_ui[i].lb->enabled = false;
	remapping_ui[i].lt->enabled = false;
	remapping_ui[i].button_selector_cursor->enabled = false;
	remapping_ui[i].support_pick->enabled = false; 

	inside = false; 

}

void RemapingScene::OpenFreeButtonList(int i)
{
	SDL_Rect viewport_rect = App->view->GetViewportRect(1);

	SDL_Rect up_left_pick = { 928, 2003, 40, 28 };
	SDL_Rect up_right_pick = { 968, 2003, 40, 28 };
	SDL_Rect left_pick = { 928, 1959, 24, 22 };
	SDL_Rect right_pick = { 298, 1981, 24, 22 };
	SDL_Rect bottom_left_pick = { 928, 2032, 40, 28 };
	SDL_Rect bottom_right_pick = { 968, 2032, 40, 28 };

	iPoint suport_pos = { viewport_rect.w / 2 - remapping_ui[i].button_support->image.w / 2, viewport_rect.h / 2 - remapping_ui[i].button_support->image.h / 2 };
	remapping_ui[i].button_support->SetPos({suport_pos.x -10, suport_pos.y - 10});
	remapping_ui[i].button_support->enabled = true; 

	remaping_state curr_state = remapping_ui[i].current_state; 
	key_mapping curr_key; 
	
	
		switch (remapping_ui[i].current_state)
		{
		case r_s_confirm:
			curr_key = App->scene->players[i].mapping->GetMapping(m_k_confirm);
			remapping_ui[i].support_pick->image = up_left_pick;
			remapping_ui[i].support_pick->SetPos({ remapping_ui[i].button_support->GetPos().x + 10, remapping_ui[i].button_support->GetPos().y - 20});
			remapping_ui[i].support_pick->enabled = true; 
			break;

		case r_s_back:
			curr_key = App->scene->players[i].mapping->GetMapping(m_k_back);
			remapping_ui[i].support_pick->image = left_pick;
			remapping_ui[i].support_pick->SetPos({ remapping_ui[i].button_support->GetPos().x - 16, remapping_ui[i].button_support->GetPos().y + 40 });
			remapping_ui[i].support_pick->enabled = true;
			break;

		case r_s_minimap:
			curr_key = App->scene->players[i].mapping->GetMapping(m_k_minimap);
			remapping_ui[i].support_pick->image = left_pick;
			remapping_ui[i].support_pick->SetPos({ remapping_ui[i].button_support->GetPos().x - 16, remapping_ui[i].button_support->GetPos().y + 110 });
			remapping_ui[i].support_pick->enabled = true;
			break;

		case r_s_shop:
			curr_key = App->scene->players[i].mapping->GetMapping(m_k_shop);
			remapping_ui[i].support_pick->image = bottom_left_pick;
			remapping_ui[i].support_pick->SetPos({ remapping_ui[i].button_support->GetPos().x + 10, remapping_ui[i].button_support->GetPos().y + remapping_ui[i].button_support->image.h - 8});
			remapping_ui[i].support_pick->enabled = true;
			break;

		case r_s_a1:
			curr_key = App->scene->players[i].mapping->GetMapping(m_k_ability1);
			remapping_ui[i].support_pick->image = up_right_pick;

			remapping_ui[i].support_pick->SetPos({ remapping_ui[i].button_support->GetPos().x + 15, remapping_ui[i].button_support->GetPos().y - 20 });
			remapping_ui[i].support_pick->enabled = true;
			break;

		case r_s_a2:
			curr_key = App->scene->players[i].mapping->GetMapping(m_k_ability2);
			remapping_ui[i].support_pick->image = { 298, 1981, 24, 22 };
			remapping_ui[i].support_pick->SetPos({ remapping_ui[i].button_support->GetPos().x + remapping_ui[i].button_support->image.w, remapping_ui[i].button_support->GetPos().y + 40});
			remapping_ui[i].support_pick->enabled = true;
			break;

		case r_s_a3:
			curr_key = App->scene->players[i].mapping->GetMapping(m_k_ability3);
			remapping_ui[i].support_pick->image = { 298, 1981, 24, 22 };
			remapping_ui[i].support_pick->SetPos({ remapping_ui[i].button_support->GetPos().x + remapping_ui[i].button_support->image.w, remapping_ui[i].button_support->GetPos().y + 110 });
			remapping_ui[i].support_pick->enabled = true;
			break;

		case r_s_a4:
			curr_key = App->scene->players[i].mapping->GetMapping(m_k_ability4);
			remapping_ui[i].support_pick->image = bottom_right_pick;
			remapping_ui[i].support_pick->SetPos({ remapping_ui[i].button_support->GetPos().x + 15, remapping_ui[i].button_support->GetPos().y + remapping_ui[i].button_support->image.h - 8 });
			remapping_ui[i].support_pick->enabled = true;
			break;
		}
	
	SetButtonsFromID(curr_key, i);

	remapping_ui[i].button_selector_cursor->SetPos({ suport_pos.x + 2, suport_pos.y });
	remapping_ui[i].button_selector_cursor->enabled = true; 

}

bool RemapingScene::IsButton(int id)
{
	if (id == SDL_CONTROLLER_BUTTON_LEFTSHOULDER || id == SDL_CONTROLLER_BUTTON_RIGHTSHOULDER || id == LEFT_TRIGGER || id == RIGHT_TRIGGER)
		return false;

	else
		return true; 
}

void RemapingScene::SetButtonsFromID(key_mapping curr_key, int viewport)
{
	iPoint positions[3];

	positions[0] = { remapping_ui[viewport].button_support->GetPos().x + 15, remapping_ui[viewport].button_support->GetPos().y + 13 };
	positions[1] = { remapping_ui[viewport].button_support->GetPos().x + 15, remapping_ui[viewport].button_support->GetPos().y + 70 };
	positions[2] = { remapping_ui[viewport].button_support->GetPos().x + 15, remapping_ui[viewport].button_support->GetPos().y + 127 };

	remapping_ui[viewport].sdl_code.clear(); 

	key_mapping new_key; 

	switch (curr_key.key_id)
	{
	case SDL_CONTROLLER_BUTTON_A:
	{
		remapping_ui[viewport].b->SetPos(positions[0]);
		remapping_ui[viewport].b->enabled = true;
		new_key.key_id = SDL_CONTROLLER_BUTTON_B; 
		new_key.is_button = true; 
		remapping_ui[viewport].sdl_code.push_back(new_key);

		remapping_ui[viewport].x->SetPos(positions[1]);
		remapping_ui[viewport].x->enabled = true;
		new_key.key_id = SDL_CONTROLLER_BUTTON_X;
		new_key.is_button = true;
		remapping_ui[viewport].sdl_code.push_back(new_key);

		remapping_ui[viewport].y->SetPos(positions[2]);
		remapping_ui[viewport].y->enabled = true;
		new_key.key_id = SDL_CONTROLLER_BUTTON_Y;
		new_key.is_button = true;
		remapping_ui[viewport].sdl_code.push_back(new_key);

	}
	break;

	case SDL_CONTROLLER_BUTTON_B:
	{
		remapping_ui[viewport].a->SetPos(positions[0]);
		remapping_ui[viewport].a->enabled = true;
		new_key.key_id = SDL_CONTROLLER_BUTTON_A;
		new_key.is_button = true;
		remapping_ui[viewport].sdl_code.push_back(new_key);

		remapping_ui[viewport].x->SetPos(positions[1]);
		remapping_ui[viewport].x->enabled = true;
		new_key.key_id = SDL_CONTROLLER_BUTTON_X;
		new_key.is_button = true;
		remapping_ui[viewport].sdl_code.push_back(new_key);

		remapping_ui[viewport].y->SetPos(positions[2]);
		remapping_ui[viewport].y->enabled = true;
		new_key.key_id = SDL_CONTROLLER_BUTTON_Y;
		new_key.is_button = true;
		remapping_ui[viewport].sdl_code.push_back(new_key);

	}
	break;

	case SDL_CONTROLLER_BUTTON_X:
	{
		remapping_ui[viewport].b->SetPos(positions[0]);
		remapping_ui[viewport].b->enabled = true;
		new_key.key_id = SDL_CONTROLLER_BUTTON_B;
		new_key.is_button = true;
		remapping_ui[viewport].sdl_code.push_back(new_key);

		remapping_ui[viewport].a->SetPos(positions[1]);
		remapping_ui[viewport].a->enabled = true;
		new_key.key_id = SDL_CONTROLLER_BUTTON_A;
		new_key.is_button = true;
		remapping_ui[viewport].sdl_code.push_back(new_key);

		remapping_ui[viewport].y->SetPos(positions[2]);
		remapping_ui[viewport].y->enabled = true;
		new_key.key_id = SDL_CONTROLLER_BUTTON_Y;
		new_key.is_button = true;
		remapping_ui[viewport].sdl_code.push_back(new_key);
	}
	break;

	case SDL_CONTROLLER_BUTTON_Y:
	{
		remapping_ui[viewport].b->SetPos(positions[0]);
		remapping_ui[viewport].b->enabled = true;
		new_key.key_id = SDL_CONTROLLER_BUTTON_B;
		new_key.is_button = true;
		remapping_ui[viewport].sdl_code.push_back(new_key);

		remapping_ui[viewport].x->SetPos(positions[1]);
		remapping_ui[viewport].x->enabled = true;
		new_key.key_id = SDL_CONTROLLER_BUTTON_X;
		new_key.is_button = true;
		remapping_ui[viewport].sdl_code.push_back(new_key);

		remapping_ui[viewport].a->SetPos(positions[2]);
		remapping_ui[viewport].a->enabled = true;
		new_key.key_id = SDL_CONTROLLER_BUTTON_A;
		new_key.is_button = true;
		remapping_ui[viewport].sdl_code.push_back(new_key);

	}
	break;

	case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
	{
		remapping_ui[viewport].rt->SetPos(positions[0]);
		remapping_ui[viewport].rt->enabled = true;
		new_key.key_id =  RIGHT_TRIGGER;
		new_key.is_button = false;
		remapping_ui[viewport].sdl_code.push_back(new_key);

		remapping_ui[viewport].lb->SetPos(positions[1]);
		remapping_ui[viewport].lb->enabled = true;
		new_key.key_id = SDL_CONTROLLER_BUTTON_LEFTSHOULDER;
		new_key.is_button = true;
		remapping_ui[viewport].sdl_code.push_back(new_key);

		remapping_ui[viewport].lt->SetPos(positions[2]);
		remapping_ui[viewport].lt->enabled = true;
		new_key.key_id = LEFT_TRIGGER;
		new_key.is_button = false;
		remapping_ui[viewport].sdl_code.push_back(new_key);

	}
	break;

	case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
	{
		if (curr_key.is_button)
		{
			remapping_ui[viewport].rb->SetPos(positions[0]);
			remapping_ui[viewport].rb->enabled = true;
			new_key.key_id = SDL_CONTROLLER_BUTTON_RIGHTSHOULDER;
			new_key.is_button = true;
			remapping_ui[viewport].sdl_code.push_back(new_key);

			remapping_ui[viewport].lt->SetPos(positions[1]);
			remapping_ui[viewport].lt->enabled = true;
			new_key.key_id = SDL_CONTROLLER_BUTTON_LEFTSHOULDER;
			new_key.is_button = false;
			remapping_ui[viewport].sdl_code.push_back(new_key);

			remapping_ui[viewport].rt->SetPos(positions[2]);
			remapping_ui[viewport].rt->enabled = true;
			new_key.key_id = RIGHT_TRIGGER;
			new_key.is_button = false;
			remapping_ui[viewport].sdl_code.push_back(new_key);

		}
		else
		{
			remapping_ui[viewport].rb->SetPos(positions[0]);
			remapping_ui[viewport].rb->enabled = true;
			new_key.key_id = SDL_CONTROLLER_BUTTON_LEFTSHOULDER;
			new_key.is_button = true;
			remapping_ui[viewport].sdl_code.push_back(new_key);

			remapping_ui[viewport].lb->SetPos(positions[1]);
			remapping_ui[viewport].lb->enabled = true;
			new_key.key_id = SDL_CONTROLLER_BUTTON_RIGHTSHOULDER;
			new_key.is_button = true;
			remapping_ui[viewport].sdl_code.push_back(new_key);

			remapping_ui[viewport].rt->SetPos(positions[2]);
			remapping_ui[viewport].rt->enabled = true;
			new_key.key_id = RIGHT_TRIGGER;
			new_key.is_button = false;
			remapping_ui[viewport].sdl_code.push_back(new_key);

		}

	}
	break;

	case LEFT_TRIGGER:
	{
		remapping_ui[viewport].rb->SetPos(positions[0]);
		remapping_ui[viewport].rb->enabled = true;
		new_key.key_id = SDL_CONTROLLER_BUTTON_B;
		new_key.is_button = true;
		remapping_ui[viewport].sdl_code.push_back(new_key);

		remapping_ui[viewport].lb->SetPos(positions[1]);
		remapping_ui[viewport].lb->enabled = true;
		new_key.key_id = SDL_CONTROLLER_BUTTON_B;
		new_key.is_button = true;
		remapping_ui[viewport].sdl_code.push_back(new_key);

		remapping_ui[viewport].rt->SetPos(positions[2]);
		remapping_ui[viewport].rt->enabled = true;
		new_key.key_id = SDL_CONTROLLER_BUTTON_B;
		new_key.is_button = false;
		remapping_ui[viewport].sdl_code.push_back(new_key);

	}
	break;

	}

}

void RemapingScene::MoveSelectorCursor(bool up, int viewport)
{

	if (up) 
	{
		if ((remapping_ui[viewport].button_selector_cursor->GetPos().y - remapping_ui[viewport].button_selector_cursor->image.h) > remapping_ui[viewport].button_support->GetPos().y)
		{
			remapping_ui[viewport].button_selector_cursor->SetPos({ remapping_ui[viewport].button_selector_cursor->GetPos().x, remapping_ui[viewport].button_selector_cursor->GetPos().y - 57 });
		}
		
		return;
	}
	else
	{
		if ((remapping_ui[viewport].button_selector_cursor->GetPos().y + remapping_ui[viewport].button_selector_cursor->image.h*2) < remapping_ui[viewport].button_support->GetPos().y + remapping_ui[viewport].button_support->image.h)
		{
			remapping_ui[viewport].button_selector_cursor->SetPos({ remapping_ui[viewport].button_selector_cursor->GetPos().x, remapping_ui[viewport].button_selector_cursor->GetPos().y + 57 });
		}

		return;

	}
	
	
}

void RemapingScene::RemapKey(remaping_state curr_state, int sdl_scancode, bool isbutton, int viewport)
{
	switch (curr_state)
	{
	case r_s_confirm:
		App->scene->players[viewport].mapping->MapKey(m_k_confirm, sdl_scancode, isbutton); 
		break; 

	case r_s_back:
		App->scene->players[viewport].mapping->MapKey(m_k_back, sdl_scancode, isbutton);
		break;

	case r_s_minimap:
		App->scene->players[viewport].mapping->MapKey(m_k_minimap, sdl_scancode, isbutton);
		break;

	case r_s_shop:
		App->scene->players[viewport].mapping->MapKey(m_k_shop, sdl_scancode, isbutton);
		break;

	case r_s_a1:
		App->scene->players[viewport].mapping->MapKey(m_k_ability1, sdl_scancode, isbutton);
		break;

	case r_s_a2:
		App->scene->players[viewport].mapping->MapKey(m_k_ability2, sdl_scancode, isbutton);
		break;

	case r_s_a3:
		App->scene->players[viewport].mapping->MapKey(m_k_ability3, sdl_scancode, isbutton);
		break;

	case r_s_a4:
		App->scene->players[viewport].mapping->MapKey(m_k_ability4, sdl_scancode, isbutton);
		break;



	}
}



