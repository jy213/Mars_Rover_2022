#include <Arduino.h>
#include "Robojax_L298N_DC_motor.h"
#include "PID_BLOCK.h"
#include <cmath>
#include <cstring>
#include "DRIVE_CONTROL.h"
#include <vector>
#include <stdio.h>
#include "SPI.h"
#include "ADNS3080.h"

//optical sensor settings
#define PIN_SS        5  //D7
#define PIN_MISO      19 //D5
#define PIN_MOSI      23 //D2
#define PIN_SCK       18 //D6
#define PIN_MOUSECAM_RESET     35 //A5
#define PIN_MOUSECAM_CS        5  //D7
#define LED_CHECK 32


void straight_control_simple(float DUTY_LHS, float DUTY_RHS, float bound, float INTEGRAL_MAX, PID_TypeDef * PID, std::vector<float> distance, Robojax_L298N_DC_motor robot, int motor1, int motor2){
    PID->DUTY_CYCLE_LHS = DUTY_LHS - PID_calculate(PID, bound, 0, distance[0], INTEGRAL_MAX);
    PID->DUTY_CYCLE_RHS = DUTY_RHS + PID_calculate(PID, bound, 0, distance[0], INTEGRAL_MAX);
    /*
    Serial.println("Current Error = " + String(PID->CURRENT_ERROR));
    Serial.println("Integral Value = " + String(PID->INTEGRAL_VALUE));

    Serial.println("Delta Duty= " + String(PID->DELTA_DUTY_CYCLE*100));
    Serial.println("Duty LHS= " + String(PID->DUTY_CYCLE_LHS*100));
    Serial.println("Duty RHS= " + String(PID->DUTY_CYCLE_RHS*100));
    Serial.print('\n');
    */
    robot.rotate(motor1, PID->DUTY_CYCLE_RHS*100, 1); //RHS
    robot.rotate(motor2, PID->DUTY_CYCLE_LHS*100, 1); //LHS

}

void straight(float ref_distance, float DUTY_LHS, float DUTY_RHS, float bound, float INTEGRAL_MAX, PID_TypeDef * PID, std::vector<float> distance, Robojax_L298N_DC_motor robot, int motor1, int motor2){ 
        if(ref_distance > distance[1]){
            PID->DUTY_CYCLE_LHS = DUTY_LHS - PID_calculate(PID, bound, 0, distance[0], INTEGRAL_MAX);
            PID->DUTY_CYCLE_RHS = DUTY_RHS + PID_calculate(PID, bound, 0, distance[0], INTEGRAL_MAX);
        }
        else{
            PID->DUTY_CYCLE_LHS = 0;
            PID->DUTY_CYCLE_RHS = 0;
        }
        robot.rotate(motor1, PID->DUTY_CYCLE_RHS*100, 1); //RHS
        robot.rotate(motor2, PID->DUTY_CYCLE_LHS*100, 1); //LHS
}

void rotation(float angle, int direction, float DUTY_LHS, float DUTY_RHS, PID_TypeDef * PID, std::vector<float> distance, Robojax_L298N_DC_motor robot, int motor1, int motor2){
float radius = 12.5;
float pi = 3.1415926;
double arc_length = angle * pi / 180 * radius;
if(abs(distance[0])<arc_length){
            if(direction == 0){
                PID->DUTY_CYCLE_LHS = DUTY_LHS + PID_calculate(PID, 0.1, 0, distance[1], 0);
                PID->DUTY_CYCLE_RHS = DUTY_RHS - PID_calculate(PID, 0.1, 0, distance[1], 0);
                robot.rotate(motor1, PID->DUTY_CYCLE_RHS*100, 1); //RHS
                robot.rotate(motor2, PID->DUTY_CYCLE_LHS*100, 0); //LHS
            }
            else{
                PID->DUTY_CYCLE_LHS = DUTY_LHS - PID_calculate(PID, 0.1, 0, distance[1], 0);
                PID->DUTY_CYCLE_RHS = DUTY_RHS + PID_calculate(PID, 0.1, 0, distance[1], 0);
                robot.rotate(motor1, PID->DUTY_CYCLE_RHS*100, 0); //RHS
                robot.rotate(motor2, PID->DUTY_CYCLE_LHS*100, 1); //LHS
            }
        }
        else{
            robot.brake(motor1);
            robot.brake(motor2);
        }
}


