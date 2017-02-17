#ifndef _MAINSCENE_H_
#define _MAINSCENE_H_

#include "Scene.h"
#include "j1Render.h"

class GameObject;
class b2Fixture;
class Parallax;
class Player;

class MainScene : public Scene 
{
public:
	MainScene();
	~MainScene();

	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	void OnColl(PhysBody* bodyA, PhysBody* bodyB, b2Fixture* fixtureA, b2Fixture* fixtureB);

	GameObject* go = nullptr;
	Parallax* p1 = nullptr;

	fPoint pos;

private:
	Player* test_player;
	Player* test_player2;

};


#endif // !_MAINSCENE_H_