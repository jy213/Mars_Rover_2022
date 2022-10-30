# Mars_Rover_2022
Year 2 Mars Rover Project 

The rover is composed of seven subsystems: energy, drive, vision, control, command, radar, and integration.

1) Energy Subsystem

The energy subsystem will provide the rover with charged power bank batteries using solar 
panels. There will be solar panels, an SMPS kit with an adapter board, and 2 channel relay 
board. Status of the battery when it is charging and when the rover is operational should be 
displayed. Rover range and capability estimation could also be tracked. 

2) Drive Subsystem

This module aims to control the movement of the Mars Rover and measure the distance 
travelled in x and y directions. This information can be transmitted to other modules to 
estimate the exact location of the Rover and to control its movement to achieve various 
objectives. This module consists of two main parts: 1) a motor drive PCB and 2) an optical 
sensor PCB.

3) FPGA and camera

The rover kit includes a Terasic D8M-GPIO camera to use with the DE10-lite FPGA board for 
mapping the environment. The aliens and their buildings have distinct visual characteristics 
that you can identify and locate in the field of view. A starter project is provided that sets up 
a video processing pipeline in Verilog with a live video output with an overlay for debugging. 
The system includes a NIOS II soft processor that can interface between your custom video 
processing logic and the ESP32 microcontroller for data uplink.

4) Control Subsystem

A powerful 32-bit microcontroller from Espressif is used to control the Mars Rover. The 
microcontroller should communicate with the FPGA, the motor drivers, and doppler sensors.
The ESP32 has Wi-Fi and BLE capabilities that allow the rover to communicate with other 
devices to send the rover status and receive commands. The ESP32 is placed on a shield to 
allow direct connection with the DE10-lite FPGA. 

5) Command Subsystem

The role of the command subsystem is to connect the Mars rover with a webserver to display 
its status and to control the rover remotely, if required, during the exploration phase. A web 
application is required to be developed to connect with the Mars rover. 

6) Radar Subsystem

Our intelligence sources reveal that aliens use subsurface fans to pump air 
underground. Radar can detect moving objects, and use it to locate the blades 
of a rotating fan.

7) Integration Subsystem

The integration subsystem allows the test of the rover's capabilities to move, avoid obstacles, 
and communicate with the command subsystem. Students should design their project in 
order to be able to run test routines, test the rover capabilities to reach a specific target and 
detect any operational issues. 


