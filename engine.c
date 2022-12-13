#include "mu_headers.h"

MU_Engine *mu_engine_init(MU_Allocator *mem_manager, MU_Graphics_Manager *graphics_manager, MU_Timer *timer)
{
	MU_Engine *engine = (MU_Engine*) malloc(sizeof(MU_Engine));

	engine->mem_manager = mem_manager;
	engine->graphics_manager = graphics_manager;
	engine->timer = timer;
	engine->p1 = player_init();
	engine->p2 = player_init();
	engine->p3 = NULL; // Only init these players if necessary, no need to waste the memory otherwise
	engine->p4 = NULL;

	mu_set_player_pointers(engine->p1, graphics_manager, &mem_manager[P1], timer);
	mu_set_player_pointers(engine->p2, graphics_manager, &mem_manager[P2], timer);

	// engine->vm_p1 = vm_init();
	// engine->vm_p2 = vm_init();
	// vm_set_players(engine->vm_p1, engine->p1, engine->p2);
	// vm_set_players(engine->vm_p2, engine->p2, engine->p1);
	// engine->p1->vm = engine->vm_p1;
	// engine->p2->vm = engine->vm_p2;

	// TODO: the following values should be read out of the stage def
	// Fix this when sprites have proper scaling
	engine->p1->ground = 350;
	engine->p2->ground = 350;

	// TODO: calculate these values based on XMAX and YMAX (graphics_manager.h)
	engine->p1->x = 70;
	engine->p1->y = engine->p1->ground;
	engine->p2->x = 570; // 640 - 70
	engine->p2->y = engine->p2->ground;

	char *p1_name = "kfm";
	char *p2_name = "kfm";
	load_player(engine->p1, p1_name);
	load_player(engine->p2, p2_name);

	engine->p2->right_faced = false;
	engine->p1->debug_info = true;

	return engine;
}

void mu_run_engine(MU_Engine *engine)
{
	/*
	================================================================================
	Upates all players and draws them to the screen
	Currently the functions this function calls are very barebones, and do not fully
	handle AI and player movement.
	================================================================================
	*/

	update_player(engine->p1);
	update_player(engine->p2);

	draw_player(engine->p1);
	draw_player(engine->p2);
}