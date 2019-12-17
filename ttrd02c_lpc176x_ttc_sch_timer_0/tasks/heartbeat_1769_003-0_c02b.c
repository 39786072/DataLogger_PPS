/*--------------------------------------------------------------------*-

  heartbeat_1769_003-0_c02b.c (Released 2015-02)

  --------------------------------------------------------------------
   
  Simple 'Heartbeat LED' function for LPC1769.

  Incorporates "injected" task overrun (~3.6 ms) after 15 seconds.

  Usually released once per second. 

  If everything is OK, flashes at 50% duty cycle.

  If there is a fault code active, this is displayed.

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

// Task header
#include "heartbeat_1769_003-0_c02b.h"

// ------ Public variable declarations -----------------------------

// See scheduler module for definition
extern uint32_t Fault_code_G;

/*--------------------------------------------------------------------*-

  HEARTBEAT_Init()

  Prepare for HEARTBEAT_Update() function - see below.

-*--------------------------------------------------------------------*/
void HEARTBEAT_Init(void)
   {
   // Set up LED2 as an output pin
   // Params: Port : Pin : 1 for o/p, 0 for i/p
   GPIO_SetDir(HEARTBEAT_LED_PORT, HEARTBEAT_LED_PIN, 1);
   }

/*--------------------------------------------------------------------*-

  HEARTBEAT_Update()

  Flashes at 0.5 Hz if fault code is 0 (i.e. no fault code).

  Otherwise, displays fault code.

  * Incorporates fault injection (task overrun) after 15 secs *

  Must schedule every second (soft deadline).

-*--------------------------------------------------------------------*/
void HEARTBEAT_Update(void)
   {
   static uint32_t Heartbeat_state = 0;
   static uint32_t Fault_state = 0;

   uint32_t Delay, j;
   static uint32_t Task_overrun_counter = 0;

   // Force task overrun after 15 seconds (test / demo purposes)
   if (Task_overrun_counter++ == 15)
      {
      Task_overrun_counter = 0;

      // Trigger temporary task overrun (for demo purposes)
      // This gives delay of ~3.6 ms
      for (Delay = 0; Delay < 20000; Delay++)
         {
         j *= 3;
         }
      }

   if (Fault_code_G == 0)
      {
      // No faults recorded
      // - just flash at 0.5 Hz

      // Change the LED from OFF to ON (or vice versa)
      if (Heartbeat_state == 1)
         {
         Heartbeat_state = 0;
         GPIO_ClearValue(HEARTBEAT_LED_PORT, HEARTBEAT_LED_PIN);
         }
      else
         {
         Heartbeat_state = 1;
         GPIO_SetValue(HEARTBEAT_LED_PORT, HEARTBEAT_LED_PIN);
         }
      }
   else
      {
      // If we are here, there is a (non-zero) fault code ...
      Fault_state++;

      if (Fault_state < Fault_code_G*2)
         {
         Heartbeat_state = 0;
         GPIO_ClearValue(HEARTBEAT_LED_PORT, HEARTBEAT_LED_PIN);
         }
      else
         {
         if (Fault_state < Fault_code_G*4)
            {
            // Change the LED from OFF to ON (or vice versa)
            if (Heartbeat_state == 1)
               {
               Heartbeat_state = 0;
               GPIO_ClearValue(HEARTBEAT_LED_PORT, HEARTBEAT_LED_PIN);
               }
            else
               {
               Heartbeat_state = 1;
               GPIO_SetValue(HEARTBEAT_LED_PORT, HEARTBEAT_LED_PIN);
               }
            }
         else
            {
            Fault_state = 0;
            }
         }
      }
   }

/*--------------------------------------------------------------------*-
  ------ END OF FILE -------------------------------------------------
-*--------------------------------------------------------------------*/
