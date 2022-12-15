#ifndef __MUCMDMANAGER__
#define __MUCMDMANAGER__

Command_Key *command_key_init();
Command *command_init(int command_count);
Cmd_Frame_Input *command_frame_input_init(int key_buffer_size);
MU_CMD_Manager *cmd_manager_init(int key_buffer_size);

bool load_cmd_file(MU_CMD_Manager *cmd_manager, const char *file);
int adjust_key_index(MU_CMD_Manager *cmd_manager, int key_index, int increment);
void mu_cmd_update(MU_CMD_Manager *cmd_manager, Key *keys, bool facing_right); // TODO: rename keys to kb for consistency?

// These might not be needed, as they're simple getters and setters
const char *get_current_command_name();
void set_game_timer(MU_Timer *timer);

#endif