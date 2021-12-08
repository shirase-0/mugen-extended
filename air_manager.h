#ifndef __MUAIRMANAGER__
#define __MUAIRMANAGER__

MU_Air_Manager *air_manager_init();

void reset_air_manager(MU_Air_Manager *air_manager);
void reallocate_action_block(MU_Air_Manager *air_manager);
void add_action(MU_Air_Manager *air_manager, s32 n_action_number);
void add_element(MU_Air_Manager *air_manager, s16 n_group_number, s16 n_image_number, s16 x, s16 y, s16 n_during_time, u16 n_flip, u32 n_colour_flag);
void set_loop(MU_Air_Manager *air_manager);
void add_clsn_box(MU_Air_Manager *air_manager, s16 x, s16 y, s16 w, s16 h, int n_number_of_clsn);
void create_clsn_box(MU_Air_Manager *air_manager, u16 n_number_of_clsn, bool b_clsn1, bool b_default);
void open_air(MU_Air_Manager *air_manager, char *str_filename);
Action_Element *get_action(MU_Air_Manager *air_manager, s32 n_action_number);
void mu_cleanup_air_manager();

#endif