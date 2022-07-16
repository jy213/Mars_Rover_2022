#ifndef PID_BLOCK_H
#define PID_BLOCK_H

typedef struct 
{
    float KP;
    float KI;
    float KD;
    float REFERENCE_VALUE;
    float CURRENT_VALUE;
    float CURRENT_ERROR;
    float PRE_ERROR;
    float INTEGRAL_VALUE;
    float DELTA_DUTY_CYCLE;
    float DUTY_CYCLE_LHS;
    float DUTY_CYCLE_RHS;

} PID_TypeDef;

void PID_init(PID_TypeDef *PID, float KP, float KI, float KD);
float PID_calculate (PID_TypeDef *PID, float bound, float REFERENCE, float MEASURED_DISTANCE, float INTEGRAL_MAX);


#endif