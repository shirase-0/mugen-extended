#include "mu_headers.h"

// TODO: Check Elecbyte documentation to see if there are any OPs missing from this list
char *str_opcode[] =
{
	"OP_PUSH",
	"OP_POP",
	"OP_ADD",
	"OP_SUB",
	"OP_MUL",
	"OP_DIV",
	"OP_EQUAL",
	"OP_NOTEQUAL",
	"OP_LESS",
	"OP_GREATER",
	"OP_LESSEQUAL",
	"OP_GRAETEREQUAL",
	"OP_INTERVALOP1",
	"OP_INTERVALOP2",
	"OP_INTERVALOP3",
	"OP_INTERVALOP4",
	"OP_INTERVALOP5",
	"OP_INTERVALOP6",
	"OP_INTERVALOP7",
	"OP_INTERVALOP8",
	"OP_LOGNOT",
	"OP_LOGAND",
	"OP_LOGOR",
	"OP_LOGXOR",
	"OP_NOT",
	"OP_AND",
	"OP_OR",
	"OP_XOR",
	"OP_SQUARE",
	"OP_NEG",
	"OP_Abs",
	"OP_Acos",
	"OP_Alive",
	"OP_Anim",
	"OP_AnimElem",
	"OP_AnimElemNo",
	"OP_AnimElemTime",
	"OP_AnimExist",
	"OP_AnimTime",
	"OP_Asin",
	"OP_Atan",
	"OP_AuthorName",
	"OP_BackEdgeBodyDist",
	"OP_BackEdgeDist",
	"OP_CanRecover",
	"OP_Ceil",
	"OP_Command",
	"OP_Const",
	"OP_Cos",
	"OP_Ctrl",
	"OP_DrawGame",
	"OP_EOP",
	"OP_Exp",
	"OP_Facing",
	"OP_Floor",
	"OP_FrontEdgeBodyDist",
	"OP_FrontEdgeDist",
	"OP_FVar",
	"OP_GameTime",
	"OP_GetHitVar",
	"OP_HitCount",
	"OP_HitDefAttr",
	"OP_HitFall",
	"OP_HitOver",
	"OP_HitPauseTime",
	"OP_HitShakeOver",
	"OP_HitVel",
	"OP_ID",
	"OP_IfElse",
	"OP_InGuardDist",
	"OP_IsHelper",
	"OP_IsHomeTeam",
	"OP_Life",
	"OP_LifeMax",
	"OP_Log",
	"OP_Ln",
	"OP_Lose",
	"OP_MatchNo",
	"OP_MatchOver",
	"OP_MoveContact",
	"OP_MoveGuarded",
	"OP_MoveHit",
	"OP_MoveType",
	"OP_MoveReversed",
	"OP_Name",
	"OP_NumEnemy",
	"OP_NumExplod",
	"OP_NumHelper",
	"OP_NumPartner",
	"OP_NumProj",
	"OP_NumProjID",
	"OP_NumTarget",
	"OP_P1Name",
	"OP_P2BodyDist",
	"OP_P2Dist",
	"OP_P2Life",
	"OP_P2MoveType",
	"OP_P2Name",
	"OP_P2StateNo",
	"OP_P2StateType",
	"OP_P3Name",
	"OP_P4Name",
	"OP_PalNo",
	"OP_ParentDist",
	"OP_Pi",
	"OP_Pos",
	"OP_Power",
	"OP_PowerMax",
	"OP_PlayerIDExist",
	"OP_PrevStateNo",
	"OP_ProjCancelTime",
	"OP_ProjContact",
	"OP_ProjContactTime",
	"OP_ProjGuarded",
	"OP_ProjGuardedTime",
	"OP_ProjHit",
	"OP_ProjHitTime",
	"OP_Random",
	"OP_RootDist",
	"OP_RoundNo",
	"OP_RoundsExisted",
	"OP_RoundState",
	"OP_ScreenPos",
	"OP_SelfAnimExist",
	"OP_Sin",
	"OP_StateNo",
	"OP_StateType",
	"OP_SysFVar",
	"OP_SysVar",
	"OP_Tan",
	"OP_TeamMode",
	"OP_TeamSide",
	"OP_TicksPerSecond",
	"OP_Time",
	"OP_TimeMod",
	"OP_UniqHitCount",
	"OP_Var",
	"OP_Vel",
	"OP_Win",
	"OP_MOD",
	"OP_NOP",
};

