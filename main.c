/*
Main Program

Setup:
3.3 power 
P9
Pin 17 & 18 I2C2 (0x1D)
Pin 19 & 20 I2C1 (0x1C and 0x1D)
No resistors needed
*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include "filter.h"

// The SparkFun breakout board defaults to 1, set to 0 if SA0 jumper on the bottom of the board is set
#define MMA8452_ADDRESS 0x1D  // 0x1D if SA0 is high, 0x1C if low
#define MMA8452_ADDRESS2 0x1C

//Define a few of the registers that we will be accessing on the MMA8452
#define OUT_X_MSB 0x01
#define OUT_Y_MSB 0x03
#define OUT_Z_MSB 0x05
#define XYZ_DATA_CFG  0x0E
#define WHO_AM_I   0x0D
#define CTRL_REG1  0x2A

#define GSCALE 2 // Sets full-scale range to +/-2, 4, or 8g. Used to calc real g values.

//Variables for stroke rate
FILE *fp;
float prev;
float current;
double timer;
double timerBetweenMax;
double strokeRate;
int close;
int i;
int start;
int maxCounter;
int dataPoints;
int foundMax;

//Constants
static double THRESHOLD = 0.03;
static double MIN_TIME_BETWEEN_STROKES = 0.5;
static double TIMER_INTERVAL = 0.05;

//Variables for returning read values
//Shoulder
float X1;
float Y1;
float Z1;
//Hip
float X2;
float Y2;
float Z2;
//Knee
float X3;
float Y3;
float Z3;

double resultSX;
double resultKX:

//Used for interrupt
int waiting;

//Used for user interface
int stopped;

void timer_handler (int signum)
{
 //Get, filter, display Shoulder X
 readA1();
 resultSX = Shoulder_X_Filter(X1);
 //display result

 //Get and filter Knee X for stroke rate
 readA3();
 resultKX = Knee_X_Filter(X3);
 waiting = 0;

 //Check if user stopped program

}

int main(void){

	//initialize variables
	close = 0;
	i = 0;
	start = 0;
	maxCounter = 0;
	dataPoints = 0;
	foundMax = 0;
	prev = 0;
	current = 0;
	timer = 0;
	timerBetweenMax = 0;
	stopped = 1;

	init(); //Initialize accelerometers

	//Wait for start
	printf("Press any key to start");
	scanf("%d", &close);
	while(stopped); //block until start

	startTimer(); //Start Timer

	//find first max
	while(start == 0){
		prev = current;
		fscanf(fp,"%f", &current);
		if(prev > THRESHOLD){
			if(current < prev){
				start = 1;
				printf("Max:%.4f\n",prev);
			}
		}	
	}
	
	while(!stopped){
		while(maxCounter < 5){
			prev = current;
			while(waiting); //wait for interrupt
			current = resultKX;
			waiting = 1;
			dataPoints++;
			timer += TIMER_INTERVAL;
			timerBetweenMax += TIMER_INTERVAL;
			if(foundMax == 0){
				if(prev > THRESHOLD){
					if(current < prev){
						if(timerBetweenMax > MIN_TIME_BETWEEN_STROKES){
							maxCounter++;
							foundMax = 1;
							printf("Max:%.4f Time:%.2f\n",prev, timerBetweenMax);
							timerBetweenMax = 0;
						}
					}
				}	
			}
			if(prev < THRESHOLD){
				foundMax = 0;
			}
		}

		strokeRate = (5 / timer) * 60;
		//print to display
		printf("Stroke Rate:%.2f strokes/min Points:%d\n",strokeRate, dataPoints);
		dataPoints = 0;
		timer = 0;
		maxCounter = 0;
	}
}

void init(){
  MMA8452Standby1();  
  MMA8452Standby2(); 
  MMA8452Standby3(); 
  
  
  // Set up the full scale range to 2, 4, or 8g.
  uint8_t fsr = GSCALE;
  if(fsr > 8) fsr = 8; //Easy error check
  fsr >>= 2; // Neat trick. 00 = 2G, 01 = 4A, 10 = 8G
  
  //Set GSCALE and ODR to 200Hz
  writeRegister1(XYZ_DATA_CFG, fsr);
  writeRegister1(CTRL_REG1, 0b00010000);
  
  writeRegister2(XYZ_DATA_CFG, fsr);
  writeRegister2(CTRL_REG1, 0b00010000);
  
  writeRegister3(XYZ_DATA_CFG, fsr);
  writeRegister3(CTRL_REG1, 0b00010000);


 // Set to active to start reading
  MMA8452Active1(); 
  MMA8452Active2();
  MMA8452Active3();

}


void startTimer(){

 struct sigaction sa;
 struct itimerval timer;

 /* Install timer_handler as the signal handler for SIGVTALRM. */
 memset (&sa, 0, sizeof (sa));
 sa.sa_handler = &timer_handler;
 sigaction (SIGVTALRM, &sa, NULL);

 /* Configure the timer to expire after 50 msec... */
 timer.it_value.tv_sec = 0;
 timer.it_value.tv_usec = 50000;
 /* ... and every 50 msec after that. */
 timer.it_interval.tv_sec = 0;
 timer.it_interval.tv_usec = 50000;
 /* Start a virtual timer. It counts down whenever this process is
   executing. */
 setitimer (ITIMER_VIRTUAL, &timer, NULL);

}



