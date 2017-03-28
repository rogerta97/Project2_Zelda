#ifndef _AESTHETICSMANAGER_H_
#define _AESTHETICSMANAGER_H_

#include <vector>
#include <list>
#include "j1Entity.h"
#include "p2Point.h"

class Entity;
class Bush; 
class Tree;
class Eyes;

enum tree_color
{
	green_tree, 
	yellow_tree, 
	purple_tree
};

enum bush_color
{
	green_bush,
	purple_bush
};

struct TreeNode
{
	iPoint			tree_pos; 
	tree_color		color; 
};

struct BushNode
{
	iPoint			bush_pos;
	bush_color		color;
};

class AestheticsManager
{
public:
	AestheticsManager();
	~AestheticsManager();

	void Start(); 

	void CleanUp(); 

private:
	// Trunk
	Entity*					trunk_entity = nullptr; 

	// Trees
	std::vector<Tree*>		trees_entity;

	std::vector<TreeNode*>	trees_nodes; 

	// Bushes 
	std::vector<Bush*>		bushes_entity;

	std::vector<BushNode*>	bushes_nodes;

	// Eyes
	std::vector<Eyes*>		eyes;

};

#endif

