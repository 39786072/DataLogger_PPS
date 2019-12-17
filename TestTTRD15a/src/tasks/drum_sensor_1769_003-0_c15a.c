/*--------------------------------------------------------------------*-

  drum_sensor_1769_003-0_c15a.c (Released 2015-02)

  --------------------------------------------------------------------

  Placeholder module.

  See "ERES(1769)" book, Chapter 15 (details below).

  Part of final case study (washing machine controller)
  
  In the finished system, this module will indicate whether
  the drum is rotating.
  
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
#include "drum_sensor_1769_003-0_c15a.h"

// Fault-injection options
#include "../fault_injection/fault_injection_1769_003-0_c15a.h"

// ------ Public variable declarations -------------------------------

extern uint32_t Drum_motor_required_speed_G;
extern uint32_t Drum_motor_required_speed_iG;

// ------ Public variable definitions --------------------------------

uint32_t Drum_speed_reading_G;
uint32_t Drum_speed_reading_iG;

/*--------------------------------------------------------------------*-

  DRUM_SENSOR_Init()

  Prepare for DRUM_SENSOR_Update() task - see below.

-*--------------------------------------------------------------------*/
void DRUM_SENSOR_Init(void)
   {
   // Assume drum not turning
   // Note: no attempt made to simulate this
   Drum_speed_reading_G = 0;
   Drum_speed_reading_iG = ~0;
   }

/*--------------------------------------------------------------------*-

  DRUM_SENSOR_Update().

  Placeholder task.

  In finished system, this task will read the water level in the 
  washing-machine drum.

  Here we run a simple model.
  
  This task framework incorporates lightweight Resource Barriers.

-*--------------------------------------------------------------------*/
uint32_t DRUM_SENSOR_Update(void)
   {
   uint32_t Return_value = RETURN_NORMAL;
   static uint32_t Drum_speed = 0;  // Part of simple model

   // Check data integrity (lightweight resource barriers)
   if (Drum_speed_reading_G != ~Drum_speed_reading_iG)
      {
      Return_value = RETURN_FAULT;
      }

   // Note: we don't check the values used for modelling
   // (because these won't appear in the final system)

   // ---

   // If we have met the pre-conditions, we proceed
   if (Return_value == RETURN_NORMAL)
      {
      // Simple model used here (in place of sensor reading)
      
      // Default values
      Drum_speed_reading_G = 0;
      Drum_speed_reading_iG = ~0;

      // Is the drum turning?
      if ((Drum_motor_required_speed_G != 0) && !INJ_DRUM_MOTOR_FAULT)
         {
         // Drum motor is being powered
         Drum_speed++;

         // Assume task is called every 10 ms
         // Assumed to take drum ~5 seconds to start turning
         // (and roughly the same time to slow down again)
         if (Drum_speed >= 500)
            {
            Drum_speed = 500;
            Drum_speed_reading_G = 1;
            Drum_speed_reading_iG = ~1;
            }
         }
      else
         {
         if (Drum_speed > 0)
            {
            // Drum is turning            
            Drum_speed_reading_G = 1;
            Drum_speed_reading_iG = ~1;
            
            // Drum slowing down
            Drum_speed--;            
            }
         }
      }

   return Return_value;
   }

/*--------------------------------------------------------------------*-
  ---- END OF FILE ---------------------------------------------------
-*--------------------------------------------------------------------*/
