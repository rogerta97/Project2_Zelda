#ifndef _ZELDA_H_
#define _ZELDA_H_

#include "Entity.h"
#include "j1Timer.h"

enum zelda_states
{
	z_s_wait,
	z_s_move_to_path,
	z_s_idle,
	z_s_move,
	z_s_null,
};

class j1Timer;

class Zelda : public Entity
{
public:
	Zelda(iPoint pos);
	~Zelda();

	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool Draw(float dt);
	bool PostUpdate();
	bool CleanUp();

	// Set Timer if not defined previously
	// Return true if set.
	bool SetTimer(j1Timer* timer);

	void SetInitialPath(std::vector<iPoint>& path);
	void SetPath(std::vector<iPoint>& path);

private:

	void MoveUp(float speed);
	void MoveDown(float speed);
	void MoveLeft(float speed);
	void MoveRight(float speed);

	void MoveUpRight(float speed);
	void MoveDownRight(float speed);
	void MoveUpLeft(float speed);
	void MoveDownLeft(float speed);

	void CheckState();

public:

private:
	uint				team1_players = 0;
	uint				team2_players = 0;

	bool				flip = false;

	int					radius = 0;

	SDL_Rect			area_image = NULLRECT;

	zelda_states		state = z_s_null;

	j1Timer*			game_timer = nullptr;

	std::vector<iPoint> main_path;
	uint				path_pos = 0;

	std::vector<iPoint> initial_path;
	uint				initial_path_pos = 0;

	bool				active = false;
	uint				activation_time = 0;
};


#endif // !_ZELDA_H_

