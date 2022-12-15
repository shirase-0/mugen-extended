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

void check_input(Game *game)
{
	while(SDL_PollEvent(&game->event))
	{
		if(game->event.type == SDL_MOUSEMOTION || 
		   game->event.type == SDL_MOUSEBUTTONDOWN || 
		   game->event.type == SDL_MOUSEBUTTONUP ||
		   game->event.type == SDL_MOUSEWHEEL)
		{
			continue;
		}

		if(game->event.type == SDL_QUIT || game->event.key.keysym.sym == SDLK_ESCAPE)
		{
			game->in_game = false;
		}
	}
}

void run_game(Game *game)
{
	MU_Graphics_Manager *graphics = game->graphics_manager;
	SDL_Texture *texture = SDL_CreateTexture(graphics->renderer, 
											 graphics->colour_format->format, 
											 SDL_TEXTUREACCESS_STREAMING, 
											 XMAX, YMAX);
	//==================debug code only, to be removed==================
	uint32_t current_ticks = 0;

	int i = 0;
	int next_anim = 0;
	int prev_anim = 0;

	uint32_t current_key = SDL_SCANCODE_P;

	//==================================================================
	while(game->in_game)
	{
		// Check SDL Event for input
		check_input(game);

		mu_update_timer(game->timer);

		mu_process_input(game->engine->p1->kb, game->event);
		mu_process_input(game->engine->p2->kb, game->event);

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

		//=================debug code only, to be removed==============
		if(game->event.key.keysym.scancode == SDL_SCANCODE_P && current_key != SDL_SCANCODE_P)
		{
			prepare_anim(game->engine->p1->sff_manager, 0);
			current_key = SDL_SCANCODE_P;
		}
		else if(game->event.key.keysym.scancode == SDL_SCANCODE_D && current_key != SDL_SCANCODE_D)
		{
			prepare_anim(game->engine->p1->sff_manager, 20);
			current_key = SDL_SCANCODE_D;
		}
		else if(game->event.key.keysym.scancode == SDL_SCANCODE_A && current_key != SDL_SCANCODE_A)
		{
			prepare_anim(game->engine->p1->sff_manager, 21);
			current_key = SDL_SCANCODE_A;
		}
		if(game->event.key.keysym.scancode == SDL_SCANCODE_KP_0 && current_key != SDL_SCANCODE_KP_0)
		{
			prepare_anim(game->engine->p2->sff_manager, 0);
			current_key = SDL_SCANCODE_KP_0;
		}
		else if(game->event.key.keysym.scancode == SDL_SCANCODE_LEFT && current_key != SDL_SCANCODE_LEFT)
		{
			prepare_anim(game->engine->p2->sff_manager, 20);
			current_key = SDL_SCANCODE_LEFT;
		}
		else if(game->event.key.keysym.scancode == SDL_SCANCODE_RIGHT && current_key != SDL_SCANCODE_RIGHT)
		{
			prepare_anim(game->engine->p2->sff_manager, 21);
			current_key = SDL_SCANCODE_RIGHT;
		}
		//==============================================================

		mu_draw(graphics, texture);

		//==================== debug code only, to be removed=================
		// Test code which cycles through every animation on a character
		current_ticks = SDL_GetTicks();
		i = current_ticks / 5000;
		if(prev_anim != i)
		{
			next_anim = game->engine->p1->air_manager->action_list[i].action_num;
			prepare_anim(game->engine->p1->sff_manager, next_anim);
			prepare_anim(game->engine->p2->sff_manager, next_anim);
			prev_anim = i;
		}
		//===================================================================
	}
}