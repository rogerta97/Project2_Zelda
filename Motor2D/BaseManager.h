#ifndef _BASEMANAGER_H_
#define _BASEMANAGER_H_

class Base;
class Entity;

class BaseManager
{
public:
	BaseManager();
	~BaseManager();

	bool Update();
	bool CleanUp();
	void KillBase(Base* base);

	Entity* GetBase(int team)const;
	void b_Invulnerable(Base* base);

private:
	Base*			team1 = nullptr;
	Base*			team2 = nullptr;

	uint			victory_sound_effect_1_1 = 100;
	uint			victory_sound_effect_1_2 = 100;
	uint			victory_sound_effect_2_1 = 100;

};
#endif // !_BASEMANAGER_H_

