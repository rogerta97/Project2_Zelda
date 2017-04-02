#include "Quest_Manager.h"
#include "j1App.h"
#include "j1Render.h"
#include "MainScene.h"
#include "j1Scene.h"
#include "j1Viewports.h"
#include "Entity.h"
#include "PlayerManager.h"

QuestManager::QuestManager()
{

		SDL_Rect screen = App->view->GetViewportRect(1);
		int offset = 0;
		for(int i = 0;i<3;i++)
		{
			placer = iPoint(screen.w - 30, screen.h - 30);
			App->scene->main_scene->main_window_1->CreateImage(placer, { 472, 812-offset, 24 ,24 }, false);

			player_1_text.push_back(App->scene->main_scene->main_window_1->CreateText(iPoint(placer.x+6,placer.y), App->font->game_font_small));
			player_1_text[i]->SetText("0");

			screen.h = screen.h - 30;
			offset += 24;
		}
		screen = App->view->GetViewportRect(1);
		offset = 0;
		for (int i = 0; i<3; i++)
		{
			placer = iPoint(screen.w - 30, screen.h - 30);
			App->scene->main_scene->main_window_2->CreateImage(placer, { 472, 812 - offset, 24 ,24 }, false);

			player_2_text.push_back(App->scene->main_scene->main_window_2->CreateText(iPoint(placer.x + 6, placer.y), App->font->game_font_small));
			player_2_text[i]->SetText("0");

			screen.h = screen.h - 30;
			offset += 24;
		}
		screen = App->view->GetViewportRect(1);
		offset = 0;
		for (int i = 0; i<3; i++)
		{
			placer = iPoint(screen.w - 30, screen.h - 30);
			App->scene->main_scene->main_window_3->CreateImage(placer, { 472, 812 - offset, 24 ,24 }, false);

			player_3_text.push_back(App->scene->main_scene->main_window_3->CreateText(iPoint(placer.x + 6, placer.y), App->font->game_font_small));
			player_3_text[i]->SetText("0");

			screen.h = screen.h - 30;
			offset += 24;
		}
		screen = App->view->GetViewportRect(1);
		offset = 0;
		for (int i = 0; i<3; i++)
		{
			placer = iPoint(screen.w - 30, screen.h - 30);
			App->scene->main_scene->main_window_4->CreateImage(placer, { 472, 812 - offset, 24 ,24 }, false);

			player_4_text.push_back(App->scene->main_scene->main_window_4->CreateText(iPoint(placer.x + 6, placer.y), App->font->game_font_small));
			player_4_text[i]->SetText("0");

			screen.h = screen.h - 30;
			offset += 24;
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
			vquest[i]->task[team-1]->current_progress++;
			break;
		}
	}
}

int QuestManager::get_progress(int id, int team)
{
	return vquest[id - 1]->task[team - 1]->times_completed;
}

void QuestManager::update_progress()
{
	for (int i = 0; i < vquest.size(); i++)
	{
		if (vquest[i]->state == active)
		{
			int j = 0;
			for (pugi::xml_node tool = quests_node.child("quest").child("task"); tool; tool = tool.next_sibling("task"))
			{
				if (vquest[i]->task[j]->current_progress == vquest[i]->task[j]->requirement)
				{
					vquest[i]->state = inactive;
					vquest[i]->task[i]->times_completed++;
					for (int k = 0; k < App->scene->main_scene->player_manager->players.size(); k++)
					{
						App->scene->main_scene->player_manager->players[k]->entity->UpdateStats(0, 0, 0);
					}

					switch (j)
					{
					case 0:
					{
						player_1_text[1]->SetText(std::to_string(vquest[i]->task[j]->times_completed));
						player_3_text[1]->SetText(std::to_string(vquest[i]->task[j]->times_completed));
						break;
					}
					case 1:
					{
						player_2_text[1]->SetText(std::to_string(vquest[i]->task[j]->times_completed));
						player_4_text[1]->SetText(std::to_string(vquest[i]->task[j]->times_completed));
						break;
					}
					default:
						break;
					}
				}
				j++;
			}

		}
	}
}
