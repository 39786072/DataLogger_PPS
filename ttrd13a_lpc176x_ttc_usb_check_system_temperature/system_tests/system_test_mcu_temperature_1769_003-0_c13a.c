/*--------------------------------------------------------------------*-

   system_test_mcu_temperature_1769_003-0_c13a.c (Released 2015-02)
   
  --------------------------------------------------------------------

   Module supports periodic system self test:
   checks that microcontroller temperature is within required range.
   
   This module employs MAX6576 sensor on LPCxpresso baseboard.
   
   See "ERES(1769)" book, Chapter 13 (details below).

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
#include "system_test_mcu_temperature_1769_003-0_c13a.h"

// ------ Public variable definitions ------------------------------

// The current temperature (Celsius x10) plus copy
int32_t Current_temperature_x10_G;
int32_t Current_temperature_x10_iG;

// ------ Private constants ----------------------------------------

// Operating temperature range for LPC1769 microcontroller
// Values in degrees Celsius
#define MCU_TEMPERATURE_MAX (+85)
#define MCU_TEMPERATURE_MIN (-40)

/*--------------------------------------------------------------------*-

  SYS_CHECK_MCU_TEMP_Init()

  Initialisation function for the module.

-*--------------------------------------------------------------------*/
void SYS_CHECK_MCU_TEMP_Init()
   {
   // Set up temperature sensor (MAX6576) pin as an input
   // Params: Port : Pin : 1 for o/p, 0 for i/p
   GPIO_SetDir(TEMP_SENSOR_PORT, TEMP_SENSOR_PIN, 0);

   // Initialisation only
   Current_temperature_x10_G = 100;
   Current_temperature_x10_iG = ~Current_temperature_x10_G;
   }

/*--------------------------------------------------------------------*-

  SYS_CHECK_MCU_TEMP_Update()
  
  Reads from MAX6576 sensor on LPCxpresso baseboard.

  This function should be called once per millisecond 
  (or adapted for an alternative update frequency).

-*--------------------------------------------------------------------*/
void SYS_CHECK_MCU_TEMP_Update(void)
   {
   uint8_t state = 0;
   static uint8_t old_state = 0;
   static uint32_t state_count = 0;
   static uint32_t call_count = 0;

   // Check data integrity
   if (Current_temperature_x10_G != (~Current_temperature_x10_iG))
      {
      SYSTEM_Perform_Safe_Shutdown();  
      }

   // ------

   // Sample waveform from MAX6576
   state = ((GPIO_ReadValue(TEMP_SENSOR_PORT) & TEMP_SENSOR_PIN) != 0);

   if ((old_state == 0) && (state == 1))
      {
      // Start of new temperature reading
      state_count = 0;
      }

   if ((old_state == 1) && (state == 1))
      {
      // Temperature reading in progress
      state_count++;
      }

   if ((old_state == 1) && (state == 0))
      {
      // Ready to process temperature reading
      // state_count value represents the temperature
      //
      // Assume TS0 = TS1 = 1;
      //
      // From MAX6576 datasheet:
      // [Period is in usec on datasheet - we measure ms: count *= 1000]
      // [In addition, we measure only half of the waveform: count *= 2]
      // T (Celsius) = ((state_count * 2000)/640) - 273
      // [In addition, we return temperature * 10]
      Current_temperature_x10_G = (((state_count * 2000) / 640) - 273) * 10;
      }

   // Update inverted copy and old_state      
   Current_temperature_x10_iG = ~Current_temperature_x10_G;
   old_state = state;    

   // We wait ~ 1 second before checking temperature
   // to ensure that we are working with valid data
   if (++call_count >= 1000)
      {
      call_count = 1000;
            
      // Check temperature against max allowed value
      // Note: we test against *90%* of the max temp here
      if (Current_temperature_x10_G > (9 * MCU_TEMPERATURE_MAX))
         {
         // Temperature is approaching maximum allowed
         SYSTEM_Perform_Safe_Shutdown();
         }

      // Check temperature against min allowed value
      // Note: we test against *90%* of the min temp here
      if (Current_temperature_x10_G < (9 * MCU_TEMPERATURE_MIN))
         {
         // Temperature is approaching maximum allowed
         SYSTEM_Perform_Safe_Shutdown();
         }
      }
   }

/*--------------------------------------------------------------------*-
  ------ END OF FILE -------------------------------------------------
-*--------------------------------------------------------------------*/
