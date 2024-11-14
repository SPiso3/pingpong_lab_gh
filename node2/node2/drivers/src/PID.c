#include "../include/PID.h"
#include "../include/motor.h"

#define kp 4  //1,5   | 2.5 - 4
#define ki 0.0  //0,005 | 0.0 - 0
#define kd 20 //0,15  | 0.25 - 20
#define sf 450
#define antiwindup_val 1000

int64_t e_past = 0;
int64_t e_accum = 0;

int32_t pid(int32_t r, int32_t y){
	int64_t u;
	int32_t e = r-y;
	int32_t e_dot = (e - e_past);
	int16_t sf_term = (e>0) ? sf : -sf;
	u = kp*e + kd*e_dot + ki*e_accum + sf_term;
	
	//update values
	e_past = e;
	//e_accum += e;
	
	//anti-windup
	//if (e_accum >= antiwindup_val)
		//e_accum = +antiwindup_val;
	//else if (e_accum <= -antiwindup_val)
		//e_accum = -antiwindup_val;
	
	//saturation to upper pwm limit
	if (u >= CPRD_VALUE_MOTOR)
		u = +CPRD_VALUE_MOTOR;
	else if (u <= -CPRD_VALUE_MOTOR)
		u = -CPRD_VALUE_MOTOR;
	
	return (int32_t)u;
}