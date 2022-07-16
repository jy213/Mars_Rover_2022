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
#include <math.h>

// optical sensor settings
#define PIN_SS 5              // D7
#define PIN_MISO 19           // D5
#define PIN_MOSI 23           // D2
#define PIN_SCK 18            // D6
#define PIN_MOUSECAM_RESET 35 // A5
#define PIN_MOUSECAM_CS 5     // D7
#define LED_CHECK 32

std::vector<float> go_straight(std::vector<float> position, float facing, float ref_distance, float DUTY_LHS, float DUTY_RHS, PID_TypeDef *PID, Robojax_L298N_DC_motor robot, int motor1, int motor2)
{

    MD md;
    float total_x = 0;
    float total_y = 0;
    std::vector<float> distance{0, 0, 0, 0};

    while (ref_distance > distance[1])
    {
        digitalWrite(LED_CHECK, HIGH);
        mousecam_read_motion(PIN_MOUSECAM_CS, &md);
        ADNS3080_RESOLUTION_CHECK(PIN_MOUSECAM_CS, md);
        if (md.squal / 4 < 12)
        {
            digitalWrite(LED_CHECK, LOW);
        }
        distance = ADNS3080_DISTANCE(total_x, total_y, PIN_MOUSECAM_CS, md);
        total_x = distance[0] * 157 / 58;
        total_y = distance[1] * 157 / 58;
        Serial.println("distance x : " + String(distance[0]));
        Serial.println("distance y : " + String(distance[1]));
        PID->DUTY_CYCLE_LHS = DUTY_LHS - PID_calculate(PID, 0.2, 0, distance[0], 0);
        PID->DUTY_CYCLE_RHS = DUTY_RHS + PID_calculate(PID, 0.2, 0, distance[0], 0);
        robot.rotate(motor1, PID->DUTY_CYCLE_RHS * 100, 1); // RHS
        robot.rotate(motor2, PID->DUTY_CYCLE_LHS * 100, 1); // LHS
        delay(10);
        // PID_calculate (PID_TypeDef *PID, float bound, float REFERENCE, float MEASURED_DISTANCE, float INTEGRAL_MAX);
    }
    robot.brake(motor1);
    robot.brake(motor2);
    if (facing == 0)
    {
        position[1] += ref_distance;
    }
    else if (facing == 180)
    {
        position[1] -= ref_distance;
    }
    else if (facing == 90)
    {
        position[0] += ref_distance;
    }
    else
    {
        position[0] -= ref_distance;
    }

    return position;
}

std::vector<float> go_straight_gyro(std::vector<float> position, float facing, float ref_distance, Robojax_L298N_DC_motor robot, int motor1, int motor2, MPU6050 mpu)
{

    MD md;
    float ang = 0;
    float total_x = 0;
    float total_y = 0;
    float change_x = 0;
    float change_y = 0;
    std::vector<float> distance{0, 0, 0, 0};
    int speed1 = 70;//RHS
    int speed2 = 70;//LHS
    mousecam_read_motion(PIN_MOUSECAM_CS, &md);
    distance = ADNS3080_DISTANCE(total_x, total_y, PIN_MOUSECAM_CS, md);
    float init_distance_x = distance[0];
    float init_distance_y = distance[1];
    while (init_distance_y + ref_distance > distance[1])
    {
        digitalWrite(LED_CHECK, HIGH);
        mousecam_read_motion(PIN_MOUSECAM_CS, &md);
        ADNS3080_RESOLUTION_CHECK(PIN_MOUSECAM_CS, md);
        if (md.squal / 4 < 12)
        {
            digitalWrite(LED_CHECK, LOW);
        }
        distance = ADNS3080_DISTANCE(total_x, total_y, PIN_MOUSECAM_CS, md);
        total_x = distance[0] * 157 / 58;
        total_y = distance[1] * 157 / 58;
        Serial.println("distance y : " + String(distance[0] - init_distance_y));
        Serial.println("distance x : " + String(distance[1] - init_distance_x));
        if (ang < 0)
        {
            speed1 = 75;
            speed2 = 63;
        }
        else if (ang > 0)
        {
            speed1 = 65;
            speed2 = 75;
        }
        else
        {
            speed1 = 70;
            speed2 = 67;
        }
        robot.rotate(motor1, speed1, 1);
        robot.rotate(motor2, speed2, 1);
        delay(10);
        mpu.update();
        ang = mpu.getAngleZ();
        // PID_calculate (PID_TypeDef *PID, float bound, float REFERENCE, float MEASURED_DISTANCE, float INTEGRAL_MAX);
    }
    robot.brake(motor1);
    robot.brake(motor2);
    if (facing == 0)
    {
        position[1] += ref_distance;
    }
    else if (facing == 180)
    {
        position[1] -= ref_distance;
    }
    else if (facing == 90)
    {
        position[0] += ref_distance;
    }
    else
    {
        position[0] -= ref_distance;
    }

    return position;
}

// direction = 0 Left
// direction = 1 Right

float go_rotation_gyro(float facing, float angle, int direction, float DUTY_LHS, float DUTY_RHS, Robojax_L298N_DC_motor robot, int motor1, int motor2, MPU6050 mpu)
{
    MD md;
    mpu.calcGyroOffsets();
    mpu.update();
    float ang = -mpu.getAngleZ();
    float setpoint = ang + angle;
    Serial.println("setpoint" + String(setpoint));
    if (direction)
    { // turn right
        mpu.update();
        facing += angle;
        float ang = -mpu.getAngleZ();
        float setpoint = ang + angle;
        Serial.println("setpoint" + String(setpoint));

        while (ang < setpoint)
        {
            mousecam_read_motion(PIN_MOUSECAM_CS, &md);
            robot.rotate(motor1, DUTY_RHS * 100, 0); // RHS
            robot.rotate(motor2, DUTY_LHS * 100, 1); // LHS
            // Serial.println("facing : "+String(facing));
            Serial.println("angle z : " + String(ang));
            delay(10);
            mpu.update();
            ang = -mpu.getAngleZ();
        }
        while (ang > setpoint)
        {
            robot.rotate(motor1, 20, 1); // RHS
            robot.rotate(motor2, 20, 0); // LHS
            delay(10);
            mpu.update();
            ang = -mpu.getAngleZ();
        }
        Serial.print("brake");
        robot.brake(motor1);
        robot.brake(motor2);
    }
    else
    { // turn left
        facing = 360 + (facing - angle);
        mpu.update();
        float ang = mpu.getAngleZ();
        float setpoint = ang + angle;
        Serial.println("setpoint" + String(setpoint));
        while (ang < setpoint)
        {
            mousecam_read_motion(PIN_MOUSECAM_CS, &md);
            robot.rotate(motor1, DUTY_RHS * 100, 1); // RHS
            robot.rotate(motor2, DUTY_LHS * 100, 0); // LHS
            Serial.println("angle z : " + String(mpu.getAngleZ()));
            // Serial.println("facing: "+String(facing));
            delay(10);
            mpu.update();
            ang = mpu.getAngleZ();
        }
        while (ang > setpoint)
        {
            robot.rotate(motor1, 20, 0); // RHS
            robot.rotate(motor2, 20, 1); // LHS
            delay(10);
            mpu.update();
            ang = mpu.getAngleZ();
        }
        robot.brake(motor1);
        robot.brake(motor2);
    }
    facing = remainder(facing, 360);
    if (facing < 0)
    {
        facing += 360;
    }
    return facing;
}
