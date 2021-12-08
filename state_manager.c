#include "mu_headers.h"

// Array copied from original code
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
	MU_State_Manager *state_manager = (MU_State_Manager*) malloc(sizeof(MU_State_Manager));
	memset(state_manager, 0, sizeof(MU_State_Manager));

	return state_manager;
}

void reset_state_manager(MU_State_Manager *state_manager)
{
	state_manager->n_total_statedef = 0;
	state_manager->n_total_statedef_size = 100;
	state_manager->n_current_inst = 0;
	state_manager->lp_statedef_list = (PL_Statedef*) mu_alloc(state_manager->state_allocator, sizeof(PL_Statedef) * state_manager->n_total_statedef_size);
	state_manager->n_current_trigger = 0;
}

void realloc_statedef(MU_State_Manager *state_manager, u16 index)
{
	state_manager->lp_statedef_list[index].lp_state = (PL_State*) mu_realloc(state_manager->state_allocator, state_manager->lp_statedef_list[index].lp_state, sizeof(PL_State) * state_manager->lp_statedef_list[index].n_how_many_state);

	for(int i = 0; i < state_manager->lp_statedef_list[index].n_how_many_state; i++)
	{
		state_manager->lp_statedef_list[index].lp_state[i].triggers = (PL_Trigger*) mu_realloc(state_manager->state_allocator, state_manager->lp_statedef_list[index].lp_state[i].triggers, sizeof(PL_Trigger) * state_manager->lp_statedef_list[index].lp_state[i].n_how_many_triggers);
	}
}

void realloc_state_manager(MU_State_Manager *state_manager)
{
	realloc_statedef(state_manager, state_manager->n_total_statedef - 1);
	state_manager->lp_statedef_list = (PL_Statedef*) mu_realloc(state_manager->state_allocator, state_manager->lp_statedef_list, sizeof(PL_Statedef) * state_manager->n_total_statedef);
}

bool is_statedef_available(MU_State_Manager *state_manager, s32 n_statedef_num)
{
	for(s32 i = 0; i < state_manager->n_total_statedef; i++)
	{
		if(state_manager->lp_statedef_list[i].state_number == n_statedef_num)
		{
			// Make this state invalidate to overwrite it
			state_manager->lp_statedef_list[i].state_number = -11; // Not sure why -11 specifically
			return true;
		}
	}
	// Else
	return false;
}

void set_default_statedef(MU_State_Manager *state_manager)
{
	state_manager->lp_statedef_list[state_manager->n_total_statedef - 1].type = STAND;
	state_manager->lp_statedef_list[state_manager->n_total_statedef - 1].movetype = IDLE;
	state_manager->lp_statedef_list[state_manager->n_total_statedef - 1].physics = NONE;
	state_manager->lp_statedef_list[state_manager->n_total_statedef - 1].n_anim = -1;
	state_manager->lp_statedef_list[state_manager->n_total_statedef - 1].velset.x = -3333;
	state_manager->lp_statedef_list[state_manager->n_total_statedef - 1].velset.y = -3333;
	state_manager->lp_statedef_list[state_manager->n_total_statedef - 1].b_ctrl = -1;
	state_manager->lp_statedef_list[state_manager->n_total_statedef - 1].n_poweradd = -3333;
	state_manager->lp_statedef_list[state_manager->n_total_statedef - 1].b_facep2 = false;
	state_manager->lp_statedef_list[state_manager->n_total_statedef - 1].n_juggle = -3333;
	state_manager->lp_statedef_list[state_manager->n_total_statedef - 1].b_hitdef_persist = false;
	state_manager->lp_statedef_list[state_manager->n_total_statedef - 1].b_hitcount_persist = false;
	state_manager->lp_statedef_list[state_manager->n_total_statedef - 1].n_spr_priority = 255;
}

