#ifndef _AESTHETICSMANAGER_H_
#define _AESTHETICSMANAGER_H_

#include <list>

class Entity;

enum aes_entities
{

};

class AestheticsManager
{
public:
	AestheticsManager();
	~AestheticsManager();

	void Start(); 

	void CleanUp(); 

private:

	std::list<Entity*>		trees; 
	std::list<Entity*>		bushes; 
	Entity*					trunk_entity; 

};

#endif

