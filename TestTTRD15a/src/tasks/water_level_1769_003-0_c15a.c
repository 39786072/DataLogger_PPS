/*--------------------------------------------------------------------*-

  water_level_1769_003-0_c15a.c (Released 2015-02)

  --------------------------------------------------------------------

  Placeholder module.

  See "ERES(1769)" book, Chapter 15 (details below).

  Part of final case study (washing machine controller)
  
  In the finished system, this module will measure the water level
  in the drum.
  
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
#include "water_level_1769_003-0_c15a.h"

// Fault-injection options
#include "../fault_injection/fault_injection_1769_003-0_c15a.h"

// ------ Public variable declarations -------------------------------

extern uint32_t Water_pump_required_state_G;
extern uint32_t Water_pump_required_state_iG;
extern uint32_t Water_valve_required_state_G;
extern uint32_t Water_valve_required_state_iG;

// ------ Public variable definitions --------------------------------

// Current water level
// 0 => Drum empty
// 1 => Drum contains water (not enough to wash)
// 2 => Drum contains enough water to wash
uint32_t Water_level_reading_G;
uint32_t Water_level_reading_iG;

// Used to reset the simple model of water level
// Not duplicated (has no role in a production system)
uint32_t Model_reset_G = 0;

// ------ Private variable definitions -------------------------------

//static uint32_t Water_level = 0;

/*--------------------------------------------------------------------*-

  WATER_LEVEL_SENSOR_Init()

  Prepare for WATER_LEVEL_SENSOR_Update() task - see below.

-*--------------------------------------------------------------------*/
void WATER_LEVEL_SENSOR_Init(void)
   {
   // Provide an initial value (updated on first task release)      
   Water_level_reading_G = 0;
   Water_level_reading_iG = ~0;

   // Simple model of water in drum (value stored in NVRAM
   // => still water in drum after reset.
   
   // Model is reset if CFC jumper is inserted
   // See Port Header and System Mode module  
   if (Model_reset_G)
      {
      // GP reg in NVRAM used to represent the water level
      LPC_RTC->GPREG4 = 0;
      }
   else
      {
      // Do simple sanity checks on GPREG4 data
      // (will be "random" on first run)
      uint32_t Test_level = (uint32_t) LPC_RTC->GPREG4;

      if ((Test_level < 0) || (Test_level > 50))
         {
         // Data out of range => reset
         LPC_RTC->GPREG4 = 0;
         }
      }
   }

/*--------------------------------------------------------------------*-

  WATER_LEVEL_SENSOR_Update().

  Placeholder task.

  In finished system, this task will read the water level in the 
  washing-machine drum.

  Here we run a simple model.
  
  This task framework incorporates lightweight Resource Barriers.

-*--------------------------------------------------------------------*/
uint32_t WATER_LEVEL_SENSOR_Update(void)
   {
   uint32_t Return_value = RETURN_NORMAL;
   uint32_t Water_level;

   // Check data integrity (lightweight resource barriers)
   if (Water_level_reading_G != ~Water_level_reading_iG)
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
      
      // Read current water level from GPREG4
      // This NVRAM location allows us to maintain the record of the 
      // water level between resets (a simple model).
      Water_level = LPC_RTC->GPREG4;

      // Is the drum filling?
      if ((Water_valve_required_state_G == OPEN) && !INJ_WATER_VALVE_FAULT)
         {
         // Drum is filling
         Water_level++;

         // Is the drum full of water (ready to wash)?
         if (Water_level >= 50)
            {
            Water_level = 50;
            }
         }

      // Is the drum being drained?
      if ((Water_pump_required_state_G == ON) && !INJ_WATER_PUMP_FAULT)
         {
         // Drum is being drained
         if (Water_level > 0)
            {
            // Drum is draining
            Water_level--;              
            }
         }
         
      // Set default value for Water_level_reading_G   
      Water_level_reading_G = 0;
      Water_level_reading_iG = ~0;   
      
      // Check if drum is empty      
      if (Water_level > 0)
         {
         // Drum has water (not necessarily enough for washing)
         Water_level_reading_G = 1;
         Water_level_reading_iG = ~1;
         }

      // Check if drum is full enough for washing   
      if (Water_level > 25)
         {
         // Drum has enough water for washing
         Water_level_reading_G = 2;
         Water_level_reading_iG = ~2;
         }
         
      // Update GPREG4
      LPC_RTC->GPREG4 = Water_level;
      }

   return Return_value;
   }

/*--------------------------------------------------------------------*-
  ---- END OF FILE ---------------------------------------------------
-*--------------------------------------------------------------------*/