int go_straight(float ref_distance, float DUTY_LHS, float DUTY_RHS, PID_TypeDef * PID, Robojax_L298N_DC_motor robot, int motor1, int motor2){ 

    MD md;
    float total_x = 0;
    float total_y = 0;
    std::vector<float>distance{0,0,0,0};

    while(ref_distance > distance[1]){
        digitalWrite (LED_CHECK,HIGH);
        mousecam_read_motion(PIN_MOUSECAM_CS, &md);
        ADNS3080_RESOLUTION_CHECK(PIN_MOUSECAM_CS, md);
        if(md.squal/4 < 12){
            digitalWrite (LED_CHECK,LOW);
        }
        distance = ADNS3080_DISTANCE (total_x, total_y, PIN_MOUSECAM_CS,  md); 
        total_x = distance[0]*157/50;
        total_y = distance[1]*157/50;
        PID->DUTY_CYCLE_LHS = DUTY_LHS - PID_calculate(PID, 0.2, 0, distance[0], 0);
        PID->DUTY_CYCLE_RHS = DUTY_RHS + PID_calculate(PID, 0.2, 0, distance[0], 0);
        robot.rotate(motor1, PID->DUTY_CYCLE_RHS*100, 1); //RHS
        robot.rotate(motor2, PID->DUTY_CYCLE_LHS*100, 1); //LHS
        Serial.println(String(distance[1]) +"      //////////////////////////////////////");
        delay(10);
        //PID_calculate (PID_TypeDef *PID, float bound, float REFERENCE, float MEASURED_DISTANCE, float INTEGRAL_MAX);
    }
        robot.brake(motor1);
        robot.brake(motor2);
    // Serial.println("Distance_x = " + String(distance[0]));
    // Serial.println("Distance_y = " + String(distance[1]));
    // Serial.println("Duty Left = " + String(PID->DUTY_CYCLE_LHS));
    // Serial.println("Duty Right = " + String(PID->DUTY_CYCLE_RHS));
    return 1;
}
//direction = 0 Left
//direction = 1 Right
int go_rotation(float angle, int direction, float DUTY_LHS, float DUTY_RHS, PID_TypeDef * PID, Robojax_L298N_DC_motor robot, int motor1, int motor2){
    float radius = 12.5;
    float pi = 3.1415926;
    double arc_length = angle * pi / 180 * radius;
    float total_x = 0;
    float total_y = 0;
    MD md;
    std::vector<float>distance{0,0,0,0};

    while(abs(distance[0])<arc_length){
        digitalWrite (LED_CHECK,HIGH);
        mousecam_read_motion(PIN_MOUSECAM_CS, &md);
        ADNS3080_RESOLUTION_CHECK(PIN_MOUSECAM_CS, md);
        if(md.squal/4 < 12){
            digitalWrite (LED_CHECK,LOW);
        }
        distance = ADNS3080_DISTANCE (total_x, total_y, PIN_MOUSECAM_CS,  md); 
        total_x = distance[0]*157/50;
        total_y = distance[1]*157/50;

        if(direction == 0){
            PID->DUTY_CYCLE_LHS = DUTY_LHS + PID_calculate(PID, 0.1, 0, distance[1], 0);
            PID->DUTY_CYCLE_RHS = DUTY_RHS - PID_calculate(PID, 0.1, 0, distance[1], 0);
            robot.rotate(motor1, PID->DUTY_CYCLE_RHS*100, 1); //RHS
            robot.rotate(motor2, PID->DUTY_CYCLE_LHS*100, 0); //LHS
        }
        else{
            PID->DUTY_CYCLE_LHS = DUTY_LHS - PID_calculate(PID, 0.1, 0, distance[1], 0);
            PID->DUTY_CYCLE_RHS = DUTY_RHS + PID_calculate(PID, 0.1, 0, distance[1], 0);
            robot.rotate(motor1, PID->DUTY_CYCLE_RHS*100, 0); //RHS
            robot.rotate(motor2, PID->DUTY_CYCLE_LHS*100, 1); //LHS
        }
        delay(10);
    }
    robot.brake(motor1);
    robot.brake(motor2);

    return 1;
}

