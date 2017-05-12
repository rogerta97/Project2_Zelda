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
#include "Mapping.h"
#include "j1Scene.h"
#include "j1Audio.h"

#define WINDOW_SPEED 500

QuestManager::QuestManager()
{
	player_text_list.clear();
	quest_fx = App->audio->LoadFx("Audio/Voice act/new_quest_1.wav");
	SDL_Rect screen = App->view->GetViewportRect(1);
	int offset = 0;

	App->xml->LoadXML("quest_rects.xml", quests_animations_file);

	PlayerText* curr_player_text = nullptr; 

	for (int i = 0; i < 4; i++)
	{
		windows_to_move.push_back(false);
	}
	int button_it = 0;
	for (vector<MainSceneViewport>::iterator it = App->scene->main_scene->ui_viewports.begin(); it != App->scene->main_scene->ui_viewports.end(); it++)
	{
		player_quest_windows.push_back(it->viewport_window->CreateImage(iPoint(screen.w, 50), SDL_Rect{ 729,2204,178,107}, true));
		//BUTTON REMAPPING
		key_mapping shop_key = App->scene->players[button_it].mapping->GetMapping(m_k_shop);
		SDL_Rect button_pos = { 703,2334,28,26 };
		switch (shop_key.key_id)
		{
		case SDL_CONTROLLER_BUTTON_A:
			button_pos = { 703,2360,28,26 };
			break;
		case SDL_CONTROLLER_BUTTON_B:
			button_pos = { 703,2386,28,26 };
			break;
		case SDL_CONTROLLER_BUTTON_X:
			button_pos = { 703,2412,28,26 };
			break;
		case SDL_CONTROLLER_BUTTON_Y:
			button_pos = { 703,2334,28,26 };
			break;
		}
		player_remap_button.push_back(it->viewport_window->CreateImage(iPoint(screen.w-27,59),button_pos));
		button_it++;
		//
		curr_player_text = new PlayerText(); 

		for (int i = 0; i<3; i++)
		{
			curr_player_text->quest_balls_animator.push_back(new Animator());
			placer = iPoint(screen.w - 40, screen.h - 60);
			curr_player_text->quest_balls_images.push_back(new UI_Image);
			curr_player_text->quest_balls_images[i] = it->viewport_window->CreateImage(placer, { 440,812 - offset,24,24 }, false);

			curr_player_text->player_text.push_back(it->viewport_window->CreateText(iPoint(placer.x - 6, placer.y), App->font->game_font_small));
			curr_player_text->player_text[i]->SetText("0");

			screen.h = screen.h - 30;
			offset += 26;
		}
		curr_player_text->active_quest_text = (it->viewport_window->CreateText(iPoint(screen.w + 22, 55), App->font->game_font_25, 17, false, 255, 215, 0));


		curr_player_text->active_quest_text->SetText(" ");

		screen = App->view->GetViewportRect(1);
		offset = 0;

		player_text_list.push_back(curr_player_text);
	}

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
		(*it)->active_quest_text->enabled = false; 
		stop_window.push_back(false);
	}
	//CREATE ANIMATORS
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			player_text_list[i]->quest_balls_animator[j]->LoadAnimationsFromXML(quests_animations_file, "animations");
			switch (j)
			{
			case 0: 
				player_text_list[i]->quest_balls_animator[j]->SetAnimation("idle_power");
				break;
			case 1: 
				player_text_list[i]->quest_balls_animator[j]->SetAnimation("idle_speed");
				break;
			case 2: 
				player_text_list[i]->quest_balls_animator[j]->SetAnimation("idle_health");
				break;
			default:
				break;
			}
			
		}
	}
}

QuestManager::~QuestManager()
{
}

