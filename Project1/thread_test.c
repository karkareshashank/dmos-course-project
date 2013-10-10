#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "threads.h"

#define BUF_SIZE	10
#define PRODUCE_LIMIT   20

int buffer[BUF_SIZE];
int count = 0;
int id	  = 0;


void producer()
{
	int i;
	for(i = 0; i < PRODUCE_LIMIT;)
	{
		if(count == BUF_SIZE)	
			yield();
		else
		{
			printf("%d --- %d --- count \n",count,PRODUCE_LIMIT);
			buffer[count] = id;
			count++;
			id++;
			i++;
		}
	}
}




void consumer()
{
	int flag = 0;
	int item;

	while(flag != 2)
	{
		if(count == 0)
		{
			yield();
			flag++;
		}
		else
		{
			flag = 0;
			item = buffer[--count];
			printf("%d \n",item);
		}

	}


}


int main(int argc,char** argv)
{	

	start_thread(producer);
	start_thread(producer);	
	start_thread(consumer);

	join_all_thread();

	run();

	printf("ending from main() \n");

}
