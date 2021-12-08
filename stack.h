#ifndef __MUSTACK__
#define __MUSTACK__

MU_Stack *mu_stack_init();
void mu_reset_stack(MU_Stack *stack);

void mu_push(MU_Stack *stack, float n_value, char *str_value);
Stacktype mu_pop(MU_Stack *stack); // Change return type to pointer?

#endif