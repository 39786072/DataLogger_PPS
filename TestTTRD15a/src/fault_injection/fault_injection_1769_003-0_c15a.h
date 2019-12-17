/*--------------------------------------------------------------------*-

   fault_injection_1769_003-0_c15a.h (Released 2015-02)

  --------------------------------------------------------------------

   This module provides a simple way of simulating faults in the 
   system, for use during the early phases of system development.

   Note: this very simple framework is primarily intended as a means
   of demonstrating that the architecture responds to particular faults
   in the expected manner.  It is *not* expected that this file will
   included in the project after these initial tests have been
   completed: references to the file (and contents of the file) will
   then be removed from the project.
   
   Please see ERES(1769) Chapter 15 for information about this
   fault-injection mechanism (and other options).

-*--------------------------------------------------------------------*/
/*--------------------------------------------------------------------*-

  This code is copyright (c) 2014-2015 SafeTTy Systems Ltd.

  This code forms part of a Time-Triggered Reference Design (TTRD)
  that is documented in the following book: 

  Pont, M.J. (2014) 
  "The Engineering of Reliable Embedded Systems: LPC1769 edition", 
  Published by SafeTTy Systems Ltd. ISBN: 978-0-9930355-0-0.

  Both the TTRDs and the above book ("the ERES book") describe 
  patented* technology and are subject to copyright and other 
  restrictions.

  This code may be used without charge: [i] by universities and 
  colleges in courses for which a degree up to and including MSc 
  level (or equivalent) is awarded; [ii] for non-commercial projects 
  carried out by individuals and hobbyists.

  Where this code has been provided to you as part of a training 
  course delivered by SafeTTy Systems Ltd, you may use it for 
  evaluation purposes for a period of up to 12 months from the 
  end date of this course, subject to the condition that no 
  products (of any kind) based either on this code, or on the 
  patented technology described in the ERES book, are released.

  Any and all other use of this code and / or the patented 
  technology described in the ERES book requires purchase of a 
  ReliabiliTTy Technology Licence:
  http://www.safetty.net/products/reliabilitty

  Please contact SafeTTy Systems Ltd if you require clarification
  of these licensing arrangements:
  http://www.safetty.net/contact

  MoniTTor is a registered trademark of SafeTTy Systems Ltd.
  PredicTTor is a registered trademark of SafeTTy Systems Ltd.
  ReliabiliTTy is a registered trademark of SafeTTy Systems Ltd.
  WarranTTor is a registered trademark of SafeTTy Systems Ltd.

  * Patents applied for.

-*--------------------------------------------------------------------*/

#ifndef _FAULT_INJECTION_H
#define _FAULT_INJECTION_H 1

/*--------------------------------------------------------------------*

  NOTES:

  Refer to the relevant tasks for info about these faults.
  
  If multiple faults are selected, the first one detected will be reported.

  Other simple fault-injection options 
  (not controlled via this file):

  1. To "inject" a simple watchdog fault, remove the WDT jumper
     (before powering up the board)
     
  2. To simulate a power failure mid wash / user cycling power mid wash
     use the reset switch on the EA Baseboard.  If there is water
     in the drum when the reset is performed, the system will respond
     to this when it starts up (unless the CFC_JUMPER is inserted).     

-*--------------------------------------------------------------------*/

// Fault detected during system configuration checks
#define INJ_CONFIGURATION_FAULT (0)

// Fault detected during POSTs
#define INJ_POST_FAULT (0)

// Fault detected during BISTs
#define INJ_BIST_FAULT (0)

// Fault with door sensor *or* door lock
#define INJ_DOOR_FAULT (0)

// Fault with drum motor or drum sensor
#define INJ_DRUM_MOTOR_FAULT (0)

// Fault with water heater or water temp sensor
#define INJ_WATER_HEATER_FAULT (0)

// Fault with water pump or water level sensor
#define INJ_WATER_PUMP_FAULT (0)

// Fault with water valve or water level sensor
#define INJ_WATER_VALVE_FAULT (0)

// Setting this flag causes the Heartbeat task to overrun after ~5 seconds
// => this fault should be detected by the MoniTTor unit
#define INJ_TASK_OVERRUN_FAULT (0)

// Changes the task sequence
// => this fault should be detected by the PredicTTor unit
// => see "Heartbeat" task for details
#define INJ_TASK_SEQUENCE_FAULT (0)

// Injects resource fault
// => this fault should be detected by the (lightweight) resource barriers
// => see "Heartbeat" task for details.
#define INJ_RESOURCE_FAULT (0)

#endif

/*--------------------------------------------------------------------*-
  ------ END OF FILE -------------------------------------------------
-*--------------------------------------------------------------------*/

