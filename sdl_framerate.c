#include "mu_headers.h"

// Initialises the framerate manager
void init_framerate(MU_FPS_Manager *fps_manager)
{
	fps_manager->framecount = 0;
	fps_manager->rate = FPS_DEFAULT;
	fps_manager->rateticks = (1000.0 / (double) FPS_DEFAULT);
	fps_manager->lastticks = SDL_GetTicks();
}

// Sets the framerate to rate based on the lower and upper limits of the framerate
int set_framerate(MU_FPS_Manager *fps_manager, int rate)
{
	if((rate >= FPS_LOWER_LIMIT) && (rate <= FPS_UPPER_LIMIT))
	{
		fps_manager->framecount = 0;
		fps_manager->rate = rate;
		fps_manager->rateticks = (1000.0 / (double) rate);
		return 0;
	}
	else
	{
		return -1;
	}
}

int get_framerate(MU_FPS_Manager *fps_manager)
{
	if(fps_manager != NULL)
	{
		return (fps_manager->rate);
	}
	else
	{
		return -1;
	}
}

// Delay execution to maintain a constant framerate. Calculate FPS
void framerate_delay(MU_FPS_Manager *fps_manager)
{
	uint32_t current_ticks;
	uint32_t target_ticks;
	uint32_t delay;

	fps_manager->framecount++;

	current_ticks = SDL_GetTicks();
	target_ticks = fps_manager->lastticks + (uint32_t) ((double) fps_manager->framecount * fps_manager->rateticks);

	if(current_ticks <= target_ticks)
	{
		delay = target_ticks - current_ticks;
		SDL_Delay(delay);
	}
	else
	{
		fps_manager->framecount = 0;
		fps_manager->lastticks = SDL_GetTicks();
	}
}