#ifndef _REMAPINGSCENE_H_
#define _REMAPINGSCENE_H_
//
#include "Scene.h"
#include "j1Gui.h"
#include "Mapping.h"
#include "j1App.h"
#include "j1Viewports.h"

enum remaping_state
{
	r_s_state_null,

	r_s_confirm,
	r_s_back,
	r_s_minimap,
	r_s_shop,
	r_s_a1,
	r_s_a2,
	r_s_a3,
	r_s_a4,

	r_s_a,
	r_s_b,
	r_s_x,
	r_s_y,
	r_s_rb,
	r_s_rt,
	r_s_lb,
	r_s_lt,

};

struct remap_ui
{
	UI_Image* confirm_key = nullptr;
	UI_Image* back_key = nullptr;
	UI_Image* minimap_key = nullptr;
	UI_Image* shop_key = nullptr;
	UI_Image* a1_key = nullptr;
	UI_Image* a2_key = nullptr;
	UI_Image* a3_key = nullptr;
	UI_Image* a4_key = nullptr;

	UI_Image* confirm_background = nullptr;
	UI_Image* back_background = nullptr;
	UI_Image* minimap_background = nullptr;
	UI_Image* shop_background = nullptr;
	UI_Image* a1_background	= nullptr;
	UI_Image* a2_background	= nullptr;
	UI_Image* a3_background	= nullptr;
	UI_Image* a4_background	= nullptr;

	UI_Text* confirm_text = nullptr;
	UI_Text* back_text = nullptr;
	UI_Text* minimap_text = nullptr;
	UI_Text* shop_text = nullptr;
	UI_Text* a1_text = nullptr;
	UI_Text* a2_text = nullptr;
	UI_Text* a3_text = nullptr;
	UI_Text* a4_text = nullptr;

	UI_Image* a = nullptr;
	UI_Image* b = nullptr;
	UI_Image* x = nullptr;
	UI_Image* y = nullptr;
	UI_Image* rb = nullptr;
	UI_Image* lb = nullptr;
	UI_Image* rt = nullptr;
	UI_Image* lt = nullptr;

	UI_Image* cursor = nullptr;
	UI_Image* button_selector_cursor = nullptr; 
	UI_Image* button_support = nullptr; 

	remaping_state current_state = r_s_confirm;
	remaping_state prev_state = r_s_confirm; 

	vector<UI_Image*> selected_buttons; 
};

class RemapingScene : public Scene
{
public:

	RemapingScene();
	~RemapingScene();

	bool Start();
	bool Update(float dt);

	bool CleanUp();

	UI_Window* window; 

private:

	// Gets the position of the cursor depending on what the state is
	iPoint GetCursorPosFromCurrentState(remaping_state curr_state);

	// Change the current key content 
	void UpdateKeys(int i); 

	// Executed when the player press A to open the list of avalible buttons
	void EnterOption(int i); 

	// Hides button selection information 
	void QuitOption(int i); 

	// Check what buttons have to appear and puts them in the support
	void OpenFreeButtonList(int i);

	// Check if it's a button or a back-triger 
	bool IsButton(int id); 

	void SetButtonsFromID(key_mapping curr_key, int viewport);

	void MoveSelectorCursor(bool up, int viewport);

	
private:

	remap_ui		   remapping_ui[4];
	SDL_Texture*	   background_image = nullptr; 

	bool			   inside = false; 

};

#endif
