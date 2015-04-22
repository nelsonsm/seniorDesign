/*
Main Header File

*/

#include <stdint.h>

#ifndef MAIN_H
#define MAIN_H

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

void init();
void startTimer();

#endif