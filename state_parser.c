#include "mu_headers.h"

/* ############################################
   This file contains the finite state machine that effectively runs the core of the game. It's currently under
   development, and as such, is not working as intended. When last tested, using this code caused segfaults
   unpredictably, sometimes triggering before any states are parsed, and sometimes after many have been parsed.
   This leads me to believe that the segfaults might actually be caused somewhere else, however further testing
   will be required for this. Given that my intention is to refactor the entire engine and start from scratch,
   trying to trace down the cause of these segfaults and fix them is not in my best interest. Once the data 
   structures the engine is built on are better organised and written with modularity and ease of use in mind,
   bugs like these will be much easier to trace back to their origin. Additionally, the rewriting of the entire
   engine will allow me to build in rigorous unit testing from the beginning, allowing me to catch bugs as soon 
   as they appear. With all of this in mind, the following file has been left uncommented and unoptimised.
   ############################################ */

// Arrays copied directly from previous code
// These are all the documented controllers
//TODO: find the undocumented controllers
static char *str_controller_types[] = {
	"AfterImage",
	"AfterImageTime",
	"AllPalFX",
	"AngleAdd",
	"AngleDraw",
	"AngleMul",
	"AngleSet",
	"AppendToClipboard",
	"AssertSpecial",
	"AttackDist",
	"AttackMulSet",
	"BGPalFX",
	"BindToParent",
	"BindToRoot",
	"BindToTarget",
	"ChangeAnim",
	"ChangeAnim2",
	"ChangeState",
	"ClearClipboard",
	"CtrlSet",
	"DefenceMulSet",
	"DestroySelf",
	"DisplayToClipboard",
	"EnvColor",
	"EnvShake",
	"Explod",
	"ExplodBindTime",
	"ForceFeedback",
	"FallEnvShake",
	"GameMakeAnim",
	"Gravity",
	"Helper",
	"HitAdd",
	"HitBy",
	"HitDef",
	"HitFallDamage",
	"HitFallSet",
	"HitFallVel",
	"HitOverride",
	"HitVelSet",
	"LifeAdd",
	"LifeSet",
	"MakeDust",
	"ModifyExplod",
	"MoveHitReset",
	"NotHitBy",
	"Null",
	"Offset",
	"PalFX",
	"ParentVarAdd",
	"ParentVarSet",
	"Pause",
	"PlayerPush",
	"PlaySnd",
	"PosAdd",
	"PosFreeze",
	"PosSet",
	"PowerAdd",
	"PowerSet",
	"Projectile",
	"RemoveExplod",
	"ReversalDef",
	"ScreenBound",
	"SelfState",
	"SprPriority",
	"StateTypeSet",
	"SndPan",
	"StopSnd",
	"SuperPause",
	"TargetBind",
	"TargetDrop",
	"TargetFacing",
	"TargetLifeAdd",
	"TargetPowerAdd",
	"TargetState",
	"TargetVelAdd",
	"TargetVelSet",
	"Trans",
	"Turn",
	"VarAdd",
	"VarRandom",
	"VarRangeSet",
	"VarSet",
	"VelAdd",
	"VelMul",
	"VelSet",
	"Width",
	0,
};
       
//All triggers       
static char *str_trigger_type[] = {
	"Abs",
	"Acos",
	"Alive",
	"Anim",
	"AnimElem",
	"AnimElemNo",
	"AnimElemTime",
	"AnimExist",
	"AnimTime",
	"Asin",
	"Atan",
	"AuthorName",
	"BackEdgeBodyDist",
	"BackEdgeDist",
	"CanRecover",
	"Ceil",
	"Command",
	"Const",
	"Cos",
	"Ctrl",
	"DrawGame",
	"E",
	"Exp",
	"Facing",
	"Floor",
	"FrontEdgeBodyDist",
	"FrontEdgeDist",
	"FVar",
	"GameTime",
	"GetHitVar",
	"HitCount",
	"HitDefAttr",
	"HitFall",
	"HitOver",
	"HitPauseTime",
	"HitShakeOver",
	"HitVel",
	"ID",
	"IfElse",
	"InGuardDist",
	"IsHelper",
	"IsHomeTeam",
	"Life",
	"LifeMax",
	"Log",
	"Ln",
	"Lose",
	"MatchNo",
	"MatchOver",
	"MoveContact",
	"MoveGuarded",
	"MoveHit",
	"MoveType",
	"MoveReversed",
	"Name",
	"NumEnemy",
	"NumExplod",
	"NumHelper",
	"NumPartner",
	"NumProj",
	"NumProjID",
	"NumTarget",
	"P1Name",
	"P2BodyDist",
	"P2Dist",
	"P2Life",
	"P2MoveType",
	"P2Name",
	"P2StateNo",
	"P2StateType",
	"P3Name",
	"P4Name",
	"PalNo",
	"ParentDist",
	"Pi",
	"Pos",
	"Power",
	"PowerMax",
	"PlayerIDExist",
	"PrevStateNo",
	"ProjCancelTime",
	"ProjContact",
	"ProjContactTime",
	"ProjGuarded",
	"ProjGuardedTime",
	"ProjHit",
	"ProjHitTime",
	"Random",
	"RootDist",
	"RoundNo",
	"RoundsExisted",
	"RoundState",
	"ScreenPos",
	"SelfAnimExist",
	"Sin",
	"StateNo",
	"StateType",
	"SysFVar",
	"SysVar",
	"Tan",
	"TeamMode",
	"TeamSide",
	"TicksPerSecond",
	"Time",
	"TimeMod",
	"UniqHitCount",
	"Var",
	"Vel",
	"Win",
	0,
};

