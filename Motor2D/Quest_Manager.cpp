#include "Quest_Manager.h"
#include "j1App.h"
#include "j1Render.h"
#include "MainScene.h"
#include "j1Viewports.h"
#include "Entity.h"
#include "PlayerManager.h"
#include "j1XMLLoader.h"
#include "j1Map.h"
#include "j1Pathfinding.h"

QuestManager::QuestManager()
{
	SDL_Rect screen = App->view->GetViewportRect(1);
	int offset = 0;

	PlayerText* curr_player_text = nullptr; 

	for (vector<MainSceneViewport>::iterator it = App->scene->main_scene->ui_viewports.begin(); it != App->scene->main_scene->ui_viewports.end(); it++)
	{
		curr_player_text = new PlayerText(); 

		for (int i = 0; i<3; i++)
		{
			placer = iPoint(screen.w - 30, screen.h - 30);
			it->main_window->CreateImage(placer, { 472, 812 - offset, 24 ,24 }, false);

			curr_player_text->player_text.push_back(it->main_window->CreateText(iPoint(placer.x + 6, placer.y), App->font->game_font_small));
			curr_player_text->player_text[i]->SetText("0");

			screen.h = screen.h - 30;
			offset += 24;
		}

		curr_player_text->active_quest_text.push_back(it->main_window->CreateText(iPoint(screen.w / 4 - 80, 50), App->font->game_font_12, 0, false, 255, 215, 0));
		curr_player_text->active_quest_text[0]->SetText(" ");

		screen = App->view->GetViewportRect(1);
		offset = 0;

		player_text_list.push_back(curr_player_text); 
	}

	//for(int i = 0;i<3;i++)
	//{
	//	placer = iPoint(screen.w - 30, screen.h - 30);
	//	App->scene->main_scene->main_window_1->CreateImage(placer, { 472, 812-offset, 24 ,24 }, false);

	//	player_1_text.push_back(App->scene->main_scene->main_window_1->CreateText(iPoint(placer.x+6,placer.y), App->font->game_font_small));
	//	player_1_text[i]->SetText("0");

	//	screen.h = screen.h - 30;
	//	offset += 24;
	//}
	//active_quest_text.push_back(App->scene->main_scene->main_window_1->CreateText(iPoint(screen.w/4 - 80 , 50), App->font->game_font_12, 0, false, 255, 215, 0));
	//active_quest_text[0]->SetText(" ");

	//screen = App->view->GetViewportRect(1);
	//offset = 0;

	//for (int i = 0; i<3; i++)
	//{
	//	placer = iPoint(screen.w - 30, screen.h - 30);
	//	App->scene->main_scene->main_window_2->CreateImage(placer, { 472, 812 - offset, 24 ,24 }, false);

	//	player_2_text.push_back(App->scene->main_scene->main_window_2->CreateText(iPoint(placer.x + 6, placer.y), App->font->game_font_small));
	//	player_2_text[i]->SetText("0");

	//	screen.h = screen.h - 30;
	//	offset += 24;
	//}
	//active_quest_text.push_back(App->scene->main_scene->main_window_2->CreateText(iPoint(screen.w / 4 - 80, 50), App->font->game_font_12, 0, false, 255, 215, 0));
	//active_quest_text[1]->SetText(" ");

	//screen = App->view->GetViewportRect(1);
	//offset = 0;

	//for (int i = 0; i<3; i++)
	//{
	//	placer = iPoint(screen.w - 30, screen.h - 30);
	//	App->scene->main_scene->main_window_3->CreateImage(placer, { 472, 812 - offset, 24 ,24 }, false);

	//	player_3_text.push_back(App->scene->main_scene->main_window_3->CreateText(iPoint(placer.x + 6, placer.y), App->font->game_font_small));
	//	player_3_text[i]->SetText("0");

	//	screen.h = screen.h - 30;
	//	offset += 24;
	//}
	//active_quest_text.push_back(App->scene->main_scene->main_window_3->CreateText(iPoint(screen.w / 4 - 80, 50), App->font->game_font_12, 0, false, 255, 215, 0));
	//active_quest_text[2]->SetText(" ");

	//screen = App->view->GetViewportRect(1);
	//offset = 0;

	//for (int i = 0; i<3; i++)
	//{
	//	placer = iPoint(screen.w - 30, screen.h - 30);
	//	App->scene->main_scene->main_window_4->CreateImage(placer, { 472, 812 - offset, 24 ,24 }, false);

	//	player_4_text.push_back(App->scene->main_scene->main_window_4->CreateText(iPoint(placer.x + 6, placer.y), App->font->game_font_small));
	//	player_4_text[i]->SetText("0");

	//	screen.h = screen.h - 30;
	//	offset += 24;
	//}	
	//active_quest_text.push_back(App->scene->main_scene->main_window_4->CreateText(iPoint(screen.w / 4 - 80, 50), App->font->game_font_12,0,false,255, 215, 0));
	//active_quest_text[3]->SetText(" ");


	App->xml->LoadXML("Quests.xml", quests_file);
	quests_node = quests_file.child("quests");
	for (pugi::xml_node loop_tool = quests_node.child("quest"); loop_tool; loop_tool = loop_tool.next_sibling("quest"))
	{
		quest* q = new quest();
		q->id = loop_tool.attribute("id").as_int();

		q->name = loop_tool.attribute("name").as_string();

		q->description = loop_tool.attribute("description").as_string();

		switch (loop_tool.attribute("state").as_int())
		{
		case 0: {
			q->state = (inactive);
			break; }
		case 1: {
			q->state = (active);
			break; }
		default:
			break;
		}
		for (pugi::xml_node tool = quests_node.child("quest").child("task"); tool; tool = tool.next_sibling("task"))
		{
			objectives* obj = new objectives();
			obj->current_progress = loop_tool.child("task").attribute("current").as_int();
			obj->requirement = loop_tool.child("task").attribute("complete").as_int();
			obj->times_completed = loop_tool.child("task").attribute("times_completed").as_int();
			q->task.push_back(obj);
			
		}
		vquest.push_back(q);
	}


	for (vector<PlayerText*>::iterator it = player_text_list.begin(); it != player_text_list.end(); it++)
	{
		(*it)->active_quest_text.at(0)->enabled = false; 
	}

}

