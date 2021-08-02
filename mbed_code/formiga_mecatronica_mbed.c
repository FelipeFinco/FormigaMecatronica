#include "mbed.h"
#include "math.h"
#include "Servo.h"
 
#define ODD_LEGS_MBED  1                 //This MBED will be used for the odd legs of the robot
//#define EVEN_LEGS_MBED  1              //This MBED will be used for the even legs of the robot

//if you want the code to send messages by CAN
//#define RECEIVE_BY_CAN 1

#ifdef RECEIVE_BY_CAN
CANMessage msg;
#else
char msg[5];
#endif

 
DigitalOut led1(LED1);
CAN can1(p9, p10);

//variáveis para setar qual serão os máximos e mínimos dos motores
const int MOT_PWM_BACK = 0;
const int MOT_PWM_FRONT = 1;
const int MOT_PWM_DOWN = 0;
const int MOT_PWM_UP = 1;


const int system_frequency = 50;        //frequency of processing and PWM atualization in hz
const int period_of_full_movement = 1;  //period of each movement of the robot in seconds

Servo servo_vertical_legA(p21);
Servo servo_horizontal_legA(p22);
Servo servo_vertical_legB(p23);
Servo servo_horizontal_legB(p24);
Servo servo_vertical_legC(p25);
Servo servo_horizontal_legC(p26);

//The struct of each servo. I'd like to put the servo declaration in it too, but I couldn't
typedef struct {
    int leg;                            //number of the leg in which this servo is located
    int vert;                           //if this servo acts in the vertical, this veriable is 1; if it moves in the horizontal, it will be 0
    int phase;                          //percent of phase of the full movement
    int direction;                      //if it is 1 (forward), than the progress will increase with time, if it is 0 (backward), than the progress will decrease with time
    double output;                      //position of the servo output
} ServoParameters;
ServoParameters servos[6];



//Different state of MBED controller
enum states {
    WAITING_COMMAND,
    SENDING_MOVEMENT
} state;

uint64_t clock_ms() { return us_ticker_read() / 1000; }

double map(double x, double in_min, double in_max, double out_min, double out_max) 
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int horizontal_movement(double progress)
{
    double angle = map(progress, 0, 100, 0, 2*3.14159);
    double sine_result = -cos(angle);
    double result = map(sine_result, -1, 1, MOT_PWM_BACK, MOT_PWM_FRONT);
    return (int) result;
}

int vertical_movement(double progress)
{
    double angle = map(progress, 0, 100, 0, 2*3.14159);
    double cossine_result = sin(angle);
    if(cossine_result < 0)
    {
        cossine_result = 0;
    }
    double result = map(cossine_result, 0, 1, MOT_PWM_DOWN, MOT_PWM_UP);
    return (int) result;
}

void set_servos()
{
    for (size_t i = 0; i < 6; i++)
    {
        int leg = servos[i].leg;
        int bit = (leg-1) * 2; //first bit's location
        int byte = (int)(floor((double)(bit / 8))); //in which byte it must be written on
        bit = bit - byte * 8;
        if((msg[byte] | 1<<bit) == 0)
        {
            servos[i].phase = 0;
        }
        else
        {
            servos[i].phase = 50;
        }
        bit = (leg-1) * 2;
        byte = (int)(floor((double)(bit / 8)));
        bit = bit - byte * 8;
        if((msg[byte] | 1<<bit) == 0)
        {
            servos[i].direction = 0;
        }
        else
        {
            servos[i].direction = 1;
        }
    }
}

int receive_msg()
{
    #ifdef RECEIVE_BY_CAN
    if(can1.read(msg)) {
        printf("Message received: %d\n", msg.data[0]);
        led1 = !led1;
        set_servos();
        return 1;
    }
    #else
    scanf("%x %x %x %x %x", msg[0], msg[1], msg[2], msg[3], msg[4]);
    set_servos();
    return 1;
    #endif
    return 0;
}

//receive progress of the movement, update servos output and send the PWM to the ports
void move_legs(double progress)
{
    for (size_t i = 0; i < 6; i++)
    {
        if(servos[i].vert == 0)
        {
            servos[i].output = horizontal_movement(servos[i].phase+progress);
        }
        else
        {
            servos[i].output = vertical_movement(servos[i].phase+progress);
        }
    }
    /*
    servo_vertical_legA = servos[0].output;
    servo_horizontal_legA = servos[1].output;
    servo_vertical_legB = servos[2].output;
    servo_horizontal_legB = servos[3].output;
    servo_vertical_legC = servos[4].output;
    servo_horizontal_legC = servos[5].output;
    */
    printf("%f,%f",servos[0].phase+progress,servos[0].output);
}

int main() {
    
#ifdef ODD_LEGS_MBED
    for (size_t i = 0; i < 6; i++)
    {
        if(i % 2 == 0)
        {
            servos[i].leg = i+1;
            servos[i].vert = 1;
        }
        else
        {
            servos[i].leg = i;
            servos[i].vert = 0;
        }
    }

#endif

#ifdef ODD_LEGS_MBED
    for (size_t i = 0; i < 6; i++)
    {
        if(i % 2 == 0)
        {
            servos[i].leg = i+2;
            servos[i].vert = 1;
        }
        else
        {
            servos[i].leg = i+1;
            servos[i].vert = 0;
        }
    }
#endif
    
    
    int timer = clock_ms();
    double progress = 0;
    int movement_finished = 0;

    state = WAITING_COMMAND;

    while(1) {
        if ((clock_ms()-timer) >= 1000/system_frequency)
        {
            timer = clock_ms();
            switch(state) {
                case WAITING_COMMAND:
                    if (receive_msg())
                    {
                        progress = 0;
                        movement_finished = 0;
                        state = SENDING_MOVEMENT;
                    }
                    
                case SENDING_MOVEMENT:
                    if(movement_finished == 1)
                    {
                        state = WAITING_COMMAND;
                    }
                    else
                    {
                        progress = progress + 100/(system_frequency*period_of_full_movement);
                        move_legs(progress);
                        if(progress >= 100)
                        {
                            movement_finished = 1;
                        }
                    }
                    break;
            }
        }
        
    }
}