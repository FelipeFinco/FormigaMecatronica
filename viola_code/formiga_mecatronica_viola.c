#include <stdio.h>
#include <time.h>     
#include <unistd.h>
#include <string.h>



double map(double x, double in_min, double in_max, double out_min, double out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


//Different state of an ant robot
enum states {
    SENDING_COMMAND,
    WAITING_MOVEMENT,
	EXIT
} state;


//Different type events
enum events {
    COMMAND_SENT,
    RESTART_LOOP,
    STOP_LOOPING,
};


int main(int argc, char **argv)
{
	//start robot waiting to send command
	state =  SENDING_COMMAND;
	enum events event = RESTART_LOOP;	

	//indicates that the robot will locomove forward
	bool forward = true;					


	switch(state) {
		case SENDING_COMMAND:
			char response[256];
			printf("\nPor favor entrar um comando. (frente/tras/sair)\n");
			scanf( "%s" , response);
			if ((strcmp (response, "frente")) == 0)
			{
				event = COMMAND_SENT;
				bool forward = true;
				printf("\nAndando para frente!\n");
			}
			else if ((strcmp (response, "tras")) == 0)
			{
				event = COMMAND_SENT;
				bool forward = false;
				printf("\nAndando para tras!\n");
			}
			else if ((strcmp (response, "sair")) == 0)
			{
				event = STOP_LOOPING;
			}
			else 
			{
				printf("\nNao foi possivel compreender o comando!!!\n");
				event = RESTART_LOOP;
			}
		
			switch(event) {
			case COMMAND_SENT:
				state = WAITING_MOVEMENT;
				break;
			default:
				exit(1);
				break;
			}       
			break;
			/*
		case LOOP:
			switch(event) {
			case PRINT_HELLO:
				printf("Hello World!\n");
				break;
			case STOP_LOOPING:
				state = END;
				break;
			default:
				exit(1);
				break;
			}
			break;
		case END:
			exit(1);
			break;
		}
		*/

















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
	
	printf("\nHello World !!!\n");        // print "Hello World" message on screen
	return(0);
}



