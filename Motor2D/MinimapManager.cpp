#include "MinimapManager.h"
#include "Functions.h"
#include "j1Textures.h"
#include "j1FileSystem.h"
#include "j1Viewports.h"
#include "j1Scene.h"
#include "MainScene.h"
#include "ZeldaManager.h"
#include "PlayerManager.h"
#include "Zelda.h"

MinimapManager::MinimapManager()
{
}

MinimapManager::~MinimapManager()
{
}

bool MinimapManager::Start()
{
	bool ret = true;

	zelda_rect = { 0, 0, 31, 28 };
	link_1_rect = { 348, 535, 32, 29 };
	link_2_rect = { 380, 535, 32, 29 };
	navi_1_rect = { 412, 535, 34, 29 };
	navi_2_rect = { 445, 535, 34, 29 };
	ganon_1_rect = {476, 535, 32, 26};
	ganon_2_rect = { 508, 535, 32, 26 };

	// --------------------------------------------
	// Minimap setup ------------------------------
	// --------------------------------------------

	SDL_Rect view = App->view->GetViewportRect(1);

	SDL_Rect minimap_rect = {0, 621, 468, 235};
	SDL_Texture* minimap_texture = App->tex->LoadTexture("gui/UI_sheet_final.png");
	iPoint minimap_pos = {(int)(view.w*0.5) - (int)(minimap_rect.w * 0.5), (int)(view.h*0.5) - (int)(minimap_rect.h * 0.5) };

	SDL_Rect real_map_rect = {430, -80, 4490, 2300}; // X and Y are for the map offset
	SDL_Texture* minimap_points_texture = minimap_texture;

	if (minimap_texture != nullptr && minimap_points_texture != nullptr)
		SetMinimapInfo(minimap_pos, minimap_texture, minimap_rect, real_map_rect, minimap_points_texture);

	// --------------------------------------------

	if (!active)
		return true;

	// --------------------------------------------
	// Add points here ----------------------------
	// --------------------------------------------

	// Zelda
	AddPoint("zelda", zelda_rect)->show = false;

	// Players
	if (App->scene->main_scene->player_manager != nullptr && App->scene->main_scene->player_manager->players.size() >= 4)
	{
		Player* curr_player = nullptr;

		curr_player = App->scene->main_scene->player_manager->players.at(0);
		AddPoint("player1", GetPlayerRect(curr_player));

		curr_player = App->scene->main_scene->player_manager->players.at(1);
		AddPoint("player2", GetPlayerRect(curr_player));

		curr_player = App->scene->main_scene->player_manager->players.at(2);
		AddPoint("player3", GetPlayerRect(curr_player));

		curr_player = App->scene->main_scene->player_manager->players.at(3);
		AddPoint("player4", GetPlayerRect(curr_player));
	}

	// --------------------------------------------
	
	return ret;
}

void MinimapManager::UpdatePointsPos()
{
	// Zelda
	if (App->scene->main_scene->zelda_manager != nullptr && App->scene->main_scene->zelda_manager->GetZelda()->GetZeldaState() != zelda_states::z_s_wait)
	{
		minimap_point* zelda = GetPoint("zelda");
		zelda->show = true;
		zelda->real_pos = App->scene->main_scene->zelda_manager->GetZeldaPos();
	}

	// Players
	if (App->scene->main_scene->player_manager != nullptr && App->scene->main_scene->player_manager->players.size() >= 4)
	{
		Player* curr_player = nullptr;

		// Player 1
		curr_player = App->scene->main_scene->player_manager->players.at(0);
		minimap_point* p1 = GetPoint("player1");

		if (curr_player->is_dead)
		{
			p1->show = false;
		}
		else if (curr_player->entity != nullptr)
		{
			p1->real_pos = curr_player->entity->GetPos();
			p1->show = true;
		}

		// Player 2
		curr_player = App->scene->main_scene->player_manager->players.at(1);
		minimap_point* p2 = GetPoint("player2");

		if (curr_player->is_dead)
		{
			p2->show = false;
		}
		else if (curr_player->entity != nullptr)
		{
			p2->real_pos = curr_player->entity->GetPos();
			p2->show = true;
		}

		// Player 3
		curr_player = App->scene->main_scene->player_manager->players.at(2);
		minimap_point* p3 = GetPoint("player3");

		if (curr_player->is_dead)
		{
			p3->show = false;
		}
		else if (curr_player->entity != nullptr)
		{
			p3->real_pos = curr_player->entity->GetPos();
			p3->show = true;
		}

		// Player 4
		curr_player = App->scene->main_scene->player_manager->players.at(3);
		minimap_point* p4 = GetPoint("player4");

		if (curr_player->is_dead)
		{
			p4->show = false;
		}
		else if (curr_player->entity != nullptr)
		{
			p4->real_pos = curr_player->entity->GetPos();
			p4->show = true;
		}
	}
}

