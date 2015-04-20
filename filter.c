/*
4th Order Lowpass Butterworth Filter with Cutoff Frequency of 4 Hz
y[n] = 0.000013294 x[n] + 0.000053175 x[n-1] + 0.000079762 x[n-2] + 0.000053175 x[n-3] + 0.000013294 x[n-4] 
		+ 3.6717 y[n-1] - 5.0680 y[n-2] - 3.1160 y[n-3] - 0.7199 y[n-4]
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

float X_in[500];
float Y_in[500];
float Z_in[500];
float X_out[500];
float Y_out[500];
float Z_out[500];

float x0_SX = 0, x1_SX = 0, x2_SX = 0, x3_SX = 0, x4_SX = 0;
double y0_SX = 0, y1_SX = 0, y2_SX = 0, y3_SX = 0, y4_SX = 0;

float x0_SY = 0, x1_SY = 0, x2_SY = 0, x3_SY = 0, x4_SY = 0;
double y0_SY = 0, y1_SY = 0, y2_SY = 0, y3_SY = 0, y4_SY = 0;

float x0_SZ = 0, x1_SZ = 0, x2_SZ = 0, x3_SZ = 0, x4_SZ = 0;
double y0_SZ = 0, y1_SZ = 0, y2_SZ = 0, y3_SZ = 0, y4_SZ = 0;

double filt_SX, filt_SY, filt_SZ;

double Shoulder_X_Filter(float value);
double Shoulder_Y_Filter(float value);
double Shoulder_Z_Filter(float value);
double Hip_X_Filter(float value);
double Hip_Y_Filter(float value);
double Hip_Z_Filter(float value);
double Knee_X_Filter(float value);
double Knee_Y_Filter(float value);
double Knee_Z_Filter(float value);

int main(void)
{
	
	int i = 0;
	
	FILE *fp;
	fp = fopen("accel1_test1.txt", "r");

	// test for files not existing. 
    if (fp== NULL) 
    {   
		printf("Error! Could not open file\n"); 
		exit(0);
    }
	for(i=0; i<500; i++){
		fscanf(fp,"%f",&X_in[i]);
		filt_SX = Shoulder_X_Filter(X_in[i]);
		printf("%.4f ",filt_SX);

		fscanf(fp,"%f",&Y_in[i]);
		filt_SY = Shoulder_Y_Filter(Y_in[i]);
		printf("%.4f ",filt_SY);

		fscanf(fp,"%f",&Z_in[i]);
		filt_SZ = Shoulder_Z_Filter(Z_in[i]);
		printf("%.4f \n",filt_SZ);

		i++;
	}

	
	fclose(fp);
	
	return 0;
}


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

double Shoulder_Y_Filter(float value){
	y4_SY = y3_SY;
	y3_SY = y2_SY;
	y2_SY = y1_SY;
	y1_SY = y0_SY;
	x4_SY = x3_SY;
	x3_SY = x2_SY;
	x2_SY = x1_SY;
	x1_SY = x0_SY;
	x0_SY = value;

	y0_SY = 0.000013294 * x0_SY + 0.000053175 * x1_SY + 0.000079762 * x2_SY + 0.000053175 * x3_SY + 0.000013294 * x4_SY 
		+ 3.6717 * y1_SY - 5.0680 * y2_SY + 3.1160 * y3_SY - 0.7199 * y4_SY;
	return y0_SY;
}

double Shoulder_Z_Filter(float value){
	y4_SZ = y3_SZ;
	y3_SZ = y2_SZ;
	y2_SZ = y1_SZ;
	y1_SZ = y0_SZ;
	x4_SZ = x3_SZ;
	x3_SZ = x2_SZ;
	x2_SZ = x1_SZ;
	x1_SZ = x0_SZ;
	x0_SZ = value;

	y0_SZ = 0.000013294 * x0_SZ + 0.000053175 * x1_SZ + 0.000079762 * x2_SZ + 0.000053175 * x3_SZ + 0.000013294 * x4_SZ 
		+ 3.6717 * y1_SZ - 5.0680 * y2_SZ + 3.1160 * y3_SZ - 0.7199 * y4_SZ;
	return y0_SZ;
}

double Hip_X_Filter(float value){

}

double Hip_Y_Filter(float value){

}

double Hip_Z_Filter(float value){

}

double Knee_X_Filter(float value){

}

double Knee_Y_Filter(float value){

}

double Knee_Z_Filter(float value){

}