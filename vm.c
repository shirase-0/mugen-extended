#include "mu_headers.h"

VM *vm_init()
{
	VM *vm = (VM*) malloc(sizeof(VM));
	vm->stack = mu_stack_init();
	mu_reset_stack(vm->stack);
	vm->n_current_ins = 0;
	vm_init_func_table(vm);

	return vm;
}

void vm_set_players(VM *vm, Player *p1, Player *p2)
{
	vm->p1 = p1;
	vm->p2 = p2;
}

void vm_init_func_table(VM *vm)
{
	vm->p_func_table[0] = &vm_push_value;
	vm->p_func_table[1] = &vm_pop_value;
	vm->p_func_table[2] = &vm_add_op;
	vm->p_func_table[3] = &vm_sub_op;
	vm->p_func_table[4] = &vm_mul_op;
	vm->p_func_table[5] = &vm_div_op;
	vm->p_func_table[6] = &vm_equal_op;
	vm->p_func_table[7] = &vm_not_equal;
	vm->p_func_table[8] = &vm_less;
	vm->p_func_table[9] = &vm_greater;
	vm->p_func_table[10] = &vm_less_equal;
	vm->p_func_table[11] = &vm_greater_equal;
	vm->p_func_table[12] = &vm_inter_val_op1;
	vm->p_func_table[13] = &vm_inter_val_op2;
	vm->p_func_table[14] = &vm_inter_val_op3;
	vm->p_func_table[15] = &vm_inter_val_op4;
	vm->p_func_table[16] = &vm_inter_val_op5;
	vm->p_func_table[17] = &vm_inter_val_op6;
	vm->p_func_table[18] = &vm_inter_val_op7;
	vm->p_func_table[19] = &vm_inter_val_op8;
	vm->p_func_table[20] = &vm_log_not;
	vm->p_func_table[21] = &vm_log_and;
	vm->p_func_table[22] = &vm_log_or;
	vm->p_func_table[23] = &vm_log_xor;
	vm->p_func_table[24] = &vm_not;
	vm->p_func_table[25] = &vm_and;
	vm->p_func_table[26] = &vm_or;
	vm->p_func_table[27] = &vm_xor;
	vm->p_func_table[28] = &vm_square;
	vm->p_func_table[29] = &vm_neg;
	vm->p_func_table[30] = &vm_abs;
	vm->p_func_table[31] = &vm_acos;
	vm->p_func_table[32] = &vm_alive;
	vm->p_func_table[33] = &vm_anim;
	vm->p_func_table[34] = &vm_anim_elem;
	//   vm->p_func_table[35] = &vm_save;
	//   vm->p_func_table[36] = &vm_restore;
	vm->p_func_table[35] = &vm_anim_elem_no;
	vm->p_func_table[36] = &vm_anim_elem_time;
	vm->p_func_table[37] = &vm_anim_exist;
	vm->p_func_table[38] = &vm_anim_time;
	vm->p_func_table[39] = &vm_asin;
	vm->p_func_table[40] = &vm_atan;
	vm->p_func_table[41] = &vm_author_name;
	vm->p_func_table[42] = &vm_back_edge_body_dist;
	vm->p_func_table[43] = &vm_back_edge_dist;
	vm->p_func_table[44] = &vm_can_recover;
	vm->p_func_table[45] = &vm_ceil;
	vm->p_func_table[46] = &vm_command;
	vm->p_func_table[47] = &vm_const;
	vm->p_func_table[48] = &vm_cos;
	vm->p_func_table[49] = &vm_ctrl;
	vm->p_func_table[52] = &vm_draw_game;
	vm->p_func_table[53] = 0;//&CVirtualMachine::E;
	vm->p_func_table[54] = &vm_exp;
	vm->p_func_table[55] = &vm_facing;
	vm->p_func_table[56] = &vm_floor;
	vm->p_func_table[57] = &vm_front_edge_body_dist;
	vm->p_func_table[58] = &vm_front_edge_dist;
	vm->p_func_table[59] = &vm_fvar;
	vm->p_func_table[60] = &vm_game_time;
	vm->p_func_table[61] = &vm_get_hit_var;
	vm->p_func_table[62] = &vm_hit_count;
	vm->p_func_table[63] = 0;//HitDefAttr
	vm->p_func_table[64] = &vm_hit_fall;
	vm->p_func_table[65] = &vm_hit_over;
	vm->p_func_table[66] = &vm_hit_pause_time;
	vm->p_func_table[67] = &vm_hit_shake_over;
	vm->p_func_table[68] = &vm_hit_vel;
	vm->p_func_table[69] = &vm_player_ident;
	vm->p_func_table[70] = &vm_if_else;
	vm->p_func_table[71] = &vm_in_guard_dist;
	vm->p_func_table[72] = &vm_is_helper;
	vm->p_func_table[73] = &vm_is_home_team;
	vm->p_func_table[74] = &vm_life;
	vm->p_func_table[75] = &vm_life_max;
	vm->p_func_table[76] = &vm_log_n;
	vm->p_func_table[77] = &vm_log;
	vm->p_func_table[78] = &vm_lose;
	vm->p_func_table[79] = &vm_match_no;
	vm->p_func_table[80] = &vm_match_over;
	vm->p_func_table[81] = &vm_move_contact;
	vm->p_func_table[82] = &vm_move_guarded;
	vm->p_func_table[83] = &vm_move_hit;
	vm->p_func_table[84] = &vm_move_type;
	vm->p_func_table[85] = &vm_move_reversed;
	vm->p_func_table[86] = &vm_name;
	vm->p_func_table[87] = &vm_num_enemy;
	vm->p_func_table[88] = &vm_num_explod;
	vm->p_func_table[89] = &vm_num_helper;
	vm->p_func_table[90] = &vm_num_partner;
	vm->p_func_table[91] = &vm_num_proj;
	vm->p_func_table[92] = &vm_num_proj_id;
	vm->p_func_table[93] = &vm_num_target;
	vm->p_func_table[94] = &vm_p2_body_dist;
	vm->p_func_table[95] = &vm_p2_dist;
	vm->p_func_table[96] = &vm_p2_life;
	vm->p_func_table[97] = &vm_p2_move_type;
	vm->p_func_table[98] = &vm_p2_name;
	vm->p_func_table[99] = &vm_p2_state_no;
	vm->p_func_table[100] = &vm_p2_state_type;
	vm->p_func_table[101] = &vm_p3_name;
	vm->p_func_table[102] = &vm_p4_name;
	vm->p_func_table[103] = &vm_pal_no;
	vm->p_func_table[104] = &vm_parent_dist;
	vm->p_func_table[105] = &vm_pi;
	vm->p_func_table[106] = &vm_pos;
	vm->p_func_table[107] = &vm_power;
	vm->p_func_table[108] = &vm_power_max;
	vm->p_func_table[109] = 0;//PlayerIDExist
	vm->p_func_table[110] = 0;//PrevStateNo
	vm->p_func_table[111] = &vm_proj_cancel_time;
	vm->p_func_table[112] = &vm_proj_contact;
	vm->p_func_table[113] = &vm_proj_contact_time;
	vm->p_func_table[114] = &vm_proj_guarded;
	vm->p_func_table[115] = &vm_proj_guarded_time;
	vm->p_func_table[116] = 0;//ProjHit
	vm->p_func_table[117] = 0;//ProjHitTime
	vm->p_func_table[118] = &vm_random;
	vm->p_func_table[119] = &vm_root_dist;
	vm->p_func_table[120] = &vm_round_no;
	vm->p_func_table[121] = &vm_rounds_existed;
	vm->p_func_table[122] = &vm_round_state;
	vm->p_func_table[123] = &vm_screen_pos;
	vm->p_func_table[124] = &vm_self_anim_exist;
	vm->p_func_table[125] = &vm_sin;
	vm->p_func_table[126] = &vm_state_no;
	vm->p_func_table[127] = &vm_state_type;
	vm->p_func_table[128] = &vm_sys_fvar;
	vm->p_func_table[129] = &vm_sys_var;
	vm->p_func_table[130] = &vm_tan;
	vm->p_func_table[131] = &vm_team_mode;
	vm->p_func_table[132] = &vm_team_side;
	vm->p_func_table[133] = &vm_ticks_per_second;
	vm->p_func_table[134] = &vm_time;
	vm->p_func_table[135] = 0;//TimeMod
	vm->p_func_table[136] = &vm_uniq_hit_count;
	vm->p_func_table[137] = &vm_var;
	vm->p_func_table[138] = &vm_vel;
	vm->p_func_table[139] = &vm_win;
}

