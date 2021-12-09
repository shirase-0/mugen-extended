#include "mu_headers.h"

PL_Command_Element *pl_command_element_init()
{
	PL_Command_Element *pl_command_element = (PL_Command_Element*) malloc(sizeof(PL_Command_Element));
	pl_command_element->keycode = 0;
	pl_command_element->key_modifier = 0;
	pl_command_element->game_ticks_for_hold = 0;

	return pl_command_element;
}

PL_Command *pl_command_init(int command_count)
{
	PL_Command *pl_command = (PL_Command*) malloc(sizeof(PL_Command) * command_count);
	memset(pl_command, 0, sizeof(PL_Command) * command_count);

	return pl_command;
}

PL_Cmd_Frame_Input *pl_command_frame_input_init(int key_buffer_size)
{
	PL_Cmd_Frame_Input *pl_cmd_frame_input = (PL_Cmd_Frame_Input*) malloc(sizeof(PL_Cmd_Frame_Input) * key_buffer_size);
	memset(pl_cmd_frame_input, 0, sizeof(PL_Cmd_Frame_Input) * key_buffer_size);
	pl_cmd_frame_input->key_bitfield = 0;
	pl_cmd_frame_input->game_ticks = 0;

	return pl_cmd_frame_input;
}

MU_CMD_Manager *cmd_manager_init(int key_buffer_size)
{
	MU_CMD_Manager *cmd_manager = (MU_CMD_Manager*) malloc(sizeof(MU_CMD_Manager));
	cmd_manager->commands = NULL;
	cmd_manager->command_count = 0;
	cmd_manager->current_command_name = NULL;

	cmd_manager->key_buffer = pl_command_frame_input_init(key_buffer_size);
	cmd_manager->key_buffer_size = key_buffer_size; // This has a default of 120 in the original version
	cmd_manager->key_index = 0;

	return cmd_manager;
}

