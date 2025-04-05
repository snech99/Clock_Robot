/*
*   Gerrit Hinrichs 06.2024
*   github.com/snech99
*
*   Clock_Robot
*   header for the kinetics
*/
#ifndef KINEMATIK_H_
#define KINEMATIK_H_

#include "Robotik.h"

#define STEP_FACTOR ((8*180)/(M_PI*1.8))
#define P_BEGIN_X 20
#define P_BEGIN_Y 108
#define P_BEGIN_Z 678

void inverse (float, float, float, float *);

#endif
