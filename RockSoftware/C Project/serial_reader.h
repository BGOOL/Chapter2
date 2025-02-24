/*
 *Serial Reader Header File
 *Author(s): Ivar Tylén
 *Purpose: Contains declarations used in serial_reader.c
 *Last Modified: 2025-02-10
 */

#ifndef serial_reader.h
#define serial_reader.h
#include <stdio.h>

/*Variable declarations/definitions*/

/*Misc may be moved*/
extern float pressureVoltage; //Used in readPressure function
extern float loadVoltage; // Used in readLoad function, see src-file.
extern float temperature; // Used in readTMP36, readTemp, readIR -||-

/*Pressure Sensor "Positions". Used for --*/
extern int const FEEDING_PRESSURE_OXIDIZER;
extern int const LINE_PRESSURE;
extern int const CHAMBER_PRESSURE;
extern int const FEEDING_PRESSURE_N2;
extern int const FEEDING_PRESSURE_OXIDIZER2;

/*Thermocouple "Locations"*/
extern int const Not_Connected_0;
extern int const Not_Connected_1;
extern int const Nozzle_TC;
extern int const Ambient_TC;

/*ADC Resolution*/
extern int const resolutionADC;

/*Max ADC Value*/
extern float const maxADC;

/*ADC Reference Voltage*/
extern float const refADC;

/*IR Sensor Min and Max values*/
extern int const minIR;
extern int const maxIR;

/*TMP 36 Min and Max values*/
int const minTMP36;
int const maxTMP36;

/*
 * Load Cell Min and Max values
 * TBD For new Load Cell
 */
int const minLoad;
float const maxLoad; // Conversion from lbf to Newtons

/*Load Cell Calibration, TBD new sensor*/

/*Pressure Sensor Calibration Data*/

/*Pressure Sensor Maximum Pressures*/
float const maxPressure_100Bar;
float const maxPressure_25Bar;

/*
 *Pressure Sensor 0 (Serial Number 667662)
 *Oxidizer Feeding 1 Pressure Sensor
 */
float const pressureZero0;       // Voltage
float const pressureSpan0;       // Voltage
float const pressureLinearity0;  // Percent. Not used for Calibration.
float const pressureLine_K0;
float const pressureLine_B0;

/*
 *Pressure Sensor 1 (Serial Number 1073014)
 *Line Pressure Sensor
 */
float const pressureZero1;        // Voltage
float const pressureSpan1;       // Voltage
float const pressureLinearity1; // Percent. Not used for Calibration.
float const pressureLine_K1;
float const pressureLine_B1;

/*
 *Pressure Sensor 2 (Serial Number 1040112)
 *Chamber Pressure Sensor
 */
float const pressureZero2;         // Voltage
float const pressureSpan2;        // Voltage
float const pressureLinearity2; // Percent. Not used for Calibration.
float const pressureLine_K2;
float const pressureLine_B2;

/*
 *Pressure Sensor 3 (Serial Number 1086286)
 *Nitrogen Feeding Pressure Sensor
 */
float const pressureZero3;       // Voltage
float const pressureSpan3;        // Voltage
float const pressureLinearity3; // Percent. Not used for Calibration.
float const pressureLine_K3;
float const pressureLine_B3;

/*
 *Pressure Sensor 4 (Serial Number 1086284)
 *Oxidizer Feeding 2 Pressure Sensor
 */
float const pressureZero4;        // Voltage
float const pressureSpan4;        // Voltage
float const pressureLinearity4; // Percent. Not used for Calibration.
float const pressureLine_K4;
float const pressureLine_B4;

/*Create Arrays containing the calibration data for the Pressure Sensors. Used in main.*/
float pressureCalibration_K[5];
float pressureCalibration_B[5];

/*Function Declarations*/
/*See source file for definitions*/

/*The following functions are defined in serial_reader_sensors.c*/
float readPressure(float sensorValue, int sensorNum);

float readLoad(float sensorValue);

float readTMP36(float sensorValue);

float readTemp(float sensorValue);

float readIR(float sensorValue);

/*The following functions are defined in serialcomms.c*/
char read_message(char ser);

void arduinoPort();

#endif