// The commands (buttons) a player needs to press to execute moves are stored in CMD files
// This function loads the CMD file passed to it into the cmd manager
bool load_cmd_file(MU_CMD_Manager *cmd_manager, const char *file)
{
	int default_command_time = 15;
	int default_buffer_time = 1;

	cmd_manager->command_count = 0;

	Tokenizer *tokenizer = tokenizer_init(100, NULL, NULL, 23);

	if(!mu_open_file(tokenizer, file))
	{
		mu_log_message("CMD Manager: Failed to open %s", file);
		return false;
	}

	// Get count first to set up memory
	while(!tokenizer->at_end_of_file)
	{
		bool found_something = false;
		if(check_token_consume(tokenizer, "command.time"))
		{
			found_something = true;
			if(!check_token_consume(tokenizer, "="))
			{

			}
			if(check_token_is_number(tokenizer))
			{
				default_command_time = mu_get_int(tokenizer);
			}
		}

		if(check_token_consume(tokenizer, "command.buffer.time"))
		{
			found_something = true;
			if(!check_token_consume(tokenizer, "="))
			{

			}
			if(check_token_is_number(tokenizer))
			{
				default_buffer_time = mu_get_int(tokenizer);
			}
		}

		if(check_token_consume(tokenizer, "["))
		{
			found_something = true;
			if(check_token_consume(tokenizer, "Command"))
			{
				if(check_token_consume(tokenizer, "]"))
				{
					cmd_manager->command_count++;
				}
			}
		}

		if(!found_something)
		{
			get_token(tokenizer);
		}
	}

	mu_close_file(tokenizer);

	if(!mu_open_file(tokenizer, file))
	{
		return false;
	}

	cmd_manager->commands = pl_command_init(cmd_manager->command_count);
	PL_Command *command = cmd_manager->commands;

	while(!tokenizer->at_end_of_file)
	{
		bool found_command = false;
		if(check_token_consume(tokenizer, "["))
		{
			if(check_token_consume(tokenizer, "Command"))
			{
				if(!check_token_consume(tokenizer, "]"))
				{

				}

				found_command = true;
				command->n_command_time = default_command_time;
				command->n_buffer_time = default_buffer_time;
				command->str_command[0] = 0;
				command->n_how_many_command = 0;

				while(command->n_how_many_command < MAX_COMMAND && 
					!check_token_no_consume(tokenizer, "[") && !tokenizer->at_end_of_file)
				{
					if(check_token_consume(tokenizer, "name"))
					{
						if(!check_token_consume(tokenizer, "="))
						{

						}

						strcpy(command->str_command, get_token(tokenizer));
					}
					else if(check_token_consume(tokenizer, "command"))
					{
						if(!check_token_consume(tokenizer, "="))
						{

						}

						// This uses macros defined in structs.h to create a bitfield for any given button input
						// This includes the difference between holding a button and simply pressing it, as well
						// as pressing multiple buttons simultaneously
						while(!tokenizer->at_end_of_line)
						{
							const char *token = get_token(tokenizer);

							if(!strcmp(token, "~"))
							{
								command->n_command[command->n_how_many_command].key_modifier += PLC_KEYMOD_ON_RELEASE;
								if(check_token_is_number(tokenizer))
								{
									command->n_command[command->n_how_many_command]. game_ticks_for_hold = mu_get_int(tokenizer);
								}
								else
								{
									command->n_command[command->n_how_many_command].game_ticks_for_hold = 1;
								}
							}
							else if(!strcmp(token, "+"))
							{

							}
							else if(!strcmp(token, "/"))
							{
								command->n_command[command->n_how_many_command].key_modifier += PLC_KEYMOD_MUST_BE_HELD;
							}
							else if(!strcmp(token, "$"))
							{
								command->n_command[command->n_how_many_command].key_modifier += PLC_KEYMOD_DETECT_AS_4WAY;
							}
							else if(!strcmp(token, ">"))
							{
								command->n_command[command->n_how_many_command].key_modifier += PLC_KEYMOD_BAN_OTHER_INPUT;
							}
							else if(!strcmp(token, ","))
							{
								command->n_how_many_command++;
							}
							else if(!strcmp(token, "D"))
							{
								command->n_command[command->n_how_many_command].keycode += PLC_KEYCODE(KEY_DOWN);
							}
							else if(!strcmp(token, "U"))
							{
								command->n_command[command->n_how_many_command].keycode += PLC_KEYCODE(KEY_UP);
							}
							else if(!strcmp(token, "B"))
							{
								command->n_command[command->n_how_many_command].keycode += PLC_KEYCODE(KEY_LEFT);
							}
							else if(!strcmp(token, "F"))
							{
								command->n_command[command->n_how_many_command].keycode += PLC_KEYCODE(KEY_RIGHT);
							}
							else if(!strcmp(token, "DB"))
							{
								command->n_command[command->n_how_many_command].keycode += PLC_KEYCODE(KEY_DOWN) + PLC_KEYCODE(KEY_LEFT);
							}
							else if(!strcmp(token, "DF"))
							{
								command->n_command[command->n_how_many_command].keycode += PLC_KEYCODE(KEY_DOWN) + PLC_KEYCODE(KEY_RIGHT);
							}
							else if(!strcmp(token, "UB"))
							{
								command->n_command[command->n_how_many_command].keycode += PLC_KEYCODE(KEY_UP) + PLC_KEYCODE(KEY_LEFT);
							}
							else if(!strcmp(token, "UF"))
							{
								command->n_command[command->n_how_many_command].keycode += PLC_KEYCODE(KEY_UP) + PLC_KEYCODE(KEY_RIGHT); // This was set to left in the original version, but I'm pretty sure that was a typo
							}
							else if(!strcmp(token, "a"))
							{
								command->n_command[command->n_how_many_command].keycode += PLC_KEYCODE(KEY_BUTTON_A);
							}
							else if(!strcmp(token, "b"))
							{
								command->n_command[command->n_how_many_command].keycode += PLC_KEYCODE(KEY_BUTTON_B);
							}
							else if(!strcmp(token, "c"))
							{
								command->n_command[command->n_how_many_command].keycode += PLC_KEYCODE(KEY_BUTTON_C);
							}
							else if(!strcmp(token, "x"))
							{
								command->n_command[command->n_how_many_command].keycode += PLC_KEYCODE(KEY_BUTTON_X);
							}
							else if(!strcmp(token, "y"))
							{
								command->n_command[command->n_how_many_command].keycode += PLC_KEYCODE(KEY_BUTTON_Y);
							}
							else if(!strcmp(token, "z"))
							{
								command->n_command[command->n_how_many_command].keycode += PLC_KEYCODE(KEY_BUTTON_Z);
							}
							else if(!strcmp(token, "s"))
							{
								command->n_command[command->n_how_many_command].keycode += PLC_KEYCODE(KEY_BUTTON_START);
							}
						}
						command->n_how_many_command++;
					}
					else if(check_token_consume(tokenizer, "time"))
					{
						if(!check_token_consume(tokenizer, "="))
						{

						}

						if(!check_token_is_number(tokenizer))
						{

						}
						command->n_command_time = mu_get_int(tokenizer);
					}
					else if(check_token_consume(tokenizer, "buffer.time"))
					{
						if(!check_token_consume(tokenizer, "="))
						{

						}

						if(!check_token_is_number(tokenizer))
						{

						}

						command->n_buffer_time = mu_get_int(tokenizer);
					}
				}
				command++;
			}
		}
		if(!found_command)
		{
			// If no command is found, skip
			get_token(tokenizer);
		}
	}
	mu_close_file(tokenizer);
	free_tokenizer(tokenizer);
	return true;
}

int adjust_key_index(MU_CMD_Manager *cmd_manager, int key_index, int increment)
{
	return((key_index + increment + cmd_manager->key_buffer_size) % cmd_manager->key_buffer_size);
}

