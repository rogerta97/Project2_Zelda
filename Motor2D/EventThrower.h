#ifndef _EVENTTHROWER_H_
#define _EVENTTHROWER_H_

#include <list>

class Entity;

enum event_type
{
	e_t_null, e_t_death, e_t_end_game,
};

struct Event
{
	event_type	type;
	int			event_id = -1;
	
	union 
	{
		Entity* entity;
		int		id;
	} event_data;
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
