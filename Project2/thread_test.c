/*
 * Name: Kedar Joshi    	(1205110916)
 *	 Shashank Karkare	(1205168441)
 *
 * PROJECT 2
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "sem.h"


int count = 0;
int id	  = 0;
int rwc	  = 0;
int wwc   = 0;
int wc	  = 0;
int rc    = 0;
int writer_count = 0;
char shared_data[50];

semaphore_t *wsem;
semaphore_t *rsem;
semaphore_t *mutex;

void reader()
{
	int i;
	int tid;
	
	tid = get_threadid();
	for(i = 0; i > -1 ;)
	{
		// Reader Entry
		P(mutex);
		if ((wwc>0) || (wc>0))
		{
        	    	rwc++;
	            	V(mutex);
            		P(rsem);
        	    	P(mutex);
	            	rwc--;  
		}
  		rc++;
		V(mutex);

		// Read Code
		printf("Thread %d : Reading the stuff : %s \n",tid,shared_data);



		// Reader Exit
		P(mutex);
		rc--;
		if ((rc == 0) && (wwc > 0))
			V(wsem);
		V(mutex);
		usleep(100000);
	}
}




void writer()
{
	int flag = 0;
	int item;
	int i;
	int tid;


	tid = get_threadid();
	while(1)
	{
		// Writer Entry
		P(mutex);
		if ( (rc>0) || (wc>0) || (rwc>0) || (wwc >0) )
         	{
	        	wwc++;
            		V(mutex);
            		P(wsem);
            		P(mutex);
            		wwc--; 
		}
 		wc++;
		V(mutex);


		// Write code
		printf("Thread %d :Writing the stuff \n",tid);
		writer_count++;
		sprintf(shared_data,"Written to me %d times !!\n",writer_count);


		// Writer Exit
		P(mutex);
   		wc-- ;  
   		if (rwc>0)
		{ 
      			for (i = 1 ; i <= rwc; i++) 
				V(rsem);
		}
   		else if (wwc>0) 
			V(wsem);
  		V(mutex);



		usleep(100000);

	}


}


int main(int argc,char** argv)
{	
	rsem = CreateSem(0);
	wsem = CreateSem(0);
	mutex = CreateSem(1);

	start_thread(reader);
	start_thread(writer);	
	start_thread(writer);
	start_thread(reader);
	start_thread(reader);
	start_thread(reader);
	start_thread(reader);

	run();

	printf("ending from main() \n");

}
