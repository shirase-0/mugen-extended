#include "mu_headers.h"

MU_Air_Manager *air_manager_init()
{
	MU_Air_Manager *air_manager = (MU_Air_Manager*) malloc(sizeof(MU_Air_Manager));
	return air_manager;
}

void action_list_init(MU_Air_Manager *air_manager)
{
	MU_Allocator *air_allocator = air_manager->air_allocator;
	uint16_t action_list_size = air_manager->action_list_size;

	air_manager->action_list = (Action*) mu_alloc(air_allocator, sizeof(Action) * action_list_size);
}

void reset_air_manager(MU_Air_Manager *air_manager)
{
	air_manager->total_action_block = 0;
	air_manager->action_list_size = 100;
	air_manager->element_list_size = 100;
	air_manager->total_clsn = 0;
	air_manager->is_default_clsn = false;
	air_manager->action_list = NULL;
	action_list_init(air_manager);
}

void mu_free_air_manager(MU_Air_Manager *air_manager)
{
	debug_print("TODO: Free air_manager memory");
}

void reallocate_action_block(MU_Air_Manager *air_manager)
{
	uint16_t total_action_block = air_manager->total_action_block;

	if(total_action_block > 0)
	{
		Action *action_list = air_manager->action_list;
		Action *action = &(action_list[total_action_block - 1]);
		Element *anim_element = action->anim_element;
		MU_Allocator *air_allocator = air_manager->air_allocator;
		uint16_t elements_count = action->elements_count;
		if(air_allocator->lp_memlist[mu_find_address(air_allocator, anim_element)].size <= sizeof(Element) * elements_count)
		{
			air_manager->action_list[total_action_block - 1].anim_element = (Element*) mu_realloc(air_allocator, anim_element, sizeof(Element) * elements_count);	
		}
	}
}

void add_action(MU_Air_Manager *air_manager, int action_num)
{
	reallocate_action_block(air_manager);
	air_manager->anim_time = 0;
	air_manager->total_elements = 0;
	air_manager->element_list_size = 100;
	air_manager->is_default_clsn = false;

	uint16_t total_action_block = air_manager->total_action_block;
	uint16_t action_list_size = air_manager->action_list_size;
	Action *action_list = air_manager->action_list;
	MU_Allocator *air_allocator = air_manager->air_allocator;

	if(total_action_block > action_list_size - 1)
	{
		air_manager->action_list_size += 100;
		air_manager->action_list = (Action*) mu_realloc(air_allocator, action_list, sizeof(Action) * air_manager->action_list_size);
		action_list = air_manager->action_list; // mu_realloc returns a different address, so we need to update this value
	}

	Action *action_list_last_elem = &action_list[total_action_block];
	action_list_last_elem->loop_start = -1;
	action_list_last_elem->action_num = action_num;
	uint16_t element_list_size = air_manager->element_list_size;

	action_list_last_elem->anim_element = (Element*) mu_alloc(air_allocator, sizeof(Element) * element_list_size);
	air_manager->total_action_block++;
}

void add_element(MU_Air_Manager *air_manager, int16_t values[5], uint16_t flip_flags, uint32_t colour_flags)
{
	// TODO: Handle ColorFlags
    // 32bit
    // 8Bit ADDOP 8bit ADDOPVALUE 8bit SUBOP 8Bit SUBOPVALUE
	Action *action_list = air_manager->action_list;
	uint16_t total_action_block = air_manager->total_action_block;
	MU_Allocator *air_allocator = air_manager->air_allocator;
	uint16_t element_list_size = air_manager->element_list_size;
	uint16_t total_elements = air_manager->total_elements;
	uint16_t total_clsn = air_manager->total_clsn;

	Element *curr_anim_element = action_list[total_action_block - 1].anim_element;

	if(total_elements > element_list_size - 1)
	{
		air_manager->element_list_size += 100;
		element_list_size = air_manager->element_list_size;
		action_list[total_action_block - 1].anim_element = (Element*) mu_realloc(air_allocator, curr_anim_element, sizeof(Element) * element_list_size);
		curr_anim_element = action_list[total_action_block - 1].anim_element;
	}

	curr_anim_element[total_elements].group_num = values[0];
	curr_anim_element[total_elements].image_num = values[1];
	curr_anim_element[total_elements].x = values[2];
	curr_anim_element[total_elements].y = values[3];
	curr_anim_element[total_elements].during_time = values[4];
	curr_anim_element[total_elements].flip_flags = flip_flags;
	curr_anim_element[total_elements].colour_flags = colour_flags;
	curr_anim_element[total_elements].clsn_count = total_clsn;
	curr_anim_element[total_elements].clsn_data = (Clsn*) mu_alloc(air_allocator, sizeof(Clsn) * total_clsn);
	memcpy(curr_anim_element[total_elements].clsn_data, air_manager->clsn_arr, sizeof(Clsn) * total_clsn);
	action_list[total_action_block - 1].complete_anim_time += values[4];

	if(!air_manager->is_default_clsn)
	{
		air_manager->total_clsn = 0;
	}

	air_manager->total_elements++;
	action_list[total_action_block - 1].elements_count = air_manager->total_elements;
}

