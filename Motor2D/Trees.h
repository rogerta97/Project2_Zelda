
#ifndef  _TREES_H_
#define _TREES_H_

#include "j1Entity.h"
#include "Entity.h"
#include "AestheticsManager.h"

class Tree : public Entity
{
public:

	Tree(iPoint pos);

	// Draw and change animations
	bool Draw(float dt);

	// CleanUp
	bool CleanUp();

	void SetTreeColor(const char* color); 

	~Tree();

	tree_color		type;

};


#endif