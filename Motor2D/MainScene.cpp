#include "MainScene.h"
#include "j1Scene.h"
#include "p2Log.h"
#include "j1Input.h"
#include "Functions.h"
#include "j1Physics.h"
#include "GameObject.h"
#include "j1App.h"
#include "j1Gui.h"
#include "Parallax.h"
#include "j1Entity.h"
#include "CollisionFilters.h"


MainScene::MainScene()
{
}

MainScene::~MainScene()
{
}

bool MainScene::Start()
{
	bool ret = true;

	LOG("Start MainScene");

	PhysBody* b = App->physics->CreateStaticRectangle(0, 600, 10000, 50, 1, 1, 0, App->cf->CATEGORY_SCENERY, App->cf->MASK_SCENERY);
	b->type = pbody_type::p_t_world;
	b->listener = App->scene;

	App->entity->CreateEntity(player);

	return ret;
}

bool MainScene::PreUpdate()
{
	bool ret = true;

	return ret;
}

bool MainScene::Update(float dt)
{
	bool ret = true;

	//if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	//	App->render->camera.x++;
	//if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	//	App->render->camera.x--;
	//if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	//	App->render->camera.y++;
	//if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	//	App->render->camera.y--;

	return ret;
}

bool MainScene::PostUpdate()
{
	bool ret = true;


	return ret;
}

bool MainScene::CleanUp()
{
	bool ret = true;


	return ret;
}

void MainScene::OnColl(PhysBody * bodyA, PhysBody * bodyB, b2Fixture * fixtureA, b2Fixture * fixtureB)
{

}

