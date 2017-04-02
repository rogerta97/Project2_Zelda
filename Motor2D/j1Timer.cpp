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
		return SDL_GetTicks() - started_at;
	else
		return 0;
}

// ---------------------------------------------
float j1Timer::ReadSec() const
{
	if (active)
		return float(SDL_GetTicks() - started_at) / 1000.0f;
	else
		return 0;
}

void j1Timer::SubstractTimeFromStart(float sec)
{
	started_at -= (sec * 1000);
}

void j1Timer::Stop()
{
	active = false;
}

bool j1Timer::IsActive()
{
	return active;
}
