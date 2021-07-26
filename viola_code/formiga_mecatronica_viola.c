#include <stdio.h>
#include <time.h>     
#include <unistd.h>
 
int main(int argc, char **argv)
{
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
	
	printf("\nHello World !!!\n");        // print "Hello World" message on screen
	return(0);
}



double map(double x, double in_min, double in_max, double out_min, double out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
