#ifndef __STRUCTS__H
#define __STRUCTS__H

// Simple Typedefs
// TODO: replace these with definitions from stdint.h
typedef unsigned char   u8;
typedef signed   char   s8;
typedef unsigned short  u16;
typedef signed   short  s16;
typedef unsigned long   u32;
typedef signed   long   s32;

// Constants
#define MEM_MANAGER_NUMBER_OF_ALLOCATORS 7
#define MAX_COMMAND 30
#define VER "Alpha"
// More to be found in state_manager.h

// Macros
// These should probably be replaced
#define PLC_KEYMOD_MUST_BE_HELD    ( 1 << 0 )
#define PLC_KEYMOD_DETECT_AS_4WAY  ( 1 << 1 )
#define PLC_KEYMOD_BAN_OTHER_INPUT ( 1 << 2 )
#define PLC_KEYMOD_ON_RELEASE      ( 1 << 3 )
#define PLC_KEYCODE(x)             ( 1 << x )
#define IsKeyInPLCKeyCode( k, c )  ((( PLC_KEYCODE( k ) & c ) > 0 )
#define PLC_ALL_DIRECTIONS_BITFIELD ( PLC_KEYCODE( KEY_UP ) + PLC_KEYCODE( KEY_DOWN ) + PLC_KEYCODE( KEY_LEFT ) + PLC_KEYCODE( KEY_RIGHT ) )

// Taken from controller_executer.h
#define NUMCTRLFUNC 90

// Taken from stack.h
#define STACKSIZE 100

// Taken from vm.h
#define NUMFUNCT 140

// Enums

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
    KEY_COUNT			// Used in CMD Manager
};

enum MEM_ALLOCATOR_NAMES // For code readability only, currently unused
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

enum BLT_Flags // Note to self: Possibly incomplete?
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

enum STATE_TYPES
{
    STAND = 0,
    CROUCH,
    AIR,
    LIEDOWN,
    ATTACK,
    IDLE,
    HIT,
    NONE,
    UNTOUCH
};

// Copied directly from the original version
enum OPCODES
{
    OP_PUSH=0,
    OP_POP,
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_EQUAL,
    OP_NOTEQUAL,
    OP_LESS,
    OP_GREATER,
    OP_LESSEQUAL,
    OP_GREATEREQUAL,
    OP_INTERVALOP1,
    OP_INTERVALOP2,
    OP_INTERVALOP3,
    OP_INTERVALOP4,
    OP_INTERVALOP5,
    OP_INTERVALOP6,
    OP_INTERVALOP7,
    OP_INTERVALOP8,
    OP_LOGNOT,
    OP_LOGAND,
    OP_LOGOR,
    OP_LOGXOR,
    OP_NOT,
    OP_AND,
    OP_OR,
    OP_XOR,
    OP_SQUARE,
    OP_NEG,
    OP_Abs,
    OP_Acos,
    OP_Alive,
    OP_Anim,
    OP_AnimElem,
    OP_AnimElemNo,
    OP_AnimElemTime,
    OP_AnimExist,
    OP_AnimTime,
    OP_Asin,
    OP_Atan,
    OP_AuthorName,
    OP_BackEdgeBodyDist,
    OP_BackEdgeDist,
    OP_CanRecover,
    OP_Ceil,
    OP_Command,
    OP_Const,
    OP_Cos,
    OP_Ctrl,
    OP_DrawGame,
    OP_E,
    OP_Exp,
    OP_Facing,
    OP_Floor,
    OP_FrontEdgeBodyDist,
    OP_FrontEdgeDist,
    OP_FVar,
    OP_GameTime,
    OP_GetHitVar,
    OP_HitCount,
    OP_HitDefAttr,
    OP_HitFall,
    OP_HitOver,
    OP_HitPauseTime,
    OP_HitShakeOver,
    OP_HitVel,
    OP_ID,
    OP_IfElse,
    OP_InGuardDist,
    OP_IsHelper,
    OP_IsHomeTeam,
    OP_Life,
    OP_LifeMax,
    OP_Log,
    OP_Ln,
    OP_Lose,
    OP_MatchNo,
    OP_MatchOver,
    OP_MoveContact,
    OP_MoveGuarded,
    OP_MoveHit,
    OP_MoveType,
    OP_MoveReversed,
    OP_Name,
    OP_NumEnemy,
    OP_NumExplod,
    OP_NumHelper,
    OP_NumPartner,
    OP_NumProj,
    OP_NumProjID,
    OP_NumTarget,
    OP_P1Name,
    OP_P2BodyDist,
    OP_P2Dist,
    OP_P2Life,
    OP_P2MoveType,
    OP_P2Name,
    OP_P2StateNo,
    OP_P2StateType,
    OP_P3Name,
    OP_P4Name,
    OP_PalNo,
    OP_ParentDist,
    OP_Pi,
    OP_Pos,
    OP_Power,
    OP_PowerMax,
    OP_PlayerIDExist,
    OP_PrevStateNo,
    OP_ProjCancelTime,
    OP_ProjContact,
    OP_ProjContactTime,
    OP_ProjGuarded,
    OP_ProjGuardedTime,
    OP_ProjHit,
    OP_ProjHitTime,
    OP_Random,
    OP_RootDist,
    OP_RoundNo,
    OP_RoundsExisted,
    OP_RoundState,
    OP_ScreenPos,
    OP_SelfAnimExist,
    OP_Sin,
    OP_StateNo,
    OP_StateType,
    OP_SysFVar,
    OP_SysVar,
    OP_Tan,
    OP_TeamMode,
    OP_TeamSide,
    OP_TicksPerSecond,
    OP_Time,
    OP_TimeMod,
    OP_UniqHitCount,
    OP_Var,
    OP_Vel,
    OP_Win,
    OP_MOD,
    OP_NOP,
    OP_STOP,
};

