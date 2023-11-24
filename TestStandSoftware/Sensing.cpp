/* Filename:      Sensing.cpp
 * Author:        Eemeli Mykrä
 * Date:          21.11.2022
 *
 * Purpose:       This object handles the measurements of the different sensors,
 *                storing them to the LatestValues object and checking them for
 *                faults using the FaultDetection object.
 *                Contains the FreeRTOS task called senseLoop.
 */

#include <Arduino_FreeRTOS.h>
#include <Arduino.h>

#include "Sensors.h"
#include "Sensing.h"
#include "Pressure.h"
#include "LoadCell.h"
#include "Temperature.h"
#include "InfraRed.h"
#include "LatestValues.h"
#include "Globals.h"
#include "ControlSensing.h"

void initSensing(){

    xTaskCreate(
    senseLoop        // Name of the task function
    ,  "SenseLoop"   // A name just for humans
    ,  512           // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL          // Poiter to passed variable
    ,  2             // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL);        // Handle to the created task
}

void senseLoop(){
  values_t values;
  TickType_t lastSensingWakeTime = xTaskGetTickCount();
  //bool valveState;
  //bool ignitionState;
  while (true){
    values.pressure0 = readPressure5V(FEEDING_PRESSURE);    //Feeding pressure
    values.pressure1 = readPressure5V(LINE_PRESSURE);       //Oxidizer line pressure
    values.pressure2 = readPressure5V(CHAMBER_PRESSURE);    //Chamber pressure

    values.loadCell = readLoad();  //Load cell for thrust

    values.temperature0 = readTMP36();                  //Bottle/Heating blanket temperature
    values.temperature1 = readTemp(NOT_CONNECTED_1);    //Not connected
    values.temperature2 = readTemp(NOZZLE_TC);          //Nozzle temperature
    values.temperature3 = readTemp(AMBIENT_TC);         //Ambient temperature

    values.IR = readIR();   //Plume temperature
    
    //Read control signals
    values.dumpValveButton = readVentingButton();         //Purging button status (inverted afterwards due to normally open valve)
    values.heatingBlanketButton = readHeatingButton();    //Heating button status
    values.ignitionButton = readIgnitionButton();         //Ignition button status
    values.feedingButton = readFeedingValveButton();      //Feeding valve status
    values.mainValveButton = readMainValveButton();       //Main oxidizer valve status

    //Save timestamp
    values.timestamp = millis();            //Arduino time in ms
    
    sendToCheck(values);
    setLatest(values);

    xTaskDelayUntil(&lastSensingWakeTime, samplingTickDelay);
  }
}
