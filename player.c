#include "mu_headers.h"

const char *states[]={ 
	"stand",
	"crouch",
	"air",
	"liedown",
	"attack",
	"idle",
	"hit",
	"none",
	"unchange"
};

Player *player_init()
{
	Player *player = (Player*) malloc(sizeof(Player));

	player->player_allocator = NULL;
	player->graphics_manager = NULL;
	player->timer = NULL;

	player->air_manager = air_manager_init();
	player->sff_manager = sff_manager_init();
	//player->state_manager = state_manager_init();
	player->cmd_manager = cmd_manager_init(120); // Default of 120 in the original version
	player->kb = keyboard_data_init();
	memset(player->player_vars.var, 0, sizeof(int) * 60);
	memset(player->player_vars.fvar, 0, sizeof(double) * 40);
	player->right_faced = true;
	player->alive = true;
	player->debug_info = false;
	player->hitdef_persist = false;
	player->hitcount_persist = false;
	player->movehit_persist = false;
	player->hitdef = false;
	player->life = 1000;
	player->power = 1000;
	player->state_time = 0;
	player->state_type = 0;
	player->physics = 0;
	player->movetype = 0;
	player->x = 0;
	player->y = 0;
	player->xvel = 0;
	player->yvel = 0;

	return player;
}

void mu_set_player_pointers(Player *player, MU_Graphics_Manager *graphics_manager, MU_Allocator *allocator, MU_Timer *timer)
{
	player->player_allocator = allocator;
	player->graphics_manager = graphics_manager;
	player->timer = timer;

	player->air_manager->air_allocator = allocator;
	mu_set_sff_pointers(player->sff_manager, graphics_manager, allocator, timer, player->air_manager);
	//player->state_manager->state_allocator = allocator;
	player->cmd_manager->cmd_timer = timer;
}

Player_Def *parse_player_def (const char *player_name)
{
	Tokenizer *tok = tokenizer_init(100, NULL, NULL, 23);
	Player_Def *filenames = (Player_Def*) malloc(sizeof(Player_Def));
	memset(filenames, 0, sizeof(Player_Def));
	tok->case_sensitive = false;

	char player_def_filename[255];
	sprintf(player_def_filename, "chars\\%s\\%s.def", player_name, player_name);

	if(!mu_open_file(tok, player_def_filename))
	{
		debug_print("Player: Failed to open %s", player_def_filename);
	}

	while(!tok->at_eof)
	{
		if(check_token(tok, "[", true))
		{
			if(check_token(tok, "Info", true))
			{
				if(!check_token(tok, "]", true))
				{
					debug_print("Invalid Player Declaration, expected ] after Info delcaration on line %d", tok->cur_file_line);
				}
				else //grab info, TODO
				{
					while(!tok->at_eof)
					{
						if(check_token(tok, "[", false))
						{
							break;
						}
						else
						{
							get_token(tok);
						}
					}
				}
			}
			else if(check_token(tok, "Files", true))
			{
				if(!check_token(tok, "]", true))
				{
					debug_print("Invalid Player Declaration, expected ] after Info delcaration on line %d", tok->cur_file_line);
				}
				while(!tok->at_eof)
				{
					// TODO: Can this section be abstracted into its own function to reduce repeated code?
					if(check_token(tok, "sprite", false))
					{
						get_token(tok);
						if(!check_token(tok, "=", true))
						{
							debug_print("Invalid file declaration, expected = on line %d", tok->cur_file_line);
						}
						sprintf(filenames->sff_filename, "chars\\%s\\%s", player_name, get_token(tok));
						while(!tok->at_end_of_line)
						{
							sprintf(filenames->sff_filename, "%s%s", filenames->sff_filename, get_token(tok));
						}
					}
					else if(check_token(tok, "anim", false))
					{
						get_token(tok);
						if(!check_token(tok, "=", true))
						{
							debug_print("Invalid file declaration, expected = on line %d", tok->cur_file_line);
						}
						sprintf(filenames->air_filename, "chars\\%s\\%s", player_name, get_token(tok));
						while(!tok->at_end_of_line)
						{
							sprintf(filenames->air_filename, "%s%s", filenames->air_filename, get_token(tok));
						}
					}
					else if(check_token(tok, "cmd", false))
					{
						get_token(tok);
						if(!check_token(tok, "=", true))
						{
							debug_print("Invalid file declaration, expected = on line %d", tok->cur_file_line);
						}
						sprintf(filenames->cmd_filename, "chars\\%s\\%s", player_name, get_token(tok));
						while(!tok->at_end_of_line)
						{
							sprintf(filenames->cmd_filename, "%s%s", filenames->cmd_filename, get_token(tok));
						}
					}
					else if(check_token(tok, "pal1", false))
					{
						get_token(tok);
						if(!check_token(tok, "=", true))
						{
							debug_print("Invalid file declaration, expected = on line %d", tok->cur_file_line);
						}
						sprintf(filenames->act_filename, "chars\\%s\\%s", player_name, get_token(tok));
						while(!tok->at_end_of_line)
						{
							sprintf(filenames->act_filename, "%s%s", filenames->act_filename, get_token(tok));
						}
					}
					else
					{
						get_token(tok);
					}
				}
			}
		}
		else
		{
			get_token(tok);
		}
	}
	mu_close_file(tok);
	free_tokenizer(tok);
	debug_print("SFF: %s", filenames->sff_filename);
	debug_print("AIR: %s", filenames->air_filename);
	debug_print("CMD: %s", filenames->cmd_filename);
	debug_print("ACT: %s", filenames->act_filename);
	return filenames;
}