float vm_execute(VM *vm, Instruction *p_inst)
{
	vm->n_current_ins = 0;

	// Reset the stack for the next execution
	mu_reset_stack(vm->stack);

	while(p_inst[vm->n_current_ins].n_opcode != OP_STOP)
	{
		vm->p_current_ins = &p_inst[vm->n_current_ins];

		// Execute the function
		(vm->p_func_table[p_inst[vm->n_current_ins].n_opcode])(vm);
		vm->n_current_ins++;
	}
	vm_pop_value(vm);

	return vm->pop_item.value;
}

void vm_pop_value(VM *vm)
{
	vm->pop_item = mu_pop(vm->stack);
}

void vm_push_value(VM *vm)
{
	mu_push(vm->stack, vm->p_current_ins->value, vm->p_current_ins->str_value);
}

// x + y
void vm_add_op(VM *vm)
{
	vm_pop_value(vm);
	vm->temp2 = vm->pop_item.value;
	vm_pop_value(vm);
	vm->temp1 = vm->pop_item.value;

	vm->pop_item.value = vm->temp1 + vm->temp2;
	mu_log_message("vm_add_op: %f + %f = %f", vm->temp1, vm->temp2, vm->pop_item.value);
	mu_push(vm->stack, vm->pop_item.value, "#");
}

// x- y
void vm_sub_op(VM *vm)
{
	vm_pop_value(vm);
	vm->temp2 = vm->pop_item.value;
	vm_pop_value(vm);
	vm->temp1 = vm->pop_item.value;

	vm->pop_item.value = vm->temp1 - vm->temp2;
	mu_log_message("vm_sub_op: %f - %f = %f", vm->temp1, vm->temp2, vm->pop_item.value);
	mu_push(vm->stack, vm->pop_item.value, "#");
}

// x * y
void vm_mul_op(VM *vm)
{
	vm_pop_value(vm);
	vm->temp2 = vm->pop_item.value;
	vm_pop_value(vm);
	vm->temp1 = vm->pop_item.value;

	vm->pop_item.value = vm->temp1 * vm->temp2;
	mu_log_message("vm_sub_op: %f * %f = %f", vm->temp1, vm->temp2, vm->pop_item.value);
	mu_push(vm->stack, vm->pop_item.value, "#");
}

// x / y
void vm_div_op(VM *vm)
{
	vm_pop_value(vm);
	vm->temp2 = vm->pop_item.value;
	vm_pop_value(vm);
	vm->temp1 = vm->pop_item.value;

	vm->pop_item.value = vm->temp1 / vm->temp2;
	mu_log_message("vm_sub_op: %f / %f = %f", vm->temp1, vm->temp2, vm->pop_item.value);
	mu_push(vm->stack, vm->pop_item.value, "#");
}

// x == y
void vm_equal_op(VM *vm)
{
	char str_temp[50], str_temp1[50];

	vm_pop_value(vm);
	vm->temp2 = vm->pop_item.value;
	strcpy(str_temp, vm->pop_item.stack_string);

	vm_pop_value(vm);
	vm->temp1 = vm->pop_item.value;
	strcpy(str_temp1, vm->pop_item.stack_string);

	if(str_temp[0] != '#' && str_temp1[0] != '#')
	{
		if(strcmp(str_temp, str_temp1) == 0)
		{
			mu_push(vm->stack, 1, "#");
		}
		else
		{
			mu_push(vm->stack, 0, "#");
		}
		return;
	}

	if(vm->temp1 == vm->temp2)
	{
		vm->pop_item.value = 1;
	}
	else
	{
		vm->pop_item.value = 0;
	}

	mu_log_message("vm_equal_op: %f == %f", vm->temp1, vm->temp2);
	mu_push(vm->stack, vm->pop_item.value, "#");
}

// x != y
void vm_not_equal(VM *vm)
{
	vm_pop_value(vm);
	vm->temp2 = vm->pop_item.value;
	vm_pop_value(vm);
	vm->temp1 = vm->pop_item.value;

	if(vm->temp1 != vm->temp2)
	{
		vm->pop_item.value = 1;
	}
	else
	{
		vm->pop_item.value = 0;
	}

	mu_log_message("vm_not_equal: %f != %f", vm->temp1, vm->temp2);
	mu_push(vm->stack, vm->pop_item.value, "#");
}

// x < y
void vm_less(VM *vm)
{
	vm_pop_value(vm);
	vm->temp2 = vm->pop_item.value;
	vm_pop_value(vm);
	vm->temp1 = vm->pop_item.value;

	if(vm->temp1 < vm->temp2)
	{
		vm->pop_item.value = 1;
	}
	else
	{
		vm->pop_item.value = 0;
	}

	mu_log_message("vm_less: %f < %f", vm->temp1, vm->temp2);
	mu_push(vm->stack, vm->pop_item.value, "#");
}

// x > y
void vm_greater(VM *vm)
{
	vm_pop_value(vm);
	vm->temp2 = vm->pop_item.value;
	vm_pop_value(vm);
	vm->temp1 = vm->pop_item.value;

	if(vm->temp1 > vm->temp2)
	{
		vm->pop_item.value = 1;
	}
	else
	{
		vm->pop_item.value = 0;
	}

	mu_log_message("vm_greater: %f > %f", vm->temp1, vm->temp2);
	mu_push(vm->stack, vm->pop_item.value, "#");
}

// x <= y
void vm_less_equal(VM *vm)
{
	vm_pop_value(vm);
	vm->temp2 = vm->pop_item.value;
	vm_pop_value(vm);
	vm->temp1 = vm->pop_item.value;

	if(vm->temp1 <= vm->temp2)
	{
		vm->pop_item.value = 1;
	}
	else
	{
		vm->pop_item.value = 0;
	}

	mu_log_message("vm_less_equal: %f <= %f", vm->temp1, vm->temp2);
	mu_push(vm->stack, vm->pop_item.value, "#");
}

// x >= y
void vm_greater_equal(VM *vm)
{
	vm_pop_value(vm);
	vm->temp2 = vm->pop_item.value;
	vm_pop_value(vm);
	vm->temp1 = vm->pop_item.value;

	if(vm->temp1 >= vm->temp2)
	{
		vm->pop_item.value = 1;
	}
	else
	{
		vm->pop_item.value = 0;
	}

	mu_log_message("vm_greater_equal: %f >= %f", vm->temp1, vm->temp2);
	mu_push(vm->stack, vm->pop_item.value, "#");
}

// x = [y, z]
void vm_inter_val_op1(VM *vm)
{
	vm_pop_value(vm);
	vm->temp3 = vm->pop_item.value;
	vm_pop_value(vm);
	vm->temp2 = vm->pop_item.value;
	vm_pop_value(vm);
	vm->temp1 = vm->pop_item.value;

	vm->pop_item.value = 0;

	if((vm->temp1 >= vm->temp2) && (vm->temp1 <= vm->temp3))
	{
		vm->pop_item.value = 1;
	}

	mu_log_message("vm_inter_val_op1: %2f >= %2f, %2f <= %2f", vm->temp1, vm->temp2, vm->temp1, vm->temp3);
	mu_push(vm->stack, vm->pop_item.value, "#");
}

// x = [y, z)
void vm_inter_val_op2(VM *vm)
{
	vm_pop_value(vm);
	vm->temp3 = vm->pop_item.value;
	vm_pop_value(vm);
	vm->temp2 = vm->pop_item.value;
	vm_pop_value(vm);
	vm->temp1 = vm->pop_item.value;

	vm->pop_item.value = 0;

	if((vm->temp1 >= vm->temp2) && (vm->temp1 < vm->temp3))
	{
		vm->pop_item.value = 1;
	}
	mu_log_message("vm_inter_val_op2: %2f >= %2f, %2f < %2f", vm->temp1, vm->temp2, vm->temp1, vm->temp3);
	mu_push(vm->stack, vm->pop_item.value, "#");
}

// x = (y, z]
void vm_inter_val_op3(VM *vm)
{
	vm_pop_value(vm);
	vm->temp3 = vm->pop_item.value;
	vm_pop_value(vm);
	vm->temp2 = vm->pop_item.value;
	vm_pop_value(vm);
	vm->temp1 = vm->pop_item.value;

	vm->pop_item.value = 0;

	if((vm->temp1 > vm->temp2) && (vm->temp1 <= vm->temp3))
	{
		vm->pop_item.value = 1;
	}
	mu_log_message("vm_inter_val_op3: %2f > %2f, %2f <= %2f", vm->temp1, vm->temp2, vm->temp1, vm->temp3);
	mu_push(vm->stack, vm->pop_item.value, "#");
}