// Copied from game.h
enum
{ GMENU =1,
  GFIGHTGAME
}; 

// ========Memory Manager===========================

struct MEMLIST
{
    u8 n_type; // Value to check whether the MEMLIST is allocated or free
    size_t n_size; // Size of the memory allocated
    void *address; // Memory address for the beginning of the allocated memory block
};
typedef struct MEMLIST MEMLIST;

struct MU_Allocator
{
    char str_alloc_name[255];   // Name of allocator
    size_t n_alloc_size;    // Allocated memory in bytes
    size_t n_alloc_number;        // Allocated number of memory
    size_t n_mem_list_size;     // The number of MEMLISTs in this allocator
    MEMLIST *lp_mem_list;       // Array of MEMLISTs
    bool b_free;                // Is the allocator empty or not
};
typedef struct MU_Allocator MU_Allocator;

// ==========FPS Manager============================

struct MU_FPS_Manager
{
    u32 framecount;
    float rateticks;
    u32 lastticks;
    u32 rate;
};
typedef struct MU_FPS_Manager MU_FPS_Manager;

// ========Video System============================

struct MUGENFONT
{
    u8 c;
    s16 x;
    u8 n_width;
};
typedef struct MUGENFONT MUGENFONT;

struct MU_Video_System
{
    SDL_Window *window;
    SDL_Surface *screen_surface;
    SDL_Renderer *renderer;
    SDL_Surface *font;

    u8 delta_ptr;
    u32 now_time;
    u32 last_time;
    u32 n_fps_count;
    float n_fps;
    MU_FPS_Manager fps_manager;
    MUGENFONT *mu_font;
};
typedef struct MU_Video_System MU_Video_System;

// =============Timer=====================

struct MU_Timer
{
    u32 n_game_time;
    bool b_pause;
};
typedef struct MU_Timer MU_Timer;

// ============Air Manager================

struct Clsn_Rect
{
    s16 x;
    s16 y;
    s16 h;
    s16 w;
};
typedef struct Clsn_Rect Clsn_Rect;

struct Clsn
{
    bool b_clsn1;
    Clsn_Rect clsn_rect;
};
typedef struct Clsn Clsn;

struct Element
{
    s16 n_group_number;
    s16 n_image_number;
    s16 x;
    s16 y;
    u32 n_during_time;
    u16 flip_flags;
    u32 colour_flags;
    Clsn *p_clsn_data;
    u16 n_number_of_clsn;
};
typedef struct Element Element;

struct Action_Element
{
    s32 n_action_number;
    Element *animation_element;
    s16 loop_start;
    u16 n_number_of_elements;
    u16 n_current_image;

    u32 n_during_time;
    s16 n_complete_anim_time;
    u32 n_current_time;
    u16 n_current_image_time;
    bool b_looped;
};
typedef struct Action_Element Action_Element;

