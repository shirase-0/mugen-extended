#ifndef __MUSTATEMANAGER__
#define __MUSTATEMANAGER__

#define S_TYPE 2
#define S_MOVETYPE 4
#define S_PHYSICS 8
#define S_ANIM 10
#define S_CTRL 12
#define S_POWERADD 14
#define S_JUGGLE 16
#define S_HITDEFPERSIST 18
#define S_MOVEHITPERSIST 20
#define S_HITCOUNTPERSIST 22
#define S_SPRPRIO 24
#define S_FACEP2 26

MU_State_Manager *state_manager_init();
void reset_state_manager(MU_State_Manager *state_manager);
void realloc_statedef(MU_State_Manager *state_manager, u16 index);
void realloc_state_manager(MU_State_Manager *state_manager);

bool is_statedef_available(MU_State_Manager *state_manager, s32 n_statedef_num);
void set_default_statedef(MU_State_Manager *state_manager);

void add_statedef(MU_State_Manager *state_manager, s32 n_statedef_num);
void add_state(MU_State_Manager *state_manager, s32 n_state_num, char *str_some_number);
void add_trigger_to_state(MU_State_Manager *state_manager, u8 n_type);
void add_instruction(MU_State_Manager *state_manager, u16 n_opcode, float value, const char *str_value);
void exchange_inst(MU_State_Manager *state_manager);

PL_Statedef *get_statedef(MU_State_Manager *state_manager, int n_state_number);
PL_Statedef *get_current_statedef(MU_State_Manager *state_manager);

// StateDef setters
void set_statedef_type(MU_State_Manager *state_manager, u8 n_type);
void set_state_movetype(MU_State_Manager *state_manager, u8 n_type);
void set_state_physics_type(MU_State_Manager *state_manager, u8 n_type);
void set_state_anim(MU_State_Manager *state_manager, s32 n_anim);
void set_state_ctrl(MU_State_Manager *state_manager, s8 n_ctrl);
void set_state_poweradd(MU_State_Manager *state_manager, s16 n_poweradd);
void set_state_juggle(MU_State_Manager *state_manager, s16 n_juggle);
void set_state_hitdef_persist(MU_State_Manager *state_manager, bool b_hitdef);
void set_movehit_persist(MU_State_Manager *state_manager, bool b_movehit);
void set_state_hitcounter_persist(MU_State_Manager *state_manager, bool b_hitcounter);
void set_spr_priority(MU_State_Manager *state_manager, u8 n_spr_priority);
void set_state_facep2(MU_State_Manager *state_manager, bool b_facep2);
void set_velset(MU_State_Manager *state_manager, float x, float y);

// These might not be needed, as they're simple getters and setters
Instruction *get_inst();
void set_alloc(MU_Allocator *allocator);
int get_how_many_inst(); // return state_manager->n_current_inst
void new_inst(); // state_manager->n_current_inst = 0

// These functions either don't have any code, or have commented out code
void *get_controller();
void mu_state_manager_cleanup();
void set_param(int n_param);

#endif