// x = (y, z)
void vm_inter_val_op4(VM *vm)
{
	vm_pop_value(vm);
	vm->temp3 = vm->pop_item.value;
	vm_pop_value(vm);
	vm->temp2 = vm->pop_item.value;
	vm_pop_value(vm);
	vm->temp1 = vm->pop_item.value;

	vm->pop_item.value = 0;

	if((vm->temp1 > vm->temp2) && (vm->temp1 < vm->temp3))
	{
		vm->pop_item.value = 1;
	}
	mu_log_message("vm_inter_val_op4: %2f > %2f, %2f < %2f", vm->temp1, vm->temp2, vm->temp1, vm->temp3);
	mu_push(vm->stack, vm->pop_item.value, "#");
}

// Note to self: inter_val_ops 5-8 are in the reverse order of 1-4
// This is probably unintentional, and will likely need to be fixed

// x != (y, z)
void vm_inter_val_op5(VM *vm)
{
	vm_pop_value(vm);
	vm->temp3 = vm->pop_item.value;
	vm_pop_value(vm);
	vm->temp2 = vm->pop_item.value;
	vm_pop_value(vm);
	vm->temp1 = vm->pop_item.value;

	vm->pop_item.value = 0;

	// (x < y) || (x > z)
	if((vm->temp1 < vm->temp2) || (vm->temp1 > vm->temp3))
	{
		vm->pop_item.value = 1;
	}
	mu_log_message("vm_inter_val_op5: %2f < %2f || %2f > %2f", vm->temp1, vm->temp2, vm->temp1, vm->temp3);
	mu_push(vm->stack, vm->pop_item.value, "#");
}

// x != (y, z]
void vm_inter_val_op6(VM *vm)
{
	vm_pop_value(vm);
	vm->temp3 = vm->pop_item.value;
	vm_pop_value(vm);
	vm->temp2 = vm->pop_item.value;
	vm_pop_value(vm);
	vm->temp1 = vm->pop_item.value;

	vm->pop_item.value = 0;

	// (x < y) || (x >= z)
	if((vm->temp1 < vm->temp2) || (vm->temp1 >= vm->temp3))
	{
		vm->pop_item.value = 1;
	}
	mu_log_message("vm_inter_val_op6: %2f < %2f || %2f >= %2f", vm->temp1, vm->temp2, vm->temp1, vm->temp3);
	mu_push(vm->stack, vm->pop_item.value, "#");
}

// x != [y, z)
void vm_inter_val_op7(VM *vm)
{
	vm_pop_value(vm);
	vm->temp3 = vm->pop_item.value;
	vm_pop_value(vm);
	vm->temp2 = vm->pop_item.value;
	vm_pop_value(vm);
	vm->temp1 = vm->pop_item.value;

	vm->pop_item.value = 0;

	// (x <= y) || (x > z)
	if((vm->temp1 <= vm->temp2) || (vm->temp1 > vm->temp3))
	{
		vm->pop_item.value = 1;
	}
	mu_log_message("vm_inter_val_op7: %2f <= %2f || %2f > %2f", vm->temp1, vm->temp2, vm->temp1, vm->temp3);
	mu_push(vm->stack, vm->pop_item.value, "#");
}

// x != [y, z]
void vm_inter_val_op8(VM *vm)
{
	vm_pop_value(vm);
	vm->temp3 = vm->pop_item.value;
	vm_pop_value(vm);
	vm->temp2 = vm->pop_item.value;
	vm_pop_value(vm);
	vm->temp1 = vm->pop_item.value;

	vm->pop_item.value = 0;

	// (x <= y) || (x >= z)
	if((vm->temp1 <= vm->temp2) || (vm->temp1 >= vm->temp3))
	{
		vm->pop_item.value = 1;
	}
	mu_log_message("vm_inter_val_op8: %2f <= %2f || %2f >= %2f", vm->temp1, vm->temp2, vm->temp1, vm->temp3);
	mu_push(vm->stack, vm->pop_item.value, "#");
}

// !(x > 0)
void vm_log_not(VM *vm)
{
	vm_pop_value(vm);
	vm->temp1 = vm->pop_item.value;

	if(!(vm->temp1 > 0))
	{
		mu_log_message("vm_log_not: resolved to 1");
		mu_push(vm->stack, 1, "#");
	}
	else
	{
		mu_log_message("vm_log_not: resolved to 0");
		mu_push(vm->stack, 0, "#");
	}
}

// x > 0 && y > 0
void vm_log_and(VM *vm)
{
	vm_pop_value(vm);
	vm->temp2 = vm->pop_item.value;
	vm_pop_value(vm);
	vm->temp1 = vm->pop_item.value;

	if(vm->temp1 > 0 && vm->temp2 > 0)
	{
		mu_log_message("vm_log_and: resolved to 1");
		mu_push(vm->stack, 1, "#");
	}
	else
	{
		mu_log_message("vm_log_and: resolved to 0");
		mu_push(vm->stack, 0, "#");
	}
}

// x > 0 || y > 0
void vm_log_or(VM *vm)
{
	vm_pop_value(vm);
	vm->temp2 = vm->pop_item.value;
	vm_pop_value(vm);
	vm->temp1 = vm->pop_item.value;

	if(vm->temp1 > 0 || vm->temp2 > 0)
	{
		mu_log_message("vm_log_or: resolved to 1");
		mu_push(vm->stack, 1, "#");
	}
	else
	{
		mu_log_message("vm_log_or: resolved to 0");
		mu_push(vm->stack, 0, "#");
	}
}

// (x > 0 && y == 0) || (x == 0 && y > 0)
void vm_log_xor(VM *vm)
{
	vm_pop_value(vm);
	vm->temp2 = vm->pop_item.value;
	vm_pop_value(vm);
	vm->temp1 = vm->pop_item.value;

	if(vm->temp1 > 0 && vm->temp2 == 0)
	{
		mu_log_message("vm_log_xor: resolved to 1");
		mu_push(vm->stack, 1, "#");
	}
	else if(vm->temp1 == 0 && vm->temp2 > 0)
	{
		mu_log_message("vm_log_xor: resolved to 1");
		mu_push(vm->stack, 1, "#");
	}
	else
	{
		mu_log_message("vm_log_xor: resolved to 0");
		mu_push(vm->stack, 0, "#");
	}
}

// Bitwise not
void vm_not(VM *vm)
{
	vm_pop_value(vm);
	vm->temp1 = vm->pop_item.value;

	vm->temp1 = ~(int)vm->temp1;
	mu_log_message("vm_not: resolved to %f", vm->temp1);
	mu_push(vm->stack, vm->temp1, "#");
}

// Bitwise and
void vm_and(VM *vm)
{
	vm_pop_value(vm);
	vm->temp2 = vm->pop_item.value;
	vm_pop_value(vm);
	vm->temp1 = vm->pop_item.value;

	vm->temp1 = (int)vm->temp1 & (int)vm->temp2;
	mu_log_message("vm_and: resolved to %f", vm->temp1);
	mu_push(vm->stack, vm->temp1, "#");
}

// Bitwise or
void vm_or(VM *vm)
{
	vm_pop_value(vm);
	vm->temp2 = vm->pop_item.value;
	vm_pop_value(vm);
	vm->temp1 = vm->pop_item.value;

	vm->temp1 = (int)vm->temp1 | (int)vm->temp2;
	mu_log_message("vm_or: resolved to %f", vm->temp1);
	mu_push(vm->stack, vm->temp1, "#");
}

// Bitwise xor
void vm_xor(VM *vm)
{
	vm_pop_value(vm);
	vm->temp2 = vm->pop_item.value;
	vm_pop_value(vm);
	vm->temp1 = vm->pop_item.value;

	vm->temp1 = (int)vm->temp1 ^ (int)vm->temp2;
	mu_log_message("vm_xor: resolved to %f", vm->temp1);
	mu_push(vm->stack, vm->temp1, "#");
}

// x^y
void vm_square(VM *vm)
{
	vm_pop_value(vm);
	vm->temp2 = vm->pop_item.value;
	vm_pop_value(vm);
	vm->temp1 = vm->pop_item.value;

	vm->temp1 = pow(vm->temp1, vm->temp2);
	mu_log_message("vm_square: resolved to %f", vm->temp1);
	mu_push(vm->stack, vm->temp1, "#");
}

// -x
void vm_neg(VM *vm)
{
	vm_pop_value(vm);
	vm->temp1 = vm->pop_item.value;

	vm->temp1 = -vm->temp1;
	mu_log_message("vm_neg: resolved to %f", vm->temp1);
	mu_push(vm->stack, vm->temp1, "#");
}