bool MinimapManager::Update(float dt)
{
	bool ret = true;

	if (!active)
		return true;

	// Update positions
	UpdatePointsPos();

	// Iterate on viewports
	for (int i = 0; i < viewports.size(); i++)
	{
		viewport curr_view = viewports.at(i);

		if (!curr_view.enabled)
			continue;

		// Print minimap
		App->view->LayerBlit(MINIMAP_LAYER, minimap_texture, minimap_position, minimap_rect, curr_view.view, -1.0f, false);

		// Iterate on points
		for (int i = 0; i < minimap_points.size(); i++)
		{
			minimap_point* curr_point = minimap_points.at(i);

			if (!curr_point->show)
				continue;

			iPoint map_pos = { minimap_position.x + (int)((curr_point->real_pos.x - real_map_rect.x)*multiplication_factor_x) - (int)(curr_point->image.w*0.5f), minimap_position.y + (int)((curr_point->real_pos.y -real_map_rect.y)*multiplication_factor_y) - (int)(curr_point->image.h*0.5f) };
			App->view->LayerBlit(MINIMAP_LAYER + curr_point->real_pos.y, minimap_points_texture, map_pos, curr_point->image, curr_view.view, -1.0f, false);
		}

	}

	return ret;
}

bool MinimapManager::CleanUp()
{
	bool ret = true;

	if (!minimap_points.empty())
	{
		for (vector<minimap_point*>::iterator it = minimap_points.begin(); it != minimap_points.end();)
		{			
			RELEASE(*it);
			it = minimap_points.erase(it);
		}
	}

	return ret;
}

void MinimapManager::SetMinimapInfo(iPoint _minimap_position, SDL_Texture * _minimap_texture, SDL_Rect _minimap_rect, SDL_Rect _real_map_rect, SDL_Texture * _minimap_points_texture, int _viewport)
{
	minimap_position = _minimap_position;
	minimap_texture = _minimap_texture;
	minimap_rect = _minimap_rect;
	real_map_rect = _real_map_rect;
	minimap_points_texture = _minimap_points_texture;
	active = true;

	// Calculations
	multiplication_factor_x = (float)minimap_rect.w / (float)real_map_rect.w;
	multiplication_factor_y = (float)minimap_rect.h / (float)real_map_rect.h;

	if (_viewport != 0)
	{
		viewport view;
		view.enabled = true;
		view.view = _viewport;
		viewports.push_back(view);
	}
	else
	{
		for (int i = 1; i <= 4; i++)
		{
			viewport view;
			view.enabled = false;
			view.view = i;
			viewports.push_back(view);
		}
	}
}

minimap_point* MinimapManager::GetPoint(char * name)
{
	minimap_point* m_p = nullptr;

	if (!minimap_points.empty())
	{
		for (int i = 0; i < minimap_points.size(); i++)
		{
			if (TextCmp(name, minimap_points.at(i)->name.c_str()))
			{
				m_p = minimap_points.at(i);
				break;
			}
		}
	}

	return m_p;
}

minimap_point * MinimapManager::AddPoint(char * name, SDL_Rect image, int viewport)
{
	minimap_point * ret = nullptr;

	minimap_point* m_p = new minimap_point(name, image, viewport);
	minimap_points.push_back(m_p);

	if (m_p != nullptr)
		ret = m_p;

	return ret;
}

void MinimapManager::DeletePoint(char * name)
{
	if (!minimap_points.empty())
	{
		for(vector<minimap_point*>::iterator it = minimap_points.begin(); it != minimap_points.end();)
		{
			if (TextCmp(name, (*it)->name.c_str()))
			{
				RELEASE(*it);
				minimap_points.erase(it);
				break;
			}
			else
				++it;
		}
	}
}

void MinimapManager::SetActive(bool set, int viewport)
{
	for (int i = 0; i < viewports.size(); i++)
	{
		if (viewports.at(i).view == viewport)
		{
			viewports.at(i).enabled = set;
			break;
		}
	}
}

SDL_Rect MinimapManager::GetPlayerRect(Player * curr_player)
{
	SDL_Rect ret = NULLRECT;

	switch (curr_player->team)
	{
	case 1:
		switch (curr_player->type)
		{
		case entity_name::link:
			ret = link_1_rect;
			break;
		case entity_name::navi:
			ret = navi_1_rect;
			break;
		case entity_name::ganon:
			ret = ganon_1_rect;
			break;
		}
		break;
	case 2:
		switch (curr_player->type)
		{
		case entity_name::link:
			ret = link_2_rect;
			break;
		case entity_name::navi:
			ret = navi_2_rect;
			break;
		case entity_name::ganon:
			ret = ganon_2_rect;
			break;
		}
	}

	return ret;
}
