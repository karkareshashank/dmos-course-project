/*
 * msgs_test.c
 *------------------------------------
 * Client message sent is of length 10 but the first element of the 
 * message is the reply port. So effectively msg is of length 
 * 9.
 * Server message sent is of length 10 but the last elemnet is a dummy 0 
 * beacuse actual message length was 9 as sent from the client.
 *
 *  Created on: Nov 16, 2013
 *      Author: Shashank Karkare (1205168441)
 *	Author: Kedar    Joshi 	 (1205110916)
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "msgs.h"


#define SERVER_PORT1	98
#define SERVER_PORT2	99


int	client_port  = 0;
semaphore_t*	cpsem;


// Server function //
void server()
{
	int msg1[10];
	int msg2[10];
	int i;
	int reply_port;	

	tid = get_threadid();

	while(1){
		// Recieving msg from the server port
		recv(SERVER_PORT1,msg1);
		// Extracting the reply port 
		reply_port = msg1[0];

		// Printing data
		printf("Server Tid = %d : Data Recv : ",tid);
		for(i = 0;i < 10;i++){
			printf("%d  ",msg1[i]);
		}
		printf("\n\n");

		// Creating response to client
		for(i = 1;i < 10;i++)
			msg2[i-1] = msg1[i]*99;
		msg2[9] = 0;

		// Sending response to client
		printf("Server Tid = %d : Sending data to port %d \n\n",tid,reply_port);
		send(reply_port,msg2);
		
		sleep(1);
	}
}



// client function //
void client()
{
	int tid;
	int send_msg[10];
	int recv_msg[10];
	int i;
	int reply_port;

	P(cpsem);
	reply_port = client_port++;
	V(cpsem);

	tid = get_threadid();

	// Creating a msg to send
	for(i = 1;i < 10;i++){
		send_msg[i] = tid*10 + i;
	}
	// Adding reply port at its beginning 
	send_msg[0] = reply_port;

	while(1){
		// Sending msg to server port
		send(SERVER_PORT1,send_msg);
		printf("Client Tid = %d : Sent msg \n\n",tid);

		// Receiving the msg from the server
		recv(reply_port,recv_msg);

		// Printing the data
		printf("Client Tid = %d : Recv msg from port  %d  : ",tid ,reply_port);
		for(i = 0; i < 9 ; i++){
			printf("%d ",recv_msg[i]);
		}
		printf("\n\n");

		sleep(1);
	}
}



// Main function //
int main(int argc,char** argv)
{
	// To prevent race condition on reply port assignment
	cpsem = CreateSem(1);

	// Spawning 1 server thread and 15 client thread
	start_thread(server);

	start_thread(client);
	start_thread(client);
	start_thread(client);
	start_thread(client);
	start_thread(client);
	start_thread(client);
	start_thread(client);
	start_thread(client);
	start_thread(client);
	start_thread(client);
	start_thread(client);
	start_thread(client);
	start_thread(client);
	start_thread(client);
	start_thread(client);


	run();

}