void add_statedef(MU_State_Manager *state_manager, s32 n_statedef_num)
{
	if(is_statedef_available(state_manager, n_statedef_num))
	{
		mu_log_message("Overwriting Statedef %i", n_statedef_num);
	}

	if(state_manager->n_total_statedef != 0)
	{
		realloc_statedef(state_manager, state_manager->n_total_statedef - 1);
	}


	if(state_manager->n_total_statedef > state_manager->n_total_statedef_size - 1)
	{
		state_manager->n_total_statedef_size += 100;
		state_manager->lp_statedef_list = (PL_Statedef*) mu_realloc(state_manager->state_allocator, state_manager->lp_statedef_list, sizeof(PL_Statedef) * state_manager->n_total_statedef_size);
	}

	state_manager->lp_statedef_list[state_manager->n_total_statedef].state_number = n_statedef_num;

	state_manager->n_total_state = 0;
	state_manager->n_total_state_size = 100;
	state_manager->lp_statedef_list[state_manager->n_total_statedef].lp_state = (PL_State*) mu_alloc(state_manager->state_allocator, sizeof(PL_State) * state_manager->n_total_state_size);

	state_manager->n_total_statedef++;

	set_default_statedef(state_manager);
}

void add_state(MU_State_Manager *state_manager, s32 n_state_num, char *str_some_number)
{
	// Reallocate extra memory for states if we run out
	if(state_manager->n_total_state > state_manager->n_total_state_size - 1)
	{
		state_manager->n_total_state_size += 100;
		state_manager->lp_statedef_list[state_manager->n_total_statedef - 1].lp_state = (PL_State*) mu_realloc(state_manager->state_allocator, state_manager->lp_statedef_list[state_manager->n_total_statedef].lp_state, sizeof(PL_State) * state_manager->n_total_state_size);
	}

	state_manager->lp_statedef_list[state_manager->n_total_statedef - 1].lp_state[state_manager->n_total_state].n_state_number = n_state_num;

	state_manager->n_current_trigger = 0;
	state_manager->n_trigger_list_size = 100;

	state_manager->lp_statedef_list[state_manager->n_total_statedef - 1].lp_state[state_manager->n_total_state].triggers = (PL_Trigger*) mu_alloc(state_manager->state_allocator, sizeof(PL_Trigger) * state_manager->n_trigger_list_size);

	state_manager->n_total_state++;
	state_manager->lp_statedef_list[state_manager->n_total_statedef - 1].n_how_many_state = state_manager->n_total_state;

	state_manager->n_current_inst = 0;
}

void add_trigger_to_state(MU_State_Manager *state_manager, u8 n_type)
{
	if(state_manager->n_current_trigger > state_manager->n_trigger_list_size)
	{
		mu_log_message("State Manager: You have 100 triggers...You should not have 100 triggers...");
	}

	// Copy the instruction to the trigger
	// Create a new instance to store the instruction

	state_manager->lp_statedef_list[state_manager->n_total_statedef - 1].lp_state[state_manager->n_total_state - 1].triggers[state_manager->n_current_trigger].n_trigger_type = n_type;
	state_manager->lp_statedef_list[state_manager->n_total_statedef - 1].lp_state[state_manager->n_total_state - 1].triggers[state_manager->n_current_trigger].p_inst = (Instruction*) mu_alloc(state_manager->state_allocator, sizeof(Instruction) * state_manager->n_current_inst);

	memcpy(state_manager->lp_statedef_list[state_manager->n_total_statedef - 1].lp_state[state_manager->n_total_state - 1].triggers[state_manager->n_current_trigger].p_inst, state_manager->p_inst, sizeof(Instruction) * state_manager->n_current_inst);

	state_manager->n_current_trigger++;

	state_manager->lp_statedef_list[state_manager->n_total_statedef - 1].lp_state[state_manager->n_total_state - 1].n_how_many_triggers = state_manager->n_current_trigger;

	state_manager->n_current_inst = 0;
}

void add_instruction(MU_State_Manager *state_manager, u16 n_opcode, float value, const char *str_value)
{
	mu_log_message("State Manager: add_instruction = %s %f %s(%i)", str_opcode[n_opcode], value, str_value, n_opcode);

	state_manager->p_inst[state_manager->n_current_inst].n_opcode = n_opcode;
	state_manager->p_inst[state_manager->n_current_inst].value = value;
	state_manager->p_inst[state_manager->n_current_inst].str_value = NULL;

	// Only add a string if we need one
	if(str_value[0] != '#')
	{
		state_manager->p_inst[state_manager->n_current_inst].str_value = malloc(sizeof(char) * strlen(str_value) + 1);
		strcpy(state_manager->p_inst[state_manager->n_current_inst].str_value, str_value);
	}

	state_manager->n_current_inst++;
}

