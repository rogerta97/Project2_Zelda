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
	Base* team1 = nullptr;
	Base* team2 = nullptr;

};
#endif // !_BASEMANAGER_H_

