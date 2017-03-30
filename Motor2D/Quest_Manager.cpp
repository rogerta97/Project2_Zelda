#include "Quest_Manager.h"
#include "j1App.h"
#include "j1Render.h"
#include "MainScene.h"
#include "j1Scene.h"
#include "j1Viewports.h"

QuestManager::QuestManager()
{

		SDL_Rect screen = App->view->GetViewportRect(1);
		for(int i = 0;i<3;i++)
		{
			placer = iPoint(screen.w - 30, screen.h - 30);
			App->scene->main_scene->main_window_1->CreateImage(placer, { 182, 78, 25 ,25 }, false);
			screen.h = screen.h - 30;
		}
		screen = App->view->GetViewportRect(1);
		for (int i = 0; i<3; i++)
		{
			placer = iPoint(screen.w - 30, screen.h - 30);
			App->scene->main_scene->main_window_2->CreateImage(placer, { 182, 78, 25 ,25 }, false);
			screen.h = screen.h - 30;
		}
		screen = App->view->GetViewportRect(1);
		for (int i = 0; i<3; i++)
		{
			placer = iPoint(screen.w - 30, screen.h - 30);
			App->scene->main_scene->main_window_3->CreateImage(placer, { 182, 78, 25 ,25 }, false);
			screen.h = screen.h - 30;
		}
		screen = App->view->GetViewportRect(1);
		for (int i = 0; i<3; i++)
		{
			placer = iPoint(screen.w - 30, screen.h - 30);
			App->scene->main_scene->main_window_4->CreateImage(placer, { 182, 78, 25 ,25 }, false);
			screen.h = screen.h - 30;
		}
	


	App->LoadXML("Quests.xml", quests_file);
	quests_node = quests_file.child("quests");
	for (pugi::xml_node loop_tool = quests_node.child("quest"); loop_tool; loop_tool = loop_tool.next_sibling("quest"))
	{
		Quest* quest = new Quest;
		quest->id = loop_tool.attribute("id").as_int();

		quest->name = loop_tool.attribute("name").as_string();

		quest->description = loop_tool.attribute("description").as_string();

		switch (loop_tool.attribute("state").as_int())
		{
		case 0: {
			quest->state = (inactive);
			break; }
		case 1: {
			quest->state = (active);
			break; }
		default:
			break;
		}
		for (pugi::xml_node tool = quests_node.child("quest").child("task"); tool; tool = tool.next_sibling("task"))
		{
			Objectives* obj = new Objectives;
			obj->current_progress = loop_tool.child("task").attribute("current").as_int();
			obj->requirement = loop_tool.child("task").attribute("complete").as_int();
			obj->times_completed = loop_tool.child("task").attribute("times_completed").as_int();
			quest->task.push_back(obj);
			
		}
		vquest.push_back(quest);
	}







}

QuestManager::~QuestManager()
{
	for (int i = 0; i < vquest.size(); i++) {
		RELEASE(vquest[i]);
	}
}

void QuestManager::change_state(int id,Quest_State new_state)
{
	for (int i = 0; i < vquest.size(); i++)
	{
		if (vquest[i]->id == id)
		{
			vquest[i]->state = new_state;
			break;
		}
	}
}

void QuestManager::add_progress(int id,int team)
{
	for (int i = 0; i < vquest.size(); i++)
	{
		if (vquest[i]->id == id)
		{
			vquest[i]->task[team+1]->current_progress++;
			break;
		}
	}
}

void QuestManager::update_progress()
{
	for (int i = 0; i < vquest.size(); i++)
	{
		if (vquest[i]->state == active)
		{
			int i = 0;
			for (pugi::xml_node tool = quests_node.child("quest").child("task"); tool; tool = tool.next_sibling("task"))
			{
				if (vquest[i]->task[i]->current_progress == vquest[i]->task[i]->requirement)
				{
					vquest[i]->state = inactive;
					vquest[i]->task[i]->times_completed++;
					i++;
				}
			}

		}
	}
}
