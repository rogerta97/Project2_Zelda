#include "Quest_Manager.h"
#include "j1App.h"
QuestManager::QuestManager()
{
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
			quest->state = (unavailable);
			break; }
		case 1: {
			quest->state = (active);
			break; }
		case 2: {
			quest->state = (completed);
			break; }
		default:
			break;
		}

		quest->task.current_progress = loop_tool.child("task").attribute("current").as_int();
		quest->task.requirement = loop_tool.child("task").attribute("complete").as_int();

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

void QuestManager::add_progress(int id)
{
	for (int i = 0; i < vquest.size(); i++)
	{
		if (vquest[i]->id == id)
		{
			vquest[i]->task.current_progress++;
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
			if (vquest[i]->task.current_progress == vquest[i]->task.requirement)
				vquest[i]->state = completed;
		}
	}
}