struct MU_Air_Manager
{
    Action_Element *lp_action_list;
    MU_Allocator *air_allocator;
    u16 n_total_action_block;
    u16 n_action_list_size;
    u16 n_anim_time;
    u16 n_total_element;
    u16 n_element_list_size;
    u16 n_total_cns;
    u16 n_cns_list_size;
    bool b_default_clsn;
    bool b_is_clsn1;
    Clsn p_clsn[200];
};
typedef struct MU_Air_Manager MU_Air_Manager;

// ============Tokenizer===============================

struct Tokenizer
{
    char *buffer;
    int buffer_size;

    char filename[256];
    char *file_buffer;
    int file_size;
    int current_file_pos;
    int current_file_line;
    int last_line_pos;

    bool buffer_is_next_token;
    bool at_end_of_line;
    bool at_end_of_file;
    char *comment_chars;
    char **operators;
    int operator_count;
    int num_operator_chars_read;

    bool is_case_sensitive;
    bool return_negative_separately_from_number;
    bool last_token_was_quoted_string;
};
typedef struct Tokenizer Tokenizer;

// ============SFF Manager================================

struct PCX_Header
{
    u8 manufacturer;
    u8 version;
    u8 encoding;
    u8 bpp;
    u16 x;
    u16 y;
    u16 width;
    u16 height;
    u16 hres;
    u16 vres;
    u8 colour_map[48];
    u8 reserved;
    u8 n_planes;
    u8 bytes_per_line;
    u8 pallet_info;
    u8 filler[58];
};
typedef struct PCX_Header PCX_Header;

struct SFF_Header
{
    u8 signature[11];
    u8 ver_hi;
    u8 ver_lo;
    u8 ver_hi2;
    u8 ver_lo2;
    u32 number_of_groups;
    u32 number_of_image;
    u32 subheader_file_offset;
    u32 size_of_subheader;
    u8 palette_type;
    u8 blank[476];
};
typedef struct SFF_Header SFF_Header;

struct SFF_Subheader
{
    u32 next_subheader_file_offset;
    u32 length_of_subheader;
    s16 x;
    s16 y;
    s16 group_number;
    s16 image_number;
    s16 index_of_previous;
    bool palette_same;
    u8 blank[13];
};
typedef struct SFF_Subheader SFF_Subheader;

struct SFF_Sprite
{
    s16 x;
    s16 y;
    PCX_Header pcx_header; // Should this be a pointer?
    s16 group_number;
    s16 image_number;
    unsigned colour_palette[256]; // was u16

    u8 *by_pcx_file;
};
typedef struct SFF_Sprite SFF_Sprite;

struct MU_SFF_Manager
{
    bool b_pallet_loaded;
    FILE *lp_sff_file;
    MU_Video_System *sff_video_system;
    MU_Allocator *sff_allocator;
    MU_Timer *sff_timer;
    MU_Air_Manager *sff_air_manager;

    unsigned colour_palette[256];
    u16 n_total_images;
    u16 n_current_image;
    u16 n_image_list_size;
    u16 n_flags;

    SFF_Sprite *lp_sprite_list;
    float x_scale_value;
    float y_scale_value;
    Action_Element *anim;
};
typedef struct MU_SFF_Manager MU_SFF_Manager;

// ========CMD Manager==============================
struct Key_Element
{
    unsigned is_pressed;
    unsigned sdl_keycode;
};
typedef struct Key_Element Key_Element;

struct Keyboard_Data
{
    Key_Element key_info[KEY_COUNT]; // Wasn't a pointer in the original version
    bool b_keyboard;
};
typedef struct Keyboard_Data Keyboard_Data;

struct PL_Command_Element
{
    int keycode;
    u16 key_modifier;
    u16 game_ticks_for_hold;
};
typedef struct PL_Command_Element PL_Command_Element;

struct PL_Command
{
    PL_Command_Element n_command[MAX_COMMAND];
    u8 n_how_many_command;
    u8 n_command_time;
    u8 n_buffer_time;
    char str_command[255];
};
typedef struct PL_Command PL_Command;

struct PL_Cmd_Frame_Input
{
    u16 key_bitfield;
    u32 game_ticks;
};
typedef struct PL_Cmd_Frame_Input PL_Cmd_Frame_Input;

struct MU_CMD_Manager
{
    MU_Timer *cmd_timer;
    PL_Command *commands;
    int command_count;
    const char *current_command_name;

