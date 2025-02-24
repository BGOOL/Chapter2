/*
 * Serial Comms Setup Source Code
 * Author: Ivar Tylén
 * For use on the RAVEN Chapter 2 Test Bench
 * Sets up serial comms with both Arduinos, sends data through to GS.
 * Last modified: 2025-02-10
 */

#ifndef serialcomms.c
#define serialcomms.c
/*C Library Headers*/
#include <stdio.h>
#include <string.h>

/*Linux Specific Libraries*/
// #include <fcntl.h> //Contains file controls like O_RDWR
// #include <errno.h> //Error integer and strerror() function
// #include <termios.h> //POSIX terminal control definitions
// #include <unistd.h> //write(), read(), close()

/*User Defined Libraries*/
#include "serial_reader.h"

/*Serial Comms Setup*/
char const END_MARKER = 0x7F;
char const ESCAPE_BYTE = 0x7D;
char const ESCAPE_XOR = 0x20;

char read_message(char ser)
{
    char data[20] = {0x0};
    int i = 0;
    while(1){
        
    }
}

/*Arduino Identifier Function*/
void arduinoPort()
{
    /*Will need to read identifying information about
     *connected USB units and make sure the correct info
     *is being communicated to the right one
     */
}

#endif
