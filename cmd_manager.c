#include "mu_headers.h"

Command_Key *command_key_init()
{
	Command_Key *command_key = (Command_Key*) malloc(sizeof(Command_Key));
	command_key->keycode = 0;
	command_key->key_modifier = 0;
	command_key->game_ticks_for_hold = 0;

	return command_key;
}

// TODO: rename this to command_list_init for more clarity on what this function actually does?
Command *command_init(int command_count)
{
	Command *command = (Command*) calloc(command_count, sizeof(Command));

	return command;
}

Cmd_Frame_Input *command_frame_input_init(int key_buffer_size)
{
	Cmd_Frame_Input *cmd_frame_input = (Cmd_Frame_Input*) calloc(key_buffer_size, sizeof(Cmd_Frame_Input));

	// there were some other initialisations here in the previous version, but calloc *should* take care of all of them

	return cmd_frame_input;
}

MU_CMD_Manager *cmd_manager_init(int key_buffer_size)
{
	MU_CMD_Manager *cmd_manager = (MU_CMD_Manager*) malloc(sizeof(MU_CMD_Manager));
	cmd_manager->commands = NULL;
	cmd_manager->command_count = 0;
	cmd_manager->current_command_name = NULL;

	cmd_manager->key_buffer = command_frame_input_init(key_buffer_size);
	cmd_manager->key_buffer_size = key_buffer_size;
	cmd_manager->key_index = 0;

	return cmd_manager;
}