void mu_cmd_update(MU_CMD_Manager *cmd_manager, Keyboard_Data *keys, bool facing_right)
{
	cmd_manager->current_command_name = NULL;
	cmd_manager->key_buffer[cmd_manager->key_index].key_bitfield = 0;

	cmd_manager->key_buffer[cmd_manager->key_index].game_ticks = get_game_time(cmd_manager->cmd_timer);

	// Add current keystrokes to input buffer using macros defined in structs.h
	for(int k = 0; k < KEY_COUNT; k++)
	{
		if(keys->key_info[k].is_pressed)
		{
			if((k == KEY_LEFT) && !facing_right)
			{
				cmd_manager->key_buffer[cmd_manager->key_index].key_bitfield += PLC_KEYCODE(KEY_RIGHT);
			}
			else if((k == KEY_RIGHT) && !facing_right)
			{
				cmd_manager->key_buffer[cmd_manager->key_index].key_bitfield += PLC_KEYCODE(KEY_LEFT);
			}
			else
			{
				cmd_manager->key_buffer[cmd_manager->key_index].key_bitfield += PLC_KEYCODE(k);
			}
		}
	}

	PL_Command *current_command = cmd_manager->commands;

	for(int a = 0; a < cmd_manager->command_count; a++)
	{
		int n_time = -1;
		int n_last_time = -1;
		int current_key_index = 0;

		// For every command in the CMD file
		for(int b = current_command->n_how_many_command - 1; b >= 0; b--)
		{
			bool b_command = false;
			bool on_release = ((current_command->n_command[b].key_modifier & PLC_KEYMOD_ON_RELEASE) != 0);
			bool on_hold = ((current_command->n_command[b].key_modifier & PLC_KEYMOD_MUST_BE_HELD) != 0);
			bool use_4way = ((current_command->n_command[b].key_modifier & PLC_KEYMOD_DETECT_AS_4WAY) != 0);
			// Currently unused, uncomment this when necessary
			//bool ban_other_input = ((current_command->n_command[b].key_modifier & PLC_KEYMOD_BAN_OTHER_INPUT) != 0);
			int game_ticks_to_hold = current_command->n_command[b].game_ticks_for_hold;
			int keycode = current_command->n_command[b].keycode;

			// current_key_index is defined above, maybe it's not needed in the first part of the for statement?
			// This is how it was in the original version
			for( ; current_key_index < cmd_manager->key_buffer_size; current_key_index++) 
			{
				PL_Cmd_Frame_Input *frame_input = &cmd_manager->key_buffer[adjust_key_index(cmd_manager, cmd_manager->key_index, -current_key_index)];
				bool key_down = ((frame_input->key_bitfield & keycode) == keycode);

				if(key_down && !use_4way)
				{
					int keycode_dirs = (keycode & PLC_ALL_DIRECTIONS_BITFIELD);
					int frame_input_dirs = (frame_input->key_bitfield & PLC_ALL_DIRECTIONS_BITFIELD);
					key_down = !keycode_dirs || (keycode_dirs == frame_input_dirs);
				}

				bool button_conditions_met = false;

				// See how long it's been held
				if(on_release != key_down)
				{
					int game_ticks_held = 0;
					for(int k = current_key_index + 1; k < cmd_manager->key_buffer_size; k++)
					{
						PL_Cmd_Frame_Input *frame_input2 = &cmd_manager->key_buffer[adjust_key_index(cmd_manager, cmd_manager->key_index, -k)];
						bool key_down2 = ((frame_input2->key_bitfield & keycode) == keycode);

						if(key_down2 && !use_4way)
						{
							int keycode_dirs = (keycode & PLC_ALL_DIRECTIONS_BITFIELD);
							int frame_input_dirs = (frame_input2->key_bitfield & PLC_ALL_DIRECTIONS_BITFIELD);
							key_down2 = !keycode_dirs || (keycode_dirs == frame_input_dirs);
						}
						if(key_down2)
						{
							game_ticks_held++;
							if(on_hold)
							{
								button_conditions_met = key_down;
								break;
							}
							else if(on_release)
							{
								if(game_ticks_held >= game_ticks_to_hold)
								{
									button_conditions_met = true;
									break;
								}
							}
							else
							{
								button_conditions_met = (b < current_command->n_how_many_command - 1);
								break;
							}
						}
						else
						{
							button_conditions_met = !(on_hold || on_release);
							break;
						}
					}
				}

				if(button_conditions_met)
				{
					// If it's the first element, store the time of it
					if(b == 0)
					{
						n_time = frame_input->game_ticks;
					}

					if(b == (current_command->n_how_many_command - 1))
					{
						n_last_time = frame_input->game_ticks;
					}
					b_command = true;
					current_key_index++;
					break;
				}
			}
			if(!b_command)
			{
				break;
			}
		}

		if((n_time >= 0) && (n_last_time > 0))
		{
			// The last button of the sequence must be pressed in the current game tick to
			// be valid, and then it must be checked for how long it has taken to do the input
			//int gameTicks = GetGameTicks();
			// TODO

			if((n_last_time >= (get_game_time(cmd_manager->cmd_timer) - current_command->n_buffer_time)) && 
				(n_last_time - n_time) <= current_command->n_command_time)
			{
				cmd_manager->current_command_name = current_command->str_command;
				mu_log_message("Current command name: %s", cmd_manager->current_command_name);
				break;
			}
		}
		current_command++;
	}

	if(++cmd_manager->key_index >= cmd_manager->key_buffer_size)
	{
		cmd_manager->key_index = 0;
	}
}