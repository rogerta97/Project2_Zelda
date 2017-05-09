#ifndef _MINIMAPMANAGER_H_
#define _MINIMAPMANAGER_H_

#include "j1Render.h"

#define MINIMAP_LAYER 99999

class Player;

struct minimap_point
{
	minimap_point() {};
	minimap_point(char* _name, SDL_Rect _image, int _viewport = 0) 
	{
		name = _name; image = _image; viewport = _viewport;
	};
	~minimap_point() {};

	void SetImage(SDL_Rect _image) { image = _image; };

	bool     show = true;
	SDL_Rect image = NULLRECT;
	iPoint	 real_pos = NULLPOINT;
	string   name;

	int      viewport = 0;
};

struct viewport
{
	viewport() {};
	~viewport() {};

	int  view = 0;
	bool enabled = false;
};

class MinimapManager
{
public:
	MinimapManager();
	~MinimapManager();

	bool Start();
	bool Update(float dt);
	bool CleanUp();

	void UpdatePointsPos();

	// Set basic minimap info
	void SetMinimapInfo(iPoint minimap_position, SDL_Texture* minimap_texture, SDL_Rect minimap_rect, SDL_Rect real_map_rect, SDL_Texture* minimap_points_texture, int viewport = 0);

	// Add a minimap point
	minimap_point* AddPoint(char* name, SDL_Rect image = NULLRECT, int viewport = 0);

	// Get a minimap point
	minimap_point* GetPoint(char* name);

	// Delete a minimap point
	void DeletePoint(char* name);

	// Enable or disable the minimap on a viewport (1 to 4)
	void SetActive(bool set, int viewport);



private:
	SDL_Rect GetPlayerRect(Player* pl);

public:

private:
	iPoint		 minimap_position = NULLPOINT;
	SDL_Texture* minimap_texture = nullptr;
	SDL_Rect     minimap_rect = NULLRECT;

	SDL_Rect     real_map_rect = NULLRECT;

	SDL_Texture* minimap_points_texture = nullptr;
	vector<minimap_point*> minimap_points;

	vector<viewport> viewports;

	bool         active = false;

	float        multiplication_factor_x = 0.0f;
	float        multiplication_factor_y = 0.0f;

	SDL_Rect     zelda_rect = NULLRECT;
	SDL_Rect	 link_1_rect = NULLRECT;
	SDL_Rect     link_2_rect = NULLRECT;
	SDL_Rect     navi_1_rect = NULLRECT;
	SDL_Rect     navi_2_rect = NULLRECT;
	SDL_Rect	 ganon_1_rect = NULLRECT;
	SDL_Rect     ganon_2_rect = NULLRECT;
};
#endif // !_BASEMANAGER_H_