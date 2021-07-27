#include <stdio.h>
#include <time.h>     
#include <unistd.h>
#include <string.h>

/*

double map(double x, double in_min, double in_max, double out_min, double out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

*/


//Different state of an ant robot
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
	enum events event = RESTART_LOOP;	

	//indicates that the robot will locomove forward
	int forward = 1;					

	//state machine loop
	while(1){
		switch(state) {
			case SENDING_COMMAND:
				printf("\nPor favor entrar um comando. (frente/tras/sair)\n");
				scanf( "%s" , response);
				if ((strcmp (response, "frente")) == 0)
				{
					state = WAITING_MOVEMENT;
					printf("\nAndando para frente!\n");
				}
				else if ((strcmp (response, "tras")) == 0)
				{
					state = WAITING_MOVEMENT;
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
				int movement_finished = 0;
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



