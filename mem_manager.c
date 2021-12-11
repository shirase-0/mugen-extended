#include "mu_headers.h"

// MU_Allocator Constructor
MU_Allocator *create_mu_alloc(char *name)
{
	debug_print("Memory Manager: Initialising %s memory allocator", name);
	MU_Allocator *allocator = (MU_Allocator*) malloc(sizeof(MU_Allocator));

	// Set allocator name
	memset(allocator->alloc_name, 0, sizeof(allocator->alloc_name));
	strcpy(allocator->alloc_name, name);

	allocator->alloc_size = 0;
	allocator->alloc_count = 0;
	allocator->memlist_count = 100;
	allocator->free = true;

	// malloc & zero out the lp_memlist
	allocator->lp_memlist = (MEMLIST*) malloc(sizeof(MEMLIST) * allocator->memlist_count);
	
	set_memlist_free(allocator->lp_memlist);
	debug_print("Allocator address: %p", allocator);
	return allocator;
}

// Zero out a MEMLIST
void set_memlist_free(MEMLIST *lp_list)
{
	memset(lp_list, 0, sizeof(MEMLIST) * 100);
}

// Resets an allocator to its initial state
void reset_allocator(MU_Allocator *allocator)
{
	allocator->alloc_size = 0;
	allocator->alloc_count = 0;
	allocator->memlist_count = 100;
	allocator->free = true;

	// realloc & zero out the lp_memlist
	//allocator->lp_memlist = (MEMLIST*) malloc(sizeof(MEMLIST*) * allocator->memlist_count);
	// In theory, the above line would cause a memory leak, and is unnecessary, as memory has already
	// been allocated before this function is ever run
    // Here I've changed it from malloc to realloc, to hopefully kill two birds with one stone
    // This should reduce our memory footprint back to what it should be, without needing to 
    // free anything
    allocator->lp_memlist = (MEMLIST*) realloc(allocator->lp_memlist, sizeof(MEMLIST) * allocator->memlist_count);
	set_memlist_free(allocator->lp_memlist);
}

// Shows memory usage of given allocator
void show_mem_usage(MU_Allocator *allocator)
{
	debug_print("------------------------------------------------------------");
	debug_print("Name\t\tSize (bytes)\t\tAllocated MEMLISTs");
	debug_print("%s\t%d\t\t\t%d", allocator->alloc_name, 
								allocator->alloc_size, 
								allocator->alloc_count);
	debug_print("\nTotal MEMLISTs\tMemlist Pointer\t\tFree?");
	debug_print("%d\t\t%p\t%d", allocator->memlist_count,
								allocator->lp_memlist,
								allocator->free);
	debug_print("------------------------------------------------------------");
}

void *mu_alloc(MU_Allocator *allocator, size_t size)
{
	if(allocator->free == true)
	{
		allocator->free = false;
	}

	if(allocator->alloc_count > (allocator->memlist_count - 1))
	{
		// Grow lp_mem_list by 100
		allocator->memlist_count += 100;
		allocator->lp_memlist = (MEMLIST*) realloc(allocator->lp_memlist, sizeof(MEMLIST) * allocator->memlist_count);
		// Set memlist empty
		set_memlist_free(&allocator->lp_memlist[allocator->memlist_count - 100]);

	}

	uint32_t i;

	// Search the list for a free place to hold the address
	for (i = 0; i < allocator->memlist_count; i++)
	{
		if(allocator->lp_memlist[i].type == FREE)
		{
			break;
		}
	}

	// TODO: move this error check outside of this function?
	if(i > allocator->memlist_count)
	{
		debug_print("Memory Manager: No free block found");
	}

	allocator->lp_memlist[i].type = ALLOC;
	allocator->lp_memlist[i].address = malloc(size);

	// There was an error check here, but this should be moved elsewhere in order to ensure this function is
	// as fast as possible

	memset(allocator->lp_memlist[i].address, 0, size);
	allocator->lp_memlist[i].size = size;
	allocator->alloc_count++;
	allocator->alloc_size += size;

	return allocator->lp_memlist[i].address;
}