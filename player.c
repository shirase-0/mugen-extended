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
	//player->cmd_manager = cmd_manager_init(120); // Default of 120 in the original version
	//player->kb = keyboard_data_init();
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
	//player->cmd_manager->cmd_timer = timer;
}

// TODO: If any of these files can't be found, this function should return false
bool load_player(Player *player, const char *player_def_filename)
{
	//MU_State_Parser *state_parser = (MU_State_Parser*) malloc(sizeof(MU_State_Parser));

	// Reset/Initialise managers
	reset_allocator(player->player_allocator);
	reset_air_manager(player->air_manager);
	reset_sff_manager(player->sff_manager);
	//reset_state_manager(player->state_manager);

	// TODO: use player_def_filename to load players using their .def file
	//load_cmd_file(player->cmd_manager, "chars\\kfm\\kfm.cmd");
	// TODO: does this function really need all these arguments?
	// Maybe set the allocator for the state parser in this function
	// Also, remember to free() state_parser at the bottom of this function
	//parse_statefile(state_parser, "chars\\kfm\\kfm.cns", player->state_manager, player->player_allocator);
	open_air(player->air_manager, "chars\\kfm\\kfm.air");

	load_act_to_sff(player->sff_manager, "chars\\kfm\\kfm6.act");
	load_sff_file(player->sff_manager, "chars\\kfm\\kfm.sff");

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
	Action_Element *anim = player->sff_manager->anim;

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