void set_loop(MU_Air_Manager *air_manager)
{
	Action *action_list = air_manager->action_list;
	uint16_t total_action_block = air_manager->total_action_block;

	action_list[total_action_block - 1].loop_start = air_manager->total_elements;
}

// CHECK: int clsn_count is unused, remove, or does this need to be implemented? Also shouldn't it be uint16_t?
void add_clsn_box(MU_Air_Manager *air_manager, int16_t coor[4], int clsn_count)
{
	Clsn *clsn = &(air_manager->clsn_arr[air_manager->total_clsn]);
	clsn->x = coor[0];
	clsn->y = coor[1];
	// w and h are in the opposite order here compared to the struct's order
	// if the clsn boxes render incorrectly, try flipping these two values around
	clsn->w = coor[2]; 
	clsn->h = coor[3];
	clsn->is_clsn1 = air_manager->is_clsn1;

	air_manager->total_clsn++;
}

void create_clsn_box(MU_Air_Manager *air_manager, uint16_t clsn_count, bool is_clsn1, bool is_default_clsn)
{
	air_manager->is_default_clsn = is_default_clsn;
	air_manager->is_clsn1 = is_clsn1;
	air_manager->total_clsn = 0; // Why does this line fix repeated clsn boxes in different actions?
	// This is most probably incomplete, or maybe named incorrectly, TODO
}

