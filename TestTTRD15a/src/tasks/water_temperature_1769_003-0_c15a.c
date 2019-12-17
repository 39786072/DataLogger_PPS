/*--------------------------------------------------------------------*-

  water_temperature_1769_003-0_c15a.c (Released 2015-02)

  --------------------------------------------------------------------

  Placeholder module.

  See "ERES(1769)" book, Chapter 15 (details below).

  Part of final case study (washing machine controller)
  
  In the finished system, this module will measure the water 
  temperature.
  
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
#include "water_temperature_1769_003-0_c15a.h"

// Fault injection options
#include "../fault_injection/fault_injection_1769_003-0_c15a.h"

// ------ Public variable declarations -----------------------------

extern uint32_t Water_heater_required_state_G;

// ------ Public variable definitions ------------------------------

uint32_t Water_temperature_reading_G = 0;
uint32_t Water_temperature_reading_iG = 0;

/*------------------------------------------------------------------*-

  WATER_TEMP_SENSOR_Init()

  Prepare for WATER_TEMP_SENSOR_Update() task - see below.

-*------------------------------------------------------------------*/
void WATER_TEMP_SENSOR_Init(void)
   {
   // Set initial values
   Water_temperature_reading_G = 0;
   Water_temperature_reading_iG = ~0;
   }

/*------------------------------------------------------------------*-

  WATER_TEMP_SENSOR_Update().

  Placeholder task.

  In finished system, this task will read the temperature of the
  water in the drum.

  Here we have a simple model of the water-heating process.
  
  This task framework incorporates lightweight Resource Barriers.

-*------------------------------------------------------------------*/
uint32_t WATER_TEMP_SENSOR_Update(void)
   {
   uint32_t Return_value = RETURN_NORMAL;
   static uint32_t Water_temperature = 0;

   // Check data integrity (lightweight resource barriers)
   if (Water_temperature_reading_G != ~Water_temperature_reading_iG)
      {
      Return_value = RETURN_FAULT;
      }

   // Note: we don't check the values used for modelling
   // (because these won't appear in the final system)

   // ---

   // If we have met the preconditions, we proceed
   if (Return_value == RETURN_NORMAL)
      {
      // Simple model used here (in place of sensor reading)
      if ((Water_heater_required_state_G == ON) && (!INJ_WATER_HEATER_FAULT))
         {
         // Water is heating
         Water_temperature++;
         }
      else
         {
         if (Water_temperature > 0)
            {
            // Water is cooling
            // (ambient temperature is low ...)
            Water_temperature--;
            }
         }

      // Water at correct temperature?
      if (Water_temperature >= 50)
         {
         Water_temperature_reading_G = 1;
         Water_temperature = 50;
         }
      else
         {
         Water_temperature_reading_G = 0;
         }
      }

   // Set inverted copy
   Water_temperature_reading_iG = ~Water_temperature_reading_G;

   // Single return point
   return Return_value;
   }

/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/
