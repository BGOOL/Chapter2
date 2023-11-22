/* Filename:      Valves.cpp
 * Author:        Eemeli Mykrä
 * Date:          27.01.2023
 *
 * Purpose:       Responsible for controlling the different valves used to 
 *                control the flow of the oxidizer and purge gas.
 */
 
#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include <stdint.h>

#include "Globals.h"

// valvePins:
// Arduino Pin 2 -> ConnectorPin 1 -> mainValve -> code value 0
// Arduino Pin 3 -> ConnectorPin 2 -> dumpValve -> code value 1
// Arduino Pin 4 -> ConnectorPin 3 -> feedingValve -> code value 2

static const uint16_t valvePins[valveCount] = {MAIN_VALVE_PIN, DUMP_VALVE_PIN, FEEDING_VALVE_PIN};
static SemaphoreHandle_t valveSemaphore;

void initValves(){
  //Set up pressure sensor

  pinMode(pin_names_t::MAIN_VALVE_PIN, OUTPUT);
  digitalWrite(pin_names_t::MAIN_VALVE_PIN, LOW);
  
  pinMode(pin_names_t::DUMP_VALVE_PIN, OUTPUT);
  digitalWrite(pin_names_t::DUMP_VALVE_PIN, HIGH); // because normally open
  
  pinMode(pin_names_t::FEEDING_VALVE_PIN, OUTPUT);
  digitalWrite(pin_names_t::FEEDING_VALVE_PIN, LOW);
  
  valveSemaphore = xSemaphoreCreateMutex();
  
}

void setValve(pin_names_t valve_pin, bool state){
  if (xSemaphoreTake(valveSemaphore, 10) == pdTRUE){
    digitalWrite(valve_pin, state);
    xSemaphoreGive(valveSemaphore);
  }
}

void getValve(pin_names_t valve_pin, bool* valveState){
  if (xSemaphoreTake(valveSemaphore, 10) == pdTRUE){
    *valveState = digitalRead(valve_pin); 
    xSemaphoreGive(valveSemaphore);
  }
}
