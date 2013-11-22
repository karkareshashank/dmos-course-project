# include "q.h"
# define STACK_SIZE 8192

TCB_t *RunQ = NULL;
int tid = 0;

void start_thread(void (*function)(void))
{ 
	void   *stack   = malloc(STACK_SIZE);
	TCB_t  *tcb   	= malloc(sizeof(TCB_t));

	init_TCB(tcb, function, stack, STACK_SIZE);
	tcb->thread_id = tid++;
	AddQ(&RunQ, &tcb);
}


void run()
{ 
	TCB_t * tmp = RunQ;

    ucontext_t parent;     
    getcontext(&parent);  
    swapcontext(&parent, &(RunQ->ucontext));

}


void yield()
{
    RotateQ(&RunQ);
    swapcontext(&(RunQ->prev->ucontext),  &(RunQ->ucontext));
}

int get_threadid()
{
	return RunQ->thread_id;
}