// Used in GetHitVar triggers
// TODO: Add a means to process the remaining fall variables (see Hit_Vars in structs.h for a list)
static char *str_hitvar_type[] = 
{
	"xveladd",
	"yveladd",
	"type",
	"animtype",
	"airtype",
	"groundtype",
	"damage",
	"hitcount",
	"fallcount",
	"hitshaketime",
	"hittime",
	"slidetime",
	"ctrltime",
	"recovertime",
	"xvel",
	"yvel",
	"yaccel",
	"chainid",
	"guarded",
	"isbound",
	"fall",
	0,
};

void mu_parser_error(const char *str_error_message, Tokenizer *tokenizer)
{
	mu_log_message("Error: State Parser\nin file %s at line %i: \n%s", tokenizer->filename, tokenizer->current_file_line, str_error_message);
}                 

int get_controller_type(const char *str_type, Tokenizer *tokenizer)
{
	int i = 0;
	while(str_controller_types[i])
	{
#ifdef _WIN32
		if(stricmp(str_type, str_controller_types[i]) == 0) // this was originally a normal strcmp
#else
		if(strcasecmp(str_type, str_controller_types[i]) == 0) // this was originally a normal strcmp
#endif
		{
			return i;
		}
		i++;
	}
	mu_parser_error(str_type, tokenizer);
	return -1;
}

int get_trigger_type(const char *str_trigger, Tokenizer *tokenizer)
{
	int i = 0;
	while(str_trigger_type[i])
	{
#ifdef _WIN32
		if(stricmp(str_trigger, str_trigger_type[i]) == 0) // This was originally a normal strcmp
#else
		if(strcasecmp(str_trigger, str_trigger_type[i]) == 0) // This was originally a normal strcmp
#endif
		{
			// if(i == 106)
			// {
			// 	mu_log_message("trigger type var reached");
			// }
			return i;
		}
		i++;
	}
	mu_parser_error(str_trigger, tokenizer);
	return -1;
}

int get_hitvar_type(const char *str_hitvar, Tokenizer *tokenizer)
{
	int i = 0;
	while(str_hitvar_type[i])
	{
#ifdef _WIN32
		if(stricmp(str_hitvar, str_hitvar_type[i]) == 0) // This was originally a normal strcmp
#else
		if(strcasecmp(str_hitvar, str_hitvar_type[i]) == 0) // This was originally a normal strcmp
#endif
		{
			return i;
		}
		i++;
	}
	//mu_parser_error(str_hitvar, tokenizer);
	return -1;
}

void evaluate_expression(Tokenizer *tokenizer, MU_State_Manager *state_manager)
{
	mu_parse_term(tokenizer, state_manager);

	while(check_token_no_consume(tokenizer, "+") || check_token_no_consume(tokenizer, "-") && !tokenizer->at_end_of_line)
	{
		if(check_token_consume(tokenizer, "+"))
		{
			mu_parse_term(tokenizer, state_manager);
			add_instruction(state_manager, OP_ADD, 0, "#");
		}

		if(check_token_consume(tokenizer, "-"))
		{
			mu_parse_term(tokenizer, state_manager);
			add_instruction(state_manager, OP_SUB, 0, "#");
		}
	}
	//mu_log_message("Exiting evaluate_expression");
}

