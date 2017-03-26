#ifndef _AESTHETICSMANAGER_H_
#define _AESTHETICSMANAGER_H_

#include <vector>
#include <list>
#include "j1Entity.h"
#include "p2Point.h"

class Entity;
class Tree;

enum tree_type
{
	green_tree, 
	yellow_tree, 
	purple_tree
};

struct TreeNode
{
	iPoint			tree_pos; 
	entity_name		type; 
};

class AestheticsManager
{
public:
	AestheticsManager();
	~AestheticsManager();

	void Start(); 

	void CleanUp(); 

private:
	
	Entity*						trunk_entity = nullptr; 
	std::vector<Entity*>		trees_entity;

	std::vector<TreeNode*>		trees_pos; 

};

#endif

