#include "mu_headers.h"

void mu_reset_timer(MU_Timer *timer)
{
	timer->n_game_time = 0;
	timer->b_pause = false;
}

void mu_update_timer(MU_Timer *timer)
{
	if(!timer->b_pause)
	{
		timer->n_game_time++;
	}
}

u32 get_game_time(MU_Timer *timer)
{
	return timer->n_game_time;
}

void mu_pause(MU_Timer *timer)
{
	timer->b_pause = true;
}

void mu_resume(MU_Timer *timer)
{
	timer->b_pause = false;
}