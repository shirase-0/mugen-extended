#ifndef __MUPLAYER__
#define __MUPLAYER__

Player *player_init();
void free_player(Player *player);

void mu_set_player_pointers(Player *player, MU_Video_System *video_system, MU_Allocator *allocator, MU_Timer *timer);
bool load_player(Player *player, const char *str_player);

bool check_state(PL_State *temp_state); // See player.c
void execute_controller(PL_State *temp_state); // Empty function in original version
void handle_physics(Player *player);
void handle_fsm(); // Original code is commented out
void update_facing(Player *player);
void mu_player_debug(Player *player);

// Player controllers
void player_change_state(Player *player, s32 n_state_number);

void update_player(Player *player);
void draw_player(Player *player);

bool is_anim_available(Player *player, s32 n_anim);

// These may not be necessary, as they're simple getters/setters
//void set_vm(MU_Virtual_Machine *vm);
void set_pos(s16 x_axis, s16 y_axis);
void velset_x(float xvel);
void velset_y(float yvel);
void set_debug(bool b_debug);
void face_left();
void face_right();

// Set the ground value of the stage
void set_ground_value(float y_limit);

// Trigger functions
char *get_command();
bool is_player_alive();
s32 get_anim();
Action_Element *get_current_anim();
bool is_ctrl();

#endif