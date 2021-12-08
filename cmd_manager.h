#ifndef __MUCMDMANAGER__
#define __MUCMDMANAGER__

PL_Command_Element *pl_command_element_init();
PL_Command *pl_command_init(int command_count);
PL_Command_Frame_Input *pl_command_frame_input_init(int key_buffer_size);
MU_CMD_Manager *cmd_manager_init(int key_buffer_size);

bool load_cmd_file(MU_CMD_Manager *cmd_manager, const char *file);
int adjust_key_index(MU_CMD_Manager *cmd_manager, int key_index, int increment);
void mu_cmd_update(MU_CMD_Manager *cmd_manager, Keyboard_Data *keys, bool facing_right);

// These might not be needed, as they're simple getters and setters
const char *get_current_command_name();
void set_game_timer(MU_Timer *timer);

#endif