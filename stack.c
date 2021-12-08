#include "mu_headers.h"

MU_Stack *mu_stack_init()
{
	// CHECK: replace this malloc call with calloc and avoid the remaining steps?
	MU_Stack *stack = (MU_Stack*) malloc(sizeof(MU_Stack));
	stack->n_pos = 0; // Not sure why we're setting this to 0 if we're gonna memset the whole stack anyway
	memset(stack, 0, sizeof(MU_Stack));

	return stack;
}

void mu_reset_stack(MU_Stack *stack)
{
	stack->n_pos = 0; // Not sure why we're setting this to 0 if we're gonna memset the whole stack anyway
	memset(stack, 0, sizeof(MU_Stack));
}

void mu_push(MU_Stack *stack, float n_value, char *str_value)
{
	if(stack->n_pos > STACKSIZE)
	{
		mu_log_message("Stack: Push stack overflow");
	}

	if(str_value != NULL)
	{
		strcpy(stack->stack_arr[stack->n_pos].stack_string, str_value);
	}
	stack->stack_arr[stack->n_pos].value = n_value;

	stack->n_pos++;
}

Stacktype mu_pop(MU_Stack *stack) // Change return type to pointer?
{
	if(stack->n_pos < 0)
	{
		mu_log_message("Stack: Pop stack underflow");
	}

	return stack->stack_arr[--stack->n_pos];
}