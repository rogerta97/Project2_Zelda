#ifndef __j1SCENE_H__
#define __j1SCENE_H__

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

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

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

	// Change scene
	void ChangeScene(Scene* new_scene);

	// Blit choosing the layer
	void LayerBlit(int layer, SDL_Texture* texture, iPoint pos, const SDL_Rect section = NULLRECT, float scale = -1.0f, SDL_RendererFlip _flip = SDL_FLIP_NONE,  double angle = 0, int pivot_x = INT_MAX, int pivot_y = INT_MAX);
	
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB, b2Fixture* fixtureA, b2Fixture* fixtureB);

private:
	// Blit in the layer order
	void DoLayerBlit();

public:
	// Scenes
	MainScene*           main_scene = nullptr;

private:
	// Layer Blit list
	p2PQueue<layer_blit> layer_list;

	// Scenes list
	list<Scene*>         scenes;

	// Current scene
	Scene*               current_scene = nullptr;

};

#endif // __j1SCENE_H__