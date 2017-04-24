#include "MinimapManager.h"
#include "Functions.h"
#include "j1Textures.h"
#include "j1FileSystem.h"
#include "j1Viewports.h"
#include "j1Scene.h"
#include "MainScene.h"
#include "ZeldaManager.h"
#include "PlayerManager.h"

MinimapManager::MinimapManager()
{
}

MinimapManager::~MinimapManager()
{
}

bool MinimapManager::Start()
{
	bool ret = true;

	// --------------------------------------------
	// Minimap setup ------------------------------
	// --------------------------------------------

	SDL_Rect minimap_rect = {0, 589, 472, 312};
	SDL_Texture* minimap_texture = App->tex->LoadTexture("gui/UI_sheet_final.png");
	SDL_Rect real_map_rect = {0, 0, 5730, 2500};
	SDL_Texture* minimap_points_texture = minimap_texture;

	if (minimap_texture != nullptr && minimap_points_texture != nullptr)
		SetMinimapInfo(iPoint(0, 0), minimap_texture, minimap_rect, real_map_rect, minimap_points_texture);

	// --------------------------------------------

	if (!active)
		return true;

	// --------------------------------------------
	// Add points here ----------------------------
	// --------------------------------------------

	AddPoint("zelda", { 0, 0, 31, 28 }, { 0,0 });

	// --------------------------------------------
	
	return ret;
}

void MinimapManager::UpdatePointsPos()
{
	// Zelda
	if (App->scene->main_scene->zelda_manager != nullptr)
		GetPoint("zelda")->real_pos = App->scene->main_scene->player_manager->players.at(0)->entity->GetPos();
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

			iPoint map_pos = { minimap_position.x + (int)(curr_point->real_pos.x*multiplication_factor_x), minimap_position.y + (int)(curr_point->real_pos.y*multiplication_factor_y) };
			App->view->LayerBlit(MINIMAP_LAYER + 1, minimap_points_texture, map_pos, curr_point->image, curr_view.view, -1.0f, false);

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

	App->tex->UnLoadTexture(minimap_texture);
	App->tex->UnLoadTexture(minimap_points_texture);

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
			view.enabled = true;
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

minimap_point * MinimapManager::AddPoint(char * name, SDL_Rect image, iPoint real_pos, int viewport)
{
	minimap_point * ret = nullptr;

	minimap_point* m_p = new minimap_point(name, image, real_pos, viewport);
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
	for (int i = 1; i <= viewports.size(); i++)
	{
		if (i == viewport)
			viewports.at(i).enabled = set;
	}
}