void mu_parse_term(Tokenizer *tokenizer, MU_State_Manager *state_manager)
{
	mu_parse_primary(tokenizer, state_manager);

	//mu_log_message("mu_parse_term: buffer = %s", tokenizer->buffer);

	// Search for operators
	while(check_token_no_consume(tokenizer, "*") || check_token_no_consume(tokenizer, "/") ||
		  check_token_no_consume(tokenizer, "=") || check_token_no_consume(tokenizer, "!=") ||
		  check_token_no_consume(tokenizer, "<") || check_token_no_consume(tokenizer, "<=") ||
		  check_token_no_consume(tokenizer, ">") || check_token_no_consume(tokenizer, ">=") ||
		  check_token_no_consume(tokenizer, ":") || check_token_no_consume(tokenizer, "&&") ||
		  check_token_no_consume(tokenizer, "||") || check_token_no_consume(tokenizer, "^^") ||
		  check_token_no_consume(tokenizer, "&") || check_token_no_consume(tokenizer, "~") ||
		  check_token_no_consume(tokenizer, "|") || check_token_no_consume(tokenizer, "^") ||
		  check_token_no_consume(tokenizer, "%") && !tokenizer->at_end_of_line)
	{
		if(check_token_consume(tokenizer, "*"))
		{
			// Is it a ** operator?
			if(check_token_consume(tokenizer, "*"))
			{
				mu_parse_primary(tokenizer, state_manager);
				add_instruction(state_manager, OP_SQUARE, 0, "#");
			}
			else
			{
				mu_parse_primary(tokenizer, state_manager);
				add_instruction(state_manager, OP_MUL, 0, "#");
			}
		}

		if(check_token_consume(tokenizer, "/"))
		{
			mu_parse_primary(tokenizer, state_manager);
			add_instruction(state_manager, OP_DIV, 0, "#");
		}

		if(check_token_consume(tokenizer, "="))
		{
			// Check for interval operator
			if(check_token_consume(tokenizer, "("))
			{
				// Evaluate first expression
				evaluate_expression(tokenizer, state_manager);
				if(!check_token_consume(tokenizer, ","))
				{
					mu_parser_error("Expected a , in interval operator", tokenizer);
				}

				// Evaluate second expression
				evaluate_expression(tokenizer, state_manager);
				//intervall op =(,)
				if(check_token_consume(tokenizer, ")"))
				{
					add_instruction(state_manager, OP_INTERVALOP4, 0, "#");
				}
				else if(check_token_consume(tokenizer, "]"))
				{
					add_instruction(state_manager, OP_INTERVALOP3, 0, "#");
				}
			}
			else if(check_token_consume(tokenizer, "["))
			{
				// Evaluate the first expression
				evaluate_expression(tokenizer, state_manager);
				if(!check_token_consume(tokenizer, ","))
				{
					mu_parser_error("Expected a , in interval operator", tokenizer);
				}

				// Evaluate second expression
				evaluate_expression(tokenizer, state_manager);
				//intervall op =[,)
				if(check_token_consume(tokenizer, ")"))
				{
					add_instruction(state_manager, OP_INTERVALOP2, 0, "#");
				}
				else if(check_token_consume(tokenizer, "]"))
				{
					add_instruction(state_manager, OP_INTERVALOP1, 0, "#");
				}
			}
			else // is the = op
			{
				// Evaluate the right side of the operator
				evaluate_expression(tokenizer, state_manager);
				add_instruction(state_manager, OP_EQUAL, 0, "#");
			}
		}

		if(check_token_consume(tokenizer, ":"))
		{
			// Evaluate the right side of the operator
			if(check_token_consume(tokenizer, "="))
			{
				evaluate_expression(tokenizer, state_manager);
				mu_log_message("TODO: Handle assign operator :=");
			}
		}

		if(check_token_consume(tokenizer, "!="))
		{
			// Evaluate the right side of the operator
			// Check for interval operator
			if(check_token_consume(tokenizer, "("))
			{
				// Evaluate the first expression
				evaluate_expression(tokenizer, state_manager);
				if(!check_token_consume(tokenizer, ","))
				{
					mu_parser_error("Expected a , in interval operator", tokenizer);
				}

				// Evaluate second expression
				evaluate_expression(tokenizer, state_manager);
				//intervall op !=(,)
				if(check_token_consume(tokenizer, ")"))
				{
					add_instruction(state_manager, OP_INTERVALOP8, 0, "#");
				}
				else if(check_token_consume(tokenizer, "]"))
				{
					add_instruction(state_manager, OP_INTERVALOP7, 0, "#");
				}
			}
			else if(check_token_consume(tokenizer, "["))
			{
				// Evaluate the first expression
				evaluate_expression(tokenizer, state_manager);
				if(!check_token_consume(tokenizer, ","))
				{
					mu_parser_error("Expected a , in interval operator", tokenizer);
				}

				// Evaluate the second expression
				evaluate_expression(tokenizer, state_manager);
				//intervall op !=[,)
				if(check_token_consume(tokenizer, ")"))
				{
					add_instruction(state_manager, OP_INTERVALOP6, 0, "#");
				}
				else if(check_token_consume(tokenizer, "]"))
				{
					add_instruction(state_manager, OP_INTERVALOP5, 0, "#");
				}
			}
			else // is the != op
			{
				// Evaluate the right side of the operator
				// evaluate_expression(tokenizer, state_manager);
				// add_instruction(state_manager, OP_EQUAL, 0, "#");

				// This seems to work, but if it breaks in the future, check the commented code surrounding this
				evaluate_expression(tokenizer, state_manager);
				add_instruction(state_manager, OP_NOTEQUAL, 0, "#");
			}
			// Not sure why we're adding the equal op first, and then the notequal op
			// evaluate_expression(tokenizer, state_manager);
			// add_instruction(state_manager, OP_NOTEQUAL, 0, "#");
		}

		if(check_token_consume(tokenizer, "<"))
		{
			// Evaluate the right side of the operator
			evaluate_expression(tokenizer, state_manager);
			add_instruction(state_manager, OP_LESS, 0, "#");
		}

		if(check_token_consume(tokenizer, "<="))
		{
			// Evaluate the right side of the operator
			evaluate_expression(tokenizer, state_manager);
			add_instruction(state_manager, OP_LESSEQUAL, 0, "#");
		}

		if(check_token_consume(tokenizer, ">"))
		{
			// Evaluate the right side of the operator
			evaluate_expression(tokenizer, state_manager);
			add_instruction(state_manager, OP_GREATER, 0, "#");
		}

		if(check_token_consume(tokenizer, ">="))
		{
			// Evaluate the right side of the operator
			evaluate_expression(tokenizer, state_manager);
			add_instruction(state_manager, OP_GREATEREQUAL, 0, "#");
		}

		if(check_token_consume(tokenizer, "&&"))
		{
			// Evaluate the right side of the operator
			evaluate_expression(tokenizer, state_manager);
			add_instruction(state_manager, OP_LOGAND, 0, "#");
		}

		if(check_token_consume(tokenizer, "||"))
		{
			// Evaluate the right side of the operator
			evaluate_expression(tokenizer, state_manager);
			add_instruction(state_manager, OP_LOGOR, 0, "#");
		}

		if(check_token_consume(tokenizer, "^^"))
		{
			// is this realy needed?
            //FIXME:Cant read ^^

			// Evaluate the right side of the operator
			evaluate_expression(tokenizer, state_manager);
			add_instruction(state_manager, OP_LOGXOR, 0, "#");
		}

		if(check_token_consume(tokenizer, "&"))
		{
			// Evaluate the right side of the operator
			mu_parse_primary(tokenizer, state_manager);
			add_instruction(state_manager, OP_AND, 0, "#");
		}

		if(check_token_consume(tokenizer, "~"))
		{
			// Evaluate the right side of the operator
			mu_parse_primary(tokenizer, state_manager);
			add_instruction(state_manager, OP_NOT, 0, "#");
		}

		if(check_token_consume(tokenizer, "|"))
		{
			// Evaluate the right side of the operator
			mu_parse_primary(tokenizer, state_manager);
			add_instruction(state_manager, OP_OR, 0, "#");
		}

		if(check_token_consume(tokenizer, "^"))
		{
			// Evaluate the right side of the operator
			mu_parse_primary(tokenizer, state_manager);
			add_instruction(state_manager, OP_XOR, 0, "#");
		}

		if(check_token_consume(tokenizer, "%"))
		{
			// Evaluate the right side of the operator
			mu_parse_primary(tokenizer, state_manager);
			add_instruction(state_manager, OP_MOD, 0, "#");
		}
	}
	//mu_log_message("Exiting mu_parse_term");
}

