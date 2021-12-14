#include "mu_headers.h"

// Initialise the MU_Graphics_Manager
MU_Graphics_Manager *mu_init_graphics_manager()
{
	// SDL Initialisation
	debug_print("Graphics Manager: Initialising");
	if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0)
	{
		debug_print("Error initialising SDL: %s", SDL_GetError());
		return NULL;
	}
	debug_print("SDL Initialisation successful!");

	// Create and render a window
	MU_Graphics_Manager *graphics_manager = (MU_Graphics_Manager*) malloc(sizeof(MU_Graphics_Manager));
	graphics_manager->delta_ptr = 0;
	graphics_manager->now_time = 0;
	graphics_manager->last_time = 0;
	graphics_manager->fps_count = 0;
	graphics_manager->fps = 0;
	//graphics_manager->window = NULL;
	//graphics_manager->screen_surface = NULL;
	//graphics_manager->renderer = NULL;

	graphics_manager->window = SDL_CreateWindow(
		"Mugen Extended",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		XMAX,
		YMAX,
		SDL_WINDOW_FULLSCREEN_DESKTOP
	);
	if(graphics_manager->window == NULL)
	{
		debug_print("Could not create window: %s\n", SDL_GetError());
	}

	graphics_manager->screen_surface = SDL_GetWindowSurface(graphics_manager->window);
	graphics_manager->renderer = SDL_CreateRenderer(graphics_manager->window, -1, 0);
	if(graphics_manager->renderer == NULL)
	{
		debug_print("Could not create renderer: %s\n", SDL_GetError());
	}

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear"); // Make the scaled rendering look smoother.
	SDL_RenderSetLogicalSize(graphics_manager->renderer, XMAX, YMAX);

	// TODO: use renderer here instead of SDL_UpdateWindowSurface
	SDL_FillRect(graphics_manager->screen_surface, NULL, SDL_MapRGB(graphics_manager->screen_surface->format, 0, 0, 0));
	SDL_UpdateWindowSurface(graphics_manager->window);

	// Set the framerate to 60
	init_framerate(&graphics_manager->fps_manager);
	set_framerate(&graphics_manager->fps_manager, 60);

	// Load raster font
	//mu_load_font(graphics_manager);

	return graphics_manager;
}