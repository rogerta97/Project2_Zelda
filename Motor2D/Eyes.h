#ifndef _EYES_H_
#define _EYES_H_

#include "Entity.h"

class Eyes : public Entity
{
public:
	Eyes(iPoint pos);
	~Eyes();

	// Start variables
	bool Start();

	// Draw and change animations
	bool Draw(float dt);

private:

public:

private:

};


#endif // !_EYES_H_