// TODO: If any of these files can't be found, this function should return false
bool load_player(Player *player, const char *player_name)
{
	//MU_State_Parser *state_parser = (MU_State_Parser*) malloc(sizeof(MU_State_Parser));

	// Reset/Initialise managers
	reset_allocator(player->player_allocator);
	reset_air_manager(player->air_manager);
	reset_sff_manager(player->sff_manager);
	//reset_state_manager(player->state_manager);

	Player_Def *filenames = parse_player_def(player_name);
	load_cmd_file(player->cmd_manager, filenames->cmd_filename);
	// TODO: does this function really need all these arguments?
	// Maybe set the allocator for the state parser in this function
	// Also, remember to free() state_parser at the bottom of this function
	//parse_statefile(state_parser, "chars\\kfm\\kfm.cns", player->state_manager, player->player_allocator);

	open_air(player->air_manager, filenames->air_filename);

	load_act_to_sff(player->sff_manager, filenames->act_filename);
	load_sff_file(player->sff_manager, filenames->sff_filename);
	free(filenames);

	// Make always masked blit
	// This ensures alpha transparency on sprites
	// TODO: just set this to BLT_NORMALMASKED when we first initialise the sff manager?
	player->sff_manager->flags = BLT_NORMALMASKED;
	prepare_anim(player->sff_manager, 0);

	// TODO: implement the FSM and the relevant controllers to uncomment this line
	// ChangeState(0);

	return true;
}

void update_facing(Player *player)
{
	if(player->right_faced)
	{
		player->sff_manager->flags = BLT_NORMALMASKED;
	}
	else
	{
		player->sff_manager->flags = BLT_FLIPHMASKED;
	}
}

// Renders debug information to the screen in realtime
// Some of the information rendered may be incorrect, due to the finite state machine not being fully 
// implemented yet
void mu_player_debug(Player *player)
{
	Action *anim = player->sff_manager->anim;

	// Print information about the current animation
	mu_draw_text(player->graphics_manager, 0, 20, "Action %3i | AnimElem %3i/%3i | AnimTime %3i/%3i ", anim->action_num
																								 , anim->current_image + 1
																								 , anim->elements_count
																								 , anim->current_time
																								 , anim->complete_anim_time);
	mu_draw_text(player->graphics_manager, 0, 40, "StateType=%s | MoveType=%s | Physics=%s", states[player->state_type]
																					   , states[player->movetype]
																					   , states[player->physics]);
	mu_draw_text(player->graphics_manager, 0, 60, "x=%3f,y=%3f", player->x, player->y - player->ground);
	mu_draw_text(player->graphics_manager, 0, 80, "HitDef %i | State %i | StateTime %i", player->hitdef, 10, player->state_time);
}

void update_player(Player *player)
{
	// Update player inputs
	//mu_cmd_update(player->cmd_manager, player->kb, false);

	//handle_fsm();
	//handle_physics(player);
	update_facing(player);
	player->state_time++;
}

void draw_player(Player *player)
{
	blit_anim(player->sff_manager, (int16_t)player->x, (int16_t)player->y);

	// Draw debug info
	if(player->debug_info)
	{
		mu_player_debug(player);
	}
}

bool is_anim_available(Player *player, int anim)
{
	if(get_action(player->air_manager, anim) == NULL)
	{
		return false;
	}
	else
	{
		return true;
	}
}