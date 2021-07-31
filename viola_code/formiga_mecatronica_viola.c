#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#include <linux/can.h>
#include <linux/can/raw.h>

/*

double map(double x, double in_min, double in_max, double out_min, double out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

*/


//This function decides what each leg has to do so the robot does the movement requested
//for now, only having forward and backward movements, it only sends 0 (backward) and 1 (forward) for
//all legs, being 3 of them 180° phase of the other 3
void send_movement(int movement_number)
{
	if(movement_number == 0)
	{
		for (int leg = 0; leg < 6; leg++)
		{
			if(leg % 2 == 0)
			{
				send_message_to_leg(leg, 0, 0);
			}
			else
			{
				send_message_to_leg(leg, 0, 50);
			}
		}
	}

	if(movement_number == 1)
	{
		for (int leg = 0; leg < 6; leg++)
		{
			if(leg % 2 == 0)
			{
				send_message_to_leg(leg, 1, 0);
			}
			else
			{
				send_message_to_leg(leg, 1, 50);
			}
		}
	}
}


send_message_to_leg(int leg, int direction, int phase)
{

}

//Different state of Toradex controller
enum states {
	SENDING_COMMAND,
	WAITING_MOVEMENT,
	EXIT
} state;


int main(int argc, char **argv)
{
	char response[256];
	//start robot waiting to send command
	state =  SENDING_COMMAND;	

	//indicates that the robot will locomove forward
	int forward = 1;
	
	int movement_finished = 0;
	
	
	int s; 
	struct sockaddr_can addr;
	struct ifreq ifr;
	struct can_frame frame;					

	//openning socket
	if ((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
		perror("Socket");
		return 1;
	}
	strcpy(ifr.ifr_name, "vcan0" );
	ioctl(s, SIOCGIFINDEX, &ifr);
	memset(&addr, 0, sizeof(addr));
	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;

	if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		perror("Bind");
		return 1;
	}

	frame.can_id = 0x555;
	frame.can_dlc = 5;
	sprintf(frame.data, "Hello");

	if (write(s, &frame, sizeof(struct can_frame)) != sizeof(struct can_frame)) {
		perror("Write");
		return 1;
	}
	


	//state machine loop
	while(1){
		switch(state) {
			case SENDING_COMMAND:
				printf("\nPor favor entrar um comando. (frente/tras/sair)\n");
				scanf( "%s" , response);
				if ((strcmp (response, "frente")) == 0)
				{
					state = WAITING_MOVEMENT;
					movement_finished = 0;
					send_movement(1);
					printf("\nAndando para frente!\n");
				}
				else if ((strcmp (response, "tras")) == 0)
				{
					state = WAITING_MOVEMENT;
					movement_finished = 0;
					send_movement(0);
					printf("\nAndando para tras!\n");
				}
				else if ((strcmp (response, "sair")) == 0)
				{
					state = EXIT;
				}
				else 
				{
					printf("\nNao foi possivel compreender o comando!!!\n");
					state = SENDING_COMMAND;
				}
			
				
			case WAITING_MOVEMENT:
				if(movement_finished == 1)
				{
					state = SENDING_COMMAND;
				}
				else
				{
					state = WAITING_MOVEMENT;
				}
				break;

			case EXIT:
				exit(0);
				break;
		}
}















/*
	double timer = 0.0;
	double timer_old = 0.0;
	
	int system_frequency = 50;		//given in hz
	double system_period = 1/system_frequency;
	float movement_period;			//time to complete a full movement
 
	clock_t time_spent = clock();
	clock_t begin = clock();
	clock_t end = clock();
	
	while(((double)(end - begin) / CLOCKS_PER_SEC) < 20){
		timer = clock();
		if(((timer-timer_old)/CLOCKS_PER_SEC) >= 5){
			printf("The elapsed time is %f seconds \n", ((timer-timer_old)/CLOCKS_PER_SEC));
			timer_old = timer;
		}
		end = clock();
	}
	*/
	
	printf("\nFim do processo!\n");        // print "Hello World" message on screen
	return(0);
}



