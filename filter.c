/*
4th Order Lowpass Butterworth Filter with Cutoff Frequency of 4 Hz
y[n] = 0.000013294 x[n] + 0.000053175 x[n-1] + 0.000079762 x[n-2] + 0.000053175 x[n-3] + 0.000013294 x[n-4] 
		+ 3.6717 y[n-1] - 5.0680 y[n-2] - 3.1160 y[n-3] - 0.7199 y[n-4]
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "filter.h"

double Shoulder_X_Filter(float value){
	y4_SX = y3_SX;
	y3_SX = y2_SX;
	y2_SX = y1_SX;
	y1_SX = y0_SX;
	x4_SX = x3_SX;
	x3_SX = x2_SX;
	x2_SX = x1_SX;
	x1_SX = x0_SX;
	x0_SX = value;

	y0_SX = 0.000013294 * x0_SX + 0.000053175 * x1_SX + 0.000079762 * x2_SX + 0.000053175 * x3_SX + 0.000013294 * x4_SX 
		+ 3.6717 * y1_SX - 5.0680 * y2_SX + 3.1160 * y3_SX - 0.7199 * y4_SX;
	return y0_SX;
}

double Knee_X_Filter(float value){
	y4_KX = y3_KX;
	y3_KX = y2_KX;
	y2_KX = y1_KX;
	y1_KX = y0_KX;
	x4_KX = x3_KX;
	x3_KX = x2_KX;
	x2_KX = x1_KX;
	x1_KX = x0_KX;
	x0_KX = value;

	y0_KX = 0.000013294 * x0_KX + 0.000053175 * x1_KX + 0.000079762 * x2_KX + 0.000053175 * x3_KX + 0.000013294 * x4_KX 
		+ 3.6717 * y1_KX - 5.0680 * y2_KX + 3.1160 * y3_KX - 0.7199 * y4_KX;
	return y0_KX;

}

