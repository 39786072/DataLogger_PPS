/*--------------------------------------------------------------------*-

  door_sensor_1769_003-0_c15a.c (Released 2015-02)

  --------------------------------------------------------------------

  Placeholder module.

  See "ERES(1769)" book, Chapter 15 (details below).

  Part of final case study (washing machine controller)
  
  In the finished system, this will read the door sensor.

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

// Project header
#include "../main/main.h"

// Module header
#include "door_sensor_1769_003-0_c15a.h"

// Fault injection options
#include "../fault_injection/fault_injection_1769_003-0_c15a.h"

// ------ Public variable definitions --------------------------------

uint32_t Door_sensor_reading_G;
uint32_t Door_sensor_reading_iG;

// ------ Public variable declarations --------------------------------

extern uint32_t Door_lock_required_state_G;
extern uint32_t Door_lock_required_state_iG;

/*--------------------------------------------------------------------*-

  DOOR_SENSOR_Init()

  Prepare for DOOR_SENSOR_Update() task - see below.

-*--------------------------------------------------------------------*/
void DOOR_SENSOR_Init(void)
   {
   // Set up initial values
   Door_sensor_reading_G = OPEN;
   Door_sensor_reading_iG = ~OPEN;
   }

/*--------------------------------------------------------------------*-

  DOOR_SENSOR_Update().

  Placeholder task.

  In finished system, this task will read the door-lock sensor.

  In this system, we have a very simple model.
  
  This task framework incorporates lightweight Resource Barriers.

-*--------------------------------------------------------------------*/
uint32_t DOOR_SENSOR_Update(void)
   {
   uint32_t Return_value = RETURN_NORMAL;
   static uint32_t Call_count = 0;

   // Check data integrity (lightweight resource barriers)
   if (Door_sensor_reading_G != ~Door_sensor_reading_iG)
      {
      Return_value = RETURN_FAULT;
      }

   // Note: we don't check the values used for modelling
   // (these won't appear in the final system)

   // ---

   if (Return_value == RETURN_NORMAL)
      {
      if (Door_lock_required_state_G == LOCKED)
         {
         // Simple model used here (in place of sensor reading)

         // Assume door is closed ...
         Door_sensor_reading_G = CLOSED;
         Door_sensor_reading_iG = ~CLOSED;
         }
      else
         {
         // Assume door is open ...
         Door_sensor_reading_G = OPEN;
         Door_sensor_reading_iG = ~OPEN;
         }

      // Simulate failure of lock / sensor after ~10 seconds (if reqd)
      if ((++Call_count >= 1000) && (INJ_DOOR_FAULT))
         {
         // Assume door is open ...
         Door_sensor_reading_G = OPEN;
         Door_sensor_reading_iG = ~OPEN;
         Call_count = 1000;
         }
      }

   return Return_value;
   }

/*--------------------------------------------------------------------*-
  ---- END OF FILE ---------------------------------------------------
-*--------------------------------------------------------------------*/
