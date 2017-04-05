#ifndef _LOGOSCENE_H_
#define _LOGOSCENE_H_

#include "Scene.h"
#include "j1Gui.h"
#include "j1Render.h"
#include "j1Timer.h"

class LogoScene : public Scene
{
public:
	LogoScene();
	~LogoScene();

	bool Start();
	bool Update(float dt);
	bool CleanUp();

private:
	void FadeIn();
	void FadeOut();

public:

private:
	iPoint       win_size = NULLPOINT;

	SDL_Texture* logo = nullptr;
	SDL_Rect     logo_rect = NULLRECT;
	iPoint       logo_pos = NULLPOINT;

	j1Timer      timer;

	float        fade_value = 255.0f;

};


#endif // !_LOGO_H_
