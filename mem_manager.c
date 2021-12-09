#include "mu_headers.h"

// MU_Allocator Constructor
MU_Allocator *create_mu_alloc(char *name)
{
	mu_log_message("Memory Manager: Initialising %s memory allocator", name);
	MU_Allocator *allocator = (MU_Allocator*) malloc(sizeof(MU_Allocator));
	memset(allocator->str_alloc_name, 0, sizeof(allocator->str_alloc_name));
	strcpy(allocator->str_alloc_name, name); // Set name
	allocator->n_alloc_size = 0;
	allocator->n_alloc_number = 0;
	allocator->n_mem_list_size = 100;
	allocator->b_free = true;

	// malloc & zero out the lp_mem_list
	allocator->lp_mem_list = (MEMLIST*) malloc(sizeof(MEMLIST) * allocator->n_mem_list_size);
	
	set_memlist_free(allocator->lp_mem_list);
	mu_log_message("Allocator address: %p", allocator);
	return allocator;
}

// Zero out a MEMLIST
void set_memlist_free(MEMLIST *lp_list)
{
	memset(lp_list, 0, sizeof(MEMLIST) * 100);
}


void reset_allocator(MU_Allocator *allocator)
{
	// CHECK: Does this function cause a memory leak?
	// We're not freeing the previously allocated memory for lp_mem_list?
	allocator->n_alloc_size = 0;
	allocator->n_alloc_number = 0;
	allocator->n_mem_list_size = 100;
	allocator->b_free = true;

	// malloc & zero out the lp_mem_list
	// allocator->lp_mem_list = (MEMLIST*) malloc(sizeof(MEMLIST*) * allocator->n_mem_list_size);
	// In theory, the above line would cause a memory leak, and is unnecessary, as memory has already
	// been allocated before this function is ever run
	set_memlist_free(allocator->lp_mem_list);
}

// Shows memory usage of given allocator
void show_mem_usage(MU_Allocator *allocator)
{
	mu_log_message("---Memory usage of %s allocator", allocator->str_alloc_name);
	mu_log_message("---Allocated memory %i in bytes", allocator->n_alloc_size);
	mu_log_message("---Allocated number of memory %i", allocator->n_alloc_number);
}

// Allocates memory and saves its address to the lp_mem_list
// Note to self: the original function contained the parameter int mem_init, but never used it
void *mu_alloc(MU_Allocator *allocator, size_t n_size)
{
	if(allocator->b_free == true)
	{
		allocator->b_free = false;
	}

	if(allocator->n_alloc_number > (allocator->n_mem_list_size - 1))
	{
		// Grow lp_mem_list by 100
		allocator->n_mem_list_size += 100;
		allocator->lp_mem_list = (MEMLIST*) realloc(allocator->lp_mem_list, sizeof(MEMLIST) * allocator->n_mem_list_size);
		// Set memlist empty
		// Note to self: this might not need to be &allocator
		set_memlist_free(&allocator->lp_mem_list[allocator->n_mem_list_size-100]);
	}

	u32 i;

	// Search the list for a free place to hold the address
	for (i = 0; i < allocator->n_mem_list_size; i++)
	{
		if(allocator->lp_mem_list[i].n_type == FREE)
		{
			break;
		}
	}

	if(i > allocator->n_mem_list_size)
	{
		mu_log_message("Memory Manager: No free block found");
	}

	allocator->lp_mem_list[i].n_type = ALLOC;
	allocator->lp_mem_list[i].address = malloc(n_size);

	if(allocator->lp_mem_list[i].address == 0)
	{
		mu_log_message("Memory Manager: malloc failed");
	}

	memset(allocator->lp_mem_list[i].address, 0, n_size);
	allocator->lp_mem_list[i].n_size = n_size;

	allocator->n_alloc_number++;
	allocator->n_alloc_size += n_size;

	return allocator->lp_mem_list[i].address;
}

// Reallocates the given memory block and saves the new address in the lp_mem_list
void *mu_realloc(MU_Allocator *allocator, void *current_memory, size_t n_size)
{
	u32 i;
	bool b_found = false;

	// Iterate over lp_mem_list
	for(i = 0; i < allocator->n_mem_list_size; i++)
	{
		// If lp_mem_list[i] matches current_memory, and is allocated
		if(current_memory == allocator->lp_mem_list[i].address && allocator->lp_mem_list[i].n_type == ALLOC)
		{
			b_found = true;
			break;
		}
	}

	if(!b_found)
	{
		mu_log_message("Memory Manager: Realloc failed; %x was never allocated", current_memory);
	}

	allocator->n_alloc_size -= allocator->lp_mem_list[i].n_size;

	allocator->lp_mem_list[i].address = realloc(allocator->lp_mem_list[i].address, n_size);
	allocator->lp_mem_list[i].n_size = n_size;
	allocator->n_alloc_size += n_size;

	return allocator->lp_mem_list[i].address;
}

