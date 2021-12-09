#include "mu_headers.h"

Game *init_game()
{
	Game *game = (Game*) malloc(sizeof(Game));
	game->b_error = false;
	game->b_game = true;
	game->n_start_time = 0;
	game->n_end_time = 0;
	game->n_game_type = GFIGHTGAME;

	init_log_file();
	mu_log_message("Welcome to Mugen Extended Version %s", VER);

	game->mem_manager = init_mem_manager();
	game->sdl_manager = mu_sdl_manager_init();

	if(game->sdl_manager == NULL)
	{
		game->b_error = true;
	}

	// Init timer
	game->timer = (MU_Timer*) malloc(sizeof(MU_Timer));
	mu_reset_timer(game->timer);

	// Init the main engine
	game->engine = mu_engine_init(game->mem_manager, game->sdl_manager, game->timer);

	get_total_mem_usage(game->mem_manager);

	return game;
}

void check_sdl(Game *game)
{
	SDL_PollEvent(&game->event);

	if(game->event.type == SDL_MOUSEMOTION || 
	   game->event.type == SDL_MOUSEBUTTONDOWN || 
	   game->event.type == SDL_MOUSEBUTTONUP ||
	   game->event.type == SDL_MOUSEWHEEL)
	{
		return; 
	}

	if(game->event.type == SDL_QUIT || game->event.key.keysym.sym == SDLK_ESCAPE)
	{
		game->b_game = false;
	}
	
	// If mu_draw doesn't render the screen properly, uncomment this line?
	//SDL_UpdateWindowSurface(game->sdl_manager->window);
}

void run_game(Game *game)
{
	char *current_key = "";
	SDL_Texture *sdl_texture = SDL_CreateTexture(game->sdl_manager->renderer, 
												 game->sdl_manager->screen_surface->format->format, 
												 SDL_TEXTUREACCESS_STREAMING, 
												 XMAX, YMAX);

	while(game->b_game)
	{
		// Check SDL system and get input
		check_sdl(game);

		// Update timer
		mu_update_timer(game->timer);

		// Clear the screen
		mu_clear_screen(game->sdl_manager);

		// Check the current keyboard state
		// TODO: implement for player 2 (current button mapping is hardcoded for the p1 side)

		// Not sure if this Keyboard_Data struct can be moved out of the players and into the engine
		// I'm leaving it like this for now, because I think using separate controllers (xbox, PS, etc)
		// might make each player require their own keyboard
		mu_process_input(game->engine->p1->kb, game->event);
		mu_process_input(game->engine->p2->kb, game->event);

		// Switch to the current subsystem
		switch(game->n_game_type)
		{
			case GFIGHTGAME:
				mu_run_engine(game->engine);
				break;
			case GMENU:
				mu_log_message("You've reached the menu, which has not been implemented yet");
				break;
		}

		// Now draw the content to the screen
		// Display the key currently being pressed on the screen
		mu_draw_text(game->sdl_manager, 0, 100, SDL_GetKeyName(game->event.key.keysym.sym));

		// The following is merely demonstration code to show that animations and keyboard inputs are working
		// This test code allows player 1 to switch animations between moving forward (D), moving backwards (A),
		// and standing idle (P)
		// This will be removed once the finite state machine in state_parser.c is fully operational, and the actions
		// associated with these animations actually work as intended
		if((strcmp(SDL_GetKeyName(game->event.key.keysym.sym), "D") == 0) && !(strcmp(SDL_GetKeyName(game->event.key.keysym.sym), current_key) == 0))
		{
			prepare_anim(game->engine->p1->sff_manager, 20);
			current_key = "D";
		}
		else if((strcmp(SDL_GetKeyName(game->event.key.keysym.sym), "A") == 0) && !(strcmp(SDL_GetKeyName(game->event.key.keysym.sym), current_key) == 0))
		{
			prepare_anim(game->engine->p1->sff_manager, 21);
			current_key = "A";
		}
		else if((strcmp(SDL_GetKeyName(game->event.key.keysym.sym), "P") == 0) && !(strcmp(SDL_GetKeyName(game->event.key.keysym.sym), current_key) == 0))
		{
			prepare_anim(game->engine->p1->sff_manager, 0);
			current_key = "P";
		}

		//Limits the game speed to 60Hz (and also renders the screen?)
		// TODO: check that SDL2 rendering pipeline is working as intended
		mu_draw(game->sdl_manager, sdl_texture);
	}
}

// Incomplete, TODO
void quit_game()
{
	Mix_CloseAudio();
	SDL_Quit();
}