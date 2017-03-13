#ifndef _TEAMSELECTSCENE_H_
#define _TEAMSELECTSCENE_H_

#include "Scene.h"
#include <vector>

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

	UI_Window*				window = nullptr;

	UI_Image*				button_a = nullptr;
	UI_Image*				button_b = nullptr;
	UI_Image*				button_x = nullptr;
	UI_Image*				button_y = nullptr;

	UI_Image*				pad_view1 = nullptr;
	UI_Image*				pad_view2 = nullptr;
	UI_Image*				pad_view3 = nullptr;
	UI_Image*				pad_view4 = nullptr;

	UI_Image*				pad_middle1 = nullptr;
	UI_Image*				pad_middle2 = nullptr;
	UI_Image*				pad_middle3 = nullptr;
	UI_Image*				pad_middle4 = nullptr;

	UI_Image*				pad_1 = nullptr;
	UI_Image*				pad_2 = nullptr;
	UI_Image*				pad_3 = nullptr;
	UI_Image*				pad_4 = nullptr;

	UI_Text*				team_view1 = nullptr;
	UI_Text*				team_view2 = nullptr;
	UI_Text*				team_view3 = nullptr;
	UI_Text*				team_view4 = nullptr;

	UI_Text*				team_num_view1 = nullptr;
	UI_Text*				team_num_view2 = nullptr;
	UI_Text*				team_num_view3 = nullptr;
	UI_Text*				team_num_view4 = nullptr;

	std::vector<UI_Image*>	pads;
	std::vector<UI_Image*>	middle_pads;
	std::vector<UI_Text*>	team_nums;

	uint					team1_members = 0;
	uint					team2_members = 0;

	bool					positioned[4] = { false,false,false,false };
	bool					fixed[4] = { false,false,false,false };

};


#endif // !_TEAMSELECTSCENE_H_

