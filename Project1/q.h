#ifndef __q_H__
#define __q_H__

#include "TCB.h"


void InitQ  (TCB_t**);
void AddQ   (TCB_t**,TCB_t**);
void DelQ   (TCB_t**);
void RotateQ(TCB_t**);
void PrintQ(TCB_t*);


void InitQ(TCB_t** Q)
{
        (*Q)->prev = *Q;
        (*Q)->next = *Q;
}

void AddQ(TCB_t** Q, TCB_t** temp)
{
	InitQ(temp);
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


void DelQ(TCB_t** Q)
{
        TCB_t* temp;

        if(*Q == NULL)
                return;
        else
        {
                temp = *Q;
                *Q = (*Q)->next;

                if(*Q == temp)
                {
                        *Q = NULL;
                        free(temp);
                        return;
                }
                else
                {
                        temp->prev->next = *Q;
                        (*Q)->prev = temp->prev;
                        free(temp);
                }
        }

}




void RotateQ(TCB_t** Q)
{
        if(*Q == NULL)
                return;
        else
                (*Q) = (*Q)->next;

}


void PrintQ(TCB_t* Q)
{
        TCB_t* temp;
        temp = Q;

        if(Q == NULL)
                printf("Empty Queue\n");
        else
        {
                temp = temp->next;
                while( temp != Q)
                {
                        printf("id \t");
                        temp = temp->next;
                }
        }

        printf("\n");
}



#endif
