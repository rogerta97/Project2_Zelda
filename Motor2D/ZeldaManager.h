#ifndef _ZELDAMANAGER_H_
#define _ZELDAMANAGER_H_

#include "p2Point.h"

class Zelda;

class ZeldaManager
{
public:
	ZeldaManager();
	~ZeldaManager();

	bool Update();

	void CleanUp();

	iPoint GetZeldaPos();
	Zelda* GetZelda();

	int GetSpawnTime(); 

private:
	Zelda* zelda = nullptr;
	int zelda_anouncer;
	bool anounced = false;
};

#endif