void QuestManager::Update()
{
	if (quests_enabled)
	{
		if (App->scene->main_scene->GetGameTimer()->ReadSec() - timer_read > 2 && active_quest == -1)
		{
			App->audio->PlayFx(quest_fx);
			active_quest = GetRandomValue(1, 3);
			for (int i = 0; i < 4; i++)
			{
				windows_to_move[i] = true;
				switch (active_quest)
				{
				case 1: 
					player_text_list[i]->quest_balls_animator[0]->SetAnimation("active_power");
					break;
				case 2:
					player_text_list[i]->quest_balls_animator[1]->SetAnimation("active_speed");
					break;
				case 3:
					player_text_list[i]->quest_balls_animator[2]->SetAnimation("active_health");
					break;
					default:
						break;
				}
			}
			if (active_quest == 2)
			{
				SpawnCucos(5);
			}
			change_state(active_quest, active);
			timer_read = App->scene->main_scene->GetGameTimer()->ReadSec();
			update_progress();

			for (vector<PlayerText*>::iterator it = player_text_list.begin(); it != player_text_list.end(); it++)
			{
				(*it)->active_quest_text->enabled = true;
			}
		}

		if (active_quest != -1 && App->scene->main_scene->GetGameTimer()->ReadSec() - timer_read > 120)
		{
			if (active_quest == 2)
			{
				for (int i = 0; i < cucos.size(); i++)
				{
					App->entity->DeleteEntity(cucos[i]);
				}
				cucos.clear();
			}
			reset_progress(active_quest);
			change_state(active_quest, inactive);
			for (int i = 0; i < 4; i++)
			{
				switch (active_quest)
				{
				case 1:
					player_text_list[i]->quest_balls_animator[0]->SetAnimation("idle_power");
					break;
				case 2:
					player_text_list[i]->quest_balls_animator[1]->SetAnimation("idle_speed");
					break;
				case 3:
					player_text_list[i]->quest_balls_animator[2]->SetAnimation("idle_health");
					break;
				default:
					break;
				}
			}
			active_quest = -1;
			timer_read = App->scene->main_scene->GetGameTimer()->ReadSec();
			for (int i = 0; i < 4; i++)
			{
				windows_to_move[i] = false;
			}
			for (vector<PlayerText*>::iterator it = player_text_list.begin(); it != player_text_list.end(); it++)
			{
				(*it)->active_quest_text->enabled = false;
			}
		}
	}
}

