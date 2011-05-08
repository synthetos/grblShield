/*
  main.c - An embedded CNC Controller with rs274/ngc (g-code) support
  Part of Grbl

  Copyright (c) 2009-2011 Simen Svale Skogsrud

  Grbl is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Grbl is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Grbl.  If not, see <http://www.gnu.org/licenses/>.
*/
/* grblshield notes //@grblshield@/

  Changes from official grbl are marked by /@grblshield@/.  These are:
  	- main.c: - this header
	- protocol.c - fixed character read bug
	- stepper.c - added st_disable_steppers() and called in various places
	- settings.c - changed DEFAULT_STEPPING_INVERT_MASK to 0x1C

	Steps to set up grblshield as an AVRStudio 4 project 
  		(we don't support AS5 yet, it doesn't work well enough)

	- If you have a grblshield.aps file in the grblshield directory
		the project is already set up. You should be able to just click
		grblshield.aps and it should start an AS4 session. 
		You then need to do this:
		- select Project / Configuration from the menu bar
			- enter 16000000 (16 MHz) into the hz box (hit OK to exit)
			- select "Libraries" in the left nav bar
			- add libc.a by clicking in left window and hitting Add Library (then hit OK to exit)
		- the project should now build if you hit Build/Build
		- you will find the grblshield.hex file in the "default" directory

	Here's how to build the project if you don't have the grblshield.aps file:
		
	- go to the grblshield firmware directory
	- delete grblshield.aps (if it exists)
	- Open AVRStudio 4 (we use version 4.18.692)
	- click New Project
		- select AVR GCC
		- set Location to the grblshield firmware directory
			(do not create a new folder)
		- set project name to grblshield
		- uncheck "Create initial file" box
	- hit Next>>
		- select AVR Simulator 2
		- select ATmega328P
	- hit Finish>>
	- find the AVR GCC window in the IDE. 
		It should have the grblshield project in it
	- right-click the Source Files directory
		- select "Add Existing Source File(s)" 
		- select all .c files; from eeprom.c to wiring_serial.c
	- right-click the Header Files directory
		- select "Add Existing Header File(s)" 
		- select all .h files as above
	- finish up by doing the steps above starting with
		"select Project / Configuration from the menu bar"
*/

#include <avr/io.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include "planner.h"
#include "stepper.h"
#include "spindle_control.h"
#include "motion_control.h"
#include "gcode.h"
#include "protocol.h"
#include "limits.h"

#include "settings.h"
#include "wiring_serial.h"

// #ifndef __AVR_ATmega328P__
// #  error "As of version 0.6 Grbl only supports atmega328p. If you want to run Grbl on an 168 check out 0.51 ('git co v0_51')"
// #endif

int main(void)
{
  protocol_init();        
  settings_init();  
  plan_init();      
  st_init();        
  spindle_init();   
  gc_init();
  limits_init();  
                    
  for(;;){
    sleep_mode(); // Wait for it ...
    protocol_process(); // ... process the serial protocol
  }
  return 0;   /* never reached */
}