// |x|
void vm_abs(VM *vm)
{
	vm_pop_value(vm);
	vm->temp1 = vm->pop_item.value;

	vm->temp1 = fabs(vm->temp1);
	mu_log_message("vm_abs: resolved to %f", vm->temp1);
	mu_push(vm->stack, vm->temp1, "#");
}

// acos(x)
void vm_acos(VM *vm)
{
	vm_pop_value(vm);
	vm->temp1 = vm->pop_item.value;

	vm->temp1 = acos(vm->temp1);
	mu_log_message("vm_acos: resolved to %f", vm->temp1);
	mu_push(vm->stack, vm->temp1, "#");
}

// Is player alive?
void vm_alive(VM *vm)
{
	if(vm->p1->b_alive)
	{
		mu_log_message("vm_alive: player1 is alive");
		mu_push(vm->stack, 1.0f, "#");
	}
	else
	{
		mu_log_message("vm_alive: player1 is not alive");
		mu_push(vm->stack, 0.0f, "#");
	}
}

// Push anim to stack
void vm_anim(VM *vm)
{
	mu_log_message("vm_anim: pushed anim number %i", vm->p1->n_anim_number);
	mu_push(vm->stack, (float)vm->p1->n_anim_number, "#");
}

void vm_anim_elem(VM *vm)
{
	Action_Element *t_action_element;
	t_action_element = vm->p1->sff_manager->anim;

	// Time since this element
	mu_push(vm->stack, (float)t_action_element->n_current_time, "#");
	// Element number (note first element is not 0, it is 1)
	mu_push(vm->stack, (float)t_action_element->n_current_image + 1, "#");

	mu_log_message("vm_anim_elem: Check this function, has todo");

	/* TODO (#1#): Push the right values to the stack */
}

void vm_anim_elem_no(VM *vm)
{
	Action_Element *t_action_element;
	t_action_element = vm->p1->sff_manager->anim;

	int n_time_offset = 0;
	int n_time_check = 0;
	int n_time_to_element = 0;

	vm_pop_value(vm);
	n_time_offset = (int)vm->pop_item.value;

	n_time_check = n_time_offset + t_action_element->n_current_time;

	if(n_time_check > t_action_element->n_complete_anim_time)
	{
		//Should return SFalse
		mu_log_message("vm_anim_elem_no: pushing 0 to stack and returning");
		mu_push(vm->stack, 0, "#");
		return;
	}

	// Now check on which element the time offset is
	for(int i = 0; i < t_action_element->n_number_of_elements; i++)
	{
		n_time_to_element += t_action_element->animation_element[i].n_during_time;
		if(n_time_to_element >= n_time_check)
		{
			// Now we have found which element it would be at this time
			mu_log_message("vm_anim_elem_no: element found");
			mu_push(vm->stack, (float)i + 1, "#");
			return;
		}
	}
	mu_log_message("vm_anim_elem_no: element not found, pushing 0 to stack");
	mu_push(vm->stack, 0, "#");
}

void vm_anim_elem_time(VM *vm)
{
	Action_Element *t_action_element;
	t_action_element = vm->p1->sff_manager->anim;
	int n_element_to_check;

	vm_pop_value(vm);
	n_element_to_check = (int)vm->pop_item.value;

	// Note first element is not 0, it is 1
	if(n_element_to_check == t_action_element->n_current_image + 1 && !t_action_element->b_looped)
	{
		mu_log_message("vm_anim_elem_time: pushing anim elem time");
		mu_push(vm->stack, (float)t_action_element->n_current_image_time, "#");
	}
	else
	{
		//Should return SFalse
		mu_log_message("vm_anim_elem_time: unable to get anim elem time");
		mu_push(vm->stack, -1, "#");
	}
}

void vm_anim_exist(VM *vm)
{
	s32 n_anim;
	vm_pop_value(vm);
	n_anim = (s32)vm->pop_item.value;

	// If != NULL, push true, else push false
	if(is_anim_available(vm->p1, n_anim))
	{
		mu_log_message("vm_anim_exist: anim exists");
		mu_push(vm->stack, 1.0, "#");
	}
	else
	{
		mu_log_message("vm_anim_exist: anim does not exist");
		mu_push(vm->stack, 0.0, "#");
	}
}

void vm_anim_time(VM *vm)
{
	u16 n_anim_time;
	Action_Element *t_action_element;
	t_action_element = vm->p1->sff_manager->anim;

	n_anim_time = t_action_element->n_current_time - t_action_element->n_complete_anim_time;

	mu_log_message("vm_anim_time: pushing anim time");
	mu_push(vm->stack, (float)n_anim_time, "#");
}

// asin(x)
void vm_asin(VM *vm)
{
	vm_pop_value(vm);
	vm->temp1 = vm->pop_item.value;

	vm->temp1 = asin(vm->temp1);

	mu_log_message("vm_asin: resolved to %f", vm->temp1);
	mu_push(vm->stack, vm->temp1, "#");
}

// atan(x)
void vm_atan(VM *vm)
{
	vm_pop_value(vm);
	vm->temp1 = vm->pop_item.value;

	vm->temp1 = atan(vm->temp1);

	mu_log_message("vm_atan: resolved to %f", vm->temp1);
	mu_push(vm->stack, vm->temp1, "#");
}

// Author name
void vm_author_name(VM *vm)
{
	// TODO
	mu_log_message("vm_author_name: todo, not implemented yet");
	/*PrintMessage("AuthorName %s",((CPlayer*)m_p1)->myPlayerData.strAuthor);
    m_Stack.Push(0,((CPlayer*)m_p1)->myPlayerData.strAuthor);*/
}

void vm_back_edge_body_dist(VM *vm)
{
	// TODO: check that this is correct, might need to be changed when the window dimensions are changed
	mu_log_message("vm_back_edge_body_dist: check this function");
	mu_push(vm->stack, 123, "#");
}

void vm_back_edge_dist(VM *vm)
{
	// TODO: check that this is correct, might need to be changed when the window dimensions are changed
	mu_log_message("vm_back_edge_dist: check this function");
	mu_push(vm->stack, 123, "#");
}

void vm_can_recover(VM *vm)
{
	//Push false for the moment
	// TODO: Fix this
	mu_log_message("vm_can_recover: not working as intended, check this function");
	mu_push(vm->stack, 0, "#");
}

// ceil(x)
void vm_ceil(VM *vm)
{
	vm_pop_value(vm);
	vm->temp1 = vm->pop_item.value;

	vm->temp1 = ceil(vm->temp1);

	mu_log_message("vm_ceil: resolved to %f", vm->temp1);
	mu_push(vm->stack, vm->temp1, "#");
}

void vm_command(VM *vm)
{
	mu_log_message("vm_command: pushing command %s", vm->p1->str_command);
	mu_push(vm->stack, 0, vm->p1->str_command);
}

