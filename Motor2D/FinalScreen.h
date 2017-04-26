#ifndef _FINALSCREEN_H_
#define _FINAL_SCREEN_H_

#include "Scene.h"
#include "j1Gui.h"
#include "j1App.h"

struct final_screen_player_info
{
	int				deaths = -1; 
	int				kills = -1;
	int				minions = -1;
	int				towers = -1;

	SDL_Rect		items[3]; 

	string			character_name; 
};

struct final_screen_element
{
	UI_Image*				character_image = nullptr; 
	UI_Image*				rectangle_image = nullptr; 

	UI_Text*				kills_text = nullptr; 
	UI_Text*				deaths_text = nullptr;
	UI_Text*				minions_text = nullptr;
	UI_Text*				towers_text = nullptr;
	UI_Text*				items_text = nullptr; 

	UI_Text*				kills_num = nullptr;
	UI_Text*				deaths_num = nullptr;
	UI_Text*				minions_num = nullptr;
	UI_Text*				towers_num = nullptr;
	UI_Text*				items_num = nullptr;

	UI_Image*				item_images_1; 
	UI_Image*				item_images_2;
	UI_Image*				item_images_3;

	void SetInfo(final_screen_player_info player_info);
};

class FinalScreen : public Scene
{
public:

	bool Start();

	bool Update(float dt); 

	bool CleanUp(); 

private:

	vector<final_screen_element> elements; 
	vector<final_screen_player_info> player_info;

	UI_Window*		window = nullptr; 
	SDL_Texture*	background_image = nullptr;

	UI_Image*		title_background = nullptr; 
	UI_Text*		title_text = nullptr; 

	UI_Image*		X_changescreen_button_image = nullptr;
	UI_Image*		X_changescreen_background_image = nullptr;
	UI_Text*		changescreen_text = nullptr; 

public: 

private: 


};
#endif // !_FINALSCREEN_H_