/*
void go_straight(std::vector<float> D, float INTEGRAL_MAX, PID_TypeDef PID_STRAIGHT_CONTROL, PID_TypeDef PID_DISTANCE_CONTROL, float DUTY, int MODE, Robojax_L298N_DC_motor ROBOT, int DIRECTION, int MOTOR_1, int MOTOR_2){

}

std::vector<float> straight_control (int MODE, float DUTY,float INTEGRAL_MAX, float PRE_TAN, PID_TypeDef PID, std::vector<float> D)
{
    switch (MODE)
    {
        float DELTA_DUTY;
        case 1: 
        {
            //rover moves in x direction
            DELTA_DUTY = PID_calculate(PID, 0, D[3], INTEGRAL_MAX);
            DUTY = DUTY - DELTA_DUTY;
            PRE_TAN = PRE_TAN;
            std::vector<float> OUT={DUTY, PRE_TAN};
            //float *OUT = new float[2]{DUTY, PRE_TAN};
            return OUT;
        }
        case 2:
        {
            //rover moves in y direction
            DELTA_DUTY = PID_calculate(PID, 0, D[2], INTEGRAL_MAX);
            DUTY = DUTY - DELTA_DUTY;
            PRE_TAN = PRE_TAN;
            std::vector<float> OUT={DUTY, PRE_TAN};
            //float *OUT = new float[2]{DUTY, PRE_TAN};
            return OUT;    
        }
        case 3:
        {
            if((D[2]>0 && D[3]>0) || (D[2]<0 && D[3]<0))//rover moves in 1st and 3rd quadrant
            {
                DELTA_DUTY = PID_calculate(PID, abs(PRE_TAN), abs(D[3]/D[2]), INTEGRAL_MAX);
                DUTY = DUTY - DELTA_DUTY;
                PRE_TAN = D[3]/D[2];
            }
            else//rover moves in 2nd and 4th quadrant
            {
                DELTA_DUTY = PID_calculate(PID, abs(PRE_TAN), abs(D[3]/D[2]), INTEGRAL_MAX);
                DUTY = DUTY + DELTA_DUTY;
                PRE_TAN = D[3]/D[2];
            }
            std::vector<float> OUT={DUTY, PRE_TAN};
            //float *OUT = new float[2]{DUTY, PRE_TAN};
            return OUT;
        }
    }
}

std::vector<float> go_straight (float DISTANCE_REF, std::vector<float> D, float INTEGRAL_MAX, PID_TypeDef PID_STRAIGHT_CONTROL, PID_TypeDef PID_DISTANCE_CONTROL, float DUTY, int MODE, Robojax_L298N_DC_motor ROBOT, int DIRECTION, int MOTOR_1, int MOTOR_2, float CONTROL_OUT[4])
{
    std::vector<float> OUT={0,0,0,0};
    //float *OUT = new float[4];
    switch ((int)CONTROL_OUT[2])
    {
        case 0:
            float INITIAL_X = D[0]; 
            float INITIAL_Y = D[1];
            float PRE_TAN_0 = D[3]/D[2];
            OUT[1] = PRE_TAN_0;
            float DUTY_OUT = 0;
            for(int i=0; i<= 30; i++){
                DUTY_OUT = straight_control(MODE, i, PRE_TAN_0, INTEGRAL_MAX, PID_STRAIGHT_CONTROL, D)[0];
                ROBOT.rotate(MOTOR_1, i, DIRECTION); 
                ROBOT.rotate(MOTOR_2, DUTY_OUT, DIRECTION); 
                delay(100);
            }
            float CASE0_END_X = D[0];
            float CASE0_END_Y = D[1];
            float DISTANCE_RUN = sqrt((CASE0_END_X-INITIAL_X)*(CASE0_END_X-INITIAL_X)+(CASE0_END_Y-INITIAL_Y)*(CASE0_END_Y-INITIAL_Y));
            OUT[0] = DUTY_OUT;
            OUT[2] = 1.0;
            OUT[3] = DISTANCE_RUN;
            return OUT;

        case 1:
            float DUTY_OUT;
            if(abs(DISTANCE_REF - CONTROL_OUT[3]) > STRAIGHT_THRESHOLD){
                DUTY = PID_calculate(PID_DISTANCE_CONTROL, DISTANCE_REF, DISTANCE_RUN, INTEGRAL_MAX);
                DUTY_OUT = straight_control (MODE, DUTY, CONTROL_OUT[1], INTEGRAL_MAX, PID_STRAIGHT_CONTROL, D)[0];
                ROBOT.rotate(MOTOR_1, DUTY, DIRECTION);
                ROBOT.rotate(MOTOR_2, CONTROL_OUT[0], DIRECTION);
                OUT[3] = CONTROL_OUT[3] + sqrt(D[2]*D[2]+D[3]*D[3]);
                OUT[2] = 1;
                delay(100);
            }   
            else{
                DUTY_OUT = 0;
                ROBOT.brake(MOTOR_1);
                ROBOT.brake(MOTOR_2);
                OUT[3] = 0;
                OUT[2] = 0;
                delay(100);
                
            }
            OUT[0] = DUTY_OUT;
            OUT[1] = CONTROL_OUT[1];
            return OUT;
    }
}

void go_rotation(std::string DIRECTION, std::vector<float> D, DRIVE_ROTATION RO, int THETA, float INTEGRAL_MAX, PID_TypeDef PID_ROTATION, Robojax_L298N_DC_motor ROBOT, int MOTOR_1, int MOTOR_2)
{
    switch (RO.STATE)
    {
        case 0:
            int COS_THETA_INDEX;
            int SIN_THETA_INDEX;
            RO.CAR_ORIGIN_LENGTH = sqrt(D[0]*D[0]+D[1]*D[1]) - SENSOR_LENGTH;
            RO.CAR_ORIGIN_X      = RO.CAR_ORIGIN_LENGTH * (D[0]/sqrt(D[0]*D[0]+D[1]*D[1]));
            RO.CAR_ORIGIN_Y      = RO.CAR_ORIGIN_LENGTH * (D[1]/sqrt(D[0]*D[0]+D[1]*D[1]));
            RO.CAR_FRAME_X       = D[0] - RO.CAR_ORIGIN_X;
            RO.CAR_FRAME_Y       = D[1] - RO.CAR_ORIGIN_Y;
            if(THETA == 0){
                int COS_THETA_INDEX = 36;
                int SIN_THETA_INDEX = 18;
            }
            else{
                int COS_THETA_INDEX  = 36 + (THETA/(abs(THETA)))/5;
                int SIN_THETA_INDEX  = (COS_THETA_INDEX - 18 + 73)%73;
            }
            float cos_theta      = COSINE[COS_THETA_INDEX];
            float sin_theta      = COSINE[SIN_THETA_INDEX];
            RO.CAR_FRAME_X_END   = cos_theta * RO.CAR_FRAME_X - sin_theta * RO.CAR_FRAME_Y;
            RO.CAR_FRAME_Y_END   = sin_theta * RO.CAR_FRAME_X + cos_theta * RO.CAR_FRAME_Y;
            RO.CAR_FINAL_X       = RO.CAR_ORIGIN_X + RO.CAR_FRAME_X_END;
            RO.CAR_FINAL_Y       = RO.CAR_ORIGIN_Y + RO.CAR_FRAME_Y_END;
            RO.ROTATE_REFERENCE  = sqrt(RO.CAR_FINAL_X*RO.CAR_FINAL_X + RO.CAR_FINAL_Y*RO.CAR_FINAL_Y);
            RO.STATE = 1;    
        case 1:
            float MEASURED_ROTATION = sqrt(D[0]*D[0] + D[1]*D[1]);
            if(abs(RO.ROTATE_REFERENCE - MEASURED_ROTATION) > ROTATION_THRESHOLD){
                float DUTY = PID_calculate (PID_ROTATION, RO.ROTATE_REFERENCE, MEASURED_ROTATION, INTEGRAL_MAX);
                if(DIRECTION == "R"){
                    ROBOT.rotate(MOTOR_1, DUTY, 0);
                    ROBOT.rotate(MOTOR_2, DUTY, 1);
                    delay(100);
                }
                else{
                    ROBOT.rotate(MOTOR_1, DUTY, 1);
                    ROBOT.rotate(MOTOR_2, DUTY, 0);
                    delay(100);
                }
            }
            else{
                ROBOT.brake(MOTOR_1);
                ROBOT.brake(MOTOR_2);
                delay(100);
                RO.STATE = 0;
            } 
    }
}
*/
