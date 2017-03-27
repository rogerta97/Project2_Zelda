#pragma once

#ifndef  _BUSHES_H_
#define _BUSHES_H_

#include "Entity.h"

class Bush : public Entity
{
public:

	Bush(iPoint pos);

	// Draw and change animations
	bool Draw(float dt);

	void SetBushColor(const char* color); 
	bool IsInside(); 

	// CleanUp
	bool CleanUp();

	~Bush();

	bool		player_inside; 
};

#endif