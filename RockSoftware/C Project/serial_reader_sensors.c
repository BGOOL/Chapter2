
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

#ifndef serial_reader_src.c
#define serial_reader_src.c

/*Variable Definitions*/
/*Misc may be moved*/
float pressureVoltage = 0;
float loadVoltage = 0; // Used in readLoad function.
float temperature = 0; // Used in readTMP36, readTemp, readIR -||-

/*Pressure Sensor "Positions". Used for --*/
int const FEEDING_PRESSURE_OXIDIZER = 0;
int const LINE_PRESSURE = 1;
int const CHAMBER_PRESSURE = 2;
int const FEEDING_PRESSURE_N2 = 3;
int const FEEDING_PRESSURE_OXIDIZER2 = 4;

/*Thermocouple "Locations"*/
int const Not_Connected_0 = 0;
int const Not_Connected_1 = 1;
int const Nozzle_TC = 2;
int const Ambient_TC = 3;


/*ADC Resolution*/
int const resolutionADC = 10;

/*Max ADC Value*/
float const maxADC = 1023.00;

/*ADC Reference Voltage*/
 float const refADC;

/*IR Sensor Min and Max values*/
int const minIR = -50;
int const maxIR = 1030;

/*TMP 36 Min and Max values*/
int const minTMP36 = -40;
int const maxTMP36 = 125;

/*
 * Load Cell Min and Max values
 * TBD For new Load Cell
 */
int const minLoad = 0;
float const maxLoad = 250 * 4.44822; // Conversion from lbf to Newtons

/*Load Cell Calibration, TBD new sensor*/

/*Pressure Sensor Calibration Data*/

/*Pressure Sensor Maximum Pressures*/
float const maxPressure_100Bar = 100.0;
float const maxPressure_25Bar = 25.0;

/*
 *Pressure Sensor 0 (Serial Number 667662)
 *Oxidizer Feeding 1 Pressure Sensor
 */
float const pressureZero0 = -0.003;       // Voltage
float const pressureSpan0 = 5.003;        // Voltage
float const pressureLinearity0 = 0.12493; // Percent. Not used for Calibration.
float const pressureLine_K0 = maxPressure_100Bar / pressureSpan0;
float const pressureLine_B0 = maxPressure_100Bar - pressureLine_K0 * (pressureSpan0 + pressureZero0);

/*
 *Pressure Sensor 1 (Serial Number 1073014)
 *Line Pressure Sensor
 */
float const pressureZero1 = 0.01;        // Voltage
float const pressureSpan1 = 4.997;       // Voltage
float const pressureLinearity = 0.10154; // Percent. Not used for Calibration.
float const pressureLine_K1 = maxPressure_100Bar / pressureSpan1;
float const pressureLine_B1 = maxPressure_100Bar - pressureLine_K1 * (pressureSpan1 + pressureZero1);

/*
 *Pressure Sensor 2 (Serial Number 1040112)
 *Chamber Pressure Sensor
 */
float const pressureZero2 = 0.00;         // Voltage
float const pressureSpan2 = 4.996;        // Voltage
float const pressureLinearity2 = 0.03146; // Percent. Not used for Calibration.
float const pressureLine_K2 = maxPressure_100Bar / pressureSpan2;
float const pressureLine_B2 = maxPressure_100Bar - pressureLine_K2 * (pressureSpan2 + pressureZero2);

/*
 *Pressure Sensor 3 (Serial Number 1086286)
 *Nitrogen Feeding Pressure Sensor
 */
float const pressureZero3 = -0.005;       // Voltage
float const pressureSpan3 = 5.007;        // Voltage
float const pressureLinearity3 = 0.03709; // Percent. Not used for Calibration.
float const pressureLine_K3 = maxPressure_100Bar / pressureSpan3;
float const pressureLine_B3 = maxPressure_100Bar - pressureLine_K3 * (pressureSpan3 + pressureZero3);

/*
 *Pressure Sensor 4 (Serial Number 1086284)
 *Oxidizer Feeding 2 Pressure Sensor
 */
float const pressureZero4 = 0.0091;        // Voltage
float const pressureSpan4 = 5.0001;        // Voltage
float const pressureLinearity4 = -0.03750; // Percent. Not used for Calibration.
float const pressureLine_K4 = maxPressure_100Bar / pressureSpan4;
float const pressureLine_B4 = maxPressure_100Bar - pressureLine_K4 * (pressureSpan4 + pressureZero4);

/*Create Arrays containing the calibration data for the Pressure Sensors. Used in main.*/
float pressureCalibration_K[5] = {pressureLine_K0, pressureLine_K1, pressureLine_K2, pressureLine_K3, pressureLine_K4};
float pressureCalibration_B[5] = {pressureLine_B0, pressureLine_B1, pressureLine_B2, pressureLine_B3, pressureLine_B4};


/*Function Definitions*/

/*Pressure Sensor (RS:797-5005) reader function. Converts from raw data to bar.*/
float readPressure(float sensorValue, int sensorNum)
{
    pressureVoltage = refADC * (sensorValue / maxADC);
    return pressureCalibration_K[sensorNum] * pressureVoltage + pressureCalibration_B[sensorNum];
}

/*Load cell reader function. Converts from raw data to Newtons.*/
float readLoad(float sensorValue)
{
    /*TBD, new load cell.*/
}

/*TMP36 (Heating Blanket temp-sensors) reader function, from raw data to temperature in deg C.*/
float readTMP36(float sensorValue)
{
    return temperature = ((sensorValue / maxADC) * refADC - 0.5) * 100;
}

/*Thermocouple reader function, takes something gives something back*/
float readTemp(float sensorValue)
{
    sensorValue = sensorValue * 0.25;
    return sensorValue;
}

/*IR sensor reader function, from raw data to temperature in deg C*/
float readIR(float sensorValue)
{
    return temperature = (sensorValue / maxADC) * (maxIR - minIR) + minIR;
}

#endif