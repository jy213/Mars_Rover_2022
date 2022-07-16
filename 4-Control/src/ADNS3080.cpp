#include <Arduino.h>
#include "SPI.h"  
#include "ADNS3080.h"
#include <vector>

int convTwosComp(int BINARY){
  //Convert from 2's complement
  if(BINARY & 0x80){
    BINARY = -1 * ((BINARY ^ 0xff) + 1);
    }
  return BINARY;
  }


void mousecam_reset(int PIN)
{
  digitalWrite(PIN,HIGH);
  delay(1); // reset pulse >10us
  digitalWrite(PIN,LOW);
  delay(35); // 35ms from reset to functional
}

int mousecam_init(int PIN_RESET, int PIN_CS)
{
  pinMode(PIN_RESET,OUTPUT);
  pinMode(PIN_CS,OUTPUT);

  digitalWrite(PIN_CS,HIGH);

  mousecam_reset(PIN_RESET);

  return 1;
}

void mousecam_write_reg(int PIN_CS, int REGISTER, int VALUE)
{
  digitalWrite(PIN_CS, LOW);
  SPI.transfer(REGISTER | 0x80);
  SPI.transfer(VALUE);
  digitalWrite(PIN_CS,HIGH);
  delayMicroseconds(50);
}

int mousecam_read_reg(int PIN_CS, int REGISTER)
{
  digitalWrite(PIN_CS, LOW);
  SPI.transfer(REGISTER);
  delayMicroseconds(75);
  int ret = SPI.transfer(0xff);
  digitalWrite(PIN_CS,HIGH);
  delayMicroseconds(1);
  return ret;
}

void mousecam_read_motion(int PIN_CS, MD *POSITION)
{
  digitalWrite(PIN_CS, LOW);
  SPI.transfer(ADNS3080_MOTION_BURST);
  delayMicroseconds(75);
  POSITION->motion =  SPI.transfer(0xff);
  POSITION->dx =  SPI.transfer(0xff);
  POSITION->dy =  SPI.transfer(0xff);
  POSITION->squal =  SPI.transfer(0xff);
  POSITION->shutter =  SPI.transfer(0xff)<<8;
  POSITION->shutter |=  SPI.transfer(0xff);
  POSITION->max_pix =  SPI.transfer(0xff);
  digitalWrite(PIN_CS,HIGH);
  delayMicroseconds(5);
}

int mousecam_frame_capture(int PIN_CS, byte *P_DATA)
{
  mousecam_write_reg(PIN_CS, ADNS3080_FRAME_CAPTURE,0x83);

  digitalWrite(PIN_CS, LOW);

  SPI.transfer(ADNS3080_PIXEL_BURST);
  delayMicroseconds(50);

  int pix;
  byte started = 0;
  int count;
  int timeout = 0;
  int ret = 0;
  for(count = 0; count < ADNS3080_PIXELS_X * ADNS3080_PIXELS_Y; )
  {
    pix = SPI.transfer(0xff);
    delayMicroseconds(10);
    if(started==0)
    {
      if(pix&0x40)
        started = 1;
      else
      {
        timeout++;
        if(timeout==100)
        {
          ret = -1;
          break;
        }
      }
    }
    if(started==1)
    {
      P_DATA[count++] = (pix & 0x3f)<<2; // scale to normal grayscale byte range
    }
  }

  digitalWrite(PIN_CS,HIGH);
  delayMicroseconds(14);
  
  return ret;
}

void ADNS3080_INIT(int PINSS, int PINMISO, int PINMOSI, int PINSCK)
{
  pinMode(PINSS,OUTPUT);
  pinMode(PINMISO,INPUT);
  pinMode(PINMOSI,OUTPUT);
  pinMode(PINSCK,OUTPUT);

  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV32);
  SPI.setDataMode(SPI_MODE3);
  SPI.setBitOrder(MSBFIRST);
}

void ADNS3080_INIT_CHECK(int PIN_CS, int PIN_RESET)
{
    if(mousecam_init(PIN_RESET, PIN_CS)==-1)
  {
    Serial.println("Mouse cam failed to init");
    while(1);
  }
}

char asciiart(int K)
{
  static char foo[] = "WX86*3I>!;~:,`. ";
  return foo[K>>4];
}

void ADNS3080_RESOLUTION_CHECK(int PIN_CS, MD X)
{
  int val = mousecam_read_reg(PIN_CS, ADNS3080_PIXEL_SUM);
  mousecam_read_motion(PIN_CS, &X);
  Serial.print(String(X.squal/4));
  for(int i=0; i<X.squal/4; i++)
  Serial.print('*');
  Serial.print(' ');
  Serial.print((val*100)/351);
  Serial.print(' ');
  Serial.print(X.shutter); Serial.print(" (");
  Serial.print((int)X.dx); Serial.print(',');
  Serial.print((int)X.dy); Serial.println(')');

  // Serial.println(md.max_pix);
 
}

std::vector<float> ADNS3080_DISTANCE (float TOTAL_X, float TOTAL_Y, int PIN_CS, MD X)
{
    mousecam_read_motion(PIN_CS, &X);
    float DISTANCE_X = convTwosComp(X.dx);
    float DISTANCE_Y = convTwosComp(X.dy);
    TOTAL_X = TOTAL_X + DISTANCE_X;
    TOTAL_Y = TOTAL_Y + DISTANCE_Y;
    //DISTANCE_X=0.08 -> 1cm
    TOTAL_X = TOTAL_X/157*58;
    TOTAL_Y = TOTAL_Y/157*58;
    //float *D   = new float[4]{final_x, final_y, DISTANCE_X, DISTANCE_Y};
    std::vector<float> D ={TOTAL_X, TOTAL_Y, DISTANCE_X, DISTANCE_Y};
    return D;
}