    // Keyboard buffer
    PL_Cmd_Frame_Input *key_buffer;
    short key_buffer_size;
    short key_index;
};
typedef struct MU_CMD_Manager MU_CMD_Manager;

// =========State Manager==============================
struct XY_Value
{
    float x;
    float y;
};
typedef struct XY_Value XY_Value;

struct Instruction
{
    u16 n_opcode;
    float value;
    char *str_value;
};
typedef struct Instruction Instruction;

struct PL_Trigger
{
    u8 n_trigger_type;
    Instruction *p_inst;
};
typedef struct PL_Trigger PL_Trigger;

struct PL_State
{
    s32 n_state_number;
    u16 n_type;
    PL_Trigger *triggers;
    u16 n_how_many_triggers;
    bool b_persist;
    bool b_ignore_pause;
    void *controller;
};
typedef struct PL_State PL_State;

struct PL_Statedef
{
    // Recommended parameters
    s32 state_number;
    u8 type;
    u8 movetype;
    u8 physics;
    s32 n_anim;

    // Optional parameters
    XY_Value velset;
    s8 b_ctrl;
    s16 n_poweradd;
    s16 n_juggle;
    bool b_facep2;

    // Some flags
    bool b_hitdef_persist;
    bool b_movehit_persist;
    bool b_hitcount_persist;

    ////////////////
    u8 n_spr_priority;
    u16 n_how_many_state;

    PL_State *lp_state;
};
typedef struct PL_Statedef PL_Statedef;

struct Hit_Vars
{
    float xveladd;
    float yveladd;
    // These data types have been guessed, if they don't work, try switching to int
    short type;
    short animtype;
    u8 airtype;
    u8 groundtype;
    // -------------------------
    int damage;
    int hitcount;
    int fallcount;
    int hitshaketime;
    int hittime;
    int slidetime;
    int ctrltime;
    int recovertime;
    float xvel;
    float yvel;
    float yaccel;
    int chainid;
    bool guarded;
    bool isbound;
    bool fall;
    int fall_damage;
    float fall_xvel;
    float fall_yvel;
    bool fall_recover;
    int fall_recovertime;
    int fall_kill;
    int fall_envshake_time;
    float fall_envshake_freq;
    int fall_envshake_ampl;
    float fall_envshake_phase;
};
typedef struct Hit_Vars Hit_Vars;

struct MU_State_Manager
{
    MU_Allocator *state_allocator;
    Instruction p_inst[200];
    PL_Statedef *lp_statedef_list;
    Hit_Vars hitvars;

    u16 n_total_statedef;
    u16 n_total_statedef_size;
    u16 n_total_state;
    u16 n_total_state_size;
    u16 n_current_trigger;
    u16 n_trigger_list_size;
    u16 n_current_inst;
    u16 n_current_param_inst;
    u16 n_current_param;

    bool b_param;
};
typedef struct MU_State_Manager MU_State_Manager;

// ==========State Parser============================
struct Changestate
{
    Instruction *value;
    Instruction *ctrl;
    Instruction *anim;
};
typedef struct Changestate Changestate;

struct MU_State_Parser
{
    MU_Allocator *parser_allocator;
    int n_controller;
};
typedef struct MU_State_Parser MU_State_Parser;

// ==========Player==================================
struct PL_Data
{
    s16 n_life;
    s16 n_attack;
    s16 n_defence;
    s16 n_fall_defence_up;
    s16 n_liedown_time;
    s16 n_air_juggle;
    s16 n_sparkno;
    s16 n_guard_sparkno;
    bool b_ko_echo;
    s16 n_volumen;
    u8 n_int_persist_index;
    u8 n_float_persist_index;
};
typedef struct PL_Data PL_Data;

struct Player_Size
{
    float n_xscale;
    float n_yscale;
    s16 n_ground_back;
    s16 n_ground_front;
    s16 n_air_back;
    s16 n_air_front;
    s16 n_height;
    XY_Value n_attack_width;
    s16 n_attack_distance;
    s16 n_proj_attack_distance;
    bool b_proj_do_scale;
    XY_Value n_head_pos;
    XY_Value n_mid_pos;
    s16 n_shadow_offset;
    XY_Value n_draw_offset;
};
typedef struct Player_Size Player_Size;

