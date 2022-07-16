#include <Arduino.h>
#include "SPI.h"
#include "ADNS3080.h"
#include <Robojax_L298N_DC_motor.h>
#include "PID_BLOCK.h"
#include "DRIVE_CONTROL.h"
#include "CONTROL.h"
#include <stdio.h>

#include "time.h"

// database
// Provide the token generation process info.
#include "addons/TokenHelper.h"
// Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

#include "ADNS3080.h"
#include "SPI.h"

#define API_KEY "AIzaSyDQvkbQMazYHiTdUFLC9F4o71uCP6MEqI0"
#define DATABASE_URL "https://rover-back-end-default-rtdb.europe-west1.firebasedatabase.app"

#define WIFI_SSID "HUAWEI Mate 20 Pro"
#define WIFI_PASSWORD "xjb07124116"

// Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
int counter = 0;
int count = 0;
int mode;
int mannual_sig;
bool signupOK = false;

// Database main path (to be updated in setup with the user UID)
String databasePath;
// Database child nodes
String rover_loc;
String y_value = "/y_value";
String x_value = "/x_value";
String color="/color";
String Server_Status = "/Server_Status";
// Parent Node (to be updated in every loop)
String parentPath;

FirebaseJson json;

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
#define ENA 33 // A3 PWMA Athis pin must be PWM enabled pin if Arduino board is used
#define IN1 25 // A2 AIN2
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

float DUTY_LHS_straight = 0.73;
float DUTY_RHS_straight = 0.70;

float DUTY_LHS_rotate = 0.3;
float DUTY_RHS_rotate = 0.3;

MPU6050 mpu(Wire);

int next_desination_index = 0;
float Facing = 0;
std::vector<float> Position{0,0}; //{x,y}
std::vector<float> Destination{0,0};
std::queue<float> action;
std::vector<std::vector<float>> balls_info(6, std::vector<float>(2,0));
std::vector<std::vector<float>> balls_position(6, std::vector<float>(2,0));
std::vector<std::vector<float>> Map(11, std::vector<float>(7,0));
std::vector<std::vector<float>> destination_position(8, std::vector<float>(2,0));
std::vector<std::vector<float>> route;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial2.begin(115200);

  // optical sensor setup
  ADNS3080_INIT(PIN_SS, PIN_MISO, PIN_MOSI, PIN_SCK);
  ADNS3080_INIT_CHECK(PIN_MOUSECAM_CS, PIN_MOUSECAM_RESET);
  pinMode(LED_CHECK, OUTPUT);

  // motor setup
  robot.begin();

  // PID setup
  PID_init(&PID_straight, 0.015, 0.01, 0);
  PID_init(&PID_rotation, 0.01, 0, 0);
  // PID_TypeDef PID, float KP, float KI, float KD

  //database
  robot.begin();
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  ADNS3080_INIT(PIN_SS, PIN_MISO, PIN_MOSI, PIN_SCK);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }

  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", ""))
  {
    Serial.println("ok");
    signupOK = true;
  }
  else
  {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  // gyro setup
  Wire.begin();
  byte status = mpu.begin();
  Serial.print(F("MPU6050 status: "));
  Serial.println(status);
  while (status != 0)
  {
  } // stop everything if could not connect to MPU6050
  Serial.println(F("Calculating offsets, do not move MPU6050"));
  delay(1000);
  mpu.calcGyroOffsets(); // gyro and accelero
  Serial.println("Done!\n");

  delay(1000);
}

