#ifndef __STRUCTS__H
#define __STRUCTS__H

// Constants
#define MEM_MANAGER_ALLOCATOR_COUNT 7
#define TOKEN_DEFAULT_OPERATORS_COUNT 23
#define BIT_DEPTH 8
#define COLOUR_PALETTE_SIZE 256
#define ONE_SECOND 1000
#define HALF_SECOND 500
#define MAX_COMMANDS 30
#define VER "Alpha"

// Macros
// These are used in cmd_manager.c to create and manage the key bitfield
#define KEYMOD_MUST_BE_HELD    ( 1 << 0 )
#define KEYMOD_DETECT_AS_4WAY  ( 1 << 1 )
#define KEYMOD_BAN_OTHER_INPUT ( 1 << 2 )
#define KEYMOD_ON_RELEASE      ( 1 << 3 )
#define KEYCODE(x)             ( 1 << x )
#define IS_KEY_IN_KEYCODE( k, c )  ((( KEYCODE( k ) & c ) > 0 )
#define ALL_DIRECTIONS_BITFIELD ( KEYCODE( KEY_UP ) + KEYCODE( KEY_DOWN ) + KEYCODE( KEY_LEFT ) + KEYCODE( KEY_RIGHT ) )

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

enum KEY_NAMES
{
    KEY_UP,
    KEY_DOWN,
    KEY_LEFT,
    KEY_RIGHT,
    KEY_BUTTON_A,
    KEY_BUTTON_B,
    KEY_BUTTON_C,
    KEY_BUTTON_X,
    KEY_BUTTON_Y,
    KEY_BUTTON_Z,
    KEY_BUTTON_START,
    KEY_BUTTON_PAUSE,
    KEY_COUNT           // Used in CMD Manager
};

enum KEY_PRESSED
{
    NOT_PRESSED = 0,
    PRESSED = 1
};

enum Program_State
{
	GMENU = 1,
	GFIGHTGAME
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
    SDL_Surface *screen_surface;
    SDL_Renderer *renderer;
    SDL_Texture *font_texture;
    SDL_PixelFormat *colour_format;
    SDL_Rect whole_screen_rect;
    SDL_Rect characters_rect; // This will probably not be needed once SCREEN_SCALE_FACTOR is properly utilised
    // Do we need a backgrounds texture?
    // Do we need an FX texture?
    // Do we need a UI texture?
    SDL_Texture *text_texture;

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

// =============Player Def================

struct Player_Def
{
    char air_filename[255];
    char sff_filename[255];
    char act_filename[255];
    char cmd_filename[255];
    //char *cns_filename;
    //char *snd_filename;
};
typedef struct Player_Def Player_Def;

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
	uint16_t flip_flags; // Horizontal/Vertical sprite flipping
	uint32_t colour_flags; // Transparency blending (colour addition/subtraction)
	Clsn *clsn_data;
	uint16_t clsn_count;
};
typedef struct Element Element;

struct Action
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
typedef struct Action Action;

// This struct has a lot of temp data fields for initialising a character's .air file
// TODO: declare all temp variables on the heap so they can be freed once used? (especially clsn_arr)
struct MU_Air_Manager
{
	Action *action_list;
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
    uint8_t colour_map[48]; // Unused?
    uint8_t reserved;
    uint8_t planes_count;
    uint8_t bytes_per_line;
    uint8_t palette_info; // Unused?
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
    uint8_t palette_type; // Unused?
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
    Action *anim;
};
typedef struct MU_SFF_Manager MU_SFF_Manager;

// ==============Input==============================
struct Key
{
    uint8_t is_pressed; // 0 = not pressed, 1 = key is pressed
    unsigned sdl_keycode;
};
typedef struct Key Key;

// ========CMD Manager==============================
struct Command_Key
{
    int keycode;
    uint16_t key_modifier;
    uint16_t game_ticks_for_hold;
};
typedef struct Command_Key Command_Key;

struct Command
{
    Command_Key cmd_keys[MAX_COMMANDS];
    uint8_t cmd_keys_count;
    uint8_t command_time;
    uint8_t buffer_time;
    char str_command[255];
};
typedef struct Command Command;

struct Cmd_Frame_Input
{
    uint16_t key_bitfield;
    uint32_t game_ticks;
};
typedef struct Cmd_Frame_Input Cmd_Frame_Input;

struct MU_CMD_Manager
{
    MU_Timer *cmd_timer;
    Command *commands;
    int command_count;
    const char *current_command_name;

    // Keyboard buffer
    Cmd_Frame_Input *key_buffer;
    int16_t key_buffer_size;
    int16_t key_index;
};
typedef struct MU_CMD_Manager MU_CMD_Manager;


// Skipping a few sections that require a lot of reworking, to get to a "bare minimum" working game_time
// Will come back to these and rework them once I can render animated sprites to the screen

// =========State Manager==============================
// ==========State Parser============================
// ==========Player==================================
struct Player_Vars
{
	int var[60];
	double fvar[40]; // Was float in the previous version
};
typedef struct Player_Vars Player_Vars;

struct Player
{
	MU_Allocator *player_allocator;
	MU_Graphics_Manager *graphics_manager;
	MU_SFF_Manager *sff_manager;
	MU_Air_Manager *air_manager;
	MU_Timer *timer;
	// MU_State_Manager *state_manager;
	// VM *vm;
	// MU_Controller_Executer *controller_exec;
	MU_CMD_Manager *cmd_manager;

	// Variable player information
	// These doubles were formerly floats
	double x;
	double y;
	double xvel;
	double yvel;
	double ground; // Rename to something more descriptive?
	int anim_num;
	uint32_t state_time;
	uint8_t state_type;
	uint8_t physics;
	uint8_t movetype;
	uint8_t spr_prio;
	int16_t life;
	int16_t power;
	bool hitdef_persist;
	bool hitcount_persist;
	bool movehit_persist;
	bool ctrl;
	bool right_faced;
	bool debug_info;
	bool hitdef;
	bool alive;
	char command[50];
	//Statedef *current_statedef;
	Player_Vars player_vars;
	// Player_Const player_const;

    Key *kb;
};
typedef struct Player Player;

// ==========Controller Executer===================
// ===========Stack================================
// ==========VM====================================
// ===========Engine===============================
struct MU_Engine
{
	Player *p1;
	Player *p2;
	// These last two should be null unless needed
	Player *p3;
	Player *p4;

	MU_Allocator *mem_manager;
	MU_Graphics_Manager *graphics_manager;
	MU_Timer *timer;
	// VM *vm_p1;
	// VM *vm_p2;
};
typedef struct MU_Engine MU_Engine;

// =========Game==================================
struct Game
{
	MU_Graphics_Manager *graphics_manager;
	MU_Allocator *mem_manager;
	MU_Timer *timer;
	MU_Engine *engine;

	bool error;
	bool in_game; // This will be used to determine if you're in the menu or in an actual game
	uint16_t game_type;
	// These doubles were originally floats
	double start_time;
	double end_time;
	double framerate;

	SDL_Event event; // Should this be a pointer?
};
typedef struct Game Game;

#endif