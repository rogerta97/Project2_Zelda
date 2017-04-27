// ----------------------------------------------------
// j1Timer.cpp
// Fast timer with milisecons precision
// ----------------------------------------------------

#include "j1Timer.h"
#include "SDL\include\SDL_timer.h"

// ---------------------------------------------
j1Timer::j1Timer()
{
	Start();
}

j1Timer::~j1Timer()
{
}

// ---------------------------------------------
void j1Timer::Start()
{
	started_at = SDL_GetTicks();
	active = true;
}

// ---------------------------------------------
uint32 j1Timer::Read() const
{
	if (active)
	{
		if (paused)
			return (paused_at - started_at);
		else
			return (SDL_GetTicks() - started_at);
	}
	else
		return 0;
}

// ---------------------------------------------
float j1Timer::ReadSec() const
{
	return float(Read()) / 1000.0f;
}

void j1Timer::SubstractTimeFromStart(float sec)
{
	started_at -= (sec * 1000);
}

void j1Timer::Stop()
{
	active = false;
}

void j1Timer::PauseOn()
{
	if (paused == false)
	{
		paused_at = SDL_GetTicks();
		paused = true;
	}
}

void j1Timer::PauseOff()
{	
	if (paused == true)
	{
		started_at += (SDL_GetTicks() - paused_at);

		paused_at = 0;
		paused = false;
	}
}

bool j1Timer::IsActive()
{
	return active;
}
