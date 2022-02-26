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

enum FLIPFLAGS_ENUM
{
    HFLIPFLAG=1,
    VFLIPFLAG=2,
    HVFLIPFLAG=3
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

// ============Air Manager================

struct Clsn
{
	bool is_clsn1; // Clsn1 is for attacks, Clsn2 is for normal collision boxes
	int16_t x;
	int16_t y;
	int16_t h;
	int16_t w;
};
typedef struct Clsn Clsn;

// TODO: implement scale/angle parameters for MUGEN 1.1 compatibility
// TODO: implement interpolation (offset, blend, scale, angle) for MUGEN 1.1 compatibility
struct Element 
{
	int16_t group_num; // Sprite group number
	int16_t image_num; // Sprite image number
	int16_t x;
	int16_t y;
	uint32_t during_time; // Number of ticks the current sprite is displayed in this animation
	// TODO: Couldn't flip_flags be an int8_t? Or even a uint8_t
	uint16_t flip_flags; // Horizontal/Vertical sprite flipping
	uint32_t colour_flags; // Transparency blending (colour addition/subtraction)
	Clsn *clsn_data;
	uint16_t clsn_count;
};
typedef struct Element Element;

struct Action_Element
{
	int action_num;
	Element *anim_element;
	int16_t loop_start;
	uint16_t elements_count;
	uint16_t current_image;

	uint32_t during_time;
	int16_t complete_anim_time;
	uint32_t current_time;
	uint16_t current_image_time;
	bool looped; 
};
typedef struct Action_Element Action_Element;

// This struct has a lot of temp data fields for initialising a character's .air file
// TODO: declare all temp variables on the heap so they can be freed once used? (especially clsn_arr)
struct MU_Air_Manager
{
	Action_Element *action_list;
	MU_Allocator *air_allocator;
	uint16_t total_action_block;
	uint16_t action_list_size;
	uint16_t anim_time;
	uint16_t total_elements;
	uint16_t element_list_size;
	uint16_t total_clsn;
	uint16_t clsn_list_size;
	bool is_default_clsn;
	bool is_clsn1;
	Clsn clsn_arr[200]; // temp storage space whilst initialising clsn data
};
typedef struct MU_Air_Manager MU_Air_Manager;

#endif