MU_State_Manager *state_manager_init()
{
	MU_State_Manager *state_manager = (MU_State_Manager*) calloc(1, sizeof(MU_State_Manager));

	return state_manager;
}

void reset_state_manager(MU_State_Manager *state_manager)
{
	state_manager->total_statedef = 0;
	state_manager->current_inst = 0;
	state_manager->total_triggers = 0;
	// TODO: make sure this doesn't cause a memory leak
	// If statedef_list already has some memory allocated, it **must** be freed before this function is run
	state_manager->alloc_statedef_size = 100;
	uint16_t size = state_manager->alloc_statedef_size;
	state_manager->statedef_list = (Statedef*) mu_alloc(state_manager->state_allocator, sizeof(Statedef) * size);
}

// TODO: refactor this function to not be as difficult to read
void realloc_statedef(MU_State_Manager *state_manager, uint16_t index)
{
	uint16_t state_count = state_manager->statedef_list[index].state_count;
	state_manager->statedef_list[index].state_list = (State*) mu_realloc(state_manager->state_allocator, 
																	state_manager->statedef_list[index].state_list,
																	sizeof(State) * state_count);

	for(int i = 0; i < state_count; i++)
	{
		uint16_t trigger_count = state_manager->statedef_list[index].state_list[i].trigger_count;
		state_manager->statedef_list[index].state_list[i].triggers = (Trigger*) mu_realloc(state_manager->state_allocator,
																					  state_manager->statedef_list[index].state_list[i].triggers,
																					  sizeof(Trigger) * trigger_count);
	}
}

// TODO: is this function even used?
// If not, remove it
void realloc_state_manager(MU_State_Manager *state_manager)
{
	realloc_statedef(state_manager, state_manager->total_statedef - 1);
	state_manager->statedef_list = (Statedef*) mu_realloc(state_manager->state_allocator,
														  state_manager->statedef_list,
														  sizeof(Statedef) * state_manager->total_statedef);
}

bool is_statedef_available(MU_State_Manager *state_manager, int statedef_num)
{
	for(int i = 0; i < state_manager->total_statedef; i++)
	{
		if(state_manager->statedef_list[i].state_number == statedef_num)
		{
			// Make this state invalidate to overwrite it
			// Not sure why -11 specifically though
			state_manager->statedef_list[i].state_number = -11;
			return true;
		}
	}
	// Else
	return false;
}

void set_default_statedef(MU_State_Manager *state_manager)
{
	Statedef *current_statedef = &(state_manager->statedef_list[state_manager->total_statedef - 1]);
	current_statedef->type = STAND;
	current_statedef->movetype = IDLE;
	current_statedef->physics = NONE;
	current_statedef->anim = -1;
	current_statedef->velset_x = -3333;
	current_statedef->velset_y = -3333;
	current_statedef->has_ctrl = -1;
	current_statedef->poweradd = -3333;
	current_statedef->facep2 = false;
	current_statedef->juggle = -3333;
	current_statedef->hitdef_persist = false;
	current_statedef->hitcount_persist = false;
	current_statedef->spr_priority = 255;
}

