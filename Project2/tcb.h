# include<ucontext.h>
# include<string.h>
typedef struct TCB
{
	int thread_id;
	ucontext_t ucontext;
	struct TCB *prev;
	struct TCB *next; 
}TCB_t;

void init_TCB (TCB_t *tcb, void *function, void *stackP, int stack_size)
{
	 memset(tcb, 0, sizeof(TCB_t));       
	 getcontext(&tcb->ucontext);
	 tcb->ucontext.uc_stack.ss_sp = stackP;
	 tcb->ucontext.uc_stack.ss_size = (size_t) stack_size;
	 makecontext(&tcb->ucontext, function, 0);
}