void vm_const(VM *vm)
{
	vm_pop_value(vm);
	vm->temp1 = vm->pop_item.value;

	switch((int)vm->temp1)
	{
		// data.life
		// Note to self: not sure why we're using this second set of pointers to players .-.
		case 0:
			mu_log_message("vm_const: case 0, pushing life");
			mu_push(vm->stack, (float)((Player*)vm->player1)->my_player_const.player_data.n_life, "#");
			break;

		// data.attack
		case 1:
			mu_log_message("vm_const: case 1, pushing attack");
			mu_push(vm->stack, vm->p1->my_player_const.player_data.n_attack, "#");
			break;

		// data.defence
		case 2:
			mu_log_message("vm_const: case 2, pushing defence");
			mu_push(vm->stack, vm->p1->my_player_const.player_data.n_defence, "#");
			break;

		// data.fall.defence_mul
		case 3:
			mu_log_message("vm_const: case 3, pushing fall defence multiplier");
			vm->temp1 = 100 / (vm->p1->my_player_const.player_data.n_fall_defence_up + 100);
			mu_push(vm->stack, vm->temp1, "#");
			break;

		// data.liedown.time
		case 4:
			mu_log_message("vm_const: case 4, pushing liedown time");
			mu_push(vm->stack, vm->p1->my_player_const.player_data.n_liedown_time, "#");
			break;

		// data.airjuggle
		case 5:
			mu_log_message("vm_const: case 5, pushing airjuggle");
			mu_push(vm->stack, vm->p1->my_player_const.player_data.n_air_juggle, "#");
			break;

		// data.sparkno
		case 6:
			mu_log_message("vm_const: case 6, pushing sparkno");
			mu_push(vm->stack, vm->p1->my_player_const.player_data.n_sparkno, "#");
			break;

		// data.guard.sparkno
		case 7:
			mu_log_message("vm_const: case 7, pushing guard sparkno");
			mu_push(vm->stack, vm->p1->my_player_const.player_data.n_guard_sparkno, "#");
			break;

		// data.KO.echo
		case 8:
			mu_log_message("vm_const: case 8, pushing KO echo");
			mu_push(vm->stack, vm->p1->my_player_const.player_data.b_ko_echo, "#");
			break;

		// data.int_persist_index
		case 9:
			mu_log_message("vm_const: case 9, pushing int persist index");
			mu_push(vm->stack, vm->p1->my_player_const.player_data.n_int_persist_index, "#");
			break;

		// data.float_persist_index
		case 10:
			mu_log_message("vm_const: case 10, pushing float persist index");
			mu_push(vm->stack, vm->p1->my_player_const.player_data.n_float_persist_index, "#");
			break;

		// size.xscale
		case 11:
			mu_log_message("vm_const: case 11, pushing player size xscale");
			mu_push(vm->stack, vm->p1->my_player_const.player_size.n_xscale, "#");
			break;

		// size.yscale
		case 12:
			mu_log_message("vm_const: case 12, pushing player size yscale");
			mu_push(vm->stack, vm->p1->my_player_const.player_size.n_yscale, "#");
			break;

		// size.ground.back
		case 13:
			mu_log_message("vm_const: case 13, pushing player size ground back");
			mu_push(vm->stack, vm->p1->my_player_const.player_size.n_ground_back, "#");
			break;

		// size.ground.front
		case 14:
			mu_log_message("vm_const: case 14, pushing player size ground front");
			mu_push(vm->stack, vm->p1->my_player_const.player_size.n_ground_front, "#");
			break;

		// size.air.back
		case 15:
			mu_log_message("vm_const: case 15, pushing player size air back");
			mu_push(vm->stack, vm->p1->my_player_const.player_size.n_air_back, "#");
			break;

		// size.air.front
		case 16:
			mu_log_message("vm_const: case 16, pushing player size air front");
			mu_push(vm->stack, vm->p1->my_player_const.player_size.n_air_front, "#");
			break;

		// size.attack.dist
		case 17:
			mu_log_message("vm_const: case 17, pushing player size attack distance");
			mu_push(vm->stack, vm->p1->my_player_const.player_size.n_attack_distance, "#");
			break;

		// size.proj.attack.dist
		case 18:
			mu_log_message("vm_const: case 18, pushing player size projectile attack distance");
			mu_push(vm->stack, vm->p1->my_player_const.player_size.n_proj_attack_distance, "#");
			break;

		// size.proj.doscale
		case 19:
			mu_log_message("vm_const: case 19, pushing player size projectile scaling");
			mu_push(vm->stack, vm->p1->my_player_const.player_size.b_proj_do_scale, "#");
			break;

		// size.head.pos.x
		case 20:
			mu_log_message("vm_const: case 20, pushing player size head position x");
			mu_push(vm->stack, vm->p1->my_player_const.player_size.n_head_pos.x, "#");
			break;

		// size.head.pos.y
		case 21:
			mu_log_message("vm_const: case 21, pushing player size head position y");
			mu_push(vm->stack, vm->p1->my_player_const.player_size.n_head_pos.y, "#");
			break;

		// size.mid.pos.x
		case 22:
			mu_log_message("vm_const: case 22, pushing player size mid position x");
			mu_push(vm->stack, vm->p1->my_player_const.player_size.n_mid_pos.x, "#");
			break;

		// size.mid.pos.y
		case 23:
			mu_log_message("vm_const: case 23, pushing player size mid position y");
			mu_push(vm->stack, vm->p1->my_player_const.player_size.n_mid_pos.y, "#");
			break;

		// size.shadowoffset
		case 24:
			mu_log_message("vm_const: case 24, pushing player size shadow offset");
			mu_push(vm->stack, vm->p1->my_player_const.player_size.n_shadow_offset, "#");
			break;

		// size.draw.offset.x
		case 25:
			mu_log_message("vm_const: case 25, pushing player size draw offset x");
			mu_push(vm->stack, vm->p1->my_player_const.player_size.n_draw_offset.x, "#");
			break;

		// size.draw.offset.y
		// Note to self: in the original version, case 26 was a copy of case 25
		// I've changed it here to be offset y, since I think that's what it was supposed to be
		case 26:
			mu_log_message("vm_const: case 26, pushing player size draw offset y");
			mu_push(vm->stack, vm->p1->my_player_const.player_size.n_draw_offset.y, "#");
			break;

		// velocity.walk.fwd.x
		case 27:
			mu_log_message("vm_const: case 27, pushing player velocity walk forward");
			mu_push(vm->stack, vm->p1->my_player_const.player_velocity.n_walk_fwd, "#");
			break;

		// velocity.walk.back.x
		case 28:
			mu_log_message("vm_const: case 28, pushing player velocity walk backward");
			mu_push(vm->stack, vm->p1->my_player_const.player_velocity.n_walk_back, "#");
			break;

		// velocity.run.fwd.x
		case 29:
			mu_log_message("vm_const: case 29, pushing player velocity run forward x");
			mu_push(vm->stack, vm->p1->my_player_const.player_velocity.run_fwd.x, "#");
			break;

		// velocity.run.fwd.y
		case 30:
			mu_log_message("vm_const: case 30, pushing player velocity run forward y");
			mu_push(vm->stack, vm->p1->my_player_const.player_velocity.run_fwd.y, "#");
			break;

		// velocity.run.back.x
		case 31:
			mu_log_message("vm_const: case 31, pushing player velocity run backward x");
			mu_push(vm->stack, vm->p1->my_player_const.player_velocity.run_back.x, "#");
			break;

		// velocity.run.back.y
		case 32:
			mu_log_message("vm_const: case 32, pushing player velocity run backward y");
			mu_push(vm->stack, vm->p1->my_player_const.player_velocity.run_back.y, "#");
			break;

		// velocity.jump.neu.x
		case 33:
			mu_log_message("vm_const: case 33, pushing player velocity jump neutral x");
			mu_push(vm->stack, vm->p1->my_player_const.player_velocity.jump_neu.x, "#");
			break;

		// velocity.jump.back.x
		case 34:
			mu_log_message("vm_const: case 34, pushing player velocity jump back x");
			mu_push(vm->stack, vm->p1->my_player_const.player_velocity.jump_back.x, "#");
			break;

		// velocity.jump.fwd.x
		case 35:
			mu_log_message("vm_const: case 35, pushing player velocity jump forward x");
			mu_push(vm->stack, vm->p1->my_player_const.player_velocity.jump_fwd.x, "#");
			break;

		// velocity.runjump.back.x
		// Note to self: in the original version, this was a copy of case 31
		// I think it's supposed to be run_jump_back.x though, so I changed it to that
		case 36:
			mu_log_message("vm_const: case 36, pushing player velocity run jump backward x");
			mu_push(vm->stack, vm->p1->my_player_const.player_velocity.run_jump_back.x, "#");
			break;

		// velocity.runjump.fwd.x
		// Note to self: in the original version, this was a copy of case 29
		// I think it's supposed to be run_jump_fwd.x though, so I changed it to that
		case 37:
			mu_log_message("vm_const: case 37, pushing player velocity run jump forward x");
			mu_push(vm->stack, vm->p1->my_player_const.player_velocity.run_jump_fwd.x, "#");
			break;

		// velocity.airjump.neu.x
		case 38:
			mu_log_message("vm_const: case 38, pushing player velocity air jump neutral x");
			mu_push(vm->stack, vm->p1->my_player_const.player_velocity.air_jump_neu.x, "#");
			break;

		// velocity.airjump.back.x
		case 39:
			mu_log_message("vm_const: case 39, pushing player velocity air jump backward x");
			mu_push(vm->stack, vm->p1->my_player_const.player_velocity.air_jump_back.x, "#");
			break;

		// velocity.airjump.fwd.x
		case 40:
			mu_log_message("vm_const: case 40, pushing player velocity air jump forward x");
			mu_push(vm->stack, vm->p1->my_player_const.player_velocity.air_jump_fwd.x, "#");
			break;

		// movement.airjump.num
		case 41:
			mu_log_message("vm_const: case 41, pushing player movement air jump number");
			mu_push(vm->stack, vm->p1->my_player_const.player_movement.air_jump_num, "#");
			break;

		// movement.airjump.height
		case 42:
			mu_log_message("vm_const: case 42, pushing player movement air jump height");
			mu_push(vm->stack, vm->p1->my_player_const.player_movement.n_air_jump_height, "#");
			break;

		// movement.yaccel
		case 43:
			mu_log_message("vm_const: case 43, pushing player movement y acceleration");
			mu_push(vm->stack, vm->p1->my_player_const.player_movement.yaccel, "#");
			break;

		// movement.stand.friction
		case 44:
			mu_log_message("vm_const: case 44, pushing player movement stand friction");
			mu_push(vm->stack, vm->p1->my_player_const.player_movement.stand_friction, "#");
			break;

		// movement.crouch.friction
		case 45:
			mu_log_message("vm_const: case 45, pushing player movement crouch friction");
			mu_push(vm->stack, vm->p1->my_player_const.player_movement.crouch_friction, "#");
			break;

		// velocity.jump.y
		case 46:
			mu_log_message("vm_const: case 46; check this, I'm not sure if it's correct");
			mu_push(vm->stack, 0, "#");
			break;

		// velocity.airjump.y
		case 47:
			mu_log_message("vm_const: case 47; check this, I'm not sure if it's correct");
			mu_push(vm->stack, 0, "#");
			break;

		default:
			mu_log_message("vm_const: invalid const parameter");
			break;
	}
}