//ACCELEROMTER 1


// Sets the MMA8452 to standby mode. It must be in standby to change most register settings
void MMA8452Standby1()
{
  uint8_t c = readRegister1(CTRL_REG1);
  writeRegister1(CTRL_REG1, c & ~(0x01)); //Clear the active bit to go into standby
}

// Sets the MMA8452 to active mode. Needs to be in this mode to output data
void MMA8452Active1()
{
  uint8_t c = readRegister1(CTRL_REG1);
  writeRegister1(CTRL_REG1, c | 0x01);
}

// Read a single byte from addressToRead and return it as a byte
uint8_t readRegister1(char address)
{
     //  cout << "Starting BMA180 I2C sensor state byte read" << endl;
    char namebuf[64];
   	snprintf(namebuf, sizeof(namebuf), "/dev/i2c-1");
    int file;
    if ((file = open(namebuf, O_RDWR)) < 0){
            printf("Failed to open\n");
    }
    if (ioctl(file, I2C_SLAVE, MMA8452_ADDRESS) < 0){
            printf("I2C_SLAVE address failed \n");
    }

    char buffer[20] = {0};
    if ( read(file, buffer, 20) != 20) {
        printf("Failure to read value from I2C Device address.\n");
    }
    uint8_t data = (int)buffer[address];
    close(file);
    return data;
}

// Writes a single byte (dataToWrite) into addressToWrite
void writeRegister1(char address, char value)
{
    char namebuf[64];
   	snprintf(namebuf, sizeof(namebuf), "/dev/i2c-1");
    int file;
    if ((file = open(namebuf, O_RDWR)) < 0){
            printf("Failed to open\n");
    }
   
    if (ioctl(file, I2C_SLAVE, MMA8452_ADDRESS) < 0){
            printf("I2C_SLAVE address failed \n");
    }

    char buffer[2];
    	buffer[0] = address;
    	buffer[1] = value;
    if ( write(file, buffer, 2) != 2) {
        printf("Failure to write values to I2C Device address.\n");
    }
    close(file);
}

//ACCELEROMETER 2

// Sets the MMA8452 to standby mode. It must be in standby to change most register settings
void MMA8452Standby2()
{
  uint8_t c = readRegister2(CTRL_REG1);
  writeRegister2(CTRL_REG1, c & ~(0x01)); //Clear the active bit to go into standby
}

// Sets the MMA8452 to active mode. Needs to be in this mode to output data
void MMA8452Active2()
{
  uint8_t c = readRegister2(CTRL_REG1);
  writeRegister2(CTRL_REG1, c | 0x01);
}


// Read a single byte from addressToRead and return it as a byte
uint8_t readRegister2(char address)
{
     //  cout << "Starting BMA180 I2C sensor state byte read" << endl;
    char namebuf[64];
   	snprintf(namebuf, sizeof(namebuf), "/dev/i2c-1");
    int file;
    if ((file = open(namebuf, O_RDWR)) < 0){
            printf("Failed to open\n");
    }
    if (ioctl(file, I2C_SLAVE, MMA8452_ADDRESS2) < 0){
            printf("I2C_SLAVE address failed \n");
    }

    char buffer[20] = {0};
    if ( read(file, buffer, 20) != 20) {
        printf("Failure to read value from I2C Device address.\n");
    }
    uint8_t data = (int)buffer[address];
    close(file);
    return data;
}

// Writes a single byte (dataToWrite) into addressToWrite
void writeRegister2(char address, char value)
{
    char namebuf[64];
   	snprintf(namebuf, sizeof(namebuf), "/dev/i2c-1");
    int file;
    if ((file = open(namebuf, O_RDWR)) < 0){
            printf("Failed to open\n");
    }
   
    if (ioctl(file, I2C_SLAVE, MMA8452_ADDRESS2) < 0){
            printf("I2C_SLAVE address failed \n");
    }

    char buffer[2];
    	buffer[0] = address;
    	buffer[1] = value;
    if ( write(file, buffer, 2) != 2) {
        printf("Failure to write values to I2C Device address.\n");
    }
    close(file);
}


//ACCELEROMETER 3

