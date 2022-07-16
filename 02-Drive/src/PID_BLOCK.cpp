#include <Arduino.h>    
#include "PID_BLOCK.h"

void PID_init(PID_TypeDef *PID, float KP, float KI, float KD)
{
    PID->KP = KP;
    PID->KI = KI;
    PID->KD = KD;   
    PID->REFERENCE_VALUE = 0.0;
    PID->CURRENT_VALUE   = 0.0;
    PID->CURRENT_ERROR   = 0.0;
    PID->PRE_ERROR       = 0.0;
    PID->INTEGRAL_VALUE  = 0.0;
    PID->DELTA_DUTY_CYCLE    = 0.0;
    PID->DUTY_CYCLE_LHS     = 0.0;
    PID->DUTY_CYCLE_RHS     = 0.0;
}

float PID_calculate (PID_TypeDef *PID, float bound, float REFERENCE, float MEASURED_DISTANCE, float INTEGRAL_MAX)
{
    PID->REFERENCE_VALUE = REFERENCE;
    PID->CURRENT_VALUE   = MEASURED_DISTANCE;
    PID->CURRENT_ERROR   = PID->REFERENCE_VALUE - PID->CURRENT_VALUE;//0.8
    PID->INTEGRAL_VALUE  = PID->CURRENT_ERROR + PID->PRE_ERROR;
    float P = PID->CURRENT_ERROR;
    float I = PID->INTEGRAL_VALUE;
    float D = PID->CURRENT_ERROR - PID->PRE_ERROR;
    PID->PRE_ERROR = PID->CURRENT_ERROR;
    I = (I < -INTEGRAL_MAX) ? -INTEGRAL_MAX : I;
    I = (I >  INTEGRAL_MAX) ?  INTEGRAL_MAX : I;

    PID->DELTA_DUTY_CYCLE = PID->KP * P + PID->KI *I + PID->KD * D;
    if(PID->DELTA_DUTY_CYCLE > bound){
        PID->DELTA_DUTY_CYCLE = bound;
    }
    else if(PID->DELTA_DUTY_CYCLE < -bound){
        PID->DELTA_DUTY_CYCLE = -bound;
    }
    return PID->DELTA_DUTY_CYCLE;
}   