// cos(x)
void vm_cos(VM *vm)
{
	vm_pop_value(vm);
	vm->temp1 = vm->pop_item.value;

	vm->temp1 = cos(vm->temp1);
	mu_log_message("vm_cos: resolved to %f", vm->temp1);
	mu_push(vm->stack, vm->temp1, "#");
}

// Is the player in control?
void vm_ctrl(VM *vm)
{
	if(vm->p1->b_ctrl)
	{
		mu_log_message("vm_ctrl: player is in control");
		mu_push(vm->stack, 1.0, "#");
	}
	else
	{
		mu_log_message("vm_ctrl: player is not in control");
		mu_push(vm->stack, 0.0, "#");
	}
}

void vm_draw_game(VM *vm)
{
	// TODO: Complete this
	mu_log_message("vm_draw_game: this does nothing so far, check function for more info");
	mu_push(vm->stack, 0.0, "#");
}

// e^x
void vm_exp(VM *vm)
{
	vm_pop_value(vm);
	vm->temp1 = vm->pop_item.value;

	vm->temp1 = pow(2.71828182f, vm->temp1);

	mu_log_message("vm_exp: resolved to %f", vm->temp1);
	mu_push(vm->stack, vm->temp1, "#");
}

// Pushes the facing direction to the stack
void vm_facing(VM *vm)
{
	mu_log_message("vm_facing: this is commented out, check function for more info");
	//true = right facing = 1
	//flase = left facing = -1
	/*  if(m_pPlayer1->PlRtInfo.bFacing)
	m_Stack.Push(1.0,"#");
	else
	m_Stack.Push(-1.0,"#");*/
}

// floor(x)
void vm_floor(VM *vm)
{
	vm_pop_value(vm);
	vm->temp1 = vm->pop_item.value;

	vm->temp1 = floor(vm->temp1);

	mu_log_message("vm_floor: resolved to %f", vm->temp1);
	mu_push(vm->stack, vm->temp1, "#");
}

void vm_front_edge_body_dist(VM *vm)
{
	// TODO: compute the distance between the front and the player
	mu_log_message("vm_front_edge_body_dist: TODO, check function for details");
	mu_push(vm->stack, 124, "#");
}

void vm_front_edge_dist(VM *vm)
{
	// TODO: compute the distance between the x-axis of the player and screen
	mu_log_message("vm_front_edge_dist: TODO, check function for details");
	mu_push(vm->stack, 124, "#");
}

void vm_fvar(VM *vm)
{
	mu_log_message("vm_fvar: commented out, check function for details");
	/* PopValue();
	temp1=m_pop.Value;

	temp1=m_pPlayer1->GetFVar((int)temp1);
	m_Stack.Push(temp1,"#");
	*/
}

void vm_game_time(VM *vm)
{
	mu_log_message("vm_game_time: commented out, check function for details");
	// TODO: Put the real game time on Stack
	// m_Stack.Push((float)GetGameTicks(),"#");
}

void vm_get_hit_var(VM *vm)
{
	// TODO: get the correct Hitvar and push it on the stack
	vm_pop_value(vm);
	vm->temp1 = vm->pop_item.value;

	mu_log_message("vm_get_hit_var: this does nothing for now, fix this");
	mu_push(vm->stack, 0, "#");
}

void vm_hit_count(VM *vm)
{
	// TODO: push the correct value on stack
	mu_log_message("vm_hit_count: TODO, this does nothing for now, fix this");
	mu_push(vm->stack, 0, "#");
}

void vm_hit_fall(VM *vm)
{
	// TODO: push the correct value on stack
	mu_log_message("vm_hit_fall: TODO, this does nothing for now, fix this");
	mu_push(vm->stack, 0, "#");
}

void vm_hit_over(VM *vm)
{
	// TODO: push the correct value on stack
	mu_log_message("vm_hit_over: TODO, this does nothing for now, fix this");
	mu_push(vm->stack, 0, "#");
}

void vm_hit_pause_time(VM *vm)
{
	// TODO: push the correct value on stack
	mu_log_message("vm_hit_pause_time: TODO, this does nothing for now, fix this");
	mu_push(vm->stack, 0, "#");
}

void vm_hit_shake_over(VM *vm)
{
	// TODO: push the correct value on stack
	mu_log_message("vm_hit_shake_over: TODO, this does nothing for now, fix this");
	mu_push(vm->stack, 0, "#");
}

void vm_hit_vel(VM *vm)
{
	vm_pop_value(vm);
	vm->temp1 = vm->pop_item.value;

	// TODO: pop the component value
	// 1 == x 2 == y

	mu_log_message("vm_hit_vel: this does nothing for now, fix this");
	mu_push(vm->stack, 0, "#");
}

void vm_player_ident(VM *vm)
{
	mu_log_message("vm_player_ident: commented out, check function for details");
	// m_Stack.Push((float)m_pPlayer1->PlRtInfo.nID,"#");
}

void vm_if_else(VM *vm)
{
	// exp_false
	vm_pop_value(vm);
	vm->temp3 = vm->pop_item.value;
	// exp_true
	vm_pop_value(vm);
	vm->temp2 = vm->pop_item.value;
	// exp_cond
	vm_pop_value(vm);
	vm->temp1 = vm->pop_item.value;

	if(vm->temp1 == 1.0f)
	{
		mu_log_message("vm_if_else: resolved true, pushing %f", vm->temp2);
		mu_push(vm->stack, vm->temp2, "#");
	}
	else
	{
		mu_log_message("vm_if_else: resolved false, pushing %f", vm->temp3);
		mu_push(vm->stack, vm->temp3, "#");
	}
}

void vm_in_guard_dist(VM *vm)
{
	// TODO: implement this
	/*        InGuardDist 
	Returns 1 if the player is within guarding distance of an opponent's physicalor projectile attack.
	The guarding distance is the value of the guard.dist parameter of the
	opponent's HitDef. Returns 0 if out of guard distance, or the opponent is not attacking.
	*/
	mu_log_message("vm_in_guard_dist: this does nothing for now, check function for details");
	mu_push(vm->stack, 0, "#");
}

void vm_is_helper(VM *vm)
{
	mu_log_message("vm_is_helper: this does nothing for now, fix this");
	mu_push(vm->stack, 0, "#");
}

void vm_is_home_team(VM *vm)
{
	mu_log_message("vm_is_home_team: this does nothing for now, fix this");
	mu_push(vm->stack, 0, "#");
}

void vm_life(VM *vm)
{
	mu_log_message("vm_life: commented out, check function for details");
	//  m_Stack.Push((float)m_pPlayer1->PlRtInfo.nLife,"#");
}

void vm_life_max(VM *vm)
{
	mu_log_message("vm_life_max: commented out, check function for details");
	//  m_Stack.Push((float)m_pPlayer1->myPlayerConst.PlayerData.nLife,"#");
}

void vm_log_n(VM *vm)
{
	mu_log_message("vm_log_n: Pretty sure this isn't working as intended, check function for details");
	vm_pop_value(vm);
	vm->temp1 = vm->pop_item.value;

	mu_push(vm->stack, vm->temp1, "#");
}

void vm_log(VM *vm)
{
	// Push the two arguments
	vm_pop_value(vm);
	vm->temp2 = vm->pop_item.value;
	vm_pop_value(vm);
	vm->temp1 = vm->pop_item.value;

	mu_log_message("vm_log: Pretty sure this isn't working as intended, check function for details");
	mu_push(vm->stack, 0, "#");
}