bool load_cmd_file(MU_CMD_Manager *cmd_manager, const char *file)
{
	int default_command_time = 15;
	int default_buffer_time = 1;

	cmd_manager->command_count = 0;

	Tokenizer *tok = tokenizer_init(100, NULL, NULL, 23);

	if(!mu_open_file(tok, file))
	{
		debug_print("CMD Manager: Failed to open %s", file);
		return false;
	}

	// Get count first to set up memory
	while(!tok->at_eof)
	{
		bool found_something = false;
		if(check_token(tok, "command.time", true))
		{
			found_something = true;
			if(!check_token(tok, "=", true))
			{
				debug_print("CMD Manager: syntax error on line %d, expected =, will try to continue.", tok->cur_file_line);
			}
			if(is_token_number(tok))
			{
				default_command_time = mu_get_int(tok);
			}
		}

		if(check_token(tok, "command.buffer.time", true))
		{
			found_something = true;
			if(!check_token(tok, "=", true))
			{

			}
			if(is_token_number(tok))
			{
				default_buffer_time = mu_get_int(tok);
			}
		}

		if(check_token(tok, "[", true))
		{
			found_something = true;
			if(check_token(tok, "Command", true))
			{
				if(check_token(tok, "]", true))
				{
					cmd_manager->command_count++;
				}
			}
		}

		if(!found_something)
		{
			get_token(tok);
		}
	}

	mu_close_file(tok);

	// TODO: do I actually need to close and reopen the file? Can't I just rewind to the start?

	if(!mu_open_file(tok, file))
	{
		debug_print("CMD Manager: Failed to open %s", file);
		return false;
	}

	cmd_manager->commands = command_init(cmd_manager->command_count);
	Command *commands = cmd_manager->commands;

	while(!tok->at_eof)
	{
		bool found_command = false;
		if(check_token(tok, "[", true))
		{
			if(check_token(tok, "Command", true))
			{
				if(!check_token(tok, "]", true))
				{
					debug_print("CMD Manager: syntax error on line %d, expected ], will try to continue.", tok->cur_file_line);
				}

				found_command = true;
				commands->command_time = default_command_time;
				commands->buffer_time = default_buffer_time;
				commands->str_command[0] = 0; // CHECK: does this cause a memory leak?
				commands->cmd_keys_count = 0;

				while(commands->cmd_keys_count < MAX_COMMANDS &&
					  !check_token(tok, "[", false) &&
					  !tok->at_eof)
				{
					if(check_token(tok, "name", true))
					{
						if(!check_token(tok, "=", true))
						{
							debug_print("CMD Manager: syntax error on line %d, expected =, will try to continue.", tok->cur_file_line);
						}

						strcpy(commands->str_command, get_token(tok));
					}
					else if(check_token(tok, "command", true))
					{
						if(!check_token(tok, "=", true))
						{
							debug_print("CMD Manager: syntax error on line %d, expected =, will try to continue.", tok->cur_file_line);
						}

						while(!tok->at_end_of_line)
						{
							const char *token = get_token(tok);

							if(!strcmp(token, "~"))
							{
								commands->cmd_keys[commands->cmd_keys_count].key_modifier += KEYMOD_ON_RELEASE;
								if(is_token_number(tok))
								{
									commands->cmd_keys[commands->cmd_keys_count].game_ticks_for_hold = mu_get_int(tok);
								}
								else
								{
									commands->cmd_keys[commands->cmd_keys_count].game_ticks_for_hold = 1;
								}
							}
							else if(!strcmp(token, "+"))
							{
								// is this check needed if we're not doing anything with it anyway?
							}
							else if(!strcmp(token, "/"))
							{
								commands->cmd_keys[commands->cmd_keys_count].key_modifier += KEYMOD_MUST_BE_HELD;
							}
							else if(!strcmp(token, "$"))
							{
								commands->cmd_keys[commands->cmd_keys_count].key_modifier += KEYMOD_DETECT_AS_4WAY;
							}
							else if(!strcmp(token, ">"))
							{
								commands->cmd_keys[commands->cmd_keys_count].key_modifier += KEYMOD_BAN_OTHER_INPUT;
							}
							else if(!strcmp(token, ","))
							{
								commands->cmd_keys_count++;
							}
							else if(!strcmp(token, "D"))
							{
								commands->cmd_keys[commands->cmd_keys_count].keycode += KEYCODE(KEY_DOWN);
							}
							else if(!strcmp(token, "U"))
							{
								commands->cmd_keys[commands->cmd_keys_count].keycode += KEYCODE(KEY_UP);
							}
							else if(!strcmp(token, "B"))
							{
								commands->cmd_keys[commands->cmd_keys_count].keycode += KEYCODE(KEY_LEFT);
							}
							else if(!strcmp(token, "F"))
							{
								commands->cmd_keys[commands->cmd_keys_count].keycode += KEYCODE(KEY_RIGHT);
							}
							else if(!strcmp(token, "DB"))
							{
								commands->cmd_keys[commands->cmd_keys_count].keycode += KEYCODE(KEY_DOWN) + KEYCODE(KEY_LEFT);
							}
							else if(!strcmp(token, "DF"))
							{
								commands->cmd_keys[commands->cmd_keys_count].keycode += KEYCODE(KEY_DOWN) + KEYCODE(KEY_RIGHT);
							}
							else if(!strcmp(token, "UB"))
							{
								commands->cmd_keys[commands->cmd_keys_count].keycode += KEYCODE(KEY_UP) + KEYCODE(KEY_LEFT);
							}
							else if(!strcmp(token, "UF"))
							{
								commands->cmd_keys[commands->cmd_keys_count].keycode += KEYCODE(KEY_UP) + KEYCODE(KEY_RIGHT);
							}
							else if(!strcmp(token, "a"))
							{
								commands->cmd_keys[commands->cmd_keys_count].keycode += KEYCODE(KEY_BUTTON_A);
							}
							else if(!strcmp(token, "b"))
							{
								commands->cmd_keys[commands->cmd_keys_count].keycode += KEYCODE(KEY_BUTTON_B);
							}
							else if(!strcmp(token, "c"))
							{
								commands->cmd_keys[commands->cmd_keys_count].keycode += KEYCODE(KEY_BUTTON_C);
							}
							else if(!strcmp(token, "x"))
							{
								commands->cmd_keys[commands->cmd_keys_count].keycode += KEYCODE(KEY_BUTTON_X);
							}
							else if(!strcmp(token, "y"))
							{
								commands->cmd_keys[commands->cmd_keys_count].keycode += KEYCODE(KEY_BUTTON_Y);
							}
							else if(!strcmp(token, "z"))
							{
								commands->cmd_keys[commands->cmd_keys_count].keycode += KEYCODE(KEY_BUTTON_Z);
							}
							else if(!strcmp(token, "s"))
							{
								commands->cmd_keys[commands->cmd_keys_count].keycode += KEYCODE(KEY_BUTTON_START);
							}
						}
						commands->cmd_keys_count++;
					}
					else if(check_token(tok, "time", true))
					{
						if(!check_token(tok, "=", true))
						{
							debug_print("CMD Manager: syntax error on line %d, expected =, will try to continue.", tok->cur_file_line);
						}

						if(!is_token_number(tok))
						{
							debug_print("CMD Manager: syntax error on line %d, expected a number, will try to continue.", tok->cur_file_line);
						}
						commands->command_time = mu_get_int(tok);
					}
					else if(check_token(tok, "buffer.time", true))
					{
						if(!check_token(tok, "=", true))
						{
							debug_print("CMD Manager: syntax error on line %d, expected =, will try to continue.", tok->cur_file_line);
						}

						if(!is_token_number(tok))
						{
							debug_print("CMD Manager: syntax error on line %d, expected a number, will try to continue.", tok->cur_file_line);
						}
						commands->buffer_time = mu_get_int(tok);
					}
				}
				commands++; // Pointer arithmetic?
			}
		}
		if(!found_command)
		{
			// If no command is found, skip
			get_token(tok);
		}
	}
	mu_close_file(tok);
	free_tokenizer(tok);
	return true;
}

int adjust_key_index(MU_CMD_Manager *cmd_manager, int key_index, int increment)
{
	return((key_index + increment + cmd_manager->key_buffer_size) % cmd_manager->key_buffer_size);
}