// Free allocated block
void mu_free(MU_Allocator *allocator, void *current_memory)
{
	u32 i;
	bool b_found = false;

	for(i = 0; i < allocator->n_mem_list_size; i++)
	{
		if(current_memory == allocator->lp_mem_list[i].address && allocator->lp_mem_list[i].n_type == ALLOC)
		{
			b_found = true;
			break;
		}
	}

	if(!b_found)
	{
		mu_log_message("Memory Manager: Free failed; %x was never allocated", current_memory);
	}

	free(allocator->lp_mem_list[i].address);
	// Uncomment this when debugging state_parser.c
	//mu_log_message("Freed block %p from Memory Manager", allocator->lp_mem_list[i].address);
	allocator->lp_mem_list[i].address = NULL;
	allocator->lp_mem_list[i].n_type = FREE;
	allocator->n_alloc_size -= allocator->lp_mem_list[i].n_size;
	allocator->lp_mem_list[i].n_size = 0;

	allocator->n_alloc_number--;
}

// Free all allocated memory in allocator
void free_allocator(MU_Allocator *allocator)
{
	if(!allocator->b_free)
	{
		for(u32 i = 0; i < allocator->n_mem_list_size; i++)
		{
			if(allocator->lp_mem_list[i].n_type == ALLOC)
			{
				mu_free(allocator, allocator->lp_mem_list[i].address);
			}
		}
	}
	//free(allocator->lp_mem_list);
	// Note to self: Technically this free command should be used to free the lp_mem_list, but doing so breaks things
	// This is probably due to an implementation difference, as in the original version, lp_mem_list was
	// part of the mem manager class, rather than a pointer to a struct
	// As far as I can tell, this function works as intended
}


// Constructor for array of MU_Allocators
MU_Allocator *init_mem_manager()
{
	mu_log_message("Memory Manager: Begin Init");
	MU_Allocator *mem_manager = malloc(sizeof(MU_Allocator) * MEM_MANAGER_NUMBER_OF_ALLOCATORS);

	MU_Allocator *mem_manager_ptr = NULL;
	mem_manager_ptr = create_mu_alloc("Main Menu");
	mem_manager[MAINMENU] = *mem_manager_ptr; // This allows for full navigation of the mem_manager array even outside the scope of this function
	
	mem_manager_ptr = create_mu_alloc("Stage");
	mem_manager[STAGE] = *mem_manager_ptr;

	mem_manager_ptr = create_mu_alloc("Engine");
	mem_manager[ENGINE] = *mem_manager_ptr;

	mem_manager_ptr = create_mu_alloc("Player 1");
	mem_manager[P1] = *mem_manager_ptr;

	mem_manager_ptr = create_mu_alloc("Player 2");
	mem_manager[P2] = *mem_manager_ptr;

	mem_manager_ptr = create_mu_alloc("Player 3");
	mem_manager[P3] = *mem_manager_ptr;

	mem_manager_ptr = create_mu_alloc("Player 4");
	mem_manager[P4] = *mem_manager_ptr;

	mu_log_message("Memory Manager: Init Successful");

	return mem_manager;
}

// Returns an individual allocator for easier handling
MU_Allocator *get_allocator(MU_Allocator *mem_manager, int index)
{
	MU_Allocator *allocator_ptr = (MU_Allocator*) malloc(sizeof(MU_Allocator));
	allocator_ptr = &mem_manager[index]; // This should be *mem_manager, right? Or even just mem_manager
	return allocator_ptr;
}

// Free all allocators from memory
void free_mem_manager(MU_Allocator *mem_manager)
{
	MU_Allocator *current_allocator;
	for(int i = 0; i < MEM_MANAGER_NUMBER_OF_ALLOCATORS; i++)
	{
		current_allocator = get_allocator(mem_manager, i);
		free_allocator(current_allocator);
	}
	//free(mem_manager);
	// Note to self: Technically this free command should be used to free memory_manager, but doing so breaks things
	// For more info see free_allocator()
}

// Log detailed memory information
size_t get_total_mem_usage(MU_Allocator *mem_manager)
{
	size_t n_mem = 0;
	mu_log_message("======================Memory Manager============================");
	mu_log_message("|Name:               |Allocated bytes                          |");
	mu_log_message("----------------------------------------------------------------");

	for(int i = 0; i < MEM_MANAGER_NUMBER_OF_ALLOCATORS; i++)
	{
		mu_log_message("|%20s                  |%20i  | ", mem_manager[i].str_alloc_name, mem_manager[i].n_alloc_size);
		n_mem += mem_manager[i].n_alloc_size;
	}

	mu_log_message("Total memory usage: %i", n_mem);
	mu_log_message("================================================================");
	return n_mem;
}

