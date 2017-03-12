#ifndef _TEAMSELECTSCENE_H_
#define _TEAMSELECTSCENE_H_

#include "Scene.h"

class UI_Image;
class UI_Text;
class UI_Window;

class TeamSelectScene : public Scene
{
public:
	TeamSelectScene();

	~TeamSelectScene();

	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();


private:


public:

private:

	UI_Window*	window = nullptr;

	UI_Image*	button_a = nullptr;
	UI_Image*	button_b = nullptr;
	UI_Image*	button_x = nullptr;
	UI_Image*	button_y = nullptr;

	UI_Image*	pad_view1 = nullptr;
	UI_Image*	pad_view2 = nullptr;
	UI_Image*	pad_view3 = nullptr;
	UI_Image*	pad_view4 = nullptr;

	UI_Image*	pad_middle1 = nullptr;
	UI_Image*	pad_middle2 = nullptr;
	UI_Image*	pad_middle3 = nullptr;
	UI_Image*	pad_middle4 = nullptr;

	UI_Image*	pad_1 = nullptr;
	UI_Image*	pad_2 = nullptr;
	UI_Image*	pad_3 = nullptr;
	UI_Image*	pad_4 = nullptr;

	UI_Text*	team_view1 = nullptr;
	UI_Text*	team_view2 = nullptr;
	UI_Text*	team_view3 = nullptr;
	UI_Text*	team_view4 = nullptr;

	UI_Text*	team_num_view1 = nullptr;
	UI_Text*	team_num_view2 = nullptr;
	UI_Text*	team_num_view3 = nullptr;
	UI_Text*	team_num_view4 = nullptr;

};


#endif // !_TEAMSELECTSCENE_H_

