#ifndef __MUMEMMANAGER__
#define __MUMEMMANAGER__

enum {FREE=0x00, ALLOC=0xff};

MU_Allocator *create_mu_alloc(char *name); // MU_Allocator Constructor
void set_memlist_free(MEMLIST *lp_list); // malloc & zero out the lp_mem_list
void reset_allocator(MU_Allocator *allocator);
void show_mem_usage(MU_Allocator *allocator); // Shows memory usage of given allocator
void *mu_alloc(MU_Allocator *allocator, size_t size);
uint32_t mu_find_address(MU_Allocator *allocator, void *address); // Used to locate a memory address index in the allocator's lp_memlist
void *mu_realloc(MU_Allocator *allocator, void *address, size_t size); // Reallocates the given memory block and saves the new address in the lp_mem_list
void mu_free(MU_Allocator *allocator, void *address); // Free allocated block
void free_allocator(MU_Allocator *allocator); // Free all allocated memory

MU_Allocator *init_mem_manager(); // Constructor for array of MU_Allocators
MU_Allocator *get_allocator(MU_Allocator *mem_manager, int index); // Returns an individual allocator for easier handling
void free_mem_manager(MU_Allocator *mem_manager); // Free all allocators from memory
size_t get_total_mem_usage(MU_Allocator *mem_manager); // Log detailed memory information

#endif