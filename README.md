# Mars_Rover_2022
Year 2 Mars Rover Project includes 7 team members and consists of 6 modules
- Command Module：Front-end web pages designed using TypeScript and React frameworks, as well as back-end databases in the Node.js environment, are used to store and display the real-time location of detected objects, and to issue manual control commands
- Vision Module：programmed FPGA board in Verilog. The input RGB pixels from the camera were converted to HSV and noise were filtered to find the target edge border. The distance to the center point of the target was calculated, and finally the result was encoded and sent to the ESP32 chip based on the URAT communication
- Control Module：Used the Arduino IDE to program the ESP32 chip to complete the data transmission between modules. Used graph theory and breadth-first algorithm in C++ to differentiate the map into a matrix and independently modify the shortest route and convert it into a motor instruction to establish autonomous obstacle avoidance and route planning.


