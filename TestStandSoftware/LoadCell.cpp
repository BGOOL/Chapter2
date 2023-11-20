/* Filename:      LoadCell.cpp
 * Author:        Eemeli Mykrä
 * Date:          27.01.2023
 *
 * Purpose:       Responsible for reading the load cell used to measure the 
 *                thrust of the engine.
 */

#include <Arduino.h>
#include <stdint.h>

#include "LoadCell.h"
#include "Globals.h"

static const int16_t loadPins[loadCellCount] = {LOADCELL_INPUT_PIN};

void initLoad(){
  //Nothing to initialize currently
}

float readLoad(uint16_t cellNum){
  float sum = 0;
  float sensorValue = 0;

  for(uint16_t i = 0; i<loadCellAverageCount; i++){
    sensorValue = analogRead(loadPins[cellNum]);
    sensorValue = (sensorValue / maxADC) * refADC * calibrationADC;
    sum += sensorValue;  
  }

  sensorValue = sum/loadCellAverageCount;
  return loadCellLine_K * sensorValue + loadCellLine_B;

}