QuestManager::~QuestManager()
{
}

void QuestManager::Update()
{
	if (App->scene->main_scene->GetGameTimer()->ReadSec() - timer_read > 2 && active_quest == -1)
	{

		active_quest = 2;

		change_state(active_quest, active);
		timer_read = App->scene->main_scene->GetGameTimer()->ReadSec();
		update_progress();

		for (vector<PlayerText*>::iterator it = player_text_list.begin(); it != player_text_list.end(); it++)
		{
			(*it)->active_quest_text.at(0)->enabled = true;
		}

		/*active_quest_text[0]->enabled = true;
		active_quest_text[1]->enabled = true;
		active_quest_text[2]->enabled = true;
		active_quest_text[3]->enabled = true;*/
	}

	if (active_quest != -1 && App->scene->main_scene->GetGameTimer()->ReadSec() - timer_read > 120)
	{
		reset_progress(active_quest);
		change_state(active_quest, inactive);
		active_quest = -1;
		timer_read = App->scene->main_scene->GetGameTimer()->ReadSec();

		for (vector<PlayerText*>::iterator it = player_text_list.begin(); it != player_text_list.end(); it++)
		{
			(*it)->active_quest_text.at(0)->enabled = false;
		}


	/*	active_quest_text[0]->enabled = false;
		active_quest_text[1]->enabled = false;
		active_quest_text[2]->enabled = false;
		active_quest_text[3]->enabled = false;*/
	}
}

void QuestManager::CleanUp()
{
	// Quests
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

	// Texts

	for (vector<PlayerText*>::iterator it = player_text_list.begin(); it != player_text_list.end(); it++)
	{
		(*it)->player_text.clear();
	}
	
}

void QuestManager::DeathQuestEvent(Entity * attacant, Entity * victim)
{
	if (vquest[0]->state == active)
	{
		if (attacant != nullptr && victim != nullptr && attacant->is_player && victim->is_player)
		{
			switch (victim->GetTeam())
			{
			case 1:
			{
				App->scene->main_scene->quest_manager->add_progress(1, 2);
				break;
			}
			case 2:
			{
				App->scene->main_scene->quest_manager->add_progress(1, 1);
				break;
			}
			default:
				break;
			}
			App->scene->main_scene->quest_manager->update_progress();
		}
	}
}

void QuestManager::reset_progress(int id)
{
	for(int i = 0; i<vquest.size();i++)
	{
		if (vquest[i]->id == id) {
			for (int j = 0; j < vquest[i]->task.size(); j++)
			{
				vquest[i]->task[j]->current_progress = 0;
			}
		}
	}
}

