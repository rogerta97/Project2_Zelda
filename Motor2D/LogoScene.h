#ifndef _LOGOSCENE_H_
#define _LOGOSCENE_H_

#include "Scene.h"
#include "j1Gui.h"
#include "j1Render.h"
#include "j1Timer.h"
#include "Video.h"

class LogoScene : public Scene
{
public:
	LogoScene();
	~LogoScene();

	bool Start();
	bool Update(float dt);
	bool CleanUp();

private:
	iPoint       win_size = NULLPOINT;
};


#endif // !_LOGO_H_
