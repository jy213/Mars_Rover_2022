#ifndef DRIVE_CONTROL_H
#define DRIVE_CONTROL_H

#include <Arduino.h>
#include "Robojax_L298N_DC_motor.h"
#include "PID_BLOCK.h"
#include <cmath>
#include <cstring>
#include <vector>

#define RIGHT                    1
#define LEFT                     0  
#define STRAIGHT_THRESHOLD       0.1
#define ROTATION_THRESHOLD       0.1
//#define INTEGRAL_MAX             3.0
#define SENSOR_LENGTH            1

typedef struct 
{
    float CAR_ORIGIN_LENGTH;
    float CAR_ORIGIN_X;
    float CAR_ORIGIN_Y;
    float CAR_FRAME_X;
    float CAR_FRAME_Y;
    float CAR_FRAME_X_END;
    float CAR_FRAME_Y_END;
    float CAR_FINAL_X;
    float CAR_FINAL_Y;
    float ROTATE_REFERENCE;
    int   STATE;
}DRIVE_ROTATION;
/*
float COSINE[73] = 
{
    -1, -0.996, -0.985, -0.966, -0.94, -0.906,
    -0.866, -0.819, -0.766, -0.707, -0.643, -0.574,
    -0.5, -0.423, -0.342, -0.259, -0.174, -0.087,
    0.000, 0.087, 0.174, 0.259, 0.342, 0.423,
    0.500, 0.574, 0.643, 0.707, 0.766, 0.819,
    0.866, 0.906, 0.940, 0.966, 0.985, 0.996,
    1.00, 0.996, 0.985, 0.966, 0.940, 0.906,
    0.866, 0.819, 0.766, 0.707, 0.643, 0.574,
    0.500, 0.423, 0.342, 0.259, 0.174, 0.087,
    0, -0.087, -0.174, -0.259, -0.342, -0.423,
    -0.5, -0.574, -0.643, -0.707, -0.766, -0.819,
    -0.866, -0.906, -0.94, -0.966, -0.985, -0.996,
    -1
};
*/

void straight_control_simple(float DUTY_LHS, float DUTY_RHS, float bound, float INTEGRAL_MAX, PID_TypeDef* PID, std::vector<float> D, Robojax_L298N_DC_motor robot, int motor1, int motor2);
void straight(float ref_distance, float DUTY_LHS, float DUTY_RHS, float bound, float INTEGRAL_MAX, PID_TypeDef * PID, std::vector<float> distance, Robojax_L298N_DC_motor robot, int motor1, int motor2);
void rotation(float angle, int direction, float DUTY_LHS, float DUTY_RHS, PID_TypeDef * PID, std::vector<float> distance, Robojax_L298N_DC_motor robot, int motor1, int motor2);
int go_straight(float ref_distance, float DUTY_LHS, float DUTY_RHS, PID_TypeDef * PID, Robojax_L298N_DC_motor robot, int motor1, int motor2);
int go_rotation(float angle, int direction, float DUTY_LHS, float DUTY_RHS, PID_TypeDef * PID, Robojax_L298N_DC_motor robot, int motor1, int motor2);
/*
std::vector<float> straight_control (int MODE, float DUTY, float PRE_TAN, float INTEGRAL_MAX, PID_TypeDef PID, std::vector<float> D);
std::vector<float> (float DISTANCE_REF, std::vector<float> D, PID_TypeDef PID_STRAIGHT_CONTROL, PID_TypeDef PID_DISTANCE_CONTROL, float DUTY, int MODE, Robojax_L298N_DC_motor ROBOT, int DIRECTION, int MOTOR_1, int MOTOR_2, float CONTROL_OUT[4]);
void go_rotation(std::string DIRECTION, std::vector<float> D, DRIVE_ROTATION RO, int THETA, PID_TypeDef PID_ROTATION, Robojax_L298N_DC_motor ROBOT, int MOTOR_1, int MOTOR_2);
*/


#endif