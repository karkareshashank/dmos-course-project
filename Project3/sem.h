#include "threads.h"

typedef struct semaphore
{
	int counter;
	TCB_t *SemQ;	
}semaphore_t;


semaphore_t *CreateSem(int InputValue)
{
	semaphore_t *sem = NULL;
	sem = (semaphore_t *) malloc(sizeof(semaphore_t));
	sem->counter = InputValue;
	sem->SemQ    = NULL;
	return sem;
}

void DeleteSem(semaphore_t* sem)
{
	if(sem){
		free(sem);
		sem = NULL;
	}

}

void P(semaphore_t *sem)
{
	sem->counter--;
	if(sem->counter < 0)
	{
		TCB_t *current = DelQ(&RunQ);
		AddQ(&(sem->SemQ),&current);
		swapcontext(&(current->ucontext), &(RunQ->ucontext));	
	}
}


void V(semaphore_t *sem)
{
	sem->counter++;
	if(sem->counter <= 0)
	{
		TCB_t *thread = DelQ(&(sem->SemQ));
		AddQ(&RunQ, &thread);
	}
	yield();
}
