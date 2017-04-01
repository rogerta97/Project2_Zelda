#include "EventThrower.h"
#include "j1App.h"

EventThrower::EventThrower()
{
}

EventThrower::~EventThrower()
{
	for (std::list<Event*>::iterator it = events.begin(); it != events.end();)
	{
		RELEASE(*it);
		it = events.erase(it);
	}
	
	events.clear();
}

void EventThrower::AddEvent(Event * new_event)
{
	new_event->event_id = events.size();

	events.push_back(new_event);

	ThrowEvent(new_event->type, new_event->event_id);
}

Event * EventThrower::GetEvent(int id)
{
	int i = 0;
	for (std::list<Event*>::iterator it = events.begin(); it != events.end(); ++it)
	{
		if (i == (*it)->event_id)
			return *it;
		else
			++i;
	}
}

void EventThrower::ThrowEvent(int type, int id)
{
	App->ExpandEvent(type, this, id);
}
