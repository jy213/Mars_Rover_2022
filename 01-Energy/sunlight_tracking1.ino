#include <Servo.h>
//definiamo i servomotori orizzontale e verticale
Servo servobelow;
int servob = 0;
int servobLimitHigh = 160;
int servobLimitLow = 60;

Servo servoabove; 
int servoa = 0; 
int servoaLimitHigh = 160;
int servoaLimitLow = 60;
//Pin fotoresistenze
//int ldrtopl = A2; //top left 
//int ldrtopr = A1; //top right 
//int ldrbotl = A3; // bottom left 
//int ldrbotr = A0; // bottom right
int topl, topr, botl, botr , b;
int avgtop, avgbot, avgleft, avgright;
String dataString, test0, test2;

int test1, test3;

 void setup () 
 {
  servobelow.attach(10);
  servobelow.write(60);
  servoabove.attach(9);
  servoabove.write(100);
  Serial.begin(9600);
  delay(500);
 }

void loop()
{
  servob = servobelow.read();
  servoa = servoabove.read();

  //Valore Analogico delle fotoresistenza
  topl = map(analogRead(A2), 22, 800, 0, 100);
  topr = map(analogRead(A1), 18, 777, 0, 100);
  botl = map(analogRead(A3), 20, 795, 0, 100);
  botr = map(analogRead(A0), 26, 701, 0 ,100);
  //dataString = String(topl) + "," + String(topr) + "," + String(botl) + "," + String(botr);
  //Serial.println(dataString);
  // Calcoliamo una Media
  avgtop = (topl + topr) / 2 ; //average of top 
  avgbot = (botl + botr) / 2; //average of bottom 
  avgleft = (topl + botl) / 2 ; //average of left 
  avgright = (topr + botr) / 2; //average of right 
  //dataString = String(avgtop) + "," + String(avgbot); /*+ "," + String(avgleft) + "," + String(avgright);*/
  //Serial.println(dataString);
  
  if ( (avgtop-avgbot) > 5 )
  {
//    if (servob > servobLimitHigh) 
//     { 
//      servob = servobLimitHigh;
//      servobelow.write(servob);
//      String test0 = "exceed upper limit";
//     }
//     else{
      
      b = servob + 1;
      servobelow.write(b);
      test1 = test1 + 1;
//     }
     //delay(100);
  }
  
  else if ( (avgbot-avgtop) > 5 )
  {
    
//      if (servob < servobLimitLow)
//    {
//      servob = servobLimitLow;
//      servobelow.write(servob);
//      String test2 = "exceed lower limit";
//    }
//    else{
      servob = servob - 1;
      
      servobelow.write(servob);
      test3 = test3 + 1;
      
//    }
      //delay(100);
  }
  else
  {
    servobelow.write(servob);
  }

  //Serial.println( servobelow.read() );
  Serial.println((avgtop-avgbot));
  //Serial.println(test0);
  Serial.println(test1);
  //Serial.println(test2);
  Serial.println(test3);
  Serial.println(b);
  Serial.println(servob);
  
  
  Serial.println();

//    for (servob; (avgtop-avgbot) > 5; servob += 1) { // goes from 0 degrees to 180 degrees
//      // in steps of 1 degree
//      servobelow.write(servob);
//      delay(15);                       // waits 15ms for the servo to reach the position
//    }
//    for (servob; (avgbot-avgtop) > 5; servob -=1) { // goes from 0 degrees to 180 degrees
//      // in steps of 1 degree
//      servobelow.write(servob);
//      delay(15);                       // waits 15ms for the servo to reach the position
//    }
//    for (servob; (avgtop-avgbot) < 5; servob) { // goes from 0 degrees to 180 degrees
//    // in steps of 1 degree
//    servobelow.write(servob);
//    delay(15);                       // waits 15ms for the servo to reach the position
//    }
    
    //Serial.println((avgtop-avgbot));
  
//  for (servob = 160; servob >= 60; servob -= 1) { // goes from 180 degrees to 0 degrees
//    servobelow.write(servob);  
//    delay(15);                       // waits 15ms for the servo to reach the position
//  }
  
//  if (avgleft > avgright)
//  {
//    servoabove.write(servoa +1);
//    if (servoa > servoaLimitHigh)
//    {
//    servoa = servoaLimitHigh;
//    }
//    delay(10);
//  }
//  else if (avgright > avgleft)
//  {
//    servoabove.write(servoa -1);
//    if (servoa < servoaLimitLow)
//     {
//     servoa = servoaLimitLow;
//     }
//    delay(10);
//  }
//  else 
//  {
//    servoabove.write(servoa);
//  }
  //delay(100);
}
