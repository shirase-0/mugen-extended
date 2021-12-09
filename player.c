#include "mu_headers.h"

// Copied from original version
char *str_state[]={ 
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
	player->air_manager = air_manager_init();
	player->sff_manager = sff_manager_init();
	player->state_manager = state_manager_init();
	player->cmd_manager = cmd_manager_init(120); // Default of 120 in the original version
	player->kb = keyboard_data_init();
	memset(player->p_vars.var, 0, sizeof(int) * 60);
	memset(player->p_vars.fvar, 0, sizeof(float) * 40);
	player->b_right_faced = true;
	player->b_alive = true;
	player->b_debug_info = false;
	player->b_hitdef_persist = false;
	player->b_hitcount_persist = false;
	player->b_movehit_persist = false;
	player->b_hitdef = false;
	player->n_life = 1000;
	player->n_power = 1000;
	player->n_state_time = 0;
	player->n_state_type = 0;
	player->n_physics = 0;
	player->n_movetype = 0;
	player->x = 0;
	player->y = 0;
	player->xvel = 0;
	player->yvel = 0;

	return player;
}

void mu_set_player_pointers(Player *player, MU_Video_System *video_system, MU_Allocator *allocator, MU_Timer *timer)
{
	player->player_allocator = allocator;
	player->video_system = video_system;
	player->timer = timer;

	player->air_manager->air_allocator = allocator;
	mu_set_sff_pointers(player->sff_manager, video_system, allocator, timer, player->air_manager);
	player->state_manager->state_allocator = allocator;
	player->cmd_manager->cmd_timer = timer;
}

bool load_player(Player *player, const char *str_player)
{
	MU_State_Parser *state_parser = (MU_State_Parser*) malloc(sizeof(MU_State_Parser));

	// Reset memory allocator
	// This caused a segfault, which appears to be fixed now thanks to the removal of a possible 
	// memory leak. Should the problem crop up again, check the following note for a possible fix
	// Possible fix: check if allocator is b_free already, if so, skip the reset?
	reset_allocator(player->player_allocator);

	reset_air_manager(player->air_manager);
	reset_sff_manager(player->sff_manager);
	reset_state_manager(player->state_manager);

	// TODO: handle .def file loading
	load_cmd_file(player->cmd_manager, "kfm\\kfm.cmd");
	// The following line has been commented out, as the code it runs is currently under development
	// When testing the finite state machine, uncomment this line
	//parse_statefile(state_parser, "kfm\\kfm.cns", player->state_manager, player->player_allocator);
	open_air(player->air_manager, "kfm\\kfm.air");

	load_act_to_sff(player->sff_manager, "kfm\\kfm6.act");
	load_sff_file(player->sff_manager, "kfm\\kfm.sff");

	// The following are other characters sometimes used for testing
	// Uncomment these when in use
	// load_cmd_file(player->cmd_manager, "Iori_Yagami\\Iori_Yagami.cmd");
	// //parse_statefile(state_parser, "Iori_Yagami\\Iori_Yagami.cns", player->state_manager, player->player_allocator);
	// open_air(player->air_manager, "Iori_Yagami\\Iori_Yagami.air");

	// load_act_to_sff(player->sff_manager, "Iori_Yagami\\Paletas\\Iori_01.act");
	// load_sff_file(player->sff_manager, "Iori_Yagami\\Iori_Yagami.SFF");

	// //load_cmd_file(player->cmd_manager, "Mai_Shiranui\\cmd\\Mai.mai");
	// //parse_statefile(state_parser, "Mai_Shiranui\\cns\\Mai-N.cns", player->state_manager, player->player_allocator);
	// open_air(player->air_manager, "Mai_Shiranui\\Mai.air");

	// load_act_to_sff(player->sff_manager, "Mai_Shiranui\\pal\\Mai01.act");
 //    load_sff_file(player->sff_manager, "Mai_Shiranui\\Mai.sff");

    // Make always masked blit
    // This ensures alpha transparency on sprites
    player->sff_manager->n_flags = BLT_NORMALMASKED;
    prepare_anim(player->sff_manager, 0);

    // TODO: implement the finite state machine, and the relevant controllers to uncomment this line
    //  ChangeState(0); 

    return true;
}

// TODO: finish this function
bool check_state(PL_State *temp_state)
{
	bool b_triggerall = true;
	bool b_trigger = true;
	// Currently unused, uncomment this when this function is implemented
	//u8 n_trigger_type;

	return(b_triggerall && b_trigger);
}

// TODO: this function is likely unfinished
void handle_physics(Player *player)
{
	player->x += player->xvel;
	player->y += player->yvel;
}