// Sets the MMA8452 to standby mode. It must be in standby to change most register settings
void MMA8452Standby3()
{
  uint8_t c = readRegister3(CTRL_REG1);
  writeRegister3(CTRL_REG1, c & ~(0x01)); //Clear the active bit to go into standby
}

// Sets the MMA8452 to active mode. Needs to be in this mode to output data
void MMA8452Active3()
{
  uint8_t c = readRegister3(CTRL_REG1);
  writeRegister3(CTRL_REG1, c | 0x01);
}


// Read a single byte from addressToRead and return it as a byte
uint8_t readRegister3(char address)
{
     //  cout << "Starting BMA180 I2C sensor state byte read" << endl;
    char namebuf[64];
   	snprintf(namebuf, sizeof(namebuf), "/dev/i2c-2");
    int file;
    if ((file = open(namebuf, O_RDWR)) < 0){
            printf("Failed to open\n");
    }
    if (ioctl(file, I2C_SLAVE, MMA8452_ADDRESS) < 0){
            printf("I2C_SLAVE address failed \n");
    }

    char buffer[20] = {0};
    if ( read(file, buffer, 20) != 20) {
        printf("Failure to read value from I2C Device address.\n");
    }
    uint8_t data = (int)buffer[address];
    close(file);
    return data;
}

// Writes a single byte (dataToWrite) into addressToWrite
void writeRegister3(char address, char value)
{
    char namebuf[64];
   	snprintf(namebuf, sizeof(namebuf), "/dev/i2c-2");
    int file;
    if ((file = open(namebuf, O_RDWR)) < 0){
            printf("Failed to open\n");
    }
   
    if (ioctl(file, I2C_SLAVE, MMA8452_ADDRESS) < 0){
            printf("I2C_SLAVE address failed \n");
    }

    char buffer[2];
    	buffer[0] = address;
    	buffer[1] = value;
    if ( write(file, buffer, 2) != 2) {
        printf("Failure to write values to I2C Device address.\n");
    }
    close(file);
}


void readA1(){
    char namebuf[64];
   	snprintf(namebuf, sizeof(namebuf), "/dev/i2c-1");
    int file;
    if ((file = open(namebuf, O_RDWR)) < 0){
            printf("Failed to open\n");
    }
    if (ioctl(file, I2C_SLAVE, MMA8452_ADDRESS) < 0){
            printf("I2C_SLAVE address failed \n");
    }

    char buffer[8] = {0};
    if ( read(file, buffer, 8) != 8) {
        printf("Failure to read value from I2C Device address.\n");
    }
    
    int8_t X_data = (int)buffer[OUT_X_MSB];
    X1 = (float)X_data / 64;
    
    int8_t Y_data = (int)buffer[OUT_Y_MSB];
    Y1 = (float)Y_data / 64;
    
    int8_t Z_data = (int)buffer[OUT_Z_MSB];
    Z1 = (float)Z_data / 64;
    
    close(file);
}

void readA2(){
    char namebuf[64];
   	snprintf(namebuf, sizeof(namebuf), "/dev/i2c-1");
    int file;
    if ((file = open(namebuf, O_RDWR)) < 0){
            printf("Failed to open\n");
    }
    if (ioctl(file, I2C_SLAVE, MMA8452_ADDRESS2) < 0){
            printf("I2C_SLAVE address failed \n");
    }

    char buffer[8] = {0};
    if ( read(file, buffer, 8) != 8) {
        printf("Failure to read value from I2C Device address.\n");
    }

    int8_t X_data = (int)buffer[OUT_X_MSB];
    X2 = (float)X_data / 64;
    
    int8_t Y_data = (int)buffer[OUT_Y_MSB];
    Y2 = (float)Y_data / 64;
    
    int8_t Z_data = (int)buffer[OUT_Z_MSB];
    Z2 = (float)Z_data / 64;
    
    close(file);
    
}

void readA3(){
    char namebuf[64];
   	snprintf(namebuf, sizeof(namebuf), "/dev/i2c-2");
    int file;
    if ((file = open(namebuf, O_RDWR)) < 0){
            printf("Failed to open\n");
    }
    if (ioctl(file, I2C_SLAVE, MMA8452_ADDRESS) < 0){
            printf("I2C_SLAVE address failed \n");
    }

    char buffer[8] = {0};
    if ( read(file, buffer, 8) != 8) {
        printf("Failure to read value from I2C Device address.\n");
    }

    int8_t X_data = (int)buffer[OUT_X_MSB];
    X3 = (float)X_data / 64;
    
    int8_t Y_data = (int)buffer[OUT_Y_MSB];
    Y3 = (float)Y_data / 64;
    
    int8_t Z_data = (int)buffer[OUT_Z_MSB];
    Z3 = (float)Z_data / 64;
    
    close(file);
    
    
}