void mu_parse_primary(Tokenizer *tokenizer, MU_State_Manager *state_manager)
{
	// if(tokenizer->current_file_line >= 1425)
	// {
	// 	mu_log_message("Debugging time");
	// }

	//mu_log_message("mu_parse_primary: buffer = %s", tokenizer->buffer);
	// if(strcmp(tokenizer->buffer, "gethitvar") == 0)
	// {
	// 	mu_log_message("breaking point reached");
	// }
	// A negate operator
	if(check_token_consume(tokenizer, "-"))
	{
		//EvaluateExpression(tok,StateManager);
		mu_parse_primary(tokenizer, state_manager); // Not sure what this recursive call does .-.
		add_instruction(state_manager, OP_NEG, 0, "#");
	}
	// A number
	else if(check_token_is_number(tokenizer))
	{
		add_instruction(state_manager, OP_PUSH, mu_get_float(tokenizer), "#");
	}
	// A quoted string
	else if(check_token_is_quoted_string(tokenizer))
	{
		add_instruction(state_manager, OP_PUSH, 0, get_token(tokenizer));
	}
	// Should this be no_consume?
	else if(check_token_consume(tokenizer, "(")) // this was originally check_token_consume
	{
		//if(tokenizer->current_file_line == 1542)
		// if(tokenizer->current_file_line == 301)
		// {
		// 	mu_log_message("Debugging time");
		// }
		evaluate_expression(tokenizer, state_manager);
		// while(check_token_consume(tokenizer, "("))// this while test didn't exist in the original version
		// {
		// 	evaluate_expression(tokenizer, state_manager);
		// }

		if(!check_token_consume(tokenizer, ")"))
		{
			mu_parser_error("Missing )", tokenizer);
		}
	}
	else if(check_token_consume(tokenizer, "!"))
	{
		mu_parse_primary(tokenizer, state_manager);
		add_instruction(state_manager, OP_NOT, 0, "#");
	}
	else // Check for a trigger name
	{
		int trigger = get_trigger_type(get_token(tokenizer), tokenizer);
		evaluate_trigger(tokenizer, state_manager, trigger);
		add_trigger_to_state(state_manager, (u8)trigger);
// 		add_instruction(state_manager, i + OP_Abs, 0, "#");
// 		// Possibly add a new function here to parse trigger expressions?
// 		if(check_token_consume(tokenizer, "(")) 
// 		{
// 			// Some triggers have special parameters
// 			// If so, we should process them differently, instead of using evaluate_expression
// 			// Triggers that have been confirmed *should* go through evaluate_expression: AnimElemTime
// 			// TODO: Once processing has been implemented for all possible trigger conditions, figure
// 			// out a more efficient way to do this if need be
// 			//evaluate_expression(tokenizer, state_manager);
// 			char *trigger_expression = get_token(tokenizer);

// 			int hitvar = get_hitvar_type(trigger_expression, tokenizer);
// 			if(hitvar != -1)
// 			{
// 				add_instruction(state_manager, OP_PUSH, hitvar, "#");

// 				// If this function proves to be necessary, uncomment it
// 				// Otherwise, delete this function when optimising our code
// 				// Possible use case: getting the actual value of the hitvar
// 				// Currently we're only pushing the index of which hitvar we're trying to get
// 				// This would mean using a switch statement to check what the hitvar is, and
// 				// grabbing the appropriate variable from state_manager->hitvars
// 				// parse_hitvar(tokenizer, state_manager, hitvar);
// 			}
// 			else
// 			{
// #ifdef _WIN32
// 				if(stricmp(trigger_expression, "Vel") == 0)
// #else
// 				if(strcasecmp(trigger_expression, "Vel") == 0)
// #endif
// 				{
// 					mu_log_message("Definitely processing vel");
// 				}
// 				else
// 				{
// 					tokenizer->buffer_is_next_token = true;
// 					evaluate_expression(tokenizer, state_manager);
// 				}
// 			}

// 			if(!check_token_consume(tokenizer, ")"))
// 			{
// 				mu_parser_error("Missing ) after checking for trigger name", tokenizer);
// 			}
// 		}
 	}
	//mu_log_message("Exiting mu_parse_primary");
}

