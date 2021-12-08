#include "mu_headers.h"

// Initialises the framerate manager
void init_framerate(MU_FPS_Manager *fps_manager)
{
	fps_manager->framecount = 0;
	fps_manager->rate = FPS_DEFAULT;
	fps_manager->rateticks = (1000.0 / (float) FPS_DEFAULT);
	fps_manager->lastticks = SDL_GetTicks();
}

// Sets the framerate to rate based on the lower and upper limits of the framerate
int set_framerate(MU_FPS_Manager *fps_manager, int rate)
{
	if((rate >= FPS_LOWER_LIMIT) && (rate <= FPS_UPPER_LIMIT))
	{
		fps_manager->framecount = 0;
		fps_manager->rate = rate;
		fps_manager->rateticks = (1000.0 / (float) rate);
		return 0;
	}
	else
	{
		return -1;
	}
}

int get_framerate(MU_FPS_Manager *fps_manager)
{
	if(fps_manager == NULL)
	{
		return -1;
	}
	else
	{
		return (fps_manager->rate);
	}
}

// Delay execution to maintain a constant framerate. Calculate FPS
void framerate_delay(MU_FPS_Manager *fps_manager)
{
	u32 current_ticks;
	u32 target_ticks;
	u32 the_delay;

	fps_manager->framecount++;

	current_ticks = SDL_GetTicks();
	target_ticks = fps_manager->lastticks + (u32) ((float) fps_manager->framecount * fps_manager->rateticks);

	if(current_ticks <= target_ticks)
	{
		the_delay = target_ticks - current_ticks;
		SDL_Delay(the_delay);
	}
	else
	{
		fps_manager->framecount = 0;
		fps_manager->lastticks = SDL_GetTicks();
	}
}