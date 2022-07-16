#include <Arduino.h>
#include "Robojax_L298N_DC_motor.h"
#include "PID_BLOCK.h"
#include <cmath>

float * straight_control (int MODE, float DUTY, float PRE_TAN, float INTEGRAL_MAX, PID_TypeDef PID, float Q[4])
{
    switch (MODE)
    {
        float DELTA_DUTY;
        case 1://rover moves in x direction
            DELTA_DUTY = PID_calculate(PID, 0, Q[3], INTEGRAL_MAX);
            DUTY = DUTY - DELTA_DUTY;
            PRE_TAN = PRE_TAN;
            float *OUT = new float[2]{DUTY, PRE_TAN};
            return OUT;
        case 2://rover moves in y direction
            DELTA_DUTY = PID_calculate(PID, 0, Q[2], INTEGRAL_MAX);
            DUTY = DUTY - DELTA_DUTY;
            PRE_TAN = PRE_TAN;
            float *OUT = new float[2]{DUTY, PRE_TAN};
            return OUT;    
        case 3:
            if((Q[2]>0 && Q[3]>0) || (Q[2]<0 && Q[3]<0))
            {
                DELTA_DUTY = PID_calculate(PID, abs(PRE_TAN), abs(Q[3]/Q[2]), INTEGRAL_MAX);
                DUTY = DUTY - DELTA_DUTY;
                PRE_TAN = Q[3]/Q[2];
            }
            else
            {
                DELTA_DUTY = PID_calculate(PID, abs(PRE_TAN), abs(Q[3]/Q[2]), INTEGRAL_MAX);
                DUTY = DUTY + DELTA_DUTY;
                PRE_TAN = Q[3]/Q[2];
            }
            float *OUT = new float[2]{DUTY, PRE_TAN};
            return OUT;
    }
}

float * go_straight (float DISTANCE_REF, float Q[4], PID_TypeDef PID_STRAIGHT_CONTROL, PID_TypeDef PID_DISTANCE_CONTROL, float DUTY, int MODE, Robojax_L298N_DC_motor ROBOT, int DIRECTION, int MOTOR_1, int MOTOR_2, float CONTROL_OUT[4], float INTEGRAL_MAX)
{
    float *OUT = new float[4];
    switch ((int)CONTROL_OUT[2])
    {
        case 0:
            float INITIAL_X = Q[0]; 
            float INITIAL_Y = Q[1];
            float PRE_TAN_0 = Q[3]/Q[2];
            OUT[1] = PRE_TAN_0;
            float DUTY_OUT = 0;
            for(int i=0; i<= 30; i++){
                DUTY_OUT = straight_control(MODE, i, PRE_TAN_0, INTEGRAL_MAX, PID_STRAIGHT_CONTROL, Q)[0];
                ROBOT.rotate(MOTOR_1, i, DIRECTION); 
                ROBOT.rotate(MOTOR_2, DUTY_OUT, DIRECTION); 
                delay(100);
            }
            float CASE0_END_X = Q[0];
            float CASE0_END_Y = Q[1];
            float DISTANCE_RUN = sqrt((CASE0_END_X-INITIAL_X)*(CASE0_END_X-INITIAL_X)+(CASE0_END_Y-INITIAL_Y)*(CASE0_END_Y-INITIAL_Y));
            OUT[0] = DUTY_OUT;
            OUT[2] = 1.0;
            OUT[3] = DISTANCE_RUN;
            return OUT;

        case 1://CONTROL_OUT current rover status
            float DUTY_OUT;
            if(DISTANCE_REF - CONTROL_OUT[3] != 0){
                DUTY = PID_calculate(PID_DISTANCE_CONTROL, DISTANCE_REF, DISTANCE_RUN, INTEGRAL_MAX);
                DUTY_OUT = straight_control (MODE, DUTY, CONTROL_OUT[1], INTEGRAL_MAX, PID_STRAIGHT_CONTROL, Q)[0];
                ROBOT.rotate(MOTOR_1, DUTY, DIRECTION);
                ROBOT.rotate(MOTOR_2, CONTROL_OUT[0], DIRECTION);
                OUT[3] = CONTROL_OUT[3] + sqrt(Q[2]*Q[2]+Q[3]*Q[3]);
                delay(100);
            }
            else{
                DUTY_OUT = 0;// lowerbound duty to 40%
                ROBOT.brake(MOTOR_1);
                ROBOT.brake(MOTOR_2);
                OUT[3] = 0;
                delay(100);
            }
            OUT[0] = DUTY_OUT;
            OUT[2] = 0;
            OUT[1] = CONTROL_OUT[1];
            return OUT;
    }
}