void vm_lose(VM *vm)
{
	//Pop the parameter 1=Lose, 2=LoseKO ,3=LoseTime
	vm_pop_value(vm);
	vm->temp1 = vm->pop_item.value;

	switch((int)vm->temp1)
	{
		case 1:
			mu_push(vm->stack, 0, "#");
			break;
		case 2:
			mu_push(vm->stack, 0, "#");
			break;
		case 3:
			mu_push(vm->stack, 0, "#");
			break;
	}
	mu_log_message("vm_lose: Pretty sure this isn't working as intended, check function for details");
}

// Pushes the current match number
void vm_match_no(VM *vm)
{
	mu_log_message("vm_match_no: Pretty sure this isn't working as intended, check function for details");
	mu_push(vm->stack, 1, "#");
}

void vm_match_over(VM *vm)
{
	mu_log_message("vm_match_over: Pretty sure this isn't working as intended, check function for details");
	mu_push(vm->stack, 0, "#");
}

void vm_move_contact(VM *vm)
{
	mu_log_message("vm_move_contact: Pretty sure this isn't working as intended, check function for details");
	mu_push(vm->stack, 0, "#");
}

void vm_move_guarded(VM *vm)
{
	mu_log_message("vm_move_guarded: Pretty sure this isn't working as intended, check function for details");
	mu_push(vm->stack, 0, "#");
}

void vm_move_hit(VM *vm)
{
	mu_log_message("vm_move_hit: Pretty sure this isn't working as intended, check function for details");
	mu_push(vm->stack, 0, "#");
}

void vm_move_type(VM *vm)
{
	/*MoveType gives the player's move-type. Refer to the section
	on StateDef in the CNS documentation for more details on MoveType.
	Useful for "move interrupts" in the CMD file.
	*/

	// Pop the type to check
    // Types are A(ttack), I(dle), H(it)
	vm_pop_value(vm);

	// For now, push false on the stack
	mu_log_message("vm_move_type: not implemented, check function for details");
	mu_push(vm->stack, 0, "#");
}

void vm_move_reversed(VM *vm)
{
	// For now, push false on the stack
	mu_log_message("vm_move_reversed: not implemented, check function for details");
	mu_push(vm->stack, 0, "#");
}

void vm_name(VM *vm)
{
	//Push the name of the player
 	//   m_Stack.Push(0,m_pPlayer1->myPlayerData.strName);
 	mu_log_message("vm_name: commented out, check function for details");
}

void vm_num_enemy(VM *vm)
{
	// For now only one enemy is supported
	mu_log_message("vm_num_enemy: pushing 1, as multiple enemies is not supported yet");
	mu_push(vm->stack, 1, "#");
}

void vm_num_explod(VM *vm)
{
	vm_pop_value(vm);
	vm->temp1 = vm->pop_item.value;

	if(vm->temp1 == 1)
	{
		// Pop the parameter
		vm_pop_value(vm);
		vm->temp2 = vm->pop_item.value;
		mu_log_message("vm_num_explod: parameter is %i", (int)vm->temp2);
		mu_push(vm->stack, 0, "#");
	}
	else
	{
		mu_push(vm->stack, 0, "#");
	}
	mu_log_message("vm_num_explod: looks to be not fully implemented, check function for details");
}

void vm_num_helper(VM *vm)
{
	vm_pop_value(vm);
	vm->temp1 = vm->pop_item.value;

	if(vm->temp1 == 1)
	{
		// Pop the parameter
		vm_pop_value(vm);
		vm->temp2 = vm->pop_item.value;
		mu_log_message("vm_num_helper: parameter is %i", (int)vm->temp2);
		mu_push(vm->stack, 0, "#");
	}
	else
	{
		mu_push(vm->stack, 0, "#");
	}
	mu_log_message("vm_num_helper: looks to be not fully implemented, check function for details");
}

void vm_num_partner(VM *vm)
{
	mu_log_message("vm_num_partner: Pretty sure this isn't working as intended, check function for details");
	mu_push(vm->stack, 0, "#");
}

void vm_num_proj(VM *vm)
{
	mu_log_message("vm_num_proj: Pretty sure this isn't working as intended, check function for details");
	mu_push(vm->stack, 0, "#");
}

void vm_num_proj_id(VM *vm)
{
	mu_log_message("vm_num_proj_id: Pretty sure this isn't working as intended, check function for details");
	vm_pop_value(vm);
	mu_push(vm->stack, 0, "#");
}

void vm_num_target(VM *vm)
{
	vm_pop_value(vm);
	vm->temp1 = vm->pop_item.value;

	if(vm->temp1 == 1)
	{
		// Pop the parameter
		vm_pop_value(vm);
		vm->temp2 = vm->pop_item.value;
		mu_log_message("vm_num_target: parameter is %i", (int)vm->temp2);
		mu_push(vm->stack, 0, "#");
	}
	else
	{
		mu_push(vm->stack, 0, "#");
	}
	mu_log_message("vm_num_target: looks to be not fully implemented, check function for details");
}

void vm_p2_body_dist(VM *vm)
{
	vm_pop_value(vm);
	vm->temp1 = vm->pop_item.value;

	//1 = X Value
	if(vm->temp1 == 1)
	{
		mu_push(vm->stack, 0, "#");
	}
	else //Else y value
	{
		mu_push(vm->stack, 0, "#");
	}
	mu_log_message("vm_p2_body_dist: looks to be not fully implemented, check function for details");
}

void vm_p2_dist(VM *vm)
{
	vm_pop_value(vm);
	vm->temp1 = vm->pop_item.value;

	//1 = X Value
	if(vm->temp1 == 1)
	{
		mu_push(vm->stack, 0, "#");
	}
	else //Else y value
	{
		mu_push(vm->stack, 0, "#");
	}
	mu_log_message("vm_p2_dist: looks to be not fully implemented, check function for details");
}

void vm_p2_life(VM *vm)
{
	mu_log_message("vm_p2_life: commented out, check function for details");
	//  m_Stack.Push((float)((CPlayer*)m_p2)->PlRtInfo.nLife,"#");
}

void vm_p2_move_type(VM *vm)
{
	mu_log_message("vm_p2_move_type: Pretty sure this isn't working as intended, check function for details");
	mu_push(vm->stack, 0, "#");
}

void vm_p2_name(VM *vm)
{
	mu_log_message("vm_p2_name: commented out, check function for details");
	// m_Stack.Push(0,((CPlayer*)m_p2)->myPlayerData.strName);
}

void vm_p2_state_no(VM *vm)
{
	mu_log_message("vm_p2_state_no: commented out, check function for details");
	//   m_Stack.Push((float)((CPlayer*)m_p2)->PlRtInfo.nState,"#");
}

void vm_p2_state_type(VM *vm)
{
	mu_log_message("vm_p2_state_type: Pretty sure this isn't working as intended, check function for details");
	mu_push(vm->stack, 0, "#");
}

void vm_p3_name(VM *vm)
{
	mu_log_message("vm_p3_name: Pretty sure this isn't working as intended, check function for details");
	mu_push(vm->stack, 0, "P3Name");
}

void vm_p4_name(VM *vm)
{
	mu_log_message("vm_p4_name: Pretty sure this isn't working as intended, check function for details");
	mu_push(vm->stack, 0, "P4Name");
}

void vm_pal_no(VM *vm)
{
	mu_log_message("vm_pal_no: Pretty sure this isn't working as intended, check function for details");
	mu_push(vm->stack, 0, "#");
}

void vm_parent_dist(VM *vm)
{
	vm_pop_value(vm);
	vm->temp1 = vm->pop_item.value;

	//1 = X Value
	if(vm->temp1 == 1)
	{
		mu_push(vm->stack, 0, "#");
	}
	else //Else y value
	{
		mu_push(vm->stack, 0, "#");
	}
	mu_log_message("vm_parent_dist: looks to be not fully implemented, check function for details");
}

void vm_pi(VM *vm)
{
	mu_log_message("vm_pi: pushing pi to stack");
	mu_push(vm->stack, 3.141592653589f, "#");
}

void vm_pos(VM *vm)
{
	mu_log_message("vm_pos: commented out, check function for details");
	//1 = X Value
	/*  if(m_Stack.Pop().Value==1)
	{
		m_Stack.Push(m_pPlayer1->PlRtInfo.x-XROOT,"#");
	}//Else y value
	else
	{
		//default
		m_Stack.Push(m_pPlayer1->PlRtInfo.y,"#");
	}*/
}

void vm_power(VM *vm)
{
	mu_log_message("vm_power: commented out, check function for details");
	//  m_Stack.Push((float)m_pPlayer1->PlRtInfo.nLevelPower,"#");
}

void vm_power_max(VM *vm)
{
	mu_log_message("vm_power_max: currently pushing default of 3000, will probably need to check for an actual value in the future though");
	mu_push(vm->stack, 3000, "#");
}