void add_statedef(MU_State_Manager *state_manager, int statedef_num)
{
	uint16_t total_statedef = state_manager->total_statedef;
	// Overwrite statedef? Why is this necessary?
	if(is_statedef_available(state_manager, statedef_num))
	{
		debug_print("Overwriting Statedef %i", statedef_num);
	}

	// Allocate more memory for each statedef added (state_list, triggers)
	// How does this differ to the allocation of memory for states below?
	// What purpose does it actually serve?
	// TODO: figure out if there's a memory leak here
	if(total_statedef != 0)
	{
		realloc_statedef(state_manager, total_statedef - 1);
	}

	// Allocate more memory for statedefs
	if(total_statedef > state_manager->alloc_statedef_size - 1)
	{
		state_manager->alloc_statedef_size += 100;
		uint16_t alloc_statedef_size = state_manager->alloc_statedef_size;
		state_manager->statedef_list = (Statedef*) mu_realloc(state_manager->state_allocator,
															  state_manager->statedef_list,
															  sizeof(Statedef) * alloc_statedef_size);
	}
	state_manager->statedef_list[total_statedef].state_number = statedef_num;

	// Allocate memory for states
	state_manager->total_state = 0;
	state_manager->alloc_state_size = 100;
	uint16_t alloc_state_size = state_manager->alloc_state_size;
	state_manager->statedef_list[total_statedef].state_list = (State*) mu_alloc(state_manager->state_allocator,
																				sizeof(State) * alloc_state_size);

	state_manager->total_statedef++;

	set_default_statedef(state_manager);
}

void add_state(MU_State_Manager *state_manager, int state_num)
{
	uint16_t total_statedef = state_manager->total_statedef;
	uint16_t alloc_state_size = state_manager->alloc_state_size;
	uint16_t total_state = state_manager->total_state;

	// Reallocate extra memory for states if we run out
	if(state_manager->total_state > alloc_state_size - 1)
	{
		state_manager->alloc_state_size += 100;
		alloc_state_size = state_manager->alloc_state_size;
		// TODO: I think this line is wrong, and it should use total_statedef - 1 in the mu_realloc call
		// Fix this if it turns out that I'm right
		// For now I'll keep it how it was in the original
		state_manager->statedef_list[total_statedef - 1].state_list = (State*) mu_realloc(state_manager->state_allocator,
																					 state_manager->statedef_list[total_statedef].state_list,
																					 sizeof(State) * alloc_state_size);
	}

	state_manager->statedef_list[total_statedef - 1].state_list[total_state].state_number = state_num;

	state_manager->total_triggers = 0;
	state_manager->trigger_list_size = 100;
	uint16_t trigger_list_size = state_manager->trigger_list_size;

	state_manager->statedef_list[total_statedef - 1].state_list[total_state].triggers = (Trigger*) mu_alloc(state_manager->state_allocator,
																									   sizeof(Trigger) * trigger_list_size);

	state_manager->total_state++;
	total_state = state_manager->total_state;
	state_manager->statedef_list[total_statedef - 1].state_count = total_state;

	state_manager->current_inst = 0;
}

void add_trigger_to_state(MU_State_Manager *state_manager, uint8_t type)
{
	uint16_t total_statedef = state_manager->total_statedef;
	uint16_t total_state = state_manager->total_state;
	uint16_t total_triggers = state_manager->total_triggers;
	uint16_t current_inst = state_manager->current_inst;
	// TODO: see if this works, if not, revert the simplification
	Trigger *current_trigger = &(state_manager->statedef_list[total_statedef - 1].state_list[total_state - 1].triggers[total_triggers]);

	if(total_triggers > state_manager->trigger_list_size)
	{
		debug_print("State Manager: You have 100 triggers...You should not have 100 triggers...");
	}

	// Copy the instruction(s?) to the trigger

	current_trigger->trigger_type = type;
	current_trigger->inst = mu_alloc(state_manager->state_allocator, sizeof(Instruction) * current_inst);

	memcpy(current_trigger->inst, state_manager->inst, sizeof(Instruction) * current_inst);

	state_manager->total_triggers++;
	total_triggers = state_manager->total_triggers;
	state_manager->statedef_list[total_statedef - 1].state_list[total_state - 1].trigger_count = total_triggers;

	state_manager->current_inst = 0;
}

