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
		player_1_text.push_back(App->scene->main_scene->main_window_1->CreateText(iPoint(placer.x - 50, placer.y-150), App->font->game_font_small));
		player_1_text[3]->SetText("Active Quest");

		active_quest_text.push_back(App->scene->main_scene->main_window_1->CreateText(iPoint(placer.x - 400, placer.y - 125), App->font->game_font_small));
		active_quest_text[0]->SetText(" ");

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
		player_2_text.push_back(App->scene->main_scene->main_window_2->CreateText(iPoint(placer.x - 50, placer.y - 150), App->font->game_font_small));
		player_2_text[3]->SetText("Active Quest");

		active_quest_text.push_back(App->scene->main_scene->main_window_2->CreateText(iPoint(placer.x - 50, placer.y - 125), App->font->game_font_small));
		active_quest_text[1]->SetText(" ");

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
		player_3_text.push_back(App->scene->main_scene->main_window_3->CreateText(iPoint(placer.x - 50, placer.y - 150), App->font->game_font_small,100));
		player_3_text[3]->SetText("Active Quest");

		active_quest_text.push_back(App->scene->main_scene->main_window_3->CreateText(iPoint(placer.x - 50, placer.y - 125), App->font->game_font_small));
		active_quest_text[2]->SetText(" ");

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
		player_4_text.push_back(App->scene->main_scene->main_window_4->CreateText(iPoint(placer.x - 50, placer.y - 150), App->font->game_font_small,25));
		player_4_text[3]->SetText("Active Quest");
	
		active_quest_text.push_back(App->scene->main_scene->main_window_4->CreateText(iPoint(placer.x - 50, placer.y - 125), App->font->game_font_small));
		active_quest_text[3]->SetText(" ");


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

void QuestManager::CleanUp()
{
	//Quests
	if (!vquest.empty())
	{
		for(int i = 0;i<vquest.size();i++)
		{
			for (int j = 0; j < vquest[i]->task.size(); j++)
			{
				RELEASE(vquest[i]->task[j]);
			}
			vquest[i]->task.clear();
			RELEASE(vquest[i]);
		}
		vquest.clear();
	}
	//Texts
	if (!player_1_text.empty())
	{
		player_1_text.clear();
		player_2_text.clear();
		player_3_text.clear();
		player_4_text.clear();
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
			switch (i)
			{
			case 0:
			{
				string team_1;
				team_1 += "Power quest is active, slay 2 enemy players. Progress ";
				team_1 += std::to_string(vquest[i]->task[0]->current_progress);
				team_1 += "l2";
				active_quest_text[0]->SetText(team_1);
				active_quest_text[2]->SetText(team_1);
				string team_2;
				team_2 += "Power quest is active, slay 2 enemy players. Progress ";
				team_2 += std::to_string(vquest[i]->task[1]->current_progress);
				team_2 += +"l2";
				active_quest_text[1]->SetText(team_2);
				active_quest_text[3]->SetText(team_2);
				break;
			}
			case 1:
			{
				string team_1;
				team_1 += "SPEED quest is active, RUN FASTER THAN SANIC TO WIN. Progress ";
				team_1 += std::to_string(vquest[i]->task[0]->current_progress);
				team_1 += "l3";
				active_quest_text[0]->SetText(team_1);
				active_quest_text[2]->SetText(team_1);
				string team_2;
				team_2 += "SPEED quest is active, RUN FASTER THAN SANIC TO WIN. Progress ";
				team_2 += std::to_string(vquest[i]->task[1]->current_progress);
				team_2 += +"l3";
				active_quest_text[1]->SetText(team_2);
				active_quest_text[3]->SetText(team_2);
				break;
			}
			case 2:
			{
				string team_1;
				team_1 += "KILL STUFF TO GET HP. Progress ";
				team_1 += std::to_string(vquest[i]->task[0]->current_progress);
				team_1 += "l3";
				active_quest_text[0]->SetText(team_1);
				active_quest_text[2]->SetText(team_1);
				string team_2;
				team_2 += "KILL STUFF TO GET HP. Progress ";
				team_2 += std::to_string(vquest[i]->task[1]->current_progress);
				team_2 += +"l3";
				active_quest_text[1]->SetText(team_2);
				active_quest_text[3]->SetText(team_2);
				break;
			}
			default:
				break;
			}

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
