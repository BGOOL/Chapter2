/*
 *Serial Reader Script
 *Author(s): Ivar Tylén
 *Purpose: Contains the main loop for reading data from Arduinos
 *and writing to GS
 *Last Modified: 2025-02-10
 */

#include <stdio.h>

/*User Defined Libraries*/
#include "serial_reader.h"
#include "serial_reader_sensors.c"
#include "serialcomms.c"

int main(void)
{
    printf("Hello, seems to be working!");
    return 0;
}
