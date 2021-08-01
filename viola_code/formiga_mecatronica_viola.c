#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#include <linux/can.h>
#include <linux/can/raw.h>

int const NUMBER_OF_LEGS = 6;			//number of legs of the robot (only tested in hexapod)
const int period_of_full_movement = 3;  //period of each movement of the robot in seconds


//if you want the code to send messages by CAN
//#define SEND_BY_CAN;  


#ifdef SEND_BY_CAN
int s; 
struct sockaddr_can addr;
struct ifreq ifr;
struct can_frame frame;					
#endif




void send_message_to_leg(int msg_int[5])
{
	char msg[6];
	for (size_t i = 0; i < 6; i++)
	{
		msg[i]=msg_int[i];
	}
	msg[6] = "\0";
	
	#ifdef SEND_BY_CAN
	sprintf(frame.data, msg);
	if (write(s, &frame, sizeof(struct can_frame)) != sizeof(struct can_frame)) {
		perror("Write");
		return 1;
	}

	#else
	printf("\nA mensagem enviada para a MBED seria: %s", msg);
	printf("\nNumero 0: %c,%d", msg[0], msg[0]);
	printf("\nNumero 1: %c,%d", msg[1], msg[1]);
	printf("\nNumero 2: %c,%d", msg[2], msg[2]);
	printf("\nNumero 3: %c,%d", msg[3], msg[3]);
	printf("\nNumero 4: %c,%d", msg[4], msg[4]);
	#endif
}


//This function decides what each leg has to do so the robot does the movement requested
//for now, only having forward and backward movements, it only sends 0 (backward) and 1 (forward) for
//all legs, odd legs will be 180° phase of the even legs, sending 0 (0° phase) and 1 (180° phase).
//As each char only have 8 bits and we need 6*2=12 bits, we will use 2 of the 5 chars to send the message.
void send_movement(int movement_number)
{
	int msg[5] = {0,0,0,0,0};
	
	for (int leg = 0; leg < NUMBER_OF_LEGS; leg++)
	{
		int bit = leg * 2; //first bit's location
		int byte = floor(bit / 8); //in which byte it must be written on
		bit = bit - byte * 8;
		//setting leg phase
		if(leg % 2 == 0)
		{
			msg[byte] = msg[byte] | 1<<bit;
			printf("\nRegistrando 1 no bit %d do byte %d, valor:%d",bit, byte, msg[byte]);
		}
		else
		{
			msg[byte] = (msg[byte] | 0<<bit);
			printf("\nRegistrando 0 no bit %d do byte %d, valor:%d",bit, byte, msg[byte]);
		}

		bit++;
		byte = bit % 8;
		bit = bit - byte * 8;

		if(movement_number == 0)
		{
			msg[byte] = (msg[byte] | 0<<bit);
			printf("\nRegistrando 1 no bit %d do byte %d, valor:%d",bit, byte, msg[byte]);
		}

		else if(movement_number == 1)
		{
			msg[byte] = (msg[byte] | 1<<bit);
			printf("\nRegistrando 0 no bit %d do byte %d, valor:%d",bit, byte, msg[byte]);
		}
	}
	send_message_to_leg(msg);
}

//Different state of Toradex controller
enum states {
	SENDING_COMMAND,
	WAITING_MOVEMENT,
	EXIT
} state;


int main(int argc, char **argv)
{
	char response[32];
	//start robot waiting to send command
	state =  SENDING_COMMAND;

	clock_t t_wait = clock();
	
	int movement_finished = 0;
	
	
#ifdef SEND_BY_CAN		
	//openning socket
	if ((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
		perror("Socket");
		return 1;
	}
	strcpy(ifr.ifr_name, "can0" );
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
	

#else


#endif
	


	//state machine loop
	while(1){
		switch(state) {
			case SENDING_COMMAND:
				printf("\nPor favor entrar um comando. (frente/tras/sair)\n");
				scanf( "%s" , response);
				if ((strcmp (response, "frente")) == 0)
				{
					send_movement(1);
					state = WAITING_MOVEMENT;
					t_wait = clock();
					movement_finished = 0;
					printf("\nAndando para frente!\n");
				}
				else if ((strcmp (response, "tras")) == 0)
				{
					send_movement(0);
					state = WAITING_MOVEMENT;
					t_wait = clock();
					movement_finished = 0;
					printf("\nAndando para tras!\n");
				}
				else if ((strcmp (response, "sair")) == 0)
				{
					state = EXIT;
				}
				else 
				{
					printf("\nNao foi possivel compreender o comando!!!\n");
				}
			break;
				
			case WAITING_MOVEMENT:
				if (((clock()-t_wait) / CLOCKS_PER_SEC) > period_of_full_movement)
				{
					movement_finished = 1;
				}

				if(movement_finished == 1)
				{
					state = SENDING_COMMAND;
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
	
#ifdef SEND_BY_CAN
	if (close(s) < 0) {
		perror("Close");
		return 1;
	}
#endif

	printf("\nFim do processo!\n");
	return(0);
}