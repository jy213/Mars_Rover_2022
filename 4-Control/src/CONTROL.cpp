#include <Arduino.h>
#include "Robojax_L298N_DC_motor.h"
#include "PID_BLOCK.h"
#include <cmath>
#include <cstring>
#include "DRIVE_CONTROL.h"
#include "CONTROL.h"
#include <vector>
#include <stdio.h>
#include "SPI.h"
#include "ADNS3080.h"
#include <math.h>
#include <queue>
#include <iostream>

//optical sensor settings
#define PIN_SS        5  //D7
#define PIN_MISO      19 //D5
#define PIN_MOSI      23 //D2
#define PIN_SCK       18 //D6
#define PIN_MOUSECAM_RESET     35 //A5
#define PIN_MOUSECAM_CS        5  //D7
#define LED_CHECK 32


void UART_Syn(){
    while(Serial2.available() < 12){
        ;
    }
    int start = Serial2.read();
    int pink = Serial2.read();
    int yellow = Serial2.read();
    int blue = Serial2.read();
    int red = Serial2.read();
    int lightGreen = Serial2.read();
    int darkGreen = Serial2.read();
    int building = Serial2.read();
    if(start != 0){
        start = Serial2.read();
        start = Serial2.read();
        start = Serial2.read();
        start = Serial2.read();
    }
    while(Serial2.available() > 0){
        start = Serial2.read();
        pink = Serial2.read();
        yellow = Serial2.read();
        blue = Serial2.read();
        red = Serial2.read();
        lightGreen = Serial2.read();
        darkGreen = Serial2.read();
        building = Serial2.read();
    }
}

std::vector<std::vector<float>> get_image(std::vector<std::vector<float>> balls){
    int start = Serial2.read();
    int pink = Serial2.read();
    //Serial.println("pink : " + String(pink));
    int yellow = Serial2.read();
    //Serial.println("yellow : " + String(yellow));
    int blue = Serial2.read();
    //Serial.println("blue : " + String(blue));
    int red = Serial2.read();
    //Serial.println("red : " + String(red));
    int lightGreen = Serial2.read();
    //Serial.println("lightGreen : " + String(lightGreen));
    int darkGreen = Serial2.read();
    //Serial.println("darkGreen : " + String(darkGreen));
    int building = Serial2.read();
    balls[0][0] = pink / 16;
    balls[0][1] = (pink % 16 != 0) ? 5 + 5*(pink % 16) : 0;
    balls[1][0] = yellow / 16;
    balls[1][1] = (yellow % 16 != 0) ? 15 + 5*(yellow % 16) : 0;
    balls[2][0] = blue / 16;
    balls[2][1] = (blue % 16 != 0) ? 15 + 5*(blue % 16) : 0;
    balls[3][0] = red / 16;
    balls[3][1] = (red % 16 != 0) ? 15 + 5*(red % 16) : 0;
    balls[4][0] = lightGreen / 16;
    balls[4][1] = (lightGreen % 16 != 0) ? 15 + 5*(lightGreen % 16) : 0;
    balls[5][0] = darkGreen / 16;
    balls[5][1] = (darkGreen % 16 != 0) ? 15 + 5*(darkGreen % 16) : 0;

    return balls;
}

std::vector<float> calculate_left_position(float angle, float facing, int distance, std::vector<float> car_position){
    std::vector<float> ball_position{0,0};
    float pi = 3.141592654;
    angle = angle*pi / 180;
    float x = distance * sin(angle);
    float y = distance * cos(angle);
    if(facing == 0){
        ball_position[0] = car_position[0] - x;
        ball_position[1] = car_position[1] + y;
    }
    else if(facing == 90){
        ball_position[0] = car_position[0] + y;
        ball_position[1] = car_position[1] + x;
    }
    else if(facing == 180){
        ball_position[0] = car_position[0] + x;
        ball_position[1] = car_position[1] - y;
    }
    else if(facing == 270){
        ball_position[0] = car_position[0] - y;
        ball_position[1] = car_position[1] - x;
    }
    return ball_position;
}

