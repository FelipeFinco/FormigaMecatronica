#include "mbed.h"
#include <math.h>
 
Ticker ticker;
DigitalOut led1(LED1);
CAN can1(p9, p10);

const int MOT_PWM_BACK = 900;
const int MOT_PWM_FRONT = 2100;
const int MOT_PWM_DOWN = 1100;
const int MOT_PWM_UP = 1900;
const int System_Frequency = 50;        //frequency of processing and PWM atualization in hz
const int period_of_full_movement = 3;  //period of each movement of the robot in seconds

//Different state of MBED controller
enum states {
	WAITING_COMMAND,
	SENDING_COMMAND
} state;


double map(double x, double in_min, double in_max, double out_min, double out_max) 
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int horizontal_movement(double completion)
{
    double angle = map(completion, 0, 100, 0, 2*3.14159);
    double sine_result = -cos(angle);
    double result = map(sine_result, -1, 1, MOT_PWM_BACK, MOT_PWM_FRONT);
    return (int) result;
}

int vertical_movement(double completion)
{
    double angle = map(completion, 0, 100, 0, 2*3.14159);
    double cossine_result = sin(angle);
    if(cossine_result < 0)
    {
        cossine_result = 0;
    }
    double result = map(cossine_result, 0, 1, MOT_PWM_DOWN, MOT_PWM_UP);
    return (int) result;
}
 
int main() {
    clock_t t_system = clock();

    printf("main()\n");
    CANMessage msg;
    state = WAITING_COMMAND;

    while(1) {
        if
        switch(state) {
			case WAITING_COMMAND:
				printf("loop()\n");
                if(can1.read(msg)) {
                    printf("Message received: %d\n", msg.data[0]);
                    led1 = !led1;
                    if(msg_receiver(msg.data))
                    {
                        state = SENDING_COMMAND;
                    }
                    else
                    {

                    }
                } 
                wait(0.2);
                break;
				
			case SENDING_COMMAND:
				if(movement_finished == 1)
				{
					state = WAITING_COMMAND;
				}
				else
				{
					state = SENDING_COMMAND;
				}
				break;

			case EXIT:
				exit(0);
				break;
		}
        
    }
}