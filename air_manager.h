#ifndef __MUAIRMANAGER__
#define __MUAIRMANAGER__

MU_Air_Manager *air_manager_init();
void action_list_init(MU_Air_Manager *air_manager);
void reset_air_manager(MU_Air_Manager *air_manager);
void mu_free_air_manager(MU_Air_Manager *air_manager);

void reallocate_action_block(MU_Air_Manager *air_manager);
void add_action(MU_Air_Manager *air_manager, int action_num);
void add_element(MU_Air_Manager *air_manager, int16_t values[5], uint16_t flip_flags, uint32_t colour_flags);
void set_loop(MU_Air_Manager *air_manager);
void add_clsn_box(MU_Air_Manager *air_manager, int16_t coor[4], int clsn_count);
void create_clsn_box(MU_Air_Manager *air_manager, uint16_t clsn_count, bool is_clsn1, bool is_default_clsn);
void open_air(MU_Air_Manager *air_manager, char *filename);
Action *get_action(MU_Air_Manager *air_manager, int action_num);

#endif