void parse_trigger_expresion(Tokenizer *tokenizer, MU_State_Manager *state_manager, int trigger)
{
	// Some triggers have special parameters
	// If so, we should process them differently, instead of using evaluate_expression
	// Triggers that have been confirmed *should* go through evaluate_expression: AnimElemTime
	// TODO: Once processing has been implemented for all possible trigger conditions, figure
	// out a more efficient way to do this if need be
	//evaluate_expression(tokenizer, state_manager);
	char *trigger_expression = get_token(tokenizer);
	int hitvar; // apparently I can't declare this in a switch statement?

	
	switch(trigger)
	{
		case 29: // GetHitVar
			hitvar = get_hitvar_type(trigger_expression, tokenizer);
			if(hitvar != -1)
			{
				add_instruction(state_manager, OP_PUSH, hitvar, "#");

				// If this function proves to be necessary, uncomment it
				// Otherwise, delete this function when optimising our code
				// Possible use case: getting the actual value of the hitvar
				// Currently we're only pushing the index of which hitvar we're trying to get
				// This would mean using a switch statement to check what the hitvar is, and
				// grabbing the appropriate variable from state_manager->hitvars
				// parse_hitvar(tokenizer, state_manager, hitvar);
			}
			break;
		case 75: // Pos
#ifdef _WIN32
			if(stricmp(tokenizer->buffer, "x") == 0)
			{
				add_instruction(state_manager, OP_PUSH, 0, "x");
			}
			else if(stricmp(tokenizer->buffer, "y") == 0)
			{
				add_instruction(state_manager, OP_PUSH, 0, "y");
			}
#else
			if(strcasecmp(tokenizer->buffer, "x") == 0)
			{
				add_instruction(state_manager, OP_PUSH, 0, "x");
			}
			else if(strcasecmp(tokenizer->buffer, "y") == 0)
			{
				add_instruction(state_manager, OP_PUSH, 0, "y");
			}
#endif
			else
			{
				char error_message[255];
				sprintf(error_message, "Unknown parameter %s for trigger 'Pos'", tokenizer->buffer);
				mu_parser_error(error_message, tokenizer);
			}
			break;
		case 107: // Vel
#ifdef _WIN32
			if(stricmp(tokenizer->buffer, "x") == 0)
			{
				add_instruction(state_manager, OP_PUSH, 0, "x");
			}
			else if(stricmp(tokenizer->buffer, "y") == 0)
			{
				add_instruction(state_manager, OP_PUSH, 0, "y");
			}
#else
			if(strcasecmp(tokenizer->buffer, "x") == 0)
			{
				add_instruction(state_manager, OP_PUSH, 0, "x");
			}
			else if(strcasecmp(tokenizer->buffer, "y") == 0)
			{
				add_instruction(state_manager, OP_PUSH, 0, "y");
			}
#endif
			else
			{
				char error_message[255];
				sprintf(error_message, "Unknown parameter %s for trigger 'Vel'", tokenizer->buffer);
				mu_parser_error(error_message, tokenizer);
			}
			break;
		default:
			tokenizer->buffer_is_next_token = true;
			evaluate_expression(tokenizer, state_manager);
			break;
	}
}

void evaluate_trigger(Tokenizer *tokenizer, MU_State_Manager *state_manager, int trigger)
{
	add_instruction(state_manager, trigger + OP_Abs, 0, "#");

	if(check_token_consume(tokenizer, "(")) 
	{
		parse_trigger_expresion(tokenizer, state_manager, trigger);
		if(!check_token_consume(tokenizer, ")"))
		{
			mu_parser_error("Missing ) after checking for trigger name", tokenizer);
		}
	}
	else // Check for specific triggers that can have parameters without brackets
	{
		if(trigger == 75 || trigger == 107) // Pos, Vel
		{
			parse_trigger_expresion(tokenizer, state_manager, trigger);
		}
	}
}

void parse_changestate(MU_State_Parser *state_parser, Tokenizer *tokenizer, MU_State_Manager *state_manager)
{
	// temp_changestate is unused?
	Changestate *temp_changestate = (Changestate*) mu_alloc(state_parser->parser_allocator, sizeof(Changestate));

	//TODO: Check for required parameters and print error message
	while(!check_token_no_consume(tokenizer, "[") && !tokenizer->at_end_of_file)
	{
		state_manager->n_current_inst = 0;
		if(check_token_consume(tokenizer, "value"))
		{
			if(!check_token_consume(tokenizer, "="))
			{
				mu_parser_error("Expected =", tokenizer);
			}
			evaluate_expression(tokenizer, state_manager);
		}
		else if(check_token_consume(tokenizer, "ctrl"))
		{
			if(!check_token_consume(tokenizer, "="))
			{
				mu_parser_error("Expected =", tokenizer);
			}
			evaluate_expression(tokenizer, state_manager);
		}
		else if(check_token_consume(tokenizer, "anim"))
		{
			if(!check_token_consume(tokenizer, "="))
			{
				mu_parser_error("Expected =", tokenizer);
			}
			evaluate_expression(tokenizer, state_manager);
		}
		else
		{
			mu_log_message("State Parser: Unexpected token %s", tokenizer->buffer);
		}
	}
	state_manager->n_current_inst = 0;
}

