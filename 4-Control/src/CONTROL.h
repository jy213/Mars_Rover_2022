#ifndef CONTROL_H
#define CONTROL_H

#include <Arduino.h>
#include "Robojax_L298N_DC_motor.h"
#include "PID_BLOCK.h"
#include <cmath>
#include <cstring>
#include "DRIVE_CONTROL.h"
#include <vector>
#include "Wire.h"
#include <MPU6050_light.h>
#include <queue>

#define RIGHT                    1
#define LEFT                     0  
#define STRAIGHT_THRESHOLD       0.1
#define ROTATION_THRESHOLD       0.1
//#define INTEGRAL_MAX             3.0
#define SENSOR_LENGTH            1


void UART_Syn();
std::vector<std::vector<float>> get_image(std::vector<std::vector<float>> balls);
std::vector<float> calculate_left_position(float angle, float facing, int distance, std::vector<float> car_position);
std::vector<float> get_left_position(std::vector<float> car_position, float facing, int colour, int distance, std::vector<std::vector<float>> balls, Robojax_L298N_DC_motor robot, int motor1, int motor2, MPU6050 mpu);
std::vector<float> calculate_right_position(float angle, float facing, int distance, std::vector<float> car_position);
std::vector<float> get_right_position(std::vector<float> car_position, float facing, int colour, int distance, std::vector<std::vector<float>> balls, Robojax_L298N_DC_motor robot, int motor1, int motor2, MPU6050 mpu);
int find_colour(std::vector<std::vector<float>> balls);
std::vector<float> get_middle_position(std::vector<float> car_position, float facing, int distance, Robojax_L298N_DC_motor robot, int motor1, int motor2, MPU6050 mpu);
std::vector<std::vector<float>> rotate_360(float facing, std::vector<float> car_position, std::vector<std::vector<float>> balls, Robojax_L298N_DC_motor robot, int motor1, int motor2, MPU6050 mpu);

std::vector<std::vector<float>> route_search(std::vector<float> car_position, std::vector<float> destination_position, std::vector<std::vector<float>> map);
std::queue<float> route_to_action(std::vector<std::vector<float>> route, float facing);
int next_destination(std::vector<std::vector<float>> destination_position);
std::vector<std::vector<float>> check_balls_on_map(std::vector<std::vector<float>> destination_position,  std::vector<std::vector<float>> map);

#endif