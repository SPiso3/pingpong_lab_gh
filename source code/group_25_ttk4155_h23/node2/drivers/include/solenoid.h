#ifndef SOLENOID_H_
#define SOLENOID_H_

#include "sam3x8e.h"

void solenoid_init();
void solenoid_push();
void solenoid_release();

#endif /* SOLENOID_H_ */