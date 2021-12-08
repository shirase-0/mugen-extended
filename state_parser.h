#ifndef __MUSTATEPARSER__
#define __MUSTATEPARSER__

// Note to self: This is implemented as a class in the original version, but as it only contains a 
// MU_Allocator, I've decided simply to pass in an allocator as a parameter for all functions that
// require access to it. The original version uses the player's allocator, so that's the one that should
// be passed in (P1, P2, P3, or P4)
// ^^^^^^^^^^^^^^^^^^^^^^^
// I changed this to a struct because there's also int n_controller
// If it turns out this variable is unnecessary, refactor this code to the idea above
// ----------------------------------------------------------------------------------

// The original version uses & instead of * for these pointer arguments
// May need to change it back to that if this approach doesn't work for whatever reason

// Helper functions
void mu_parser_error(const char *str_error_message, Tokenizer *tokenizer);
int get_controller_type(const char *str_type, Tokenizer *tokenizer);
int get_trigger_type(const char *str_trigger, Tokenizer *tokenizer);
int get_hitvar_type(const char *str_hitvar, Tokenizer *tokenizer);

// Expression parser
void evaluate_expression(Tokenizer *tokenizer, MU_State_Manager *state_manager);
void mu_parse_term(Tokenizer *tokenizer, MU_State_Manager *state_manager);
void mu_parse_primary(Tokenizer *tokenizer, MU_State_Manager *state_manager);

void parse_trigger_expresion(Tokenizer *tokenizer, MU_State_Manager *state_manager, int trigger);
void evaluate_trigger(Tokenizer *tokenizer, MU_State_Manager *state_manager, int trigger);

// Controller parser
void parse_changestate(MU_State_Parser *state_parser, Tokenizer *tokenizer, MU_State_Manager *state_manager);
void parse_nothitby(Tokenizer *tokenizer, MU_State_Manager *state_manager);
void parser_controller(MU_State_Parser *state_parser, Tokenizer *tokenizer, MU_State_Manager *state_manager, u16 n_controller_type);

void parse_statedef(Tokenizer *tokenizer, MU_State_Manager *state_manager);
void parse_trigger(Tokenizer *tokenizer, MU_State_Manager *state_manager);
void parse_hitvar(Tokenizer *tokenizer, MU_State_Manager *state_manager, int hitvar);
void parse_state(MU_State_Parser *state_parser, Tokenizer *tokenizer, MU_State_Manager *state_manager);
void parse_statefile(MU_State_Parser *state_parser, const char *str_filename, MU_State_Manager *state_manager, MU_Allocator *allocator);

#endif