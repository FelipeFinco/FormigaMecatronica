#include "mbed.h"
#include <math.h>
 
Ticker ticker;
DigitalOut led1(LED1);
CAN can1(p9, p10);

const int MOT_PWM_BACK = 900;
const int MOT_PWM_FRONT = 2100;
const int MOT_PWM_DOWN = 1100;
const int MOT_PWM_UP = 1900;

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
    printf("main()\n");
    CANMessage msg;
    while(1) {
        printf("loop()\n");
        if(can1.read(msg)) {
            printf("Message received: %d\n", msg.data[0]);
            led1 = !led1;
        } 
        wait(0.2);
    }
}