#include "mu_headers.h"

void unit_test()
{
	// Logging
	init_log_file();
	debug_print("debug_print working as intended");
	mu_log_message("mu_log_message working as intended");

	// Memory Manager
	MU_Allocator *test_allocator = create_mu_alloc("Test Allocator");
	show_mem_usage(test_allocator);
	assert(test_allocator != NULL);
	void *to_remove = mu_alloc(test_allocator, 1000);
	assert(to_remove != NULL);
	assert(test_allocator->alloc_size == 1000);
	show_mem_usage(test_allocator);
	reset_allocator(test_allocator);
	show_mem_usage(test_allocator);
}