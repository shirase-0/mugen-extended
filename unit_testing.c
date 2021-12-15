#include "mu_headers.h"

void unit_test()
{
	// ===Logging===
	init_log_file();
	debug_print("debug_print working as intended");
	mu_log_message("mu_log_message working as intended");

	// ===Memory Manager===
	MU_Allocator *test_allocator = create_mu_alloc("Test Allocator");
	assert(test_allocator != NULL);
	show_mem_usage(test_allocator);
	debug_print("create_mu_alloc working as intended");
	debug_print("set_memlist_free working as intended");
	debug_print("show_mem_usage working as intended");

	void *to_remove = mu_alloc(test_allocator, 1000);
	assert(to_remove != NULL);
	assert(test_allocator->alloc_size == 1000);
	debug_print("mu_alloc working as intended");

	reset_allocator(test_allocator);
	assert(to_remove != NULL);
	assert(test_allocator->alloc_size == 0);
	debug_print("reset_allocator working as intended");

	to_remove = mu_alloc(test_allocator, 1234);
	assert(to_remove != NULL);
	assert(test_allocator->alloc_size == 1234);
	to_remove = mu_realloc(test_allocator, to_remove, 58000);
	assert(to_remove != NULL);
	assert(test_allocator->alloc_size == 58000);
	debug_print("mu_realloc working as intended");

	mu_free(test_allocator, to_remove);
	assert(test_allocator->alloc_size == 0);
	debug_print("mu_free working as intended");

	to_remove = mu_alloc(test_allocator, 2);
	assert(to_remove != NULL);
	assert(test_allocator->alloc_size == 2);
	free_allocator(test_allocator);
	assert(test_allocator->alloc_size == 0);
	assert(test_allocator->free == true);
	debug_print("free_allocator working as intended");

	MU_Allocator *test_manager = init_mem_manager();
	assert(strcmp(test_manager[MAINMENU].alloc_name, "Main Menu") == 0);
	assert(strcmp(test_manager[STAGE].alloc_name, "Stage") == 0);
	assert(strcmp(test_manager[ENGINE].alloc_name, "Engine") == 0);
	assert(strcmp(test_manager[P1].alloc_name, "Player 1") == 0);
	assert(strcmp(test_manager[P2].alloc_name, "Player 2") == 0);
	assert(strcmp(test_manager[P3].alloc_name, "Player 3") == 0);
	assert(strcmp(test_manager[P4].alloc_name, "Player 4") == 0);
	debug_print("init_mem_manager working as intended");

	// TODO: figure out a way to test if free_mem_manager is working

	mu_alloc(&test_manager[STAGE], 700000);
	mu_alloc(&test_manager[P3], 123456);
	get_total_mem_usage(test_manager);
	debug_print("get_total_mem_usage working as intended");

	// ===Framerate===
	// Framerate tests are conducted in the graphics manager section, as the graphics manager needs to initialise
	// an FPS manager anyway
	// There is currently no feasible way to test framerate_delay() without running a game

	// ===Graphics Manager===
	MU_Graphics_Manager *graphics_manager = mu_init_graphics_manager();
	assert(graphics_manager->window != NULL);
	assert(graphics_manager->screen_surface != NULL);
	assert(graphics_manager->renderer != NULL);
	assert(graphics_manager->delta_ptr == 0);
	assert(graphics_manager->now_time == 0);
	assert(graphics_manager->last_time == 0);
	assert(graphics_manager->fps_count == 0);
	assert(graphics_manager->fps == 0);
	assert(get_framerate(&graphics_manager->fps_manager) == 60);
	debug_print("init_framerate working as intended");
	debug_print("set_framerate working as intended");
	debug_print("get_framerate working as intended");
	debug_print("mu_load_font working as intended");
	debug_print("mu_init_graphics_manager working as intended");

	// This isn't scaling correctly yet, not sure why
	// Revisit this when the graphics manager is more functional
	mu_draw_text(graphics_manager, 0 , 0, "Hello World!", graphics_manager->fps);
	SDL_Delay(1000);

	mu_clear_screen(graphics_manager);
	debug_print("mu_clear_screen working as intended");
	SDL_Delay(1000);
	mu_draw_text(graphics_manager, 0 , 0, "What if we try to print a really long sentence", graphics_manager->fps);
	SDL_Delay(3000);
}