void mu_cmd_update(MU_CMD_Manager *cmd_manager, Key *keys, bool facing_right) // TODO: rename keys to kb for consistency?
{
	cmd_manager->current_command_name = NULL;
	// If this causes issues, revert all references from key_buffer to cmd_manager->key_buffer
	Cmd_Frame_Input *key_buffer = cmd_manager->key_buffer;
	key_buffer[cmd_manager->key_index].key_bitfield = 0;
	key_buffer[cmd_manager->key_index].game_ticks = get_game_time(cmd_manager->cmd_timer);

	// Add current keystrokes to input buffer
	for(int k = 0; k < KEY_COUNT; k++)
	{
		if(keys[k].is_pressed == PRESSED)
		{
			if((k == KEY_LEFT) && !facing_right)
			{
				key_buffer[cmd_manager->key_index].key_bitfield += KEYCODE(KEY_RIGHT); // CHECK: Shouldn't this be KEY_LEFT?
			}
			else if((k == KEY_RIGHT) && !facing_right)
			{
				key_buffer[cmd_manager->key_index].key_bitfield += KEYCODE(KEY_LEFT); // CHECK: Shouldn't this be KEY_RIGHT?
			}
			else
			{
				key_buffer[cmd_manager->key_index].key_bitfield += KEYCODE(k); // Does this actually work?
			}
		}
	}

	Command *current_command = cmd_manager->commands;

	for(int a = 0; a < cmd_manager->command_count; a++)
	{
		// TODO: name these variables more appropriately?
		int time = -1;
		int last_time = -1;
		int current_key_index = 0;

		// For every command in the CMD file
		for(int b = current_command->cmd_keys_count - 1; b >= 0; b--)
		{
			bool is_command = false;
			bool on_release = ((current_command->cmd_keys[b].key_modifier & KEYMOD_ON_RELEASE) != 0);
			bool on_hold = ((current_command->cmd_keys[b].key_modifier & KEYMOD_MUST_BE_HELD) != 0);
			bool use_4way = ((current_command->cmd_keys[b].key_modifier & KEYMOD_DETECT_AS_4WAY) != 0);
			// Currently unused, uncomment this when necessary
			//bool ban_other_input = ((current_command->cmd_keys[b].key_modifier & KEYMOD_BAN_OTHER_INPUT) != 0);
			int game_ticks_to_hold = current_command->cmd_keys[b].game_ticks_for_hold;
			int keycode = current_command->cmd_keys[b].keycode;

			// TODO: define current_key_index in this for loop?
			// There should be no adverse effects to doing so
			// Implement this once CMD Manager has been shown to work
			for( ; current_key_index < cmd_manager->key_buffer_size; current_key_index++)
			{
				// CHECK: make sure this line works properly
				Cmd_Frame_Input *frame_input = &cmd_manager->key_buffer[adjust_key_index(cmd_manager, cmd_manager->key_index, -current_key_index)];
				bool key_down = ((frame_input->key_bitfield & keycode) == keycode);

				if(key_down && !use_4way)
				{
					int keycode_dirs = (keycode & ALL_DIRECTIONS_BITFIELD);
					int frame_input_dirs = (frame_input->key_bitfield & ALL_DIRECTIONS_BITFIELD);
					key_down = !keycode_dirs || (keycode_dirs == frame_input_dirs);
				}

				bool button_conditions_met = false;

				// See how long the button has been held
				if(on_release != key_down)
				{
					int game_ticks_held = 0;
					for(int k = current_key_index + 1; k < cmd_manager->key_buffer_size; k++)
					{
						Cmd_Frame_Input *frame_input2 = &cmd_manager->key_buffer[adjust_key_index(cmd_manager, cmd_manager->key_index, -k)];
						bool key_down2 = ((frame_input2->key_bitfield & keycode) == keycode);

						if(key_down2 && !use_4way)
						{
							int keycode_dirs = (keycode & ALL_DIRECTIONS_BITFIELD);
							int frame_input_dirs = (frame_input2->key_bitfield & ALL_DIRECTIONS_BITFIELD);
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
								button_conditions_met = (b < current_command->cmd_keys_count - 1);
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
						time = frame_input->game_ticks;
					}

					if(b == (current_command->cmd_keys_count - 1))
					{
						last_time = frame_input->game_ticks;
					}
					is_command = true;
					current_key_index++;
					break;
				}
			}
			if(!is_command)
			{
				break;
			}
		}

		if((time >= 0) && (last_time > 0))
		{
			// The last button of the sequence must be pressed in the current game tick to
			// be valid, and then it must be checked for how long it has taken to do the input
			//int gameTicks = GetGameTicks();
			// TODO

			if((last_time >= (get_game_time(cmd_manager->cmd_timer) - current_command->buffer_time)) &&
			   (last_time - time) <= current_command->command_time)
			{
				cmd_manager->current_command_name = current_command->str_command;
				debug_print("Current command name: %s", cmd_manager->current_command_name);
				break;
			}
		}
		current_command++; // Pointer arithmetic?
	}

	if(++cmd_manager->key_index >= cmd_manager->key_buffer_size)
	{
		cmd_manager->key_index = 0;
	}
}