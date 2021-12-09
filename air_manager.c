#include "mu_headers.h"

MU_Air_Manager *air_manager_init()
{
	MU_Air_Manager *air_manager = (MU_Air_Manager*) malloc(sizeof(MU_Air_Manager));
	return air_manager;
}

void reset_air_manager(MU_Air_Manager *air_manager)
{
	air_manager->n_total_action_block = 0;
	air_manager->n_action_list_size = 100;
	air_manager->n_element_list_size = 100;
	air_manager->n_total_cns = 0;
	air_manager->b_default_clsn = false;

	air_manager->lp_action_list = (Action_Element*) mu_alloc(air_manager->air_allocator, sizeof(Action_Element) * air_manager->n_action_list_size);
}

// Allocates additional memory to an animation element
void reallocate_action_block(MU_Air_Manager *air_manager)
{
	if(air_manager->n_total_action_block > 0)
	{
		// if this ever causes issues, revert to the original (harder to read) code below
		//air_manager->lp_action_list[air_manager->n_total_action_block-1].animation_element = (Element*) mu_realloc(air_manager->air_allocator, air_manager->lp_action_list[air_manager->n_total_action_block-1].animation_element, sizeof(Element) * air_manager->lp_action_list[air_manager->n_total_action_block-1].n_number_of_elements);	
		Action_Element *action = &(air_manager->lp_action_list[air_manager->n_total_action_block-1]);
		action->animation_element = (Element*) mu_realloc(air_manager->air_allocator, action->animation_element, sizeof(Element) * action->n_number_of_elements);	
	}
}

void add_action(MU_Air_Manager *air_manager, s32 n_action_number)
{
	reallocate_action_block(air_manager);

	air_manager->n_anim_time = 0;
	air_manager->n_total_element = 0;
	air_manager->n_element_list_size = 100;
	air_manager->b_default_clsn = false;

	if(air_manager->n_total_action_block > air_manager->n_action_list_size - 1)
	{
		air_manager->n_action_list_size += 100;
		air_manager->lp_action_list = (Action_Element*) mu_realloc(air_manager->air_allocator, air_manager->lp_action_list, sizeof(Action_Element) * air_manager->n_action_list_size);
	}

	// Test Code, TODO: move this check to test.c somehow?
	// if(air_manager->n_total_action_block == 2047 || air_manager->n_total_action_block == 2050)
	// {
	// 	mu_log_message("Current total actions: %d", air_manager->n_total_action_block);
	// 	show_mem_usage(air_manager->air_allocator);
	// }

	// TODO: This seems to work fine, but an error occurred in sff_manager when trying to do something similar
	// Check this code to ensure it actually works as intended, and isn't just being glossed over somewhere
	Action_Element *lp_action_list = &air_manager->lp_action_list[air_manager->n_total_action_block];	
	lp_action_list->loop_start = -1;
	lp_action_list->n_action_number = n_action_number;
	lp_action_list->animation_element = (Element*) mu_alloc(air_manager->air_allocator, sizeof(Element) * air_manager->n_element_list_size);

	air_manager->n_total_action_block++;
}

void add_element(MU_Air_Manager *air_manager, s16 n_group_number, s16 n_image_number, s16 x, s16 y, s16 n_during_time, u16 n_flip, u32 n_colour_flag)
{
	// TODO: Handle ColorFlags
    // 32bit
    // 8Bit ADDOP 8bit ADDOPVALUE 8bit SUBOP 8Bit SUBOPVALUE
    // PrintMessage("%i %i %i %i",nGroupNumber,nImageNumber,x,y);
    Element *current_animation_element = air_manager->lp_action_list[air_manager->n_total_action_block-1].animation_element;

    if(air_manager->n_total_element > air_manager->n_element_list_size - 1)
    {
    	air_manager->n_element_list_size += 100;
    	current_animation_element = (Element*) mu_realloc(air_manager->air_allocator, current_animation_element, sizeof(Element) * air_manager->n_element_list_size);
    }

    current_animation_element[air_manager->n_total_element].n_group_number = n_group_number;
    current_animation_element[air_manager->n_total_element].n_image_number = n_image_number;
    current_animation_element[air_manager->n_total_element].x = x;
    current_animation_element[air_manager->n_total_element].y = y;
    current_animation_element[air_manager->n_total_element].n_during_time = n_during_time;
    current_animation_element[air_manager->n_total_element].flip_flags = n_flip;
    current_animation_element[air_manager->n_total_element].colour_flags = n_colour_flag;

    air_manager->lp_action_list[air_manager->n_total_action_block-1].n_complete_anim_time += n_during_time;

    current_animation_element[air_manager->n_total_element].n_number_of_clsn = air_manager->n_total_cns;

    current_animation_element[air_manager->n_total_element].p_clsn_data = (Clsn*) mu_alloc(air_manager->air_allocator, sizeof(Clsn) * air_manager->n_total_cns);
    memcpy(current_animation_element[air_manager->n_total_element].p_clsn_data, air_manager->p_clsn, sizeof(Clsn) * air_manager->n_total_cns);

    if(!air_manager->b_default_clsn)
    {
    	air_manager->n_total_cns = 0;
    }


    air_manager->n_total_element++;
    air_manager->lp_action_list[air_manager->n_total_action_block-1].n_number_of_elements = air_manager->n_total_element;
}