void parse_nothitby(Tokenizer *tokenizer, MU_State_Manager *state_manager)
{
	while(!check_token_no_consume(tokenizer, "[") && !tokenizer->at_end_of_file)
	{
		state_manager->n_current_inst = 0;
		if(check_token_consume(tokenizer, "value") || check_token_consume(tokenizer, "value2"))
		{
			if(!check_token_consume(tokenizer, "="))
			{
				mu_parser_error("Expected =", tokenizer);
			}
			evaluate_expression(tokenizer, state_manager);
		}
		else if(check_token_consume(tokenizer, "time"))
		{
			if(!check_token_consume(tokenizer, "="))
			{
				mu_parser_error("Expected =", tokenizer);
			}
			evaluate_expression(tokenizer, state_manager);
		}
		else
		{
			mu_log_message("State Parser: Unexpected token %s", tokenizer->buffer);
		}
	}
	state_manager->n_current_inst = 0;
}

void parser_controller(MU_State_Parser *state_parser, Tokenizer *tokenizer, MU_State_Manager *state_manager, u16 n_controller_type)
{
	switch(n_controller_type)
	{
		case 17: // ChangeState
			parse_changestate(state_parser, tokenizer, state_manager);
			break;
		case 45: // NotHitBy
			//parse_nothitby(tokenizer, state_manager);
			break;
	}
}