std::vector<float> get_left_position(std::vector<float> car_position, float facing, int colour, int distance, std::vector<std::vector<float>> balls, Robojax_L298N_DC_motor robot, int motor1, int motor2, MPU6050 mpu){
    std::vector<float> ball_position{0,0};
    MD md;
    UART_Syn();
    mpu.update();
    float ang = mpu.getAngleZ();
    while(balls[colour][0] == 0){       //pink = 0, yellow = 1, blue = 2, red = 3, lightGreen = 4, darkGreen = 5;
        mousecam_read_motion(PIN_MOUSECAM_CS, &md);
        robot.rotate(motor1, 30, 1);
        robot.rotate(motor2, 30, 0);
        while(Serial2.available() < 8){
            ;
        }
        robot.brake(motor1);
        robot.brake(motor2);
        balls = get_image(balls);
        mpu.update();
        ang = mpu.getAngleZ();
        distance = balls[colour][1] + 8;
        Serial.println(String(balls[colour][0]));
    }
    delay(100);
    ball_position = calculate_left_position(ang, facing, distance, car_position);
    while(ang > 0){
        mousecam_read_motion(PIN_MOUSECAM_CS, &md);
        robot.rotate(motor1, 20, 0);
        robot.rotate(motor2, 20, 1);
        delay(10);
        mpu.update();
        ang = mpu.getAngleZ();
    }
    robot.brake(motor1);
    robot.brake(motor2);
    
    return ball_position;
}


std::vector<float> calculate_right_position(float angle, float facing, int distance, std::vector<float> car_position){
    std::vector<float> ball_position{0,0};
    float pi = 3.141592654;
    angle = angle*pi/180;
    float x = distance * sin(angle);
    float y = distance * cos(angle);
    if(facing == 0){
        ball_position[0] = car_position[0] + x;
        ball_position[1] = car_position[1] + y;
    }
    else if(facing == 90){
        ball_position[0] = car_position[0] + y;
        ball_position[1] = car_position[1] - x;
    }
    else if(facing == 180){
        ball_position[0] = car_position[0] - x;
        ball_position[1] = car_position[1] - y;
    }
    else if(facing == 270){
        ball_position[0] = car_position[0] - y;
        ball_position[1] = car_position[1] + x;
    }
    return ball_position;
}

std::vector<float> get_right_position(std::vector<float> car_position, float facing, int colour, int distance, std::vector<std::vector<float>> balls, Robojax_L298N_DC_motor robot, int motor1, int motor2, MPU6050 mpu){
    std::vector<float> ball_position{0,0};
    MD md;
    UART_Syn();
    mpu.update();
    float ang = -mpu.getAngleZ();
    while(balls[colour][0] == 1){        //pink = 0, yellow = 1, blue = 2, red = 3, lightGreen = 4, darkGreen = 5;
        mousecam_read_motion(PIN_MOUSECAM_CS, &md);
        robot.rotate(motor1, 30, 0);
        robot.rotate(motor2, 30, 1);
        while(Serial2.available() < 8){
            ;
        }
        robot.brake(motor1);
        robot.brake(motor2);
        balls = get_image(balls);
        mpu.update();
        ang = -mpu.getAngleZ();
        distance = balls[colour][1] + 8;
    }
    delay(100);
    ball_position = calculate_right_position(ang, facing, distance, car_position);

    while(ang > 0){
        mousecam_read_motion(PIN_MOUSECAM_CS, &md);
        robot.rotate(motor1, 20, 1);
        robot.rotate(motor2, 20, 0);
        delay(10);
        mpu.update();
        ang = -mpu.getAngleZ();
    }
    robot.brake(motor1);
    robot.brake(motor2);
    
    return ball_position;
}


