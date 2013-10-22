#ifndef __THREADS_H__
#define __THREADS_H__

#include "q.h"

#define STACK_SIZE 8192

TCB_t*     RunQ = NULL;
int    	   join_all = 0;
int	   tid = 0;



void exit_thread(void* (*function)(void*), void* arg);
int  start_thread(void* (*function)(void*),void* arg);
int  get_threadid(void);
void yield();




/* This function initializes and adds the new thread into the runQ */
int start_thread(void* (*function)(void*),void* arg)
{
	void*    stack;
	TCB_t*	 tcb;


	/* initialize for the exit */
	stack = malloc(STACK_SIZE);
	tcb   = (TCB_t*)malloc(sizeof(TCB_t));

	init_TCB(tcb, exit_thread, stack, STACK_SIZE,function,arg);
	tcb->thread_id = tid++;

	AddQ(&RunQ,&tcb);

	return tcb->thread_id;

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


void exit_thread(void*(*function)(void*),void* arg)
{

	function(arg);



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


int get_threadid()
{
	return RunQ->thread_id;
}

#endif