void set_loop(MU_Air_Manager *air_manager)
{
	air_manager->lp_action_list[air_manager->n_total_action_block-1].loop_start = air_manager->n_total_element;
}

void add_clsn_box(MU_Air_Manager *air_manager, s16 x, s16 y, s16 w, s16 h, int n_number_of_clsn)
{
	Clsn p_clsn = air_manager->p_clsn[air_manager->n_total_cns];
	p_clsn.clsn_rect.x = x;
	p_clsn.clsn_rect.y = y;
	p_clsn.clsn_rect.w = w;
	p_clsn.clsn_rect.h = h;
	p_clsn.b_clsn1 = air_manager->b_is_clsn1;

	air_manager->n_total_cns++;
}

void create_clsn_box(MU_Air_Manager *air_manager, u16 n_number_of_clsn, bool b_clsn1, bool b_default)
{
	air_manager->b_default_clsn = b_default;
	air_manager->b_is_clsn1 = b_clsn1;
	// Note to self: probably need to add more here, check this again when implementing collisions
}

void open_air(MU_Air_Manager *air_manager, char *str_filename)
{
	Tokenizer *tokenizer = tokenizer_init(100, NULL, NULL, 23);
	if(!mu_open_file(tokenizer, str_filename))
	{
		mu_log_message("Air Manager: Failed to open %s", str_filename);
		return;
	}

	tokenizer->is_case_sensitive = false;

	while(!tokenizer->at_end_of_file)
	{
		int action_num = -1;
		if(check_token_consume(tokenizer, "["))
		{
			if(!check_token_consume(tokenizer, "Begin"))
			{
				// tokenizer->buffer = 0;
				// mu_log_message("Buffer erased");
				// mu_log_message("Next char in file_buffer: %c", tokenizer->file_buffer[tokenizer->current_file_pos+1]);
			}

			if(!check_token_consume(tokenizer, "Action"))
			{

			}

			if(!check_token_is_number(tokenizer))
			{

			}

			action_num = mu_get_int(tokenizer);

			if(!check_token_consume(tokenizer, "]"))
			{

			}

			add_action(air_manager, action_num);
		}
		else if(check_token_no_consume(tokenizer, "Clsn1Default:") || 
			    check_token_no_consume(tokenizer, "Clsn2Default:") || 
			    check_token_no_consume(tokenizer, "Clsn1:") || 
			    check_token_no_consume(tokenizer, "Clsn2:"))
		{
			const char *token = get_token(tokenizer);
			bool is_clsn1 = (token[4] == '1');
			bool is_default = (strlen(token) > 7);

			if (!check_token_is_number(tokenizer))
			{

			}

			int clsn_box_count = mu_get_int(tokenizer);
			create_clsn_box(air_manager, clsn_box_count, is_clsn1, is_default);

			for(int c = 0; c < clsn_box_count; c++)
			{
				if(!(check_token_consume(tokenizer, "Clsn1") || check_token_consume(tokenizer, "Clsn2"))) // for some reason kfm uses clsn2 sometimes in clsn1 items
				{

				}

				if(!check_token_consume(tokenizer, "["))
				{

				}

				if(mu_get_int(tokenizer) != c)
				{

				}

				if(!check_token_consume(tokenizer, "]"))
				{

				}

				if(!check_token_consume(tokenizer, "="))
				{

				}

				int coor[4];
				for(int i = 0; i < 4; i++)
				{
					if(!check_token_is_number(tokenizer))
					{

					}

					coor[i] = mu_get_int(tokenizer);

					if(i < 3)
					{
						if(!check_token_consume(tokenizer, ","))
						{

						}
					}
				}
				add_clsn_box(air_manager, coor[0], coor[1], coor[2], coor[3], c);
			}
		}
		else if (check_token_consume(tokenizer, "Loopstart"))
		{
			set_loop(air_manager);
		}
		else if(check_token_is_number(tokenizer))
		{
			int values[5];
			for(int v = 0; v < 5; v++)
			{
				if(!check_token_is_number(tokenizer))
				{

				}

				values[v] = mu_get_int(tokenizer);

				if(v < 4)
				{
					if(!check_token_consume(tokenizer, ","))
					{

					}
				}
			}
			int flip_flag = 0;
			if(check_token_consume(tokenizer, ","))
			{
				if(check_token_consume(tokenizer, "H"))
				{
					flip_flag = HFLIPFLAG;
				}
				else if(check_token_consume(tokenizer, "V"))
				{
					flip_flag = VFLIPFLAG;
				}
				else if(check_token_consume(tokenizer, "VH") || check_token_consume(tokenizer, "HV"))
				{
					flip_flag = HVFLIPFLAG;
				}
				else if(!check_token_no_consume(tokenizer, ","))
				{

				}
			}

			if(check_token_consume(tokenizer, ","))
			{
				get_token(tokenizer); // alpha param
			}

			add_element(air_manager, values[0], values[1], values[2], values[3], values[4], flip_flag, 0);
		}
		else
		{

		}
	}
	mu_close_file(tokenizer);
}

Action_Element *get_action(MU_Air_Manager *air_manager, s32 n_action_number)
{
	for(u16 i = 0; i < air_manager->n_total_action_block; i++)
	{
		if(air_manager->lp_action_list[i].n_action_number == n_action_number)
		{
			return &air_manager->lp_action_list[i];
		}
	}
	// Else
	mu_log_message("No anim block with %i", n_action_number);
	
	return NULL;
}