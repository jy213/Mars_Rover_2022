#include <vector>

#ifndef ADNS3080_H
#define ADNS3080_H

#include <Arduino.h>
#include "SPI.h"  
#include <vector>

#define ADNS3080_PIXELS_X                 30
#define ADNS3080_PIXELS_Y                 30
  
#define ADNS3080_PRODUCT_ID            0x00
#define ADNS3080_REVISION_ID           0x01
#define ADNS3080_MOTION                0x02
#define ADNS3080_DELTA_X               0x03
#define ADNS3080_DELTA_Y               0x04
#define ADNS3080_SQUAL                 0x05
#define ADNS3080_PIXEL_SUM             0x06
#define ADNS3080_MAXIMUM_PIXEL         0x07
#define ADNS3080_CONFIGURATION_BITS    0x0a
#define ADNS3080_EXTENDED_CONFIG       0x0b
#define ADNS3080_DATA_OUT_LOWER        0x0c
#define ADNS3080_DATA_OUT_UPPER        0x0d
#define ADNS3080_SHUTTER_LOWER         0x0e
#define ADNS3080_SHUTTER_UPPER         0x0f
#define ADNS3080_FRAME_PERIOD_LOWER    0x10
#define ADNS3080_FRAME_PERIOD_UPPER    0x11
#define ADNS3080_MOTION_CLEAR          0x12
#define ADNS3080_FRAME_CAPTURE         0x13
#define ADNS3080_SROM_ENABLE           0x14
#define ADNS3080_FRAME_PERIOD_MAX_BOUND_LOWER      0x19
#define ADNS3080_FRAME_PERIOD_MAX_BOUND_UPPER      0x1a
#define ADNS3080_FRAME_PERIOD_MIN_BOUND_LOWER      0x1b
#define ADNS3080_FRAME_PERIOD_MIN_BOUND_UPPER      0x1c
#define ADNS3080_SHUTTER_MAX_BOUND_LOWER           0x1e
#define ADNS3080_SHUTTER_MAX_BOUND_UPPER           0x1e
#define ADNS3080_SROM_ID               0x1f
#define ADNS3080_OBSERVATION           0x3d
#define ADNS3080_INVERSE_PRODUCT_ID    0x3f
#define ADNS3080_PIXEL_BURST           0x40
#define ADNS3080_MOTION_BURST          0x50
#define ADNS3080_SROM_LOAD             0x60

#define ADNS3080_PRODUCT_ID_VAL        0x17


struct MD
{
 byte motion;
 char dx, dy;
 byte squal;
 word shutter;
 byte max_pix;
};

int convTwosComp(int BINARY);
void mousecam_reset(int PIN);
int mousecam_init(int PIN_RESET, int PIN_CS);
void mousecam_write_reg(int PIN_CS, int REGISTER, int VALUE);
int mousecam_read_reg(int PIN_CS, int REGISTER);
void mousecam_read_motion(int PIN_CS, struct MD *POSITION);
int mousecam_frame_capture(int PIN_CS, byte *P_DATA);
void ADNS3080_INIT_CHECK(int PIN_CS, int PIN_RESET);
void ADNS3080_INIT(int PINSS, int PINMISO, int PINMOSI, int PINSCK);
char asciiart(int K);
void ADNS3080_RESOLUTION_CHECK(int PIN_CS, MD X);
std::vector<float> ADNS3080_DISTANCE (float TOTAL_X, float TOTAL_Y, int PIN_CS, MD X);


#endif 