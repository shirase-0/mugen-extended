#ifndef __MUCONTROLLEREXECUTER__
#define __MUCONTROLLEREXECUTER__

MU_Controller_Executer *controller_executer_init();

// Init function table
void init_func_table(MU_Controller_Executer *exec);
void controller_set_player(MU_Controller_Executer *exec, Player *player); // exec->player = player;

// Execute the controller
void mu_execute_controller(int n_controller);

// Controller functions
void null_controller();
void change_state();
void change_anim();
void velset();
void varset();

#endif