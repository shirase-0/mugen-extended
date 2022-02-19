#ifndef __STRUCTS__H
#define __STRUCTS__H

// Constants
#define MEM_MANAGER_ALLOCATOR_COUNT 7
#define TOKEN_DEFAULT_OPERATORS_COUNT 23

// Enums
enum MEM_ALLOCATOR_NAMES
{
    MAINMENU = 0,
    STAGE = 1,
    ENGINE = 2,
    P1 = 3,
    P2 = 4,
    P3 = 5,
    P4 = 6
};

// ========Memory Manager===========================

struct MEMLIST
{
    uint8_t type; 	// Value to check whether the MEMLIST is allocated or free; TODO: change to bool?
    size_t size;  	// Size of the memory allocated
    void *address;  // Memory address for the beginning of the allocated memory block
};
typedef struct MEMLIST MEMLIST;

struct MU_Allocator
{
    char alloc_name[255];   	// Name of allocator
    size_t alloc_size;    		// Allocated memory in bytes
    size_t alloc_count;      	// Number of allocated MEMLISTs
    size_t memlist_count;     	// The number of MEMLISTs (free and allocated) in this allocator
    MEMLIST *lp_memlist;       	// Array of MEMLISTs; keeping this naming convention for pointer lists
    bool free;                	// Is the allocator empty or not
};
typedef struct MU_Allocator MU_Allocator;

// ==========FPS Manager============================

struct MU_FPS_Manager
{
    uint32_t framecount;
    double rateticks;
    uint32_t lastticks;
    uint32_t rate;
};
typedef struct MU_FPS_Manager MU_FPS_Manager;

// ========Video System============================

struct MUGENFONT
{
    uint8_t c;
    int16_t x;
    uint8_t width;
};
typedef struct MUGENFONT MUGENFONT;

struct MU_Graphics_Manager
{
    SDL_Window *window;
    SDL_Surface *screen_surface; // TODO: Remove if unused
    SDL_Renderer *renderer;
    SDL_Texture *font_texture;

    uint8_t delta_ptr;
    uint32_t now_time;
    uint32_t last_time;
    uint32_t fps_count;
    double fps;
    MU_FPS_Manager fps_manager;
    MUGENFONT *raster_font;
};
typedef struct MU_Graphics_Manager MU_Graphics_Manager;

// ============Tokenizer===============================

struct Tokenizer
{
    char *buffer;
    int buffer_size;

    char filename[256];
    char *file_buffer;
    int filesize; // TODO: change this to size_t?
    int cur_file_pos;
    int cur_file_line;
    int last_line_pos;

    bool buffer_is_next_token;
    bool at_end_of_line;
    bool at_eof;
    char *comment_chars;
    char **operators;
    int operator_count;
    int num_operator_chars_read;

    bool case_sensitive;
    bool negative_separate_from_number;
    bool last_token_was_quoted_string;
};
typedef struct Tokenizer Tokenizer;

// =============Timer=====================

struct MU_Timer
{
    uint32_t game_time;
    bool is_paused;
};
typedef struct MU_Timer MU_Timer;

#endif