/*--------------------------------------------------------------------*-

  heartbeat_overrun_1769_003-0_c15a.c (Released 2015-02)

  --------------------------------------------------------------------
   
  Simple 'Heartbeat' task for LPC1769.
 
  Target LPCxpresso board (LED2).

  No fault reporting.
  
  Provides various fault-injection options (for test / demo purposes). 

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
#include "heartbeat_overrun_1769_003-0_c15a.h"

// Fault injection
#include "../fault_injection/fault_injection_1769_003-0_c15a.h"

/*------------------------------------------------------------------*-

  HEARTBEAT_Init()

-*------------------------------------------------------------------*/
void HEARTBEAT_Init(void)
   {
   // Set up LED2 as an output pin
   // Params: Port : Pin : 1 for o/p, 0 for i/p
   GPIO_SetDir(HEARTBEAT_PORT, HEARTBEAT_PIN, 1);
   }

/*------------------------------------------------------------------*-

  HEARTBEAT_Update()

  Changes state of associated LED (typically call once per second).
  
  PRE:  Valid pin.
  POST: No sensible checks.

  State of LED pin is monitored for each task
  (to check for interference).

-*------------------------------------------------------------------*/
uint32_t HEARTBEAT_Update(void)
   {
   // Duplicated (inverted) state variables
   static uint32_t Heartbeat_state1 = 0;
   static uint32_t Heartbeat_state2 = ~0;

   uint32_t Precond_gpio;
   uint32_t Return_value = RETURN_NORMAL;

   // Used for fault injection
   uint32_t Delay, j;
   static uint32_t Task_overrun_counter = 0;
   static uint32_t Task_resource_counter = 0;

   // Optional fault injection (task overrun)
   if (INJ_TASK_OVERRUN_FAULT == 1)
      {
      // Force task overrun after 5 seconds (test / demo purposes)
      if (Task_overrun_counter++ == 5)
         {
         Task_overrun_counter = 0;

         // Trigger temporary task overrun (for demo purposes)
         // This gives delay of ~3.6 ms
         for (Delay = 0; Delay < 20000; Delay++)
            {
            j *= 3;
            }
         }
      }

   // Optional fault injection (resource)
   if (INJ_RESOURCE_FAULT == 1)
      {
      // Inject resource fault after 10 seconds (test / demo purposes)
      if (Task_resource_counter++ >= 10)
         {
         Task_resource_counter = 10;

         // Set Heartbeat pin for **input**
         GPIO_SetDir(HEARTBEAT_PORT, HEARTBEAT_PIN, 0);
         }
      }


   // Optional fault injection (task sequence)
   // NOTE: The scheduler has NOT been configured to prevent
   // calls to SCH_Add_Task while the system is running
   // (otherwise this fault injection would not be possible ...)
   if (INJ_TASK_SEQUENCE_FAULT == 1)
      {
      // Change task sequence after 8 seconds (test / demo purposes)
      if (Task_resource_counter++ == 8)
         {
         Task_resource_counter = 8;

         // Add (another) Heartbeat task to the schedule ...
         SCH_Add_Task(HEARTBEAT_Update, 0, 10, 50, 0);
         }
      }

   // ---

   // Heartbeat pin must be set for O/P
   Precond_gpio = GPIO_Check_Output(HEARTBEAT_PORT, HEARTBEAT_PIN);

   if (Precond_gpio == RETURN_FAULT)
      {
      Return_value = RETURN_FAULT;
      }

   // Check data integrity
   if (Heartbeat_state1 != ~Heartbeat_state2)
      {
      Return_value = RETURN_FAULT;
      }

   // ---
      
   // If we have met the preconditions, we update the Heartbeat LED state
   if (Return_value == RETURN_NORMAL)
      {
      // Change the LED from OFF to ON (or vice versa)
      if (Heartbeat_state1 == 1)
         {
         // Update both copies of LED state (and the LED)
         Heartbeat_state1 = 0;
         GPIO_ClearValue(HEARTBEAT_PORT, HEARTBEAT_PIN);
         }
      else
         {
         Heartbeat_state1 = 1;
         GPIO_SetValue(HEARTBEAT_PORT, HEARTBEAT_PIN);
         }
      }

   // Store inverted copy	  
   Heartbeat_state2 = ~Heartbeat_state1;
	  
   return Return_value;
   }

/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/
