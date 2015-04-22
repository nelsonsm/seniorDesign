/*
Test Program 
Last updated: 3/19/15

Setup:
3.3 power 
P9
Pin 17 & 18 I2C2 (0x1D)
Pin 19 & 20 I2C1 (0x1C and 0x1D)
No resistors needed
*/

/*
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

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

float X1;
float Y1;
float Z1;

float X2;
float Y2;
float Z2;

float X3;
float Y3;
float Z3;

void MMA8452Standby1();
void MMA8452Active1();
uint8_t readRegister1(char address);
void writeRegister1(char address, char value);

void MMA8452Standby2();
void MMA8452Active2();
uint8_t readRegister2(char address);
void writeRegister2(char address, char value);

void MMA8452Standby3();
void MMA8452Active3();
uint8_t readRegister3(char address);
void writeRegister3(char address, char value);

void readA1();
void readA2();
void readA3();

int main()
{

// Must be in standby to change registers
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

  
//Read from all three accelerometers


    FILE *fp1;
    FILE *fp2;
    FILE *fp3;

    fp1 = fopen("acc1_test2.txt", "w");
    fp2 = fopen("acc2_test2.txt", "w");
    fp3 = fopen("acc3_test2.txt", "w");

    while(1){
        readA1();
        //Pin1920 0x1D:
        fprintf(fp1, "%.3f %.3f %.3f\n", X1, Y1, Z1);
    
        readA2();
        //Pin1920 0x1C:
        fprintf(fp2, "%.3f %.3f %.3f\n", X2, Y2, Z2);
    
        readA3();
        //Pin1718 0x1D:
        fprintf(fp3, "%.3f %.3f %.3f\n", X3, Y3, Z3);
        
        usleep(5000);
    }
    
    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
 
    exit(1);

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

*/

