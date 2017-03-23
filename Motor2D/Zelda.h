#ifndef _ZELDA_H_
#define _ZELDA_H_

#include "Entity.h"
#include "j1Timer.h"

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

private:

	void MoveUp(float speed);
	void MoveDown(float speed);
	void MoveLeft(float speed);
	void MoveRight(float speed);

	void MoveUpRight(float speed);
	void MoveDownRight(float speed);
	void MoveUpLeft(float speed);
	void MoveDownLeft(float speed);

public:

private:

};


#endif // !_ZELDA_H_

