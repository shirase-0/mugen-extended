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
	// TODO: Add tests for sdl_framerate

	// ===Graphics Manager===
	MU_Graphics_Manager *graphics_manager = mu_init_graphics_manager();
	SDL_Delay(5000);
}