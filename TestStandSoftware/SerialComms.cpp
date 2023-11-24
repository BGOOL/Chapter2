/* Filename:      SerialComms.cpp
 * Author:        Eemeli Mykrä
 * Date:          21.11.2022
 *
 * Purpose:       Responsible for sending the latest sensor measurements over
 *                the Arduino Serial interface to a Rock 4C+ microcomputer.
 */

#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <semphr.h>

#include "Globals.h"
static SemaphoreHandle_t serialMutex;
static SemaphoreHandle_t messageMutex;

static char datalineMessage[512];
static char* msg = datalineMessage;


void initSerial(){

  Serial.begin(serialBaud);
  //Serial.println(serialBaud);
  while (!Serial){}
  serialMutex = xSemaphoreCreateMutex();
  messageMutex = xSemaphoreCreateMutex();

  Serial.print(" r\n");
}

void writeValues(values_t values, statusValues_t statusValues){
  if (xSemaphoreTake(serialMutex, 10) == pdTRUE){
    Serial.print("d");
    Serial.print(",");
    Serial.print(values.timestamp);     //Arduino time in ms
    Serial.print(",");
    Serial.print(values.pressure0);     //Feeding pressure
    Serial.print(",");
    Serial.print(values.pressure1);     //Oxidizer line pressure aka after valve
    Serial.print(",");
    Serial.print(values.pressure2);     //Chamber pressure
    Serial.print(",");
    Serial.print(values.loadCell0);     //Load cell for thrust
    Serial.print(",");
    Serial.print(values.temperature0);  //Bottle/Heating blanket temperature
    Serial.print(",");
    Serial.print(values.temperature1);  //Not connected
    Serial.print(",");
    Serial.print(values.temperature2);  //Nozzle temperature
    Serial.print(",");
    Serial.print(values.temperature3);  //Ambient temperature
    Serial.print(",");
    Serial.print(values.IR);            //Plume temperature
    Serial.print(",");

    Serial.print(values.dumpValveButton);       //Manual vent button status
    Serial.print(",");
    Serial.print(values.heatingBlanketButton);  //Heating button status
    Serial.print(",");
    Serial.print(values.ignitionButton);        //Ignition button status
    Serial.print(",");
    Serial.print(values.feedingButton);         //Feeding button status
    Serial.print(",");
    Serial.print(values.mainValveButton);       //Main valve button status
    Serial.print(",");

    Serial.print(statusValues.ignitionEngagedActive);   //Ignition SW state
    Serial.print(",");
    Serial.print(statusValues.valveActive);             //Valve SW state
    Serial.print(",");
    Serial.print(statusValues.mode);                    //Current SW mode
    Serial.print(",");
    Serial.print(statusValues.subState);                //Current SW substate
    Serial.print(",");
    Serial.print(msg);                                  //Message field
    Serial.print("\n");

    //Clear message
    strcpy(msg, " ");

    xSemaphoreGive(serialMutex);
  }
}

void saveMessage(char* message){
    if (xSemaphoreTake(messageMutex, 10) == pdTRUE){
      strcat(msg, message);
      xSemaphoreGive(messageMutex);
    }
}

//Commented out for now to try out the message field in the main data line.
/*
writeMessage(char* message){
  if (xSemaphoreTake(serialMutex, 10) == pdTRUE){
    Serial.print(message);
    xSemaphoreGive(serialMutex);
  }
}

writeIntMessage(int16_t integer){
  if (xSemaphoreTake(serialMutex, 10) == pdTRUE){
    Serial.print(integer);
    xSemaphoreGive(serialMutex);
  }
}
*/