// TODO: implement this function once state_parser.c is working as intended
void handle_fsm()
{
	/*
	================================================================================
	Handles the FSM of the player
	-checks all trigger 
	-and activate controllers  
	================================================================================
	*/

	//check every state in this statedef
	  /*   for(u16 i=0;i < lpCurrStatedef->nHowManyState; i++)
	     {
	        if( CheckState(&lpCurrStatedef->lpState[i]) )
	           ExecuteController(&lpCurrStatedef->lpState[i]);
	                      
	     }
	        */
}

void update_facing(Player *player)
{
	if(player->b_right_faced)
	{
		player->sff_manager->n_flags = BLT_NORMALMASKED;
	}
	else
	{
		player->sff_manager->n_flags = BLT_FLIPHMASKED;
	}
}

// Renders debug information to the screen in realtime
// Some of the information rendered may be incorrect, due to the finite state machine not being fully 
// implemented yet
void mu_player_debug(Player *player)
{
	Action_Element *anim = player->sff_manager->anim;

	// Print information about the current animation
	mu_draw_text(player->video_system, 0, 20, "Action %3i | AnimElem %3i/%3i | AnimTime %3i/%3i ", anim->n_action_number
																								 , anim->n_current_image + 1
																								 , anim->n_number_of_elements
																								 , anim->n_current_time
																								 , anim->n_complete_anim_time);
	mu_draw_text(player->video_system, 0, 40, "StateType=%s | MoveType=%s | Physics=%s", str_state[player->n_state_type]
																					   , str_state[player->n_movetype]
																					   , str_state[player->n_physics]);
	mu_draw_text(player->video_system, 0, 60, "x=%3f,y=%3f", player->x, player->y - player->n_ground);
	mu_draw_text(player->video_system, 0, 80, "HitDef %i | State %i | StateTime %i", player->b_hitdef, 10, player->n_state_time);
}

void player_change_state(Player *player, s32 n_state_number)
{
	// Reset state_time
	player->n_state_time = 0;
	// Get the statedef
	player->lp_current_statedef = get_statedef(player->state_manager, n_state_number);

	// Set state_type
	if(player->lp_current_statedef->type != UNTOUCH)
	{
		player->n_state_type = player->lp_current_statedef->type;
	}

	// Set physics
	if(player->lp_current_statedef->physics != UNTOUCH)
	{
		player->n_physics = player->lp_current_statedef->physics;
	}

	if(player->lp_current_statedef->movetype != UNTOUCH)
	{
		player->n_movetype = player->lp_current_statedef->movetype;
	}

	// Set the ctrl flag
	if(player->lp_current_statedef->b_ctrl != -1)
	{
		player->b_ctrl = player->lp_current_statedef->b_ctrl;
	}

	player->b_hitdef_persist = player->lp_current_statedef->b_hitdef_persist;
	player->b_hitcount_persist = player->lp_current_statedef->b_hitcount_persist;
	player->b_movehit_persist = player->lp_current_statedef->b_movehit_persist;

	// Change animation if needed
	if(player->lp_current_statedef->n_anim != -1)
	{
		prepare_anim(player->sff_manager, player->lp_current_statedef->n_anim);
	}

	if(player->lp_current_statedef->velset.x != -3333)
	{
		player->xvel = player->lp_current_statedef->velset.x;
	}

	if(player->lp_current_statedef->velset.y != -3333)
	{
		player->yvel = player->lp_current_statedef->velset.y;
	}

	if(player->lp_current_statedef->n_spr_priority != 255)
	{
		player->n_spr_prio = player->lp_current_statedef->n_spr_priority;
	}

	if(player->lp_current_statedef->n_poweradd != -3333)
	{
		player->n_power += player->lp_current_statedef->n_poweradd;
	}

	if(player->lp_current_statedef->n_juggle != -3333)
	{
		mu_log_message("TODO: Handle juggle parameter of statedef");
	}

	// TODO: handle any additional persistent player variables here
}

void update_player(Player *player)
{
	// Update player inputs
	mu_cmd_update(player->cmd_manager, player->kb, false);

	handle_fsm();
	handle_physics(player);
	update_facing(player);
	player->n_state_time++;
}

void draw_player(Player *player)
{
	blit_anim(player->sff_manager, (s16)player->x, (s16)player->y); // Not sure if this cast is needed, but the original version uses it

	// Draw debug info
	if(player->b_debug_info)
	{
		mu_player_debug(player);
	}
}

bool is_anim_available(Player *player, s32 n_anim)
{
	if(get_action(player->air_manager, n_anim) == NULL)
	{
		return false;
	}
	else
	{
		return true;
	}
}