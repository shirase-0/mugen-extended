#include "mu_headers.h"

// TODO: merge the SDL Manager and Video System

// Constructor for the SDL manager
MU_Video_System *mu_sdl_manager_init()
{
	mu_log_message("SDL Manager: Initalising");

	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0)
    {
    	mu_log_message("SDL Manager: Error initialising SDL: %s", SDL_GetError());
    	return NULL;
    }
	mu_log_message("SDL Manager: Init successful");

	MU_Video_System *video_system = mu_init_video_system();

	if(video_system == NULL)
	{
		mu_log_message("SDL Manager: Video System init failed");
		return NULL;
	}
	mu_log_message("SDL Manager: Video System init successful");
	SDL_SetWindowIcon(video_system->window, SDL_LoadBMP("icon.bmp"));

	return video_system;
}