void parse_statedef(Tokenizer *tokenizer, MU_State_Manager *state_manager)
{
	mu_log_message("State Manager: adding & parsing statedef %i", state_manager->lp_statedef_list[state_manager->n_total_statedef - 1].state_number);
	
	if(state_manager->lp_statedef_list[state_manager->n_total_statedef - 1].state_number == 180)
	{
		mu_log_message("Debugging time");
	}
	while(!check_token_no_consume(tokenizer, "[") && !tokenizer->at_end_of_file)
	{
		//mu_log_message("Parsing statedef %i", state_manager->lp_statedef_list[state_manager->n_total_statedef - 1].state_number);
		// Parse state type
		if(check_token_consume(tokenizer, "type"))
		{
			if(!check_token_consume(tokenizer, "="))
			{
				mu_parser_error("Expected =", tokenizer);
			}

			// Get a single char
			char c = get_token(tokenizer)[0];
			//make sure we use uperchars (uppercase?)
			if(c >= 97)
			{
				c -=32;
			}

			switch(c)
			{
				case 'S':
					set_statedef_type(state_manager, STAND);
					break;
				case 'C':
					set_statedef_type(state_manager, CROUCH);
					break;
				case 'A':
					set_statedef_type(state_manager, AIR);
					break;
				case 'L':
					set_statedef_type(state_manager, LIEDOWN);
					break;
				case 'U':
					set_statedef_type(state_manager, UNTOUCH);
					break;
				default:
					mu_parser_error("Unknown statetype", tokenizer);
					break;
			}
		}
		else if(check_token_consume(tokenizer, "movetype"))
		{
			if(!check_token_consume(tokenizer, "="))
			{
				mu_parser_error("Expected '=' in line ", tokenizer);
			}

			// Get a single char
			char c = get_token(tokenizer)[0];
			//make sure we use uperchars (uppercase?)
			// TODO: is this a possible workaround for my strcmp_case_insensitive?
			if(c >= 97)
			{
				c -= 32;
			}

			switch(c)
			{
				case 'A':
					set_state_movetype(state_manager, ATTACK);
					break;
				case 'I':
					set_state_movetype(state_manager, IDLE);
					break;
				case 'H':
					set_state_movetype(state_manager, HIT);
					break;
				case 'U':
					set_state_movetype(state_manager, UNTOUCH);
					break;
				default:
					mu_parser_error("Unknown movetype", tokenizer);
					break;
			}
		}
		else if(check_token_consume(tokenizer, "physics"))
		{
			if(!check_token_consume(tokenizer, "="))
			{
				mu_parser_error("Expected =", tokenizer);
			}

			// Get a single char
			char c = get_token(tokenizer)[0];
			//make sure we use uperchars (uppercase?)
			if(c >= 97)
			{
				c -=32;
			}

			switch(c)
			{
				case 'S':
					set_state_physics_type(state_manager, STAND);
					break;
				case 'C':
					set_state_physics_type(state_manager, CROUCH);
					break;
				case 'A':
					set_state_physics_type(state_manager, AIR);
					break;
				case 'N':
					set_state_physics_type(state_manager, NONE);
					break;
				case 'U':
					set_state_physics_type(state_manager, UNTOUCH);
					break;
				default:
					mu_parser_error("Unknown physics type", tokenizer);
					break;
			}
		}
		else if(check_token_consume(tokenizer, "anim"))
		{
			if(!check_token_consume(tokenizer, "="))
			{
				mu_parser_error("Expected =", tokenizer);
			}

			if(!check_token_is_number(tokenizer))
			{
				mu_parser_error("Expected a number for anim", tokenizer);
			}

			set_state_anim(state_manager, mu_get_int(tokenizer));
		}
		else if(check_token_consume(tokenizer, "velset"))
		{
			if(!check_token_consume(tokenizer, "="))
			{
				mu_parser_error("Expected =", tokenizer);
			}

			float x = mu_get_float(tokenizer);

			if(!check_token_consume(tokenizer, ","))
			{
				mu_parser_error("Expected ,", tokenizer);
			}

			float y = mu_get_float(tokenizer);
			set_velset(state_manager, x, y);
		}
		else if(check_token_consume(tokenizer, "ctrl"))
		{
			if(!check_token_consume(tokenizer, "="))
			{
				mu_parser_error("Expected =", tokenizer);
			}

			if(!check_token_is_number(tokenizer))
			{
				mu_parser_error("Expected a number for ctrl", tokenizer);
			}

			set_state_ctrl(state_manager, mu_get_int(tokenizer));
		}
		else if(check_token_consume(tokenizer, "poweradd"))
		{
			if(!check_token_consume(tokenizer, "="))
			{
				mu_parser_error("Expected =", tokenizer);
			}

			if(!check_token_is_number(tokenizer))
			{
				mu_parser_error("Expected a number for poweradd", tokenizer);
			}

			set_state_poweradd(state_manager, mu_get_int(tokenizer));
		}
		else if(check_token_consume(tokenizer, "juggle"))
		{
			if(!check_token_consume(tokenizer, "="))
			{
				mu_parser_error("Expected =", tokenizer);
			}

			if(!check_token_is_number(tokenizer))
			{
				mu_parser_error("Expected a number for juggle", tokenizer);
			}

			set_state_juggle(state_manager, mu_get_int(tokenizer));
		}
		else if(check_token_consume(tokenizer, "facep2"))
		{
			if(!check_token_consume(tokenizer, "="))
			{
				mu_parser_error("Expected =", tokenizer);
			}

			if(!check_token_is_number(tokenizer))
			{
				mu_parser_error("Expected a number for facep2", tokenizer);
			}

			set_state_facep2(state_manager, mu_get_int(tokenizer));
		}
		else if(check_token_consume(tokenizer, "hitdefpersist"))
		{
			if(!check_token_consume(tokenizer, "="))
			{
				mu_parser_error("Expected =", tokenizer);
			}

			if(!check_token_is_number(tokenizer))
			{
				mu_parser_error("Expected a number for hitdefpersist", tokenizer);
			}

			set_state_hitdef_persist(state_manager, mu_get_int(tokenizer));
		}
		else if(check_token_consume(tokenizer, "movehitpersist"))
		{
			if(!check_token_consume(tokenizer, "="))
			{
				mu_parser_error("Expected =", tokenizer);
			}

			if(!check_token_is_number(tokenizer))
			{
				mu_parser_error("Expected a number for movehitpersist", tokenizer);
			}

			set_movehit_persist(state_manager, mu_get_int(tokenizer));
		}
		else if(check_token_consume(tokenizer, "hitcountpersist"))
		{
			if(!check_token_consume(tokenizer, "="))
			{
				mu_parser_error("Expected =", tokenizer);
			}

			if(!check_token_is_number(tokenizer))
			{
				mu_parser_error("Expected a number for hitcountpersist", tokenizer);
			}

			set_state_hitcounter_persist(state_manager, mu_get_int(tokenizer));
		}
		else if(check_token_consume(tokenizer, "sprpriority"))
		{
			if(!check_token_consume(tokenizer, "="))
			{
				mu_parser_error("Expected =", tokenizer);
			}

			if(!check_token_is_number(tokenizer))
			{
				mu_parser_error("Expected a number for sprpriority", tokenizer);
			}

			set_spr_priority(state_manager, mu_get_int(tokenizer));
		}
		else // Failed in statedef
		{
			mu_log_message("Unknown token at line %s", get_token(tokenizer));
			break;
		}
	}
}

void parse_trigger(Tokenizer *tokenizer, MU_State_Manager *state_manager)
{
	tokenizer->return_negative_separately_from_number = true;
	evaluate_expression(tokenizer, state_manager);
	if(state_manager->lp_statedef_list[state_manager->n_total_statedef - 1].lp_state[state_manager->n_total_state - 1].n_how_many_triggers == 0)
	{
		state_manager->lp_statedef_list[state_manager->n_total_statedef - 1].lp_state[state_manager->n_total_state - 1].triggers[state_manager->n_current_trigger].p_inst = (Instruction*) mu_alloc(state_manager->state_allocator, sizeof(Instruction) * state_manager->n_current_inst);
		memcpy(&state_manager->lp_statedef_list[state_manager->n_total_statedef - 1].lp_state[state_manager->n_total_state - 1].triggers[state_manager->n_current_trigger].p_inst, state_manager->p_inst, sizeof(Instruction) * state_manager->n_current_inst);
	}
	else
	{
		mu_realloc(state_manager->state_allocator, state_manager->lp_statedef_list[state_manager->n_total_statedef - 1].lp_state[state_manager->n_total_state - 1].triggers[state_manager->n_current_trigger - 1].p_inst, sizeof(Instruction) * (state_manager->n_current_inst + 1));
		memcpy(&state_manager->lp_statedef_list[state_manager->n_total_statedef - 1].lp_state[state_manager->n_total_state - 1].triggers[state_manager->n_current_trigger - 1].p_inst[1], state_manager->p_inst, sizeof(Instruction) * state_manager->n_current_inst);
	}
	state_manager->n_current_inst = 0;

	tokenizer->return_negative_separately_from_number = false;
}

