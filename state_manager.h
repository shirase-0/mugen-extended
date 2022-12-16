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
void realloc_statedef(MU_State_Manager *state_manager, uint16_t index);
void realloc_state_manager(MU_State_Manager *state_manager);

bool is_statedef_available(MU_State_Manager *state_manager, int statedef_num);
void set_default_statedef(MU_State_Manager *state_manager);

void add_statedef(MU_State_Manager *state_manager, int statedef_num);
void add_state(MU_State_Manager *state_manager, int state_num);
void add_trigger_to_state(MU_State_Manager *state_manager, uint8_t type);
void add_instruction(MU_State_Manager *state_manager, uint16_t opcode, float value, const char *str_value); // TODO: change value to double?
void exchange_inst(MU_State_Manager *state_manager);

Statedef *get_statedef(MU_State_Manager *state_manager, int state_num);
Statedef *get_current_statedef(MU_State_Manager *state_manager);

// Setters
void set_statedef_type(MU_State_Manager *state_manager, uint8_t type);
void set_state_movetype(MU_State_Manager *state_manager, uint8_t type);
void set_state_physics_type(MU_State_Manager *state_manager, uint8_t type);
void set_state_anim(MU_State_Manager *state_manager, int anim);
void set_state_ctrl(MU_State_Manager *state_manager, int8_t has_ctrl);
void set_state_poweradd(MU_State_Manager *state_manager, int16_t poweradd);
void set_state_juggle(MU_State_Manager *state_manager, int16_t juggle);
void set_state_hitdef_persist(MU_State_Manager *state_manager, bool hitdef_persist);
void set_movehit_persist(MU_State_Manager *state_manager, bool movehit_persist);
void set_state_hitcount_persist(MU_State_Manager *state_manager, bool hitcount_persist);
void set_spr_priority(MU_State_Manager *state_manager, uint8_t spr_priority);
void set_state_facep2(MU_State_Manager *state_manager, bool facep2);
void set_velset(MU_State_Manager *state_manager, float x, float y); // TODO: change these to doubles?

// These might not be needed, as they're simple getters and setters
Instruction *get_inst();
void set_alloc(MU_Allocator *allocator);
int get_how_many_inst(); // return state_manager->n_current_inst
void new_inst(); // state_manager->n_current_inst = 0

// These functions either don't have any code, or have commented out code
void *get_controller();
void mu_state_manager_cleanup();
void set_param(int param);

#endif