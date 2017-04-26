#include "RemapingScene.h"
#include "j1Window.h"
#include "j1Input.h"
#include "Mapping.h"
#include "j1Scene.h"
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
	SDL_Rect button_suport_rect = {583, 100, 52, 147}; 

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

		// Creating the background of the buttons

		remapping_ui[i].confirm_background = window->CreateImage({ confirm_background_pos.x + offset_x, confirm_background_pos.y + offset_y }, rectangle_background_rect);
		remapping_ui[i].back_background = window->CreateImage({ back_background_pos.x + offset_x, back_background_pos.y + offset_y }, rectangle_background_rect);
		remapping_ui[i].minimap_background = window->CreateImage({ minimap_background_pos.x + offset_x, minimap_background_pos.y + offset_y }, rectangle_background_rect);
		remapping_ui[i].shop_background = window->CreateImage({ shop_background_pos.x + offset_x, shop_background_pos.y + offset_y }, rectangle_background_rect);
		remapping_ui[i].a1_background = window->CreateImage({ a1_background_pos.x + offset_x, a1_background_pos.y + offset_y }, rectangle_background_rect);
		remapping_ui[i].a2_background = window->CreateImage({ a2_background_pos.x + offset_x, a2_background_pos.y + offset_y }, rectangle_background_rect);
		remapping_ui[i].a3_background = window->CreateImage({ a3_background_pos.x + offset_x, a3_background_pos.y + offset_y }, rectangle_background_rect);
		remapping_ui[i].a4_background = window->CreateImage({ a4_background_pos.x + offset_x, a4_background_pos.y + offset_y }, rectangle_background_rect);
		remapping_ui[i].cursor = window->CreateImage({ GetCursorPosFromCurrentState(remapping_ui[i].current_state).x + offset_x, GetCursorPosFromCurrentState(remapping_ui[i].current_state).y + offset_y }, cursor_rect);
		remapping_ui[i].button_support = window->CreateImage({ 0,0 }, button_suport_rect);
		remapping_ui[i].button_support->enabled = false; 
		// ----

		// Setting text to the buttons 

		remapping_ui[i].confirm_text = window->CreateText({ confirm_background_pos.x + 13 + offset_x, confirm_background_pos.y + 3 + offset_y}, App->font->game_font_20);
		remapping_ui[i].confirm_text->SetText("CONFIRM");
		remapping_ui[i].back_text = window->CreateText({ back_background_pos.x + 40 + offset_x, back_background_pos.y + 3 + offset_y }, App->font->game_font_20);
		remapping_ui[i].back_text->SetText("BACK");
		remapping_ui[i].minimap_text = window->CreateText({ minimap_background_pos.x + 13 + offset_x, minimap_background_pos.y + 3 + offset_y }, App->font->game_font_20);
		remapping_ui[i].minimap_text->SetText("MINIMAP");
		remapping_ui[i].shop_text = window->CreateText({ shop_background_pos.x + 40 + offset_x, shop_background_pos.y + 3 + offset_y }, App->font->game_font_20);
		remapping_ui[i].shop_text->SetText("SHOP");
		remapping_ui[i].a1_text = window->CreateText({ a1_background_pos.x + 40 + offset_x, a1_background_pos.y + 3 + offset_y }, App->font->game_font_20);
		remapping_ui[i].a1_text->SetText("HAB. 1");							 
		remapping_ui[i].a2_text = window->CreateText({ a2_background_pos.x + 40 + offset_x, a2_background_pos.y + 3 + offset_y }, App->font->game_font_20);
		remapping_ui[i].a2_text->SetText("HAB. 2");							  
		remapping_ui[i].a3_text = window->CreateText({ a3_background_pos.x + 40 + offset_x, a3_background_pos.y + 3 + offset_y }, App->font->game_font_20);
		remapping_ui[i].a3_text->SetText("HAB. 3");							  
		remapping_ui[i].a4_text = window->CreateText({ a4_background_pos.x + 40 + offset_x, a4_background_pos.y + 3 + offset_y }, App->font->game_font_20);
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

		remapping_ui[i].confirm_key = window->CreateImage({ confirm_background_pos.x + rectangle_background_rect.w + 10 + offset_x, confirm_background_pos.y + offset_y }, {0,0,0,0});
		remapping_ui[i].back_key = window->CreateImage({ back_background_pos.x + rectangle_background_rect.w + 10 + offset_x, back_background_pos.y + offset_y }, { 0,0,0,0 });
		remapping_ui[i].minimap_key = window->CreateImage({ minimap_background_pos.x + rectangle_background_rect.w + 10 + offset_x, minimap_background_pos.y + offset_y }, { 0,0,0,0 });
		remapping_ui[i].shop_key = window->CreateImage({ shop_background_pos.x + rectangle_background_rect.w + 10 + offset_x, shop_background_pos.y + offset_y }, { 0,0,0,0 });
		remapping_ui[i].a1_key = window->CreateImage({ a1_background_pos.x + rectangle_background_rect.w + 10 + offset_x, a1_background_pos.y + offset_y }, { 0,0,0,0 });
		remapping_ui[i].a2_key = window->CreateImage({ a2_background_pos.x + rectangle_background_rect.w + 10 + offset_x, a2_background_pos.y + offset_y }, { 0,0,0,0 });
		remapping_ui[i].a3_key = window->CreateImage({ a3_background_pos.x + rectangle_background_rect.w + 10 + offset_x, a3_background_pos.y + offset_y }, { 0,0,0,0 });
		remapping_ui[i].a4_key = window->CreateImage({ a4_background_pos.x + rectangle_background_rect.w + 10 + offset_x, a4_background_pos.y + offset_y}, { 0,0,0,0 });

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
			if (remapping_ui[i].current_state != r_s_confirm_out)
			{
				int curr = remapping_ui[i].current_state;
				curr--; 

				remapping_ui[i].current_state = static_cast<remaping_state> (curr);

				remapping_ui[i].cursor->SetPos(GetCursorPosFromCurrentState(remapping_ui[i].current_state));
			}
				
		}

		if (App->input->GetControllerButton(i, SDL_CONTROLLER_BUTTON_DPAD_DOWN) == KEY_DOWN)
		{
			if (remapping_ui[i].current_state != r_s_a4_out)
			{
				int curr = remapping_ui[i].current_state;
				curr++;

				remapping_ui[i].current_state = static_cast<remaping_state> (curr);

				remapping_ui[i].cursor->SetPos(GetCursorPosFromCurrentState(remapping_ui[i].current_state));
			}

		}

		if (App->input->GetControllerButton(i, SDL_CONTROLLER_BUTTON_B) == KEY_DOWN)
		{
			if(!IsInside(i))
				App->scene->ChangeScene((Scene*)App->scene->menu_scene);
			else
				QuitOption(i);
		}

		if (App->input->GetControllerButton(i, SDL_CONTROLLER_BUTTON_A) == KEY_DOWN)
		{
			EnterOption(i);
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

	case r_s_confirm_out:
		ret = {20, 60}; 
			break; 

	case r_s_back_out:
		ret = { 20, 140 };
		break;

	case r_s_minimap_out:
		ret = { 20, 220 };
		break;

	case r_s_shop_out:
		ret = { 20, 300 };
		break;

	case r_s_a1_out:
		ret = { 320, 60 };
		break;

	case r_s_a2_out:
		ret = { 320, 140 };
		break;

	case r_s_a3_out:
		ret = { 320, 220 };
		break;

	case r_s_a4_out:
		ret = { 320, 300};
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
	switch (remapping_ui[i].current_state)
	{
	case r_s_confirm_out:
		remapping_ui[i].current_state = r_s_confirm_in;
		break;

	case r_s_back_out:
		remapping_ui[i].current_state = r_s_back_in;
		break;

	case r_s_minimap_out:
		remapping_ui[i].current_state = r_s_minimap_in;
		break;

	case r_s_shop_out:
		remapping_ui[i].current_state = r_s_shop_in;
		break;

	case r_s_a1_out:
		remapping_ui[i].current_state = r_s_a1_in;
		break;

	case r_s_a2_out:
		remapping_ui[i].current_state = r_s_a2_in;
		break;

	case r_s_a3_out:
		remapping_ui[i].current_state = r_s_a3_in;
		break;

	case r_s_a4_out:
		remapping_ui[i].current_state = r_s_a4_in;
		break;
	}

	OpenFreeButtonList(i);

}

