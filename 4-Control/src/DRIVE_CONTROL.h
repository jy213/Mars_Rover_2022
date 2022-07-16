#ifndef DRIVE_CONTROL_H
#define DRIVE_CONTROL_H

#include <Arduino.h>
#include "Robojax_L298N_DC_motor.h"
#include "PID_BLOCK.h"
#include <cmath>
#include <cstring>
#include <vector>
#include "Wire.h"
#include <MPU6050_light.h>

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

std::vector<float> go_straight(std::vector<float> position, float facing, float ref_distance, float DUTY_LHS, float DUTY_RHS, PID_TypeDef * PID, Robojax_L298N_DC_motor robot, int motor1, int motor2);
std::vector<float> go_straight_gyro(std::vector<float> position, float facing, float ref_distance, Robojax_L298N_DC_motor robot, int motor1, int motor2,  MPU6050 mpu);
float go_rotation_gyro(float facing, float angle, int direction, float DUTY_LHS, float DUTY_RHS, Robojax_L298N_DC_motor robot, int motor1, int motor2, MPU6050 mpu);

#endif