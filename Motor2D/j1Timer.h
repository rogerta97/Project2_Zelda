#ifndef __j1TIMER_H__
#define __j1TIMER_H__

#include "p2Defs.h"

class j1Timer
{
public:

	// Constructors
	j1Timer();
	~j1Timer();

	void Start();
	uint32 Read() const;
	float ReadSec() const;
	void SubstractTimeFromStart(float sec);
	void Stop();

	bool IsActive();


private:
	uint32	started_at;
	bool	active = false;
};

#endif //__j1TIMER_H__