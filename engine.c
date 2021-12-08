#include "mu_headers.h"

MU_Engine *mu_engine_init(MU_Allocator *mem_manager, MU_Video_System *video_system, MU_Timer *timer)
{
	MU_Engine *engine = (MU_Engine*) malloc(sizeof(MU_Engine));

	engine->mem_manager = mem_manager;
	engine->video_system = video_system;
	engine->timer = timer;
	engine->p1 = player_init();
	engine->p2 = player_init();
	engine->p3 = player_init();
	engine->p4 = player_init();

	mu_set_player_pointers(engine->p1, video_system, get_allocator(mem_manager, P1), timer);
	mu_set_player_pointers(engine->p2, video_system, get_allocator(mem_manager, P2), timer);
	mu_set_player_pointers(engine->p3, video_system, get_allocator(mem_manager, P3), timer);
	mu_set_player_pointers(engine->p4, video_system, get_allocator(mem_manager, P4), timer);

	engine->vm_p1 = vm_init();
	engine->vm_p2 = vm_init();

	vm_set_players(engine->vm_p1, engine->p1, engine->p2);
	vm_set_players(engine->vm_p2, engine->p2, engine->p1);

	engine->p1->vm = engine->vm_p1;
	engine->p2->vm = engine->vm_p2;

	// The following values should be read out of the stage def
	engine->p1->n_ground = 220;
	engine->p2->n_ground = 220;

	engine->p1->x = 70;
	engine->p1->y = 220;
	engine->p2->x = 570; // 640 - 70
	engine->p2->y = 220;

	// strings are empty because the player being loaded is hardcoded into this function for now
	load_player(engine->p1, "");
	load_player(engine->p2, "");

	engine->p2->b_right_faced = false;
	engine->p1->b_debug_info = true;
	//    player1.VelSetY(-2.4f);
	
	return engine;
}

void mu_run_engine(MU_Engine *engine)
{
	/*
	================================================================================
	Upates all the engine members and draw them to the screen
	Handels also AI and player movement
	================================================================================
	*/

	update_player(engine->p1);
	update_player(engine->p2);

	draw_player(engine->p1);
	draw_player(engine->p2);
}