void parse_hitvar(Tokenizer *tokenizer, MU_State_Manager *state_manager, int hitvar)
{
	mu_log_message("State Parser: parsing hitvar %s", str_hitvar_type[hitvar]);
}

void parse_state(MU_State_Parser *state_parser, Tokenizer *tokenizer, MU_State_Manager *state_manager)
{
	// Relocated to add_state() in state_manager.c
	//state_manager->n_total_state++;
	while(!check_token_no_consume(tokenizer, "[") && !tokenizer->at_end_of_file)
	{
		// In this loop, we need to check every parameter **that isn't controller specific**
		// Controller specific parameters should be parsed in parser_controller()
		if(check_token_consume(tokenizer, "type"))
		{
			if(!check_token_consume(tokenizer, "="))
			{
				mu_log_message("Expected =");
			}

			// Note to self: state_parser seems to serve no purpose, so all statements that make use
			// of it will be phased out for practical implementations of its intended functionality
			//state_parser->n_controller = get_controller_type(get_token(tokenizer), tokenizer);
			//mu_log_message("State Manager: Parsing state %s", str_controller_types[state_parser->n_controller]);
			state_manager->lp_statedef_list[state_manager->n_total_statedef - 1].lp_state[state_manager->n_total_state - 1].n_type = get_controller_type(get_token(tokenizer), tokenizer);
			mu_log_message("State Manager: Parsing state %s", str_controller_types[state_manager->lp_statedef_list[state_manager->n_total_statedef - 1].lp_state[state_manager->n_total_state - 1].n_type]);
		}
		else if(check_token_consume(tokenizer, "triggerall"))
		{
			if(!check_token_consume(tokenizer, "="))
			{
				mu_parser_error("Expected =", tokenizer);
			}

			while(!tokenizer->at_end_of_line && !tokenizer->at_end_of_file)
			{
				get_token(tokenizer);
			}
		}
		else if(check_token_consume(tokenizer, "trigger1"))
		{
			if(!check_token_consume(tokenizer, "="))
			{
				mu_parser_error("Expected =", tokenizer);
			}

			parse_trigger(tokenizer, state_manager);
		}
		// experimental code to process trigger2
		else if(check_token_consume(tokenizer, "trigger2"))
		{
			if(!check_token_consume(tokenizer, "="))
			{
				mu_parser_error("Expected =", tokenizer);
			}

			parse_trigger(tokenizer, state_manager);
		}
		else
		{
			// Add check here
			// if triggers == NULL, set the trigger to 1 somehow
			// Not sure if this is correct, as there may be alternate cases where triggers is not set
			// Perhaps do some more research on this before implementing it`
			break;
		}
	}

	// if(tokenizer->current_file_line == 1637)
	// {
	// 	mu_log_message("Debugging time");
	// }

	// Parse the controller

	parser_controller(state_parser, tokenizer, state_manager, state_manager->lp_statedef_list[state_manager->n_total_statedef - 1].lp_state[state_manager->n_total_state - 1].n_type);
}

void parse_statefile(MU_State_Parser *state_parser, const char *str_filename, MU_State_Manager *state_manager, MU_Allocator *allocator)
{
	state_parser->parser_allocator = allocator;
	Tokenizer *tokenizer = tokenizer_init(100, NULL, NULL, 23);
	bool found_state = false;

	if(!mu_open_file(tokenizer, str_filename))
	{
		mu_log_message("State Parser: File %s not found", str_filename);
	}

	tokenizer->is_case_sensitive = false;
	tokenizer->return_negative_separately_from_number = false;

	while(!tokenizer->at_end_of_file)
	{
		found_state = false;
		if(check_token_consume(tokenizer, "["))
		{
			if(check_token_consume(tokenizer, "statedef"))
			{
				found_state = true;
				if(!check_token_is_number(tokenizer))
				{
					mu_parser_error("Expected a number in statedef block", tokenizer);
				}

				// This seems to work as intended
				// it sets everything to default values, which then get overwritten as necessary
				// in parse_statedef()
				add_statedef(state_manager, mu_get_int(tokenizer));

				// Skip useless stuff
				while(!tokenizer->at_end_of_line)
				{
					get_token(tokenizer);
				}

				// Parse the statedef
				parse_statedef(tokenizer, state_manager);
			}
			if(check_token_consume(tokenizer, "state"))
			{
				found_state = true;

				if(!check_token_is_number(tokenizer))
				{
					mu_parser_error("Expected a number in state block", tokenizer);
				}

				// TODO: this seems to allocate memory but not save any information to it
				// possibly intentional, and the info is saved elsewhere?
				add_state(state_manager, mu_get_int(tokenizer), 0);

				// Skip useless stuff
				while(!tokenizer->at_end_of_line)
				{
					get_token(tokenizer);
				}

				parse_state(state_parser, tokenizer, state_manager);
			}
		}

		// Skip useless stuff
		if(!found_state)
		{
			get_token(tokenizer);
		}
	}
	mu_close_file(tokenizer);
}