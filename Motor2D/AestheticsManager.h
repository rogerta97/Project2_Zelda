#ifndef _AESTHETICSMANAGER_H_
#define _AESTHETICSMANAGER_H_

#include <vector>
#include <list>
#include "j1Entity.h"
#include "p2Point.h"

class Entity;
class Tree;

enum tree_color
{
	green_tree, 
	yellow_tree, 
	purple_tree
};

struct TreeNode
{
	iPoint			tree_pos; 
	tree_color		color; 
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
	std::vector<Tree*>		trees_entity;

	std::vector<TreeNode*>		trees_nodes; 

};

#endif