// TODO: change value to double?
void add_instruction(MU_State_Manager *state_manager, uint16_t opcode, float value, const char *str_value)
{
	debug_print("State Manager: add_instruction = %s %f %s(%i)", str_opcode[opcode], value, str_value, opcode);

	Instruction *inst = &(state_manager->inst[state_manager->current_inst]);
	inst->opcode = opcode;
	inst->value = value;
	inst->str_value = NULL;

	// Only add a string if we need one
	if(str_value[0] != '#')
	{
		// I'm pretty sure this causes a memory leak, since we're never freeing this
		// TODO: ensure this doesn't cause a memory leak
		inst->str_value = malloc(sizeof(char) * strlen(str_value) + 1);
		strcpy(inst->str_value, str_value);
	}

	state_manager->current_inst++;
}

void exchange_inst(MU_State_Manager *state_manager)
{
	// CHECK: should this be a pointer?
	Instruction temp;

	memcpy(&temp, &(state_manager->inst[state_manager->current_inst - 1]), sizeof(temp));
	memcpy(&(state_manager->inst[state_manager->current_inst - 1]), &(state_manager->inst[state_manager->current_inst - 2]), sizeof(temp));
	memcpy(&(state_manager->inst[state_manager->current_inst - 2]), &temp, sizeof(temp));
}

Statedef *get_statedef(MU_State_Manager *state_manager, int state_num)
{
	for(uint16_t i = 0; i < state_manager->total_statedef; i++)
	{
		if(state_manager->statedef_list[i].state_number == state_num)
		{
			return &(state_manager->statedef_list[i]);
		}
	}
	debug_print("State Manager: Statedef %i not found", state_num);
	return NULL;
}

Statedef *get_current_statedef(MU_State_Manager *state_manager)
{
	return &(state_manager->statedef_list[state_manager->total_statedef - 1]);
}

void set_statedef_type(MU_State_Manager *state_manager, uint8_t type)
{
	state_manager->statedef_list[state_manager->total_statedef - 1].type = type;
}

void set_state_movetype(MU_State_Manager *state_manager, uint8_t type)
{
	state_manager->statedef_list[state_manager->total_statedef - 1].movetype = type;
}

void set_state_physics_type(MU_State_Manager *state_manager, uint8_t type)
{
	state_manager->statedef_list[state_manager->total_statedef - 1].physics = type;
}

void set_state_anim(MU_State_Manager *state_manager, int anim)
{
	state_manager->statedef_list[state_manager->total_statedef - 1].anim = anim;
}

void set_state_ctrl(MU_State_Manager *state_manager, int8_t has_ctrl)
{
	state_manager->statedef_list[state_manager->total_statedef - 1].has_ctrl = has_ctrl;
}

void set_state_poweradd(MU_State_Manager *state_manager, int16_t poweradd)
{
	state_manager->statedef_list[state_manager->total_statedef - 1].poweradd = poweradd;
}

void set_state_juggle(MU_State_Manager *state_manager, int16_t juggle)
{
	state_manager->statedef_list[state_manager->total_statedef - 1].juggle = juggle;
}

void set_state_hitdef_persist(MU_State_Manager *state_manager, bool hitdef_persist)
{
	state_manager->statedef_list[state_manager->total_statedef - 1].hitdef_persist = hitdef_persist;
}

void set_movehit_persist(MU_State_Manager *state_manager, bool movehit_persist)
{
	state_manager->statedef_list[state_manager->total_statedef - 1].movehit_persist = movehit_persist;
}

void set_state_hitcount_persist(MU_State_Manager *state_manager, bool hitcount_persist)
{
	state_manager->statedef_list[state_manager->total_statedef - 1].hitcount_persist = hitcount_persist;
}

void set_spr_priority(MU_State_Manager *state_manager, uint8_t spr_priority)
{
	state_manager->statedef_list[state_manager->total_statedef - 1].spr_priority = spr_priority;
}

void set_state_facep2(MU_State_Manager *state_manager, bool facep2)
{
	state_manager->statedef_list[state_manager->total_statedef - 1].facep2 = facep2;
}

// TODO: change these to doubles?
void set_velset(MU_State_Manager *state_manager, float x, float y)
{
	state_manager->statedef_list[state_manager->total_statedef - 1].velset_x = x;
	state_manager->statedef_list[state_manager->total_statedef - 1].velset_y = y;
}