void RemapingScene::QuitOption(int i)
{
	switch (remapping_ui[i].current_state)
	{
	case r_s_confirm_in:
		remapping_ui[i].current_state = r_s_confirm_out;
		break;

	case r_s_back_in:
		remapping_ui[i].current_state = r_s_back_out;
		break;

	case r_s_minimap_in:
		remapping_ui[i].current_state = r_s_minimap_out;
		break;

	case r_s_shop_in:
		remapping_ui[i].current_state = r_s_shop_out;
		break;

	case r_s_a1_in:
		remapping_ui[i].current_state = r_s_a1_out;
		break;

	case r_s_a2_in:
		remapping_ui[i].current_state = r_s_a2_out;
		break;

	case r_s_a3_in:
		remapping_ui[i].current_state = r_s_a3_out;
		break;

	case r_s_a4_in:
		remapping_ui[i].current_state = r_s_a4_out;
		break;
	}

}

void RemapingScene::OpenFreeButtonList(int i)
{
	remapping_ui[i].button_support->SetPos({ remapping_ui[i].cursor->GetPos().x + 280, remapping_ui[i].cursor->GetPos().y});
	remapping_ui[i].button_support->enabled = true; 

}

bool RemapingScene::IsInside(int i)
{
	if (remapping_ui[i].current_state == r_s_confirm_out || remapping_ui[i].current_state == r_s_back_out || remapping_ui[i].current_state == r_s_minimap_out || remapping_ui[i].current_state == r_s_shop_out || remapping_ui[i].current_state == r_s_a1_out || remapping_ui[i].current_state == r_s_a2_out || remapping_ui[i].current_state == r_s_a3_out || remapping_ui[i].current_state == r_s_a4_out)
		return false;

	else
		return true; 
}
