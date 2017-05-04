#ifndef _BASE_H_
#define _BASE_H_

#include "Entity.h"

class Base : public Entity
{
public:
	Base(iPoint pos);
	~Base();

	// Start variables
	bool Start();

	// Update
	bool Update(float dt);

	// Draw and change animations
	bool Draw(float dt);

	// CleanUp
	bool CleanUp();

	iPoint GetPos()const;

	void Die(Entity* killed_by);

private:

public:

private:

};


#endif // !_BASE_H_

