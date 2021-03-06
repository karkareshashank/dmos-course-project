#ifndef __TCB_H__
#define __TCB_H__

#include <ucontext.h>
#include <string.h>

typedef struct TCB_t{
		int		thread_id;
		struct TCB_t 	*next;
		struct TCB_t 	*prev;
		ucontext_t    	context;
}TCB_t;


void init_TCB( TCB_t *tcb, void (*exit_fn_ptr), void *stackP, int stack_size,void* (*function)(void*), void* arg)
{
	memset(tcb,'\0',sizeof(TCB_t));
	getcontext(&tcb->context);
	tcb->context.uc_stack.ss_sp = stackP;
	tcb->context.uc_stack.ss_size = (size_t)stack_size;
//	tcb->context.uc_link = exit_context;
	makecontext(&tcb->context, exit_fn_ptr,2,function,arg);
}


#endif
