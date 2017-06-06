#ifndef _ZELDAMANAGER_H_
#define _ZELDAMANAGER_H_

#include "p2Point.h"
#include "Zelda.h"

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

	zelda_states GetZeldaState(); 


private:
	Zelda* zelda = nullptr;
	uint   zelda_anouncer = 100;
	uint   zelda_intro_1 = 100;
	uint   zelda_intro_2 = 100;
	bool   anounced = false;
	int	   spawn_time = 0;
};

#endif