void exchange_inst(MU_State_Manager *state_manager)
{
	Instruction temp;

	memcpy(&temp, &state_manager->p_inst[state_manager->n_current_inst - 1], sizeof(temp));
	memcpy(&state_manager->p_inst[state_manager->n_current_inst - 1], &state_manager->p_inst[state_manager->n_current_inst - 2], sizeof(temp));
	memcpy(&state_manager->p_inst[state_manager->n_current_inst - 2], &temp, sizeof(temp));
}

PL_Statedef *get_statedef(MU_State_Manager *state_manager, int n_state_number)
{
	for(u16 i = 0; i < state_manager->n_total_statedef; i++)
	{
		if(state_manager->lp_statedef_list[i].state_number == n_state_number)
		{
			return &state_manager->lp_statedef_list[i];
		}
	}
	mu_log_message("State Manager: Statedef %i not found", n_state_number);
	return NULL;
}

PL_Statedef *get_current_statedef(MU_State_Manager *state_manager)
{
	return &state_manager->lp_statedef_list[state_manager->n_total_statedef - 1];
}

void set_statedef_type(MU_State_Manager *state_manager, u8 n_type)
{
	state_manager->lp_statedef_list[state_manager->n_total_statedef - 1].type = n_type;
}

void set_state_movetype(MU_State_Manager *state_manager, u8 n_type)
{
	state_manager->lp_statedef_list[state_manager->n_total_statedef - 1].movetype = n_type;
}

void set_state_physics_type(MU_State_Manager *state_manager, u8 n_type)
{
	state_manager->lp_statedef_list[state_manager->n_total_statedef - 1].physics = n_type;
}

void set_state_anim(MU_State_Manager *state_manager, s32 n_anim)
{
	state_manager->lp_statedef_list[state_manager->n_total_statedef - 1].n_anim = n_anim;
}

void set_state_ctrl(MU_State_Manager *state_manager, s8 n_ctrl)
{
	state_manager->lp_statedef_list[state_manager->n_total_statedef - 1].b_ctrl = n_ctrl;
}

void set_state_poweradd(MU_State_Manager *state_manager, s16 n_poweradd)
{
	state_manager->lp_statedef_list[state_manager->n_total_statedef - 1].n_poweradd = n_poweradd;
}

void set_state_juggle(MU_State_Manager *state_manager, s16 n_juggle)
{
	state_manager->lp_statedef_list[state_manager->n_total_statedef - 1].n_juggle = n_juggle;
}

void set_state_hitdef_persist(MU_State_Manager *state_manager, bool b_hitdef)
{
	state_manager->lp_statedef_list[state_manager->n_total_statedef - 1].b_hitdef_persist = b_hitdef;
}

void set_movehit_persist(MU_State_Manager *state_manager, bool b_movehit)
{
	state_manager->lp_statedef_list[state_manager->n_total_statedef - 1].b_movehit_persist = b_movehit;
}

void set_state_hitcounter_persist(MU_State_Manager *state_manager, bool b_hitcounter)
{
	state_manager->lp_statedef_list[state_manager->n_total_statedef - 1].b_hitcount_persist = b_hitcounter;
}

void set_spr_priority(MU_State_Manager *state_manager, u8 n_spr_priority)
{
	state_manager->lp_statedef_list[state_manager->n_total_statedef - 1].n_spr_priority = n_spr_priority;
}

void set_state_facep2(MU_State_Manager *state_manager, bool b_facep2)
{
	state_manager->lp_statedef_list[state_manager->n_total_statedef - 1].b_facep2 = b_facep2;
}

void set_velset(MU_State_Manager *state_manager, float x, float y)
{
	state_manager->lp_statedef_list[state_manager->n_total_statedef - 1].velset.x = x;
	state_manager->lp_statedef_list[state_manager->n_total_statedef - 1].velset.y = y;
}