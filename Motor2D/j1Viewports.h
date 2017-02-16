#ifndef __j1VIEWPORTS_H__
#define __j1VIEWPORTS_H__

#include "j1Module.h"
#include "j1Gui.h"
#include "Scene.h"

struct SDL_Texture;
class MainScene;

// LayerBlit struct
struct layer_blit
{
	layer_blit() {};

	layer_blit(SDL_Texture* _texture, iPoint _pos, const SDL_Rect _section, float _scale, SDL_RendererFlip _flip, double _angle, int _pivot_x, int _pivot_y)
	{
		texture = _texture;
		pos = _pos;
		section.x = _section.x;  section.y = _section.y; section.w = _section.w; section.h = _section.h;
		scale = _scale;
		flip = _flip;
		angle = _angle;
		pivot_x = _pivot_x;
		pivot_y = _pivot_y;
	};

	SDL_Texture*	 texture = nullptr;
	iPoint			 pos = NULLPOINT;
	SDL_Rect		 section = NULLRECT;
	float			 scale = 0.0f;
	SDL_RendererFlip flip;
	double           angle = 0;
	int              pivot_x = 0;
	int              pivot_y = 0;
};

class j1Viewports : public j1Module
{
public:

	j1Viewports();

	// Destructor
	virtual ~j1Viewports();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Blit choosing the layer
	void LayerBlit(int layer, SDL_Texture* texture, iPoint pos, const SDL_Rect section = NULLRECT, float scale = -1.0f, SDL_RendererFlip _flip = SDL_FLIP_NONE, double angle = 0, int pivot_x = INT_MAX, int pivot_y = INT_MAX);
	void LayerDrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool filled, bool use_camera = true) const;
	void LayerDrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool use_camera = true) const;
	void LayerDrawCircle(int x1, int y1, int redius, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool use_camera = true) const;
	void SetViews(uint number);
	uint GetViews();

	void OnCommand(std::list<std::string>& tokens);

	void OnCVar(std::list<std::string>& tokens);

	void SaveCVar(std::string& cvar_name, pugi::xml_node& node) const;

private:
	// Blit in the layer order
	void DoLayerBlit();

public:
	iPoint				 camera1 = NULLPOINT;
	iPoint				 camera2 = NULLPOINT;
	iPoint				 camera3 = NULLPOINT;
	iPoint				 camera4 = NULLPOINT;

private:
	// Layer Blit list
	p2PQueue<layer_blit> layer_list;

	// Win Size
	uint                 win_w = 0, win_h = 0;

	// Views
	uint				 number_of_views = 0;

	SDL_Rect			 view2_1 = NULLRECT;
	SDL_Rect			 view2_2 = NULLRECT;

	SDL_Rect			 view4_1 = NULLRECT;
	SDL_Rect			 view4_2 = NULLRECT;
	SDL_Rect			 view4_3 = NULLRECT;
	SDL_Rect			 view4_4 = NULLRECT;

};

#endif // __j1VIEWPORTS_H__