void vm_player_id_exist(VM *vm)
{
	mu_log_message("vm_player_id_exist: Pretty sure this isn't working as intended, check function for details");
	vm_pop_value(vm);
	mu_push(vm->stack, 0, "#");
}

void vm_prev_state_no(VM *vm)
{
	mu_log_message("vm_prev_state_no: commented out, check function for details");
	// m_Stack.Push((float)m_pPlayer1->PlRtInfo.nPrevState,"#");
}

void vm_proj_cancel_time(VM *vm)
{
	mu_log_message("vm_proj_cancel_time: currently pushing default of 1000, will probably need to check for an actual value in the future though");
	vm_pop_value(vm);
	mu_push(vm->stack, 1000, "#");
}

void vm_proj_contact(VM *vm)
{
	mu_log_message("vm_proj_contact: Pretty sure this isn't working as intended, check function for details");
	mu_push(vm->stack, 1, "#");
	mu_push(vm->stack, 2, "#");
}

void vm_proj_contact_time(VM *vm)
{
	mu_log_message("vm_proj_contact_time: Pretty sure this isn't working as intended, check function for details");
	vm_pop_value(vm);
	mu_push(vm->stack, 0, "#");
}

void vm_proj_guarded(VM *vm)
{
	mu_log_message("vm_proj_guarded: Pretty sure this isn't working as intended, check function for details");
	mu_push(vm->stack, 1, "#");
	mu_push(vm->stack, 2, "#");
}

void vm_proj_guarded_time(VM *vm)
{
	mu_log_message("vm_proj_guarded_time: Pretty sure this isn't working as intended, check function for details");
	vm_pop_value(vm);
	mu_push(vm->stack, 0, "#");
}

void vm_random(VM *vm)
{
	mu_log_message("vm_random: commented out, check function for details");
	/*srand(1);
	m_Stack.Push((float)rand(),"#");  */
}

void vm_root_dist(VM *vm)
{
	vm_pop_value(vm);
	vm->temp1 = vm->pop_item.value;

	//1 = X Value
	if(vm->temp1 == 1)
	{
		mu_push(vm->stack, 0, "#");
	}
	else //Else y value
	{
		mu_push(vm->stack, 0, "#");
	}
	mu_log_message("vm_root_dist: looks to be not fully implemented, check function for details");
}

void vm_round_no(VM *vm)
{
	mu_log_message("vm_round_no: Pretty sure this isn't working as intended, check function for details");
	mu_push(vm->stack, 0, "#");
}

void vm_rounds_existed(VM *vm)
{
	mu_log_message("vm_rounds_existed: Pretty sure this isn't working as intended, check function for details");
	mu_push(vm->stack, 0, "#");
}

void vm_round_state(VM *vm)
{
	// 2 = do battle
	mu_log_message("vm_round_state: pushing 2 for now, check function for details");
	mu_push(vm->stack, 2, "#");
}

void vm_screen_pos(VM *vm)
{
	mu_log_message("vm_screen_pos: commented out, check function for details");
	//1 = X Value
	 /*   if(m_Stack.Pop().Value==1)
	    {
	       m_Stack.Push(m_pPlayer1->PlRtInfo.x,"#");
	    }//Else y value
	    else
	    {
	        //default
	        m_Stack.Push(m_pPlayer1->PlRtInfo.y,"#");
	    }
	*/
}

void vm_self_anim_exist(VM *vm)
{
	mu_log_message("vm_self_anim_exist: commented out, check function for details");
	/* int nAnim;
    PopValue();
    nAnim=(int)m_pop.Value;
    //If != NULL push true else push false
    if(m_pPlayer1->m_AirManager.GetAction(nAnim) != NULL)
        m_Stack.Push(1,"#");
    else
        m_Stack.Push(0,"#");
    
	*/
}

void vm_sin(VM *vm)
{
	vm_pop_value(vm);
	vm->temp1 = vm->pop_item.value;

	vm->temp1 = sin(vm->temp1);
	mu_log_message("vm_sin: resolved to %f", vm->temp1);
	mu_push(vm->stack, vm->temp1, "#");
}

void vm_state_no(VM *vm)
{
	mu_log_message("vm_state_no: commented out, check function for details");
	//m_Stack.Push((float)m_pPlayer1->PlRtInfo.nState,"#");
}

void vm_state_type(VM *vm)
{
	mu_log_message("vm_state_type: commented out, check function for details");
	//  m_Stack.Push((float)m_pPlayer1->PlRtInfo.nStateTyp,"#");
}

void vm_sys_fvar(VM *vm)
{
	mu_log_message("vm_sys_fvar: Pretty sure this isn't working as intended, check function for details");
	vm_pop_value(vm);
	mu_push(vm->stack, 0, "#");
}

void vm_sys_var(VM *vm)
{
	mu_log_message("vm_sys_var: Pretty sure this isn't working as intended, check function for details");
	vm_pop_value(vm);
	mu_push(vm->stack, 0, "#");
}

void vm_tan(VM *vm)
{
	vm_pop_value(vm);
	vm->temp1 = vm->pop_item.value;

	vm->temp1 = tan(vm->temp1);
	mu_log_message("vm_tan: resolved to %f", vm->temp1);
	mu_push(vm->stack, vm->temp1, "#");
}

void vm_team_mode(VM *vm)
{
	mu_log_message("vm_team_mode: Pretty sure this isn't working as intended, check function for details");
	mu_push(vm->stack, 0, "#");
}

void vm_team_side(VM *vm)
{
	mu_log_message("vm_team_side: Pretty sure this isn't working as intended, check function for details");
	mu_push(vm->stack, 1, "#");
}

void vm_ticks_per_second(VM *vm)
{
	mu_log_message("vm_ticks_per_second: not sure if this is working as intended, check function for details");
	mu_push(vm->stack, 60, "#");
}

void vm_time(VM *vm)
{
	mu_log_message("vm_time: commented out, check function for details");
	//  m_Stack.Push(m_pPlayer1->PlRtInfo.nStateTime,"#");
}

void vm_uniq_hit_count(VM *vm)
{
	mu_log_message("vm_uniq_hit_count: Pretty sure this isn't working as intended, check function for details");
	mu_push(vm->stack, 1, "#");
}

void vm_var(VM *vm)
{
	mu_log_message("vm_var: Pretty sure this isn't working as intended, check function for details");
	vm_pop_value(vm);
	mu_push(vm->stack, 1, "#");
}

void vm_vel(VM *vm)
{
	mu_log_message("vm_vel: commented out, check function for details");
	//1 = X Value
    /* if(m_Stack.Pop().Value==1)
    {
       m_Stack.Push(m_pPlayer1->PlRtInfo.xvel,"#");
    }//Else y value
    else
    {
        //default
        m_Stack.Push(m_pPlayer1->PlRtInfo.xvel,"#");
    }*/
}

void vm_win(VM *vm)
{
	vm_pop_value(vm);
	vm->temp1 = vm->pop_item.value;

	switch((int)vm->temp1)
	{
		// Win
		case 1:
			mu_push(vm->stack, 0, "#");
			break;
		// WinKO
		case 2:
			mu_push(vm->stack, 0, "#");
			break;
		// WinTime
		case 3:
			mu_push(vm->stack, 0, "#");
			break;
		// WinPerfect
		case 4:
			mu_push(vm->stack, 0, "#");
			break;
	}
	mu_log_message("vm_win: looks to be not fully implemented, check function for details");
}

void vm_nop()
{
	mu_log_message("vm_nop: this function is supposed to do nothing I think");
}

void vm_modop(VM *vm)
{
	int n_dividend, n_divisor;

	vm_pop_value(vm);
	n_divisor = (int)vm->pop_item.value;
	vm_pop_value(vm);
	n_dividend = (int)vm->pop_item.value;

	n_dividend = n_dividend % n_divisor;
	mu_log_message("vm_modop: resolves to %i", n_dividend);

	if(n_dividend == 0)
	{
		mu_log_message("vm_modop: even remainder, pushing 1");
		mu_push(vm->stack, 1, "#");
	}
	else
	{
		mu_log_message("vm_modop: odd remainder, pushing 0");
		mu_push(vm->stack, 0, "#");
	}
}

void vm_save(VM *vm)
{
	vm_pop_value(vm);
	vm->n_save = vm->pop_item.value;

	mu_log_message("vm_save: saved %f", vm->n_save);
}

void vm_restore(VM *vm)
{
	mu_log_message("vm_restore: restoring %f", vm->n_save);
	mu_push(vm->stack, vm->n_save, "#");
}