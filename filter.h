
#ifndef FILTER_H
#define FILTER_H

//Shoulder X Variables
float x0_SX = 0, x1_SX = 0, x2_SX = 0, x3_SX = 0, x4_SX = 0;
double y0_SX = 0, y1_SX = 0, y2_SX = 0, y3_SX = 0, y4_SX = 0;
float SX_filt;

//Knee X Variables
float x0_KX = 0, x1_KX = 0, x2_KX = 0, x3_KX = 0, x4_KX = 0;
double y0_KX = 0, y1_KX = 0, y2_KX = 0, y3_KX = 0, y4_KX = 0;
float KX_filt;

double Shoulder_X_Filter(float value);
double Knee_X_Filter(float value);

#endif
