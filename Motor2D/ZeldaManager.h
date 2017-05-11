#ifndef _ZELDAMANAGER_H_
#define _ZELDAMANAGER_H_

#include "p2Point.h"

class Zelda;

class ZeldaManager
{
public:
	ZeldaManager();
	~ZeldaManager();

	void CleanUp();

	iPoint GetZeldaPos();
	Zelda* GetZelda();
	int GetSpawnTime(); 

private:

public:

private:
	Zelda* zelda = nullptr;
};

#endif