struct Player_Velocity
{
    float n_walk_fwd;
    float n_walk_back;
    XY_Value run_fwd;
    XY_Value run_back;
    XY_Value jump_neu;
    XY_Value jump_back;
    XY_Value jump_fwd;
    XY_Value run_jump_back;
    XY_Value run_jump_fwd;
    XY_Value air_jump_neu;
    XY_Value air_jump_back;
    XY_Value air_jump_fwd;
};
typedef struct Player_Velocity Player_Velocity;

struct Player_Movement
{
    s16 air_jump_num;
    s16 n_air_jump_height;
    float yaccel;
    float stand_friction;
    float crouch_friction;
};
typedef struct Player_Movement Player_Movement;

struct Player_Const // Might need to change these to pointers
{
    PL_Data player_data;
    Player_Size player_size;
    Player_Velocity player_velocity;
    Player_Movement player_movement;
};
typedef struct Player_Const Player_Const;

struct Player_Vars
{
    int var[60];
    float fvar[40];
};
typedef struct Player_Vars Player_Vars;

// Apparently this works as a fix for calling a struct defined further below
typedef struct MU_Controller_Executer MU_Controller_Executer;
typedef struct VM VM;

struct Player
{
    MU_Allocator *player_allocator;
    MU_Video_System *video_system;
    MU_SFF_Manager *sff_manager;
    MU_Air_Manager *air_manager;
    MU_State_Manager *state_manager;
    MU_Timer *timer;
    VM *vm;
    MU_Controller_Executer *controller_exec;
    MU_CMD_Manager *cmd_manager;

    // Player information
    float x;
    float y;
    float xvel;
    float yvel;
    float n_ground;
    s32 n_anim_number;
    u32 n_state_time;
    u8 n_state_type;
    u8 n_physics;
    u8 n_movetype;
    u8 n_spr_prio;
    s16 n_life;
    s16 n_power;
    bool b_hitdef_persist;
    bool b_hitcount_persist;
    bool b_movehit_persist;
    bool b_ctrl;
    bool b_right_faced;
    bool b_debug_info;
    bool b_hitdef;
    bool b_alive;
    char str_command[50];
    PL_Statedef *lp_current_statedef;
    Player_Vars p_vars;
    Player_Const my_player_const; // Change to pointer?

    Keyboard_Data *kb;
};
typedef struct Player Player;

// ==========Controller Executer===================

// Function pointer typedef
// Function pointer must be type void, and take no arguments
// May need to change this to take an argument of type Controller_Executer
typedef void (*pt2_ctrl_member)();

struct MU_Controller_Executer
{
    Player *player;
    pt2_ctrl_member p_func_table[NUMCTRLFUNC];
};
// typedef can be found in the Player section

// ===========Stack================================
struct Stacktype
{
    float value;
    char stack_string[50];
};
typedef struct Stacktype Stacktype;

struct MU_Stack
{
    int n_pos;
    Stacktype stack_arr[STACKSIZE];
};
typedef struct MU_Stack MU_Stack;

// ==========VM====================================
// Function pointer typedef
// Function pointer must be type void, and take no arguments
// May need to change this to take an argument of type VM
typedef void (*pt2_member)(VM*);

struct VM
{
    Player *p1;
    Player *p2;
    MU_Stack *stack;

    int n_current_ins;
    Instruction *p_current_ins;
    Stacktype pop_item;
    float temp1, temp2, temp3;
    float n_save;

    // Pointer to player
    // Not sure why we need these???
    // TODO: check if these can be removed
    void *player1;
    void *player2;

    pt2_member p_func_table[NUMFUNCT];
};
// See player section for typedef

// ===========Engine===============================
struct MU_Engine
{
    // These weren't pointers in the original version
    Player *p1;
    Player *p2;
    Player *p3;
    Player *p4;

    MU_Allocator *mem_manager;
    MU_Video_System *video_system;
    MU_Timer *timer;
    VM *vm_p1;
    VM *vm_p2;
};
typedef struct MU_Engine MU_Engine;

// =========Game==================================
struct Game
{
    MU_Video_System *sdl_manager;
    MU_Allocator *mem_manager;
    MU_Timer *timer;
    MU_Engine *engine;

    bool b_error;
    bool b_game;
    float n_start_time, n_end_time;
    u16 n_game_type;
    float n_framerate;

    SDL_Event event;
};
typedef struct Game Game;

#endif