void open_air(MU_Air_Manager *air_manager, char *filename)
{
	Tokenizer *tok = tokenizer_init(100, NULL, NULL, 23);
	if(!mu_open_file(tok, filename))
	{
		debug_print("Air Manager: Failed to open %s", filename);
		return;
	}

	tok->case_sensitive = false;

	while(!tok->at_eof)
	{
		int action_num = -1;
		if(check_token(tok, "[", true))
		{
			if(!check_token(tok, "Begin", true))
			{
				// Not sure why this was commented out, TODO: test this code and see why it's commented out
				// For now I've placed some code in here for what I **think** is supposed to happen
				// tokenizer->buffer = 0;
				// mu_log_message("Buffer erased");
				// mu_log_message("Next char in file_buffer: %c", tokenizer->file_buffer[tokenizer->current_file_pos+1]);

				debug_print("Invalid Action Declaration, expected \"Begin\" after [ on line %d", tok->cur_file_line);
				return;
			}

			if(!check_token(tok, "Action", true))
			{
				debug_print("Invalid Action Declaration, expected \"Action\" after \"Begin\" on line %d", tok->cur_file_line);
				return;
			}

			if(!is_token_number(tok))
			{
				debug_print("Invalid Action Declaration, expected action number after \"Action\" on line %d", tok->cur_file_line);
				return;
			}

			action_num = mu_get_int(tok);

			if(!check_token(tok, "]", true))
			{
				debug_print("Invalid Action Declaration, expected ] after action number on line %d", tok->cur_file_line);
				return;
			}

			add_action(air_manager, action_num);
		}
		else if(check_token(tok, "Clsn1Default:", false) ||
				check_token(tok, "Clsn2Default:", false) ||
				check_token(tok, "Clsn1:", false) ||
				check_token(tok, "Clsn2:", false))
		{
			const char *token = get_token(tok);
			bool is_clsn1 = (token[4] == '1');
			bool is_default_clsn = (strlen(token) > 7); // replace strlen() call with sizeof()?

			if(!is_token_number(tok))
			{
				debug_print("Invalid Clsn Declaration, expected number of collisions on line %d", tok->cur_file_line);
				return;
			}

			int clsn_box_count = mu_get_int(tok);
			// TODO: remember this function is incomplete, fix this up once we can draw collision boxes to the screen
			create_clsn_box(air_manager, clsn_box_count, is_clsn1, is_default_clsn);

			for(int c = 0; c < clsn_box_count; c++)
			{
				if(!(check_token(tok, "Clsn1", true) ||
					 check_token(tok, "Clsn2", true)))
				{
					debug_print("Invalid Clsn Declaration on line %d", tok->cur_file_line);
					return;
				}

				if(!check_token(tok, "[", true))
				{
					debug_print("Invalid Clsn Declaration, expected [ on line %d", tok->cur_file_line);
					return;
				}

				if(mu_get_int(tok) != c)
				{
					debug_print("Invalid Clsn number, expected %d on line %d", c, tok->cur_file_line);
					return;
				}

				if(!check_token(tok, "]", true))
				{
					debug_print("Invalid Clsn Declaration, expected ] on line %d", tok->cur_file_line);
					return;
				}

				if(!check_token(tok, "=", true))
				{
					debug_print("Invalid Clsn Declaration, expected = on line %d", tok->cur_file_line);
					return;
				}

				int16_t coor[4];
				for(int i = 0; i < 4; i++)
				{
					if(!is_token_number(tok))
					{
						debug_print("Invalid Clsn Declaration, expected x coordinate on line %d", tok->cur_file_line);
						return;
					}

					coor[i] = mu_get_int(tok);

					if(i < 3)
					{
						if(!check_token(tok, ",", true))
						{
							debug_print("Invalid Clsn Declaration, expected , on line %d", tok->cur_file_line);
							return;
						}
					}
				}
				add_clsn_box(air_manager, coor, c);
			}
		}
		else if(check_token(tok, "Loopstart", true))
		{
			set_loop(air_manager);
		}
		else if(is_token_number(tok))
		{
			int16_t values[5];
			for(int v = 0; v < 5; v++)
			{
				if(!is_token_number(tok))
				{
					debug_print("Invalid animation, expected number on line %d", tok->cur_file_line);
					return;
				}

				values[v] = mu_get_int(tok);

				if(v < 4)
				{
					if(!check_token(tok, ",", true))
					{
						debug_print("Invalid animation, expected , on line %d", tok->cur_file_line);
						return;
					}
				}
			}

			uint16_t flip_flags = 0;
			if(check_token(tok, ",", true))
			{
				if(check_token(tok, "H", true))
				{
					flip_flags = HFLIPFLAG;
				}
				else if(check_token(tok, "V", true))
				{
					flip_flags = VFLIPFLAG;
				}
				else if(check_token(tok, "VH", true) ||
						check_token(tok, "HV", true))
				{
					flip_flags = HVFLIPFLAG;
				}
			}

			if(check_token(tok, ",", true))
			{
				// Transparency blending, not implemented yet
				// TODO
				get_token(tok); 
			}

			if(check_token(tok, ",", false)) // TODO: MUGEN 1.1 parameters should be implemented here
			{
				debug_print("There are MUGEN 1.1 parameters in this AIR file on line %d, this will probably cause unexpected errors", tok->cur_file_line);
			}

			add_element(air_manager, values, flip_flags, 0);
		}
		else
		{
			debug_print("An unexpected error has likely occurred on line %d, will try to continue execution", tok->cur_file_line);
			get_token(tok);
		}
	}
	mu_close_file(tok);
}

Action *get_action(MU_Air_Manager *air_manager, int action_num)
{
	for(uint16_t i = 0; i < air_manager->total_action_block; i++)
	{
		if(air_manager->action_list[i].action_num == action_num)
		{
			return &air_manager->action_list[i];
		}
	}
	// Else
	debug_print("Animation number %d is not defined", action_num);
	return NULL;
}