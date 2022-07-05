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
	game->engine = mu_engine_init(game->mem_manager, game->graphics_manager, game->timer);

	get_total_mem_usage(game->mem_manager);

	return game;
}

void run_game(Game *game)
{
	MU_Graphics_Manager *graphics = game->graphics_manager;
	SDL_Texture *texture = SDL_CreateTexture(graphics->renderer, 
											 graphics->colour_format->format, 
											 SDL_TEXTUREACCESS_STREAMING, 
											 XMAX, YMAX);
	uint32_t current_ticks;

	int i = 0;
	int next_anim = 0;
	int prev_anim = 0;
	while(game->in_game)
	{
		mu_update_timer(game->timer);

		switch(game->game_type)
		{
			case GFIGHTGAME:
				mu_run_engine(game->engine);
				break;
			case GMENU:
				debug_print("You've reached the menu, which has not been implemented yet");
				break;
		}

		mu_draw_text(graphics, 0, 100, SDL_GetKeyName(game->event.key.keysym.sym));

		mu_draw(graphics, texture);

		current_ticks = SDL_GetTicks();
		// if(current_ticks >= 10000)
		// {
		// 	game->in_game = false;
		// }

		// Test code which cycles through every animation on a character
		i = current_ticks / 5000;
		if(prev_anim != i)
		{
			next_anim = game->engine->p1->air_manager->action_list[i].action_num;
			prepare_anim(game->engine->p1->sff_manager, next_anim);
			prepare_anim(game->engine->p2->sff_manager, next_anim);
			prev_anim = i;
		}
	}
}