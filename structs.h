#ifndef __STRUCTS__H
#define __STRUCTS__H

// Constants
#define MEM_MANAGER_ALLOCATOR_COUNT 7
#define TOKEN_DEFAULT_OPERATORS_COUNT 23
#define BIT_DEPTH 8
#define COLOUR_PALETTE_SIZE 256

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

// Note to self: Possibly incomplete?
// Additionally, why start this enum at 10?
enum BLT_Flags 
{
   BLT_NORMAL = 10,
   BLT_NORMALMASKED,
   BLT_FLIPH,
   BLT_FLIPHMASKED,
   BLT_FLIPV,
   BLT_FLIPVMASKED,
   BLT_FLIPHV,
   BLT_FLIPHVMASKED,
   BLT_ADDALPHA,
   BLT_SUBALPHA   
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
	bool has_loop; 
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

// ============SFF Manager=====================
struct PCX_Header
{
    uint8_t manufacturer;
    uint8_t version;
    uint8_t encoding;
    uint8_t bpp;
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;
    uint16_t hres;
    uint16_t vres;
    uint8_t colour_map[48];
    uint8_t reserved;
    uint8_t planes_count;
    uint8_t bytes_per_line;
    uint8_t palette_info;
    uint8_t filler[58]; // Can we remove this and just discard it when we encounter it?
};
typedef struct PCX_Header PCX_Header;

struct SFF_Header
{
    uint8_t signature[11];
    uint8_t ver_hi;
    uint8_t ver_lo;
    uint8_t ver_hi2;
    uint8_t ver_lo2;
    uint32_t groups_count;
    uint32_t images_count;
    uint32_t subheader_file_offset;
    uint32_t sizeof_subheader;
    uint8_t palette_type;
    uint8_t blank[476]; // Can we remove this and just discard it when we encounter it?
};
typedef struct SFF_Header SFF_Header;

struct SFF_Subheader
{
    uint32_t next_subheader_file_offset;
    uint32_t subheader_len;
    int16_t x;
    int16_t y;
    int16_t group_num;
    int16_t image_num;
    int16_t index_of_previous;
    bool is_same_palette;
    uint8_t blank[13]; // Can we remove this and just discard it when we encounter it?
};
typedef struct SFF_Subheader SFF_Subheader;

struct SFF_Sprite
{
    int16_t x;
    int16_t y;
    PCX_Header pcx_header; // Should this be a pointer?
    int16_t group_num;
    int16_t image_num;
    uint32_t colour_palette[256];

    uint8_t *pcx_file_bytes;
};
typedef struct SFF_Sprite SFF_Sprite;

struct MU_SFF_Manager
{
    bool is_palette_loaded; 
    FILE *sff_file;
    MU_Graphics_Manager *graphics_manager;
    MU_Allocator *sff_allocator;
    MU_Timer *timer;
    MU_Air_Manager *air_manager;

    uint32_t colour_palette[256];
    uint16_t total_images;
    uint16_t current_image;
    uint16_t image_list_size;
    uint16_t flags;

    SFF_Sprite *sprite_list;
    float x_scale_value; // TODO: if it doesn't break anything, change these to doubles
    float y_scale_value;
    Action_Element *anim;
};
typedef struct MU_SFF_Manager MU_SFF_Manager;

// Skipping a few sections that require a lot of reworking, to get to a "bare minimum" working game_time
// Will come back to these and rework them once I can render animated sprites to the screen
// ========CMD Manager==============================
// =========State Manager==============================
// ==========State Parser============================
// ==========Player==================================


#endif