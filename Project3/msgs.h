/*
 *  msgs.h
 */

#ifndef MSGS_H_
#define MSGS_H_

#include "sem.h"

#define NUM_PORTS		100
#define PORT_CAP		10
#define MSG_SIZE		10

static int	init_flag = 0;

typedef struct Q{
		struct Q* 	next;
		struct Q* 	prev;
		int		msg[10];
}Q_t;

typedef struct port{
		Q_t*		head;
}port_t;

port_t ports[NUM_PORTS];

semaphore_t* prod_sem[NUM_PORTS];
semaphore_t* cons_sem[NUM_PORTS];
semaphore_t* mutex[NUM_PORTS];





/*  our queue handling functions for the msgs on the
 *  port
 */
void my_InitQ(Q_t** Q)
{
        (*Q)->prev = *Q;
        (*Q)->next = *Q;
}

void my_AddQ(Q_t** Q, Q_t** temp)
{
	my_InitQ(temp);
        if(*Q == NULL)
                *Q = *temp;
        else
        {
                (*temp)->next = *Q;
                (*temp)->prev = (*Q)->prev;
                (*Q)->prev->next = *temp;
                (*Q)->prev = *temp;
        }
}


Q_t *my_DelQ(Q_t** Q)
{
        Q_t* temp = NULL;

        if(*Q == NULL)
                return NULL;
        else
        {
                temp = *Q;
                *Q = (*Q)->next;

                if(*Q == temp)
                {
                        *Q = NULL;
                }
                else
                {
                        temp->prev->next = *Q;
                        (*Q)->prev = temp->prev;
                }
        }
	return temp;

}


// Initializes all the semaphores for first time
void init_semaphores()
{
	int i;
	for(i = 0;i < NUM_PORTS;i++){
		prod_sem[i] = CreateSem(PORT_CAP);
		cons_sem[i] = CreateSem(0);
		mutex[i]    = CreateSem(1);
	}


}


// Send function 
void send(int port_num, int* msg)
{
	Q_t*	new1;
	int 	i;

	new1 = (Q_t*)malloc(sizeof(Q_t));
	// For first time initialization of semaphores
	if(init_flag == 0){
			init_semaphores();
			init_flag = 1;
	}

	// Send Code
	for(i = 0;i < 10;i++){
		new1->msg[i] = msg[i];
	}

	P(prod_sem[port_num]);
	P(mutex[port_num]);
	my_AddQ(&ports[port_num].head,&new1);
	V(mutex[port_num]);
	V(cons_sem[port_num]);

}



// Recv function
void recv(int port_num, int* msg)
{
	Q_t	*new1;
	int 	i;

	// For first time initialization of semaphores
	if(init_flag == 0){
		 init_semaphores();
		 init_flag = 1;
	}

	// Recv code

	P(cons_sem[port_num]);
	P(mutex[port_num]);
	new1 = my_DelQ(&ports[port_num].head);
	V(mutex[port_num]);
	V(prod_sem[port_num]);

	for(i = 0;i < MSG_SIZE;i++){
		msg[i] = new1->msg[i];
	}

	if(new1){
		free(new1);
		new1 = NULL;
	}

}













#endif /* MSGS_H_ */
