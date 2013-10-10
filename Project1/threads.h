#ifndef __THREADS_H__
#define __THREADS_H__

#include "q.h"

#define STACK_SIZE 8192

TCB_t*     RunQ = NULL;
int    	   join_all = 0;
ucontext_t exit_context;

void exit_thread();
void start_thread(void (*function)(void));
void yield();




/* This function initializes and adds the new thread into the runQ */
void start_thread(void (*function)(void))
{
	void* 	 exit_stack;
	void*    stack;
	TCB_t*	 tcb;


	/* initialize for the exit */
	exit_stack = malloc(STACK_SIZE);
	getcontext(&exit_context);
        exit_context.uc_stack.ss_sp = exit_stack;
        exit_context.uc_stack.ss_size = (size_t)STACK_SIZE;
        makecontext(&exit_context, exit_thread, 0);
	// done with exit code //
	

	stack = malloc(STACK_SIZE);
	tcb   = (TCB_t*)malloc(sizeof(TCB_t));

	init_TCB(tcb, function, stack, STACK_SIZE,&exit_context);


	AddQ(&RunQ,&tcb);
}


/* Starts the execution of the thread */
void run()
{
	ucontext_t parent;
	getcontext(&parent);
	swapcontext(&parent, &(RunQ->context));
}


void yield()
{

	TCB_t *prev;
	int 	res;
	
	prev = RunQ;
	RotateQ(&RunQ);
	res = swapcontext(&(prev->context),&(RunQ->context));

}


void exit_thread()
{
	if(join_all == 0)
	{
		exit(0);

	}
	else
	{
		ucontext_t temp;
		DelQ(&RunQ);

		if(RunQ == NULL)
		{
			exit(0);
		}
		else
		{
			getcontext(&temp);
			swapcontext(&temp,&(RunQ->context));
		}
	}
}

void join_all_thread()
{
	join_all = 1;
}


#endif
