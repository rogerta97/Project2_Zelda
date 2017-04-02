#ifndef _EVENTTHROWER_H_
#define _EVENTTHROWER_H_

#include <list>

class Entity;

enum event_type
{
	e_t_null, e_t_death, e_t_end_game,
};

class Event
{
public:
	Event(event_type type, Entity* entity = nullptr, int data_id = -1);
	Event();

	~Event();

	int GetID();

	//This is called automatically by the event thrower when creating the event
	void SetID(int id);
public:
	event_type	type = e_t_null;
	
	union 
	{
		Entity* entity = nullptr;
		int		id;
	} event_data;

private:
	int			event_id = -1;

};

class EventThrower
{
public:
	EventThrower();
	~EventThrower();

	void AddEvent(Event* new_event);

	Event* GetEvent(int id);

private:

	void ThrowEvent(int type, int id);

private:

	std::list<Event*> events;

};


#endif // !_EVENTTHROWER_H_
