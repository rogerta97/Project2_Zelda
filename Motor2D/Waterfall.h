#ifndef _WATERFALL_H_
#define _WATERFALL_H_

#include "Entity.h"

class Waterfall : public Entity
{
public:
	Waterfall(iPoint pos);
	~Waterfall();

	// Draw and change animations
	bool Draw(float dt);

private:

public:

private:

};

#endif // !_WATERFALL_H_

