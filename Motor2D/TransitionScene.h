#ifndef _TRANSITIONSCENE_H_
#define _TRANSITIONSCENE_H_

#include "Scene.h"
#include "j1Gui.h"
#include "j1Render.h"
#include "j1Timer.h"

class TransitionScene : public Scene
{
public:
	TransitionScene();
	~TransitionScene();

	bool Start();
	bool Update(float dt);
	bool CleanUp();

private:
	void FadeIn();
	void FadeOut();

private:

	SDL_Texture* screen = nullptr;

	UI_Window* window = nullptr; 

	UI_Image* background_text_image = nullptr;
	UI_Text*  continue_text = nullptr; 
};

#endif