#ifndef _ZELDAMANAGER_H_
#define _ZELDAMANAGER_H_

#include "p2Point.h"
#include "Zelda.h"

class Zelda;

enum anounced_state
{
	a_null,
	a_idle,
	a_prepare,
	a_anounced
};

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
	int	   spawn_time = 0;

	anounced_state state = a_null;
};

#endif