void loop()
{

  //Position = go_straight_gyro(Position, Facing, 30, robot, motor1, motor2, mpu);
  //Position = go_straight(Position, Facing, 30, DUTY_LHS_straight, DUTY_RHS_straight, &PID_straight, robot, motor1, motor2);
  //delay(1000);
  //Facing = go_rotation_gyro(Facing, 90, 1, DUTY_LHS_rotate, DUTY_RHS_rotate, robot, motor1, motor2, mpu);
  //delay(1000);
  //-----------------------  initialise the data

  Facing = 0;   // global angle;
  Position = {60,60};   // {x,y};
  for(int i = 0; i < 6; i++){   // check the info of each ball is [2,0];
    balls_info[i][0] = 2;
  }

  // Serial.println("/////////");
  // balls_position = rotate_360(Facing, Position, balls_info, robot, motor1, motor2, mpu);
  // for(int i = 0; i < 6; i++){
  //   Serial.println("color index: " + String(i));
  //   Serial.println("ball x : " + String(balls_position[i][0]));
  //   Serial.println("ball y : " + String(balls_position[i][1]));
  //   Serial.println(" ");
  // }
  // delay(5000);
  
  destination_position[0] = {60,60};
  destination_position[1] = {60,180};
  destination_position[2] = {120,120};
  destination_position[3] = {180,60};
  destination_position[4] = {180,180};
  destination_position[5] = {240,120};
  destination_position[6] = {300,60};
  destination_position[7] = {300,180};

  int colour = 10;
  int row = 0;
  int col = 0;
  // ----------------------------------------------
  Serial.println(1);

  while(next_desination_index != -1){
    bool on_destination = false;
    float next_action;
    //判断当前是否在目标点上并搜查下一个目标点
    for(int i = 0; i < 8; i++){
      Serial.println(i);
      if(Position == destination_position[i]){  //如果在目标点上
        on_destination = true;
        destination_position[i] = {0,0};
        balls_position = rotate_360(Facing, Position, balls_info, robot, motor1, motor2, mpu);  //在目标点转圈测球
        delay(1000);
        for(int i = 0; i < 6; i++){   //打印6个球
          Serial.println("color index: " + String(i));
          Serial.println("ball x : " + String(balls_position[i][0]));
          Serial.println("ball y : " + String(balls_position[i][1]));
          Serial.println(" ");
        }

        for(int i = 0; i < 6; i++){   //查看是否检测到球
          if(balls_position[i][0] != 0 || balls_position[i][1] != 0){ //如果检测到球则更新map
            colour = i;
            row = balls_position[i][0]/30;
            col = balls_position[i][1]/30;

            /////////////////////////////////////////////////////////////////////////////////////////////////////
            parentPath= databasePath + "/" + String("alien_loc") + "/" + String(counter);
            counter++;
            json.set(x_value.c_str(), balls_position[i][0]);
            json.set(y_value.c_str(), balls_position[i][1]);
            if(colour == 0){
              json.set(color.c_str(), String("pink"));
            }
            else if(colour == 1){
              json.set(color.c_str(), String("yellow"));
            }
            else if(colour == 2){
              json.set(color.c_str(), String("blue"));
            }
            else if(colour == 3){
              json.set(color.c_str(), String("red"));
            }
            else if(colour == 4){
              json.set(color.c_str(), String("lightgreen"));
            }
            else if(colour == 5){
              json.set(color.c_str(), String("darkgreen"));
            }
            Serial.printf("Set json... %s\n", Firebase.RTDB.setJSON(&fbdo, parentPath.c_str(), &json) ? "ok" : fbdo.errorReason().c_str());
            ///////////////////////////////////////////////////////////////////////////////////////////////////////////

            if(row-1 >= 0 && col-1 >= 0){ //左下点
              Map[row-1][col-1] = -1;
            }
            if(row-1 >= 0 && col < 7){ //左上点
              Map[row-1][col] = -1;
            }
            if(row < 11 && col-1 >= 0){ //右下点
              Map[row][col-1] = -1;
            }
            if(row < 11 && col < 7){  //右上点
              Map[row][col] = -1; 
            }
            destination_position = check_balls_on_map(destination_position, Map);
          }
        }
      }
    }
    //判断完成后根据当前位置进行操作
    if(on_destination){   //如果在点上
      next_desination_index = next_destination(destination_position);   //更新下一个目标点的系数
      if(next_desination_index != -1){  //如果有下一个目的地
        Destination = destination_position[next_desination_index];    //找出目标点坐标
        route = route_search(Position, Destination, Map); //更新路径
        action = route_to_action(route, Facing);  //更新小车行为列表
      }
      else{
        break;
      }
    }
    else{   //如果不在点上
      next_action = action.front(); //找出下一个行为
      action.pop(); //更新列表

      //操作行为
      if(next_action == 0){ //往前走30厘米
        //Position = go_straight_gyro(Position, Facing, 30, robot, motor1, motor2, mpu);
        Position = go_straight(Position, Facing, 30, DUTY_LHS_straight, DUTY_RHS_straight, &PID_straight, robot, motor1, motor2);
        /////////////////////////////////////////////////////////////////////////////////////////////////////
        parentPath= databasePath + "/" + String("rover_loc") + "/" + String(0);
        json.set(x_value.c_str(), Position[0]);
        json.set(y_value.c_str(), Position[1]);
        Serial.printf("Set json... %s\n", Firebase.RTDB.setJSON(&fbdo, parentPath.c_str(), &json) ? "ok" : fbdo.errorReason().c_str());
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////

      }
      else if(next_action == 1){  //往左转90度
        Facing = go_rotation_gyro(Facing, 90, 0, DUTY_LHS_rotate, DUTY_RHS_rotate, robot, motor1, motor2, mpu);
      }
      else if(next_action == 2){  //往右转90度
        Facing = go_rotation_gyro(Facing, 90, 1, DUTY_LHS_rotate, DUTY_RHS_rotate, robot, motor1, motor2, mpu);
      }
      else if(next_action == 3){  //往右转180度
        Facing = go_rotation_gyro(Facing, 90, 1, DUTY_LHS_rotate, DUTY_RHS_rotate, robot, motor1, motor2, mpu);
        Facing = go_rotation_gyro(Facing, 90, 1, DUTY_LHS_rotate, DUTY_RHS_rotate, robot, motor1, motor2, mpu);
      }

      next_action = action.front(); //找出下一个行为

      if(next_action == 0){ //如果需要直行
        UART_Syn(); //传输同步
        while(Serial2.available() < 8){ //等UART传最新数据
          ;
        }
        balls_info = get_image(balls_info); //拿到6个小球的数据
        for(int i = 0; i < 6; i++){
            Serial.println(String(balls_info[i][0]) + "  " + String(balls_info[i][1]));
        }
        delay(100);
        for(int i = 0; i < 6; i++){   //拿到小球数据后判断前方是否有球
          if(balls_info[i][1] != 0){  //如果有球

            //如果有小球在左边
            if(balls_info[i][0] == 0){
              balls_position[i] = get_left_position(Position, Facing, i, balls_info[i][1], balls_info, robot, motor1, motor2, mpu);
              if(balls_position[i][0] != 0 || balls_position[i][1] != 0){ //如果检测到球则更新map
                colour = i;
                row = balls_position[i][0]/30;
                col = balls_position[i][1]/30;
                /////////////////////////////////////////////////////////////////////////////////////////////////////
                parentPath= databasePath + "/" + String("alien_loc") + "/" + String(counter);
                counter++;
                json.set(x_value.c_str(), balls_position[i][0]);
                json.set(y_value.c_str(), balls_position[i][1]);
                if(colour == 0){
                  json.set(color.c_str(), String("pink"));
                }
                else if(colour == 1){
                  json.set(color.c_str(), String("yellow"));
                }
                else if(colour == 2){
                  json.set(color.c_str(), String("blue"));
                }
                else if(colour == 3){
                  json.set(color.c_str(), String("red"));
                }
                else if(colour == 4){
                  json.set(color.c_str(), String("light green"));
                }
                else if(colour == 5){
                  json.set(color.c_str(), String("dark green"));
                }
                Serial.printf("Set json... %s\n", Firebase.RTDB.setJSON(&fbdo, parentPath.c_str(), &json) ? "ok" : fbdo.errorReason().c_str());
                ///////////////////////////////////////////////////////////////////////////////////////////////////////////

                if(row-1 >= 0 && col-1 >= 0){ //左下点
                  Map[row-1][col-1] = -1;
                }
                if(row-1 >= 0 && col < 7){ //左上点
                  Map[row-1][col] = -1;
                }
                if(row < 11 && col-1 >= 0){ //右下点
                  Map[row][col-1] = -1;
                }
                if(row < 11 && col < 7){  //右上点
                  Map[row][col] = -1; 
                }
                destination_position = check_balls_on_map(destination_position, Map);
                if(destination_position[next_desination_index][0] == 0 && destination_position[next_desination_index][1] == 0){
                  next_desination_index = next_destination(destination_position);
                  if(next_desination_index == -1){
                    break;
                  }
                  else{
                    Destination = destination_position[next_desination_index];
                  }
                }
              }
              route = route_search(Position, Destination, Map); //更新路径
              action = route_to_action(route, Facing);  //更新小车行为列表
              Serial.println("position:  " + String(balls_position[i][0]) + ", " + String(balls_position[i][1]));
            }

            //如果有小球在右边
            else if(balls_info[i][0] == 1){
              balls_position[i] = get_right_position(Position, Facing, i, balls_info[i][1], balls_info, robot, motor1, motor2, mpu);
              if(balls_position[i][0] != 0 || balls_position[i][1] != 0){ //如果检测到球则更新map
                colour = i;
                row = balls_position[i][0]/30;
                col = balls_position[i][1]/30;

                /////////////////////////////////////////////////////////////////////////////////////////////////////
                parentPath= databasePath + "/" + String("alien_loc") + "/" + String(counter);
                counter++;
                json.set(x_value.c_str(), balls_position[i][0]);
                json.set(y_value.c_str(), balls_position[i][1]);
                if(colour == 0){
                  json.set(color.c_str(), String("pink"));
                }
                else if(colour == 1){
                  json.set(color.c_str(), String("yellow"));
                }
                else if(colour == 2){
                  json.set(color.c_str(), String("blue"));
                }
                else if(colour == 3){
                  json.set(color.c_str(), String("red"));
                }
                else if(colour == 4){
                  json.set(color.c_str(), String("light green"));
                }
                else if(colour == 5){
                  json.set(color.c_str(), String("dark green"));
                }
                Serial.printf("Set json... %s\n", Firebase.RTDB.setJSON(&fbdo, parentPath.c_str(), &json) ? "ok" : fbdo.errorReason().c_str());
                ///////////////////////////////////////////////////////////////////////////////////////////////////////////

                if(row-1 >= 0 && col-1 >= 0){ //左下点
                  Map[row-1][col-1] = -1;
                }
                if(row-1 >= 0 && col < 7){ //左上点
                  Map[row-1][col] = -1;
                }
                if(row < 11 && col-1 >= 0){ //右下点
                  Map[row][col-1] = -1;
                }
                if(row < 11 && col < 7){  //右上点
                  Map[row][col] = -1; 
                }
                destination_position = check_balls_on_map(destination_position, Map);
                if(destination_position[next_desination_index][0] == 0 && destination_position[next_desination_index][1] == 0){
                  next_desination_index = next_destination(destination_position);
                  if(next_desination_index == -1){
                    break;
                  }
                  else{
                    Destination = destination_position[next_desination_index];
                  }
                }
              }
              route = route_search(Position, Destination, Map); //更新路径
              action = route_to_action(route, Facing);  //更新小车行为列表
              Serial.println("position:  " + String(balls_position[i][0]) + ", " + String(balls_position[i][1]));
            }
            //如果正对小球
            else{
              balls_position[i] = get_middle_position(Position, Facing, balls_info[i][1], robot, motor1, motor2, mpu);
              if(balls_position[i][0] != 0 || balls_position[i][1] != 0){ //如果检测到球则更新map
                colour = i;
                row = balls_position[i][0]/30;
                col = balls_position[i][1]/30;

                /////////////////////////////////////////////////////////////////////////////////////////////////////
                parentPath= databasePath + "/" + String("alien_loc") + "/" + String(counter);
                counter++;
                json.set(x_value.c_str(), balls_position[i][0]);
                json.set(y_value.c_str(), balls_position[i][1]);
                if(colour == 0){
                  json.set(color.c_str(), String("pink"));
                }
                else if(colour == 1){
                  json.set(color.c_str(), String("yellow"));
                }
                else if(colour == 2){
                  json.set(color.c_str(), String("blue"));
                }
                else if(colour == 3){
                  json.set(color.c_str(), String("red"));
                }
                else if(colour == 4){
                  json.set(color.c_str(), String("light green"));
                }
                else if(colour == 5){
                  json.set(color.c_str(), String("dark green"));
                }
                Serial.printf("Set json... %s\n", Firebase.RTDB.setJSON(&fbdo, parentPath.c_str(), &json) ? "ok" : fbdo.errorReason().c_str());
                ///////////////////////////////////////////////////////////////////////////////////////////////////////////

                if(row-1 >= 0 && col-1 >= 0){ //左下点
                  Map[row-1][col-1] = -1;
                }
                if(row-1 >= 0 && col < 7){ //左上点
                  Map[row-1][col] = -1;
                }
                if(row < 11 && col-1 >= 0){ //右下点
                  Map[row][col-1] = -1;
                }
                if(row < 11 && col < 7){  //右上点
                  Map[row][col] = -1; 
                }
                destination_position = check_balls_on_map(destination_position, Map);
                if(destination_position[next_desination_index][0] == 0 && destination_position[next_desination_index][1] == 0){
                  next_desination_index = next_destination(destination_position);
                  if(next_desination_index == -1){
                    break;
                  }
                  else{
                    Destination = destination_position[next_desination_index];
                  }
                }
              }
              route = route_search(Position, Destination, Map); //更新路径
              action = route_to_action(route, Facing);  //更新小车行为列表
              Serial.println("middle" + String(balls_position[i][1]));
            }
          }
          delay(100);
        }
      }
    }
  }
  robot.rotate(motor1, 100, 1);
  robot.rotate(motor2, 100, 0);
  delay(100000);

  

  // Position = go_straight(Position, Facing, 10, DUTY_LHS_straight, DUTY_RHS_straight, &PID_straight, robot, motor1, motor2);
  // Facing = go_rotation_gyro(Facing, 90, 1, DUTY_LHS_rotate, DUTY_RHS_rotate, robot, motor1, motor2, mpu);

  // UART_Syn();
  // while(Serial2.available() < 8){
  //   ;
  // }
  // balls_info = get_image(balls_info);
  // for(int i = 0; i < 6; i++){
  //     Serial.println(String(balls_info[i][0]) + "  " + String(balls_info[i][1]));
  // }
  // delay(5000);
  // balls_position = rotate_360(Facing, Position, balls_info, robot, motor1, motor2, mpu);
  // for(int i = 0; i < 6; i++){
  //   Serial.println("color index: " + String(i));
  //   Serial.println("ball x : " + String(balls_position[i][0]));
  //   Serial.println("ball y : " + String(balls_position[i][1]));
  //   Serial.println(" ");
  // }
  // for(int i = 0; i < 6; i++){
  //   if(balls_info[i][1] != 0){
  //     if(balls_info[i][0] == 0){
  //       balls_position[i] = get_left_position(Position, Facing, i, balls_info[i][1], balls_info, robot, motor1, motor2, mpu);
  //       Serial.println("position:  " + String(balls_position[i][0]) + ", " + String(balls_position[i][1]));
  //     }
  //     else if(balls_info[i][0] == 1){
  //       balls_position[i] = get_right_position(Position, Facing, i, balls_info[i][1], balls_info, robot, motor1, motor2, mpu);
  //       Serial.println("position:  " + String(balls_position[i][0]) + ", " + String(balls_position[i][1]));
  //     }
  //     else{
  //       Serial.println("middle" + String(balls_position[i][1]));
  //     }
  //   }
  //   delay(1000);
  // }
  // Position = go_straight_gyro(Position, Facing, 30, robot, motor1, motor2, mpu);
  // delay(1000);
  // Facing = go_rotation_gyro(Facing, 90, 0, DUTY_LHS_rotate, DUTY_RHS_rotate, robot, motor1, motor2, mpu);
  // delay(1000);
  // Position = go_straight_gyro(Position, Facing, 30, robot, motor1, motor2, mpu);
  // Facing = go_rotation_gyro(Facing, 360, 0, DUTY_LHS_rotate, DUTY_RHS_rotate, robot, motor1, motor2, mpu);
}

// 24
// 52
// 100 green red yellow pink