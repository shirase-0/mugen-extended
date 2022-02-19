#include "mu_headers.h"

void mu_reset_timer(MU_Timer *timer)
{
	timer->game_time = 0;
	timer->is_paused = false;
}

void mu_update_timer(MU_Timer *timer)
{
	if(!timer->is_paused)
	{
		timer->game_time++;
	}
}

uint32_t get_game_time(MU_Timer *timer)
{
	return timer->game_time;
}

void mu_pause(MU_Timer *timer)
{
	timer->is_paused = true;
}

void mu_resume(MU_Timer *timer)
{
	timer->is_paused = false;
}