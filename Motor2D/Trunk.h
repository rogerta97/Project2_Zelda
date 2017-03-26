#pragma once
#ifndef  _TRUNK_H_
#define _TRUNK_H_

#include "Entity.h"

class Trunk : public Entity 
{
public: 

	Trunk(iPoint pos);

	// Draw and change animations
	bool Draw(float dt);

	// CleanUp
	bool CleanUp();

	~Trunk(); 


};

#endif // ! _TRUNK_H_

