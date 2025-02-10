/*
 *Serial Reader for the ROCK4C+
 *Author: Ivar Tylén, ivatyl-0@student.ltu.se
 *For use on the ROCK4C+ on the RAVEN Chapter 2 Test Bench.
 *Source code used for the main script, found in serial_reader.c.
 *Purpose: Defines functions used in main. 
 *Last modified: 2025-02-07.
 */

/*C Standard Libraries*/
#include <stdio.h>
#include "serial_reader.h"

/*LINUX Standard Libraries*/
#include <fcntl.h> //Contains file controls like O_RDWR
#include <errno.h> //Error integer and strerror() function
#include <termios.h> //POSIX terminal control definitions
#include <unistd.h> //write(), read(), close()


#ifndef serial_reader_src.c
#define serial_reader_src.c

            /*Function Definitions*/

/*Pressure Sensor (RS:797-5005) reader function. Converts from raw data to bar.*/
float readPressure(int sensorValue, int sensorNum)
{
    pressureVoltage = calibrationADC * refADC * (sensorValue/maxADC);
    return pressureCalibration_K[sensorNum] * pressureVoltage + pressureCalibration_B[sensorNum];
}

/*Load cell reader function. Converts from raw data to Newtons.*/
float readLoad(int sensorValue);
{
    /*TBD, new load cell.*/
}

/*TMP36 (Heating Blanket temp-sensors) reader function, from raw data to temperature in deg C.*/
float readTMP36(int sensorValue);
{
   return temperature = ((sensorValue / maxADC) * refADC - 0.5) * 100;
}

/*Thermocouple reader function, takes something gives something back*/
float readTemp(int sensorValue);
{
    return sensorValue * 0.25;
}

/*IR sensor reader function, from raw data to temperature in deg C*/
float readIR(int sensorValue);
{
    return temperature = (sensorValue / maxADC) * (maxIR - minIR) + minIR;
}


#endif