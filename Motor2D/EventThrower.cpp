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
	new_event->SetID(events.size());

	events.push_back(new_event);

	ThrowEvent(new_event->type, new_event->GetID());
}

Event * EventThrower::GetEvent(int id)
{
	for (std::list<Event*>::iterator it = events.begin(); it != events.end(); ++it)
	{
		if (id == (*it)->GetID())
			return *it;
	}
}

void EventThrower::ThrowEvent(int type, int id)
{
	App->ExpandEvent(type, this, id);
}

Event::Event(event_type type, Entity * _entity, int data_id) : type(type)
{
	if (_entity != nullptr)
		event_data.entity = _entity;
	else
		event_data.id = data_id;
}

Event::Event()
{
}

Event::~Event()
{
}

int Event::GetID()
{
	return event_id;
}

void Event::SetID(int id)
{
	event_id = id;
}
