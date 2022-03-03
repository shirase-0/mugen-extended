#include "mu_headers.h"

Game *init_game()
{
	Game *game = (Game*) malloc(sizeof(Game));
	game->error = false;
	game->in_game = true;
	game->start_time = 0;
	game->end_time = 0;
	game->game_type = GFIGHTGAME;

	init_log_file();
	debug_print("Welcome to Mugen Extended Version %s", VER);

	game->mem_manager = init_mem_manager();
	game->graphics_manager = mu_init_graphics_manager();

	if(game->graphics_manager == NULL)
	{
		game->error = true;
	}

	// Init timer
	game->timer = (MU_Timer*) malloc(sizeof(MU_Timer));
	mu_reset_timer(game->timer);

	// Init the main engine
	//game->engine = mu_engine_init()

	get_total_mem_usage(game->mem_manager);

	return game;
}

void run_game(Game *game)
{
	while(game->in_game)
	{
		mu_update_timer(game->timer);
		mu_clear_screen(game->graphics_manager);

		switch(game->game_type)
		{
			case GFIGHTGAME:
				//mu_run_engine(game->engine);
				break;
			case GMENU:
				debug_print("You've reached the menu, which has not been implemented yet");
				break;
		}

		mu_draw_text(game->graphics_manager, 0, 100, SDL_GetKeyName(game->event.key.keysym.sym));

		//mu_draw(game->graphics_manager, texture);
	}
}