#ifndef __MUPLAYER__
#define __MUPLAYER__

Player *player_init();
void mu_set_player_pointers(Player *player, MU_Graphics_Manager *graphics_manager, MU_Allocator *allocator, MU_Timer *timer);
Player_Def *parse_player_def (const char *player_name);
bool load_player(Player *player, const char *player_name);
//bool check_state(PL_State *temp_state); // TODO: write this once FSM is implemented
//void handle_physics(Player *player); // TODO: write this once FSM is implemented
//void handle_fsm(); // TODO: write this once FSM is implemented
void update_facing(Player *player);
void mu_player_debug(Player *player);
//void player_change_state(Player *player, s32 n_state_number) // TODO: write this once FSM is implemented
void update_player(Player *player);
void draw_player(Player *player);
bool is_anim_available(Player *player, int anim);

#endif