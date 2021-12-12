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


	// For the following section, I've chosen to disable these checks unless debugging is turned on
	// It may be useful to enable these again at some point for logging purposes (TODO: profiling)
	// Currently I've chosen to disable them, since this function should be as fast as possible
#if DEBUG == 1
	if(i > allocator->memlist_count)
	{
		debug_print("Memory Manager: No free block found");
	}
#endif

	allocator->lp_memlist[i].type = ALLOC;
	allocator->lp_memlist[i].address = malloc(size);

#if DEBUG == 1
	if(allocator->lp_memlist[i].address == 0)
	{
		debug_print("Memory Manager: malloc failed");
	}
#endif

	memset(allocator->lp_memlist[i].address, 0, size);
	allocator->lp_memlist[i].size = size;
	allocator->alloc_count++;
	allocator->alloc_size += size;

	return allocator->lp_memlist[i].address;
}

// Used to locate a memory address index in the allocator's lp_memlist
uint32_t mu_find_address(MU_Allocator *allocator, void *address)
{
	uint32_t i;

	// Iterate over lp_memlist
	for(i = 0; i < allocator->memlist_count; i++)
	{
		// If the address matches the one we're looking for and is allocated
		if(address == allocator->lp_memlist[i].address && allocator->lp_memlist[i].type == ALLOC)
		{
			return i;
			break;
		}
	}
	// If the address is not found, 
	return UINT32_MAX;
}

// Reallocates the given memory block and saves the new address in the lp_memlist
void *mu_realloc(MU_Allocator *allocator, void *address, size_t size)
{
	uint32_t i = mu_find_address(allocator, address);

	// See mu_alloc for why this is disabled outside of debugging
#if DEBUG == 1
	if(i == UINT32_MAX)
	{
		debug_print("Memory Manager: Realloc failed; %x was never allocated", address);
	}
#endif

	allocator->alloc_size -= allocator->lp_memlist[i].size;
	allocator->lp_memlist[i].address = realloc(allocator->lp_memlist[i].address, size);
	allocator->lp_memlist[i].size = size;
	allocator->alloc_size += size;

	return allocator->lp_memlist[i].address;
}

// Free allocated block
void mu_free(MU_Allocator *allocator, void *address)
{
	uint32_t i = mu_find_address(allocator, address);

	// See mu_alloc for why this is disabled outside of debugging
#if DEBUG == 1
	if(i == UINT32_MAX)
	{
		debug_print("Memory Manager: Free failed; %x was never allocated", address);
	}
#endif

	free(allocator->lp_memlist[i].address);

	// Uncomment this line if required for debugging
	//debug_print("Freed block %p from Memory Manager", allocator->lp_memlist[i].address);
	allocator->lp_memlist[i].address = NULL;
	allocator->lp_memlist[i].type = FREE;
	allocator->alloc_size -= allocator->lp_memlist[i].size;
	allocator->lp_memlist[i].size = 0;
	allocator->alloc_count--;

	if(allocator->alloc_size == 0)
	{
		allocator->free = true;
	}
}

// Free all allocated memory in the allocator
void free_allocator(MU_Allocator *allocator)
{
	if(!allocator->free)
	{
		for (uint32_t i = 0; i < allocator->memlist_count; i++)
		{
			if(allocator->lp_memlist[i].type == ALLOC)
			{
				mu_free(allocator, allocator->lp_memlist[i].address);
			}
		}
	}

	allocator->free = true;
}

