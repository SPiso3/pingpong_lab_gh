#include "../include/PID.h"

#define kp 2.5 //1,5
#define ki 0 //0,005
#define kd 0.25 //0,15

int32_t e_past = 0;
int32_t e_accum = 0;

int32_t pid(int32_t r, int32_t y){
	int64_t u;
	int32_t e = r-y;
	int32_t e_dot = (e - e_past) / 0.016;
	int16_t sf = (e>0) ? 450 : -450;
	u = kp*e + kd*e_dot + ki*e_accum + sf;
	
	e_past = e;
	e_accum += e;
	//if (e_accum >= 1000)
	//	e_accum = +1000;
	//else if (e_accum <= -1000)
	//	e_accum = -1000;
	
	return (int32_t)u;
}