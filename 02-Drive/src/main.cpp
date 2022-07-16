#include <Arduino.h>
#include "SPI.h"
#include "ADNS3080.h"
#include <Robojax_L298N_DC_motor.h>
#include "PID_BLOCK.h"
#include "DRIVE_CONTROL.h"
#include <stdio.h>

// optical sensor settings
#define PIN_SS 5              // D7
#define PIN_MISO 19           // D5
#define PIN_MOSI 23           // D2
#define PIN_SCK 18            // D6
#define PIN_MOUSECAM_RESET 35 // A5
#define PIN_MOUSECAM_CS 5     // D7
#define LED_CHECK 32

// motor 1 settings
#define CHA 0
#define ENA 17 // D8 PWMA Athis pin must be PWM enabled pin if Arduino board is used
#define IN1 16 // D9 AIN2
#define IN2 14 // D10 AIN1
// motor 2 settings
#define IN3 4  // D11
#define IN4 15 // D12
#define ENB 2  // D13 PWMB this pin must be PWM enabled pin if Arduino board is used
#define CHB 1
const int CCW = 2; // do not change
const int CW = 1;  // do not change
#define motor1 1   // do not change
#define motor2 2   // do not change

Robojax_L298N_DC_motor robot(IN1, IN2, ENA, CHA, IN3, IN4, ENB, CHB);
// for two motors with debug information
// Robojax_L298N_DC_motor robot(IN1, IN2, ENA, CHA, IN3, IN4, ENB, CHB, true);

float total_x = 0;
float total_y = 0;
float distance_x = 0;
float distance_y = 0;

PID_TypeDef PID_straight;
PID_TypeDef PID_rotation;
std::vector<float> distance;

float DUTY_LHS = 0.8;
float DUTY_RHS = 0.73;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  ADNS3080_INIT(PIN_SS, PIN_MISO, PIN_MOSI, PIN_SCK);
  ADNS3080_INIT_CHECK(PIN_MOUSECAM_CS, PIN_MOUSECAM_RESET);
  pinMode(LED_CHECK, OUTPUT);

  robot.begin();

  PID_init(&PID_straight, 0.015, 0.01, 0);
  PID_init(&PID_rotation, 0.01, 0, 0);
  // PID_TypeDef PID, float KP, float KI, float KD
  delay(10000);
}
std::string mode = "A";
void loop()
{
  int command = 0;
  if (mode == "M")
  {
    switch (command)
    {
    case 0:
      robot.brake(motor1);
      robot.brake(motor2);
      break;
    case 1: // moving forward
      robot.rotate(motor1, 70, 1);
      robot.rotate(motor2, 70, 1);
    case 2: // moving backward
      robot.rotate(motor1, 70, 0);
      robot.rotate(motor2, 70, 0);
      break;
    case 3: // turning left
      robot.rotate(motor1, 70, 0);
      robot.rotate(motor2, 70, 1);
      break;
    case 4: // turning right
      robot.rotate(motor1, 70, 1);
      robot.rotate(motor2, 70, 0);
      break;
    }
  }
  else if (mode == "A")
  {
    // MD md;
    // digitalWrite(LED_CHECK, HIGH);
    // mousecam_read_motion(PIN_MOUSECAM_CS, &md);
    // ADNS3080_RESOLUTION_CHECK(PIN_MOUSECAM_CS, md);

    // if (md.squal / 4 < 12)
    // {
    //   digitalWrite(LED_CHECK, LOW);
    // }

    // std::vector<float> distance = ADNS3080_DISTANCE(distance_x, distance_y, total_x, total_y, PIN_MOUSECAM_CS, md);
    // total_x = distance[0] * 157 / 48;
    // total_y = distance[1] * 157 / 48;

    // Serial.println('\n');
    // Serial.println("Distance_x = " + String(distance[0]));
    // Serial.println("Distance_y = " + String(distance[1]));
    // Serial.println("Duty LHS= " + String(PID_straight.DUTY_CYCLE_LHS * 100));
    // Serial.println("Duty RHS= " + String(PID_straight.DUTY_CYCLE_RHS * 100));

    // float ref_distance = 100;
    // straight(ref_distance, DUTY_LHS, DUTY_RHS, 0.2, 0, &PID_straight, distance, robot, motor1, motor2);
    // // rotation(90, 1, 0.5, 0.5, &PID_rotation, distance, robot, motor1, motor2);
    // // direction = 0 Left
    // // direction = 1 Right
    // delay(10);
    go_straight(50, DUTY_LHS, DUTY_RHS, &PID_straight, robot, motor1, motor2);
    go_rotation(90, 1, DUTY_LHS, DUTY_RHS, &PID_straight, robot, motor1, motor2);

    delay(1000);
  }
}