void QuestManager::CleanUp()
{
	// Clear Quests
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
	// Clear Texts
	for (vector<PlayerText*>::iterator it = player_text_list.begin(); it != player_text_list.end(); it++)
	{
		(*it)->player_text.clear();
	}
	

	// Clear Cucos
	if (!cucos.empty())
	{
		for (int i = 0; i < cucos.size(); i++)
		{
			App->entity->DeleteEntity(cucos.at(i));
		}

		cucos.clear();
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
				team_1 += "Power quest\nis active, slay\n2 enemy players.\nProgress ";
				team_1 += std::to_string(vquest[i]->task[0]->current_progress);
				team_1 += "l2";
				player_text_list[0]->active_quest_text->SetText(team_1);
				player_text_list[2]->active_quest_text->SetText(team_1);
				string team_2;
				team_2 += "Power quest\nis active, slay\n2 enemy players.\nProgress ";
				team_2 += std::to_string(vquest[i]->task[1]->current_progress);
				team_2 += "l2";
				player_text_list[1]->active_quest_text->SetText(team_2);
				player_text_list[3]->active_quest_text->SetText(team_2);
				break;
			}
			case 1:
			{
				string team_1;
				team_1 += "Speed quest\nis active.\nProgress ";
				team_1 += std::to_string(vquest[i]->task[0]->current_progress);
				team_1 += "l3";
				player_text_list[0]->active_quest_text->SetText(team_1);
				player_text_list[2]->active_quest_text->SetText(team_1);
				string team_2;
				team_2 += "Speed quest\nis active.\nProgress ";
				team_2 += std::to_string(vquest[i]->task[1]->current_progress);
				team_2 += "l3";
				player_text_list[1]->active_quest_text->SetText(team_2);
				player_text_list[3]->active_quest_text->SetText(team_2);
				break;
			}
			case 2:
			{
				string team_1;
				team_1 += "Health quest\nis active\nkill jungle\ncamps.\nProgress ";
				team_1 += std::to_string(vquest[i]->task[0]->current_progress);
				team_1 += "l3";
				player_text_list[0]->active_quest_text->SetText(team_1);
				player_text_list[2]->active_quest_text->SetText(team_1);
				string team_2;
				team_2 += "Health quest\nis active\nkill jungle\ncamps.\nProgress ";
				team_2 += std::to_string(vquest[i]->task[1]->current_progress);
				team_2 += "l3";
				player_text_list[1]->active_quest_text->SetText(team_2);
				player_text_list[3]->active_quest_text->SetText(team_2);
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
					if (vquest[i]->id == 2)
					{
						for (int i = 0; i < cucos.size(); i++)
						{
							App->entity->DeleteEntity(cucos[i]);
						}
						cucos.clear();
					}
					vquest[i]->state = inactive;
					reset_progress(vquest[i]->id);
					vquest[i]->task[j]->times_completed++;

					player_text_list[0]->active_quest_text->enabled = false;
					player_text_list[1]->active_quest_text->enabled = false;
					player_text_list[2]->active_quest_text->enabled = false;
					player_text_list[3]->active_quest_text->enabled = false;

				
					for (int i = 0; i < 4; i++)
					{
						windows_to_move[i] = false;
					}

					switch (j)
					{
					case 0:
					{
						player_text_list[0]->player_text[i-1]->SetText(std::to_string(vquest[i]->task[j]->times_completed));
						player_text_list[2]->player_text[i-1]->SetText(std::to_string(vquest[i]->task[j]->times_completed));
							switch (i)
							{
							case 0:
								player_text_list[0]->quest_balls_animator[0]->SetAnimation("power_completed");
								player_text_list[2]->quest_balls_animator[0]->SetAnimation("power_completed");
							case 1:
								player_text_list[0]->quest_balls_animator[1]->SetAnimation("speed_completed");
								player_text_list[2]->quest_balls_animator[1]->SetAnimation("speed_completed");
							case 2:
								player_text_list[0]->quest_balls_animator[2]->SetAnimation("health_completed");
								player_text_list[2]->quest_balls_animator[2]->SetAnimation("health_completed");
							default:
								break;
							}
						break;
					}
					case 1:
					{
						player_text_list[1]->player_text[i-1]->SetText(std::to_string(vquest[i]->task[j]->times_completed));
						player_text_list[3]->player_text[i-1]->SetText(std::to_string(vquest[i]->task[j]->times_completed));
						switch (i)
						{
						case 0:
							player_text_list[1]->quest_balls_animator[0]->SetAnimation("power_completed");
							player_text_list[3]->quest_balls_animator[0]->SetAnimation("power_completed");
						case 1:
							player_text_list[1]->quest_balls_animator[1]->SetAnimation("speed_completed");
							player_text_list[3]->quest_balls_animator[1]->SetAnimation("speed_completed");
						case 2:
							player_text_list[1]->quest_balls_animator[2]->SetAnimation("health_completed");
							player_text_list[3]->quest_balls_animator[2]->SetAnimation("health_completed");
						default:
							break;
						}
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
		iPoint pos;

		pos = iPoint(GetRandomValue(0, App->map->data.width), GetRandomValue(0, App->map->data.height));
		while (!App->pathfinding->IsWalkable(pos))
		{
			pos = iPoint(pos.x = GetRandomValue(0, App->map->data.width), pos.y = GetRandomValue(0, App->map->data.height));
		}

	//iPoint poss = App->map->MapToWorld(pos.x, pos.y);
		cucos.push_back((Cuco*)App->entity->CreateEntity(cuco,App->map->MapToWorld(pos.x,pos.y)));
	}
}

void QuestManager::SwitchWindowState(int player)
{
	if(windows_to_move[player -1] == false)
	windows_to_move[player - 1] = true;
	else 
		windows_to_move[player - 1] = false;
}
void QuestManager::UpdateWindows()
{
	int speed = WINDOW_SPEED*App->GetDT();
	for (int i = 0; i < windows_to_move.size(); i++)
	{
		if (windows_to_move[i] == true && stop_window[i] == false)
		{
			int new_x = (App->view->GetViewportRect(1).w - 177) - player_quest_windows[i]->GetPos().x;
			if (player_quest_windows[i]->GetPos().x - speed < App->view->GetViewportRect(1).w - 177)
			{
				player_quest_windows[i]->SetPos(p2Point<int>(player_quest_windows[i]->GetPos().x - new_x, player_quest_windows[i]->GetPos().y));
				player_text_list[i]->active_quest_text->SetPos(p2Point<int>(player_text_list[i]->active_quest_text->GetPos().x - new_x, player_text_list[i]->active_quest_text->GetPos().y));
				player_remap_button[i]->SetPos(p2Point<int>(player_remap_button[i]->GetPos().x - new_x, player_remap_button[i]->GetPos().y));
				stop_window[i] = true;
			}


			else if (player_quest_windows[i]->GetPos().x > App->view->GetViewportRect(1).w - 177)
			{
				player_quest_windows[i]->SetPos(p2Point<int>(player_quest_windows[i]->GetPos().x - speed, player_quest_windows[i]->GetPos().y));
				player_text_list[i]->active_quest_text->SetPos(p2Point<int>(player_text_list[i]->active_quest_text->GetPos().x - speed, player_text_list[i]->active_quest_text->GetPos().y));
				player_remap_button[i]->SetPos(p2Point<int>(player_remap_button[i]->GetPos().x - speed, player_remap_button[i]->GetPos().y));
			}
		}
		if (windows_to_move[i] == false)
		{
			if (player_quest_windows[i]->GetPos().x < App->view->GetViewportRect(1).w)
			{
				player_quest_windows[i]->SetPos(p2Point<int>(player_quest_windows[i]->GetPos().x + speed, player_quest_windows[i]->GetPos().y));
				player_text_list[i]->active_quest_text->SetPos(p2Point<int>(player_text_list[i]->active_quest_text->GetPos().x + speed, player_text_list[i]->active_quest_text->GetPos().y));
				player_remap_button[i]->SetPos(p2Point<int>(player_remap_button[i]->GetPos().x + speed, player_remap_button[i]->GetPos().y));
				stop_window[i] = false;
			}
		}
	}
}

void QuestManager::UpdateQuestAnimations(float dt)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			player_text_list[i]->quest_balls_images[j]->image = player_text_list[i]->quest_balls_animator[j]->GetCurrentAnimation()->GetAnimationFrame(dt);
		}
	}
}