void QuestManager::change_state(int id, quest_state new_state)
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
				player_text_list[0]->active_quest_text[0]->SetText(team_1);
				player_text_list[2]->active_quest_text[0]->SetText(team_1);
				string team_2;
				team_2 += "Power quest is active, slay 2 enemy players. Progress ";
				team_2 += std::to_string(vquest[i]->task[1]->current_progress);
				team_2 += "l2";
				player_text_list[1]->active_quest_text[0]->SetText(team_2);
				player_text_list[3]->active_quest_text[0]->SetText(team_2);
				break;
			}
			case 1:
			{
				string team_1;
				team_1 += "SPEED quest is active. Progress ";
				team_1 += std::to_string(vquest[i]->task[0]->current_progress);
				team_1 += "l3";
				player_text_list[0]->active_quest_text[0]->SetText(team_1);
				player_text_list[2]->active_quest_text[0]->SetText(team_1);
				string team_2;
				team_2 += "SPEED quest is active. Progress ";
				team_2 += std::to_string(vquest[i]->task[1]->current_progress);
				team_2 += "l3";
				player_text_list[1]->active_quest_text[0]->SetText(team_2);
				player_text_list[3]->active_quest_text[0]->SetText(team_2);
				break;
			}
			case 2:
			{
				string team_1;
				team_1 += "KILL STUFF TO GET HP. Progress ";
				team_1 += std::to_string(vquest[i]->task[0]->current_progress);
				team_1 += "l3";
				player_text_list[0]->active_quest_text[0]->SetText(team_1);
				player_text_list[2]->active_quest_text[0]->SetText(team_1);
				string team_2;
				team_2 += "KILL STUFF TO GET HP. Progress ";
				team_2 += std::to_string(vquest[i]->task[1]->current_progress);
				team_2 += "l3";
				player_text_list[1]->active_quest_text[0]->SetText(team_2);
				player_text_list[3]->active_quest_text[0]->SetText(team_2);
				break;
			}
			default:
				break;
			}
		}

			int j = 0;
			for (pugi::xml_node tool = quests_node.child("quest").child("task"); tool; tool = tool.next_sibling("task"))
			{
				if (vquest[i]->task[j]->current_progress == vquest[i]->task[j]->requirement)
				{
					vquest[i]->state = inactive;
					reset_progress(vquest[i]->id);
					vquest[i]->task[j]->times_completed++;
					player_text_list[0]->active_quest_text[0] ->enabled = false;
					player_text_list[1]->active_quest_text[0] ->enabled = false;
					player_text_list[2]->active_quest_text[0] ->enabled = false;
					player_text_list[3]->active_quest_text[0] ->enabled = false;

					for (int k = 0; k < App->scene->main_scene->player_manager->players.size(); k++)
					{
						//App->scene->main_scene->player_manager->players[k]->entity->UpdateStats(0, 0, 0);
					}

					switch (j)
					{
					case 0:
					{

						player_text_list[0]->player_text.at(0)->SetText(std::to_string(vquest[i]->task[j]->times_completed));
						player_text_list[2]->player_text.at(0)->SetText(std::to_string(vquest[i]->task[j]->times_completed));
//=======
//						player_1_text[1]->SetText(std::to_string(vquest[0]->task[j]->times_completed));
//						player_3_text[1]->SetText(std::to_string(vquest[0]->task[j]->times_completed));
//
//						player_1_text[0]->SetText(std::to_string(vquest[1]->task[j]->times_completed));
//						player_3_text[0]->SetText(std::to_string(vquest[1]->task[j]->times_completed));
//
//						player_1_text[2]->SetText(std::to_string(vquest[2]->task[j]->times_completed));
//						player_3_text[2]->SetText(std::to_string(vquest[2]->task[j]->times_completed));
//>>>>>>> refs/remotes/origin/development
						break;
					}
					case 1:
					{

						player_text_list[1]->player_text.at(0)->SetText(std::to_string(vquest[i]->task[j]->times_completed));
						player_text_list[3]->player_text.at(0)->SetText(std::to_string(vquest[i]->task[j]->times_completed));
//=======
//						player_2_text[1]->SetText(std::to_string(vquest[0]->task[j]->times_completed));
//						player_4_text[1]->SetText(std::to_string(vquest[0]->task[j]->times_completed));
//
//						player_2_text[0]->SetText(std::to_string(vquest[1]->task[j]->times_completed));
//						player_4_text[0]->SetText(std::to_string(vquest[1]->task[j]->times_completed));
//
//						player_2_text[2]->SetText(std::to_string(vquest[2]->task[j]->times_completed));
//						player_4_text[2]->SetText(std::to_string(vquest[2]->task[j]->times_completed));
//						break;
//>>>>>>> refs/remotes/origin/development
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

void QuestManager::SpawnCucos(int num)
{
	for (int i = 0; i < num; i++)
	{
		p2Point<int> pos;

		App->map->WorldToMap(pos.x = GetRandomValue(0, App->map->data.width), pos.y = GetRandomValue(0, App->map->data.height));
		while (!App->pathfinding->IsWalkable(pos))
		{
			App->map->WorldToMap(pos.x = GetRandomValue(0, App->map->data.width), pos.y = GetRandomValue(0, App->map->data.height));
		}
	iPoint poss = App->map->MapToWorld(pos.x, pos.y);
		cucos.push_back((Cuco*)App->entity->CreateEntity(cuco,poss));
	}
}