int find_colour(std::vector<std::vector<float>> balls){
    for(int i = 0; i < 6; i++){
        if(balls[i][0] == 2 && balls[i][1] != 0){
            return i;
        }
    }
    return 10;
}


std::vector<float> get_middle_position(std::vector<float> car_position, float facing, int distance, Robojax_L298N_DC_motor robot, int motor1, int motor2, MPU6050 mpu){
    std::vector<float> ball_position{0,0};
    if(facing == 0){
        ball_position[0] = car_position[0];
        ball_position[1] = car_position[1] + distance;
    }
    else if(facing == 90){
        ball_position[0] = car_position[0] + distance;
        ball_position[1] = car_position[1];
    }
    else if(facing == 180){
        ball_position[0] = car_position[0];
        ball_position[1] = car_position[1] - distance;
    }
    else if(facing == 270){
        ball_position[0] = car_position[0] - distance;
        ball_position[1] = car_position[1];
    }
    return ball_position;
}

std::vector<std::vector<float>> rotate_360(float facing, std::vector<float> car_position, std::vector<std::vector<float>> balls, Robojax_L298N_DC_motor robot, int motor1, int motor2, MPU6050 mpu){
    std::vector<std::vector<float>> balls_position(6,std::vector<float>(2,0));
    MD md;
    UART_Syn();
    Serial.println(111);
    mpu.update();
    float ang;
    ang = -mpu.getAngleZ();
    float setpoint = ang + 360;
    int angle = 0;
    int colour = 10;
    while(ang < setpoint){
        mousecam_read_motion(PIN_MOUSECAM_CS, &md);
        robot.rotate(motor1, 30, 0);
        robot.rotate(motor2, 30, 1);
        while(Serial2.available() < 8){
            ;
        }
        robot.brake(motor1);
        robot.brake(motor2);
        balls = get_image(balls);
        colour = find_colour(balls);
        if(colour != 10){
            if(ang > 270){
                angle = ang - 270;
                balls_position[colour] = calculate_right_position(angle, 270, balls[colour][1] + 8, car_position);
            }
            else if(ang > 180){
                angle = ang - 180;
                balls_position[colour] = calculate_right_position(angle, 180, balls[colour][1] + 8, car_position);
            }
            else if(ang > 90){
                angle = ang - 90;
                balls_position[colour] = calculate_right_position(angle, 90, balls[colour][1] + 8, car_position);
            }
            else{
                angle = ang;
                balls_position[colour] = calculate_right_position(angle, 0, balls[colour][1] + 8, car_position);
            }
        }
        mpu.update();
        ang = -mpu.getAngleZ();
    }
    while(ang > setpoint){
        mousecam_read_motion(PIN_MOUSECAM_CS, &md);
        robot.rotate(motor1, 20, 1);
        robot.rotate(motor2, 20, 0);
        delay(10);
        mpu.update();
        ang = -mpu.getAngleZ();
    }
    robot.brake(motor1);
    robot.brake(motor2);
    return balls_position;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//寻找最短路径，输入的map中结点值为0表示可以经过，节点值为-1时不可经过
std::vector<std::vector<float>> route_search(std::vector<float> car_position, std::vector<float> destination_position, std::vector<std::vector<float>> map){
    std::vector<std::vector<float>> check(11, std::vector<float>(7,0));   //坐标检查图
    // 输入的map为距离地图
    std::vector<std::vector<std::vector<float>>> link(11, std::vector<std::vector<float>>(7, std::vector<float>(2,0))); //坐标连接图
    std::vector<std::vector<float>> route;  //建立最短路径
    std::queue<std::vector<float>> neighbours;      //建立缓存列表
    car_position = {car_position[0]/30 - 1, car_position[1]/30 - 1};    //转换坐标点到节点
    destination_position = {destination_position[0]/30 - 1, destination_position[1]/30 - 1};
    neighbours.push(car_position);  //储存初始的地图坐标
    while(!neighbours.empty()){     //对列表进行广度优先遍历
        auto current_position = neighbours.front();     //取出当前遍历的坐标点
        neighbours.pop();
        float row = current_position[0]; 
        float col = current_position[1];
        check[row][col] = 1;    //将当前节点设置为已经被使用过
        if (row - 1 >= 0 && map[row-1][col] >= 0) {     // 向上遍历
            if(check[row-1][col]==0){   //判断上方节点是否被使用过
                neighbours.push({row-1, col});  //如果没有被使用过，则推进列表等待遍历
            }
            if(map[row-1][col] != 0){   //判断是否被赋过值
                if(map[row][col]+1 < map[row-1][col]){
                    map[row-1][col] = map[row][col] + 1;    //两者取最近路径
                    link[row-1][col] = {row,col};   //更新连接坐标点
                }
            }
            else{ 
                map[row-1][col] = map[row][col] + 1;    //直接赋值
                link[row-1][col] = {row,col};   //存储上一个连接坐标点
            }
        }
        if (row + 1 < 11 && map[row+1][col] >= 0) {     // 向下遍历
            if(check[row+1][col]==0){
                neighbours.push({row+1, col});
            }
            if(map[row+1][col] != 0){
                if(map[row][col]+1 < map[row+1][col]){
                    map[row+1][col] = map[row][col] + 1;   
                    link[row+1][col] = {row,col};   
                }
            }
            else{
                map[row+1][col] = map[row][col] + 1;
                link[row+1][col] = {row,col};
            }
        }
        if (col - 1 >= 0 && map[row][col-1] >= 0) {     // 向左遍历
            if(check[row][col-1]==0){
                neighbours.push({row, col-1});
            }
            if(map[row][col-1] != 0){
                if(map[row][col]+1 < map[row][col-1]){
                    map[row][col-1] = map[row][col] + 1;   
                    link[row][col-1] = {row,col};   
                }
            }
            else{
                map[row][col-1] = map[row][col] + 1;
                link[row][col-1] = {row,col};
            }
        }
        if (col + 1 < 7 && map[row][col+1] >= 0) {     // 向右遍历
            if(check[row][col+1]==0){
                neighbours.push({row, col+1});
            }
            if(map[row][col+1] != 0){
                if(map[row][col]+1 < map[row][col+1]){
                    map[row][col+1] = map[row][col] + 1;   
                    link[row][col+1] = {row,col};   
                }
            }
            else{
                map[row][col+1] = map[row][col] + 1;
                link[row][col+1] = {row,col};
            }
        }
    }   //while循环递归结束

    //下面寻找最短路径并返回
    while(destination_position != car_position){
        route.push_back({(destination_position[0]+1)*30, (destination_position[1]+1)*30});
        destination_position = link[destination_position[0]][destination_position[1]];
    }   // 这里的vector需要反转
    route.push_back({(destination_position[0]+1)*30, (destination_position[1]+1)*30});


    //反转vector路径顺序
    int len = route.size();
    std::vector<std::vector<float>> final_route;
    for(int i = len-1; i >= 0; i--){
        final_route.push_back(route[i]);
    }

    return final_route;
}

//转换路径到车子动作
// 0: 向前30， 1： 向左转90， 2: 向右转90，3: 向右转180；
std::queue<float> route_to_action(std::vector<std::vector<float>> route, float facing){
    int len = route.size();
    std::queue<float> action;
    std::vector<float> current_position;
    std::vector<float> next_position;
    for(int i = 1; i < len; i++){
        current_position = route[i-1];
        next_position = route[i];
        if(facing == 0){
            if(next_position[0] == current_position[0] && next_position[1] > current_position[1]){  //前
                action.push(0);
            }
            else if(next_position[0] == current_position[0] && next_position[1] < current_position[1]){ //后
                action.push(3);
                facing += 180;
                if(facing >= 360){
                    facing -= 360;
                }
                action.push(0);
            }
            else if(next_position[0] > current_position[0] && next_position[1] == current_position[1]){ //右
                action.push(2);
                facing += 90;
                if(facing >= 360){
                    facing -= 360;
                }
                action.push(0);
            }
            else if(next_position[0] < current_position[0] && next_position[1] == current_position[1]){ //左
                action.push(1);
                facing += 270;
                if(facing >= 360){
                    facing -= 360;
                }
                action.push(0);
            }
        }
        else if(facing == 90){
            if(next_position[0] == current_position[0] && next_position[1] > current_position[1]){  //左
                action.push(1);
                facing += 270;
                if(facing >= 360){
                    facing -= 360;
                }
                action.push(0);
            }
            else if(next_position[0] == current_position[0] && next_position[1] < current_position[1]){ //右
                action.push(2);
                facing += 90;
                if(facing >= 360){
                    facing -= 360;
                }
                action.push(0);
            }
            else if(next_position[0] > current_position[0] && next_position[1] == current_position[1]){ //前
                action.push(0);
            }
            else if(next_position[0] < current_position[0] && next_position[1] == current_position[1]){ //后
                action.push(3);
                facing += 180;
                if(facing >= 360){
                    facing -= 360;
                }
                action.push(0);
            }
        }
        else if(facing == 180){
            if(next_position[0] == current_position[0] && next_position[1] > current_position[1]){  //后
                action.push(3);
                facing += 180;
                if(facing >= 360){
                    facing -= 360;
                }
                action.push(0);
            }
            else if(next_position[0] == current_position[0] && next_position[1] < current_position[1]){ //前
                action.push(0);
            }
            else if(next_position[0] > current_position[0] && next_position[1] == current_position[1]){ //左
                action.push(1);
                facing += 270;
                if(facing >= 360){
                    facing -= 360;
                }
                action.push(0);
            }
            else if(next_position[0] < current_position[0] && next_position[1] == current_position[1]){ //右
                action.push(2);
                facing += 90;
                if(facing >= 360){
                    facing -= 360;
                }
                action.push(0);
            }
        }
        else if(facing == 270){
            if(next_position[0] == current_position[0] && next_position[1] > current_position[1]){  //右
                action.push(2);
                facing += 90;
                if(facing >= 360){
                    facing -= 360;
                }
                action.push(0);
            }
            else if(next_position[0] == current_position[0] && next_position[1] < current_position[1]){ //左
                action.push(1);
                facing += 270;
                if(facing >= 360){
                    facing -= 360;
                }
                action.push(0);
            }
            else if(next_position[0] > current_position[0] && next_position[1] == current_position[1]){ //后
                action.push(3);
                facing += 180;
                if(facing >= 360){
                    facing -= 360;
                }
                action.push(0);
            }
            else if(next_position[0] < current_position[0] && next_position[1] == current_position[1]){ //前
                action.push(0);
            }
        }
    }
    return action;
}


//查询目的地坐标
int next_destination(std::vector<std::vector<float>> destination_position){
    for(int i = 0; i < 8; i++){
        if(destination_position[i][0] != 0 && destination_position[i][1] != 0){
            return i;
        }
    }
    return -1;
}

std::vector<std::vector<float>> check_balls_on_map(std::vector<std::vector<float>> destination_position,  std::vector<std::vector<float>> map){
    if(map[1][1] == -1){
        destination_position[0] = {0,0};
    }
    if(map[1][5] == -1){
        destination_position[1] = {0,0};
    }
    if(map[3][3] == -1){
        destination_position[2] = {0,0};
    }
    if(map[5][1] == -1){
        destination_position[3] = {0,0};
    }
    if(map[5][5] == -1){
        destination_position[4] = {0,0};
    }
    if(map[7][3] == -1){
        destination_position[5] = {0,0};
    }
    if(map[9][1] == -1){
        destination_position[6] = {0,0};
    }
    if(map[9][5] == -1){
        destination_position[7] = {0,0};
    }
    return destination_position;
}



