/*--------------------------------------------------------------------*-

  door_lock_1769_003-0_c15a.c (Released 2015-02)

  --------------------------------------------------------------------

  Placeholder module.

  See "ERES(1769)" book, Chapter 15 (details below).

  Part of final case study (washing machine controller)

  In finished system, this module will control the door lock.

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
#include "door_lock_1769_003-0_c15a.h"

// ------ Public variable definitions --------------------------------

uint32_t Door_lock_required_state_G;
uint32_t Door_lock_required_state_iG;  // Inverted copy

/*--------------------------------------------------------------------*-

  DOOR_LOCK_Init()

  Prepare for DOOR_LOCK_Update() task - see below.

-*--------------------------------------------------------------------*/
void DOOR_LOCK_Init(void)
   {
   // Placeholder task

   // Here we use a green LED (LED3-G) to indicate "Unlocked"
   // and a red LED (LED3-R) to indicate "Locked"

   // Set up pins
   // Need to set up all three LEDs (or blue may be lit ...)
   GPIO_SetDir(LED3_RED_PORT, LED3_RED_PIN, 1);
   GPIO_SetDir(LED3_BLUE_PORT, LED3_BLUE_PIN, 1);
   GPIO_SetDir(LED3_GREEN_PORT, LED3_GREEN_PIN, 1);

   // Clear all three LEDs in the set
   GPIO_ClearValue(LED3_RED_PORT, LED3_RED_PIN);
   GPIO_ClearValue(LED3_BLUE_PORT, LED3_BLUE_PIN);
   GPIO_ClearValue(LED3_GREEN_PORT, LED3_GREEN_PIN);
   }

/*--------------------------------------------------------------------*-

  DOOR_LOCK_Update().

  Placeholder task.

  In finished system, this task will control the door lock.

  This task framework incorporates lightweight Resource Barriers.

-*--------------------------------------------------------------------*/
uint32_t DOOR_LOCK_Update(void)
   {
   uint32_t Return_value = RETURN_NORMAL;
   uint32_t Precond_gpio;

   // Door-lock pins must be set for output in this task framework.
   // => other checks may be required in the final system
   Precond_gpio = GPIO_Check_Output(LED3_RED_PORT, LED3_RED_PIN);
   Precond_gpio = GPIO_Check_Output(LED3_GREEN_PORT, LED3_GREEN_PIN);

   if (Precond_gpio == RETURN_FAULT)
      {
      Return_value = RETURN_FAULT;
      }

   // Check data integrity (lightweight resource barriers)
   if (Door_lock_required_state_G != ~Door_lock_required_state_iG)
      {
      Return_value = RETURN_FAULT;
      }

   // ---
 	  
   // If we have met the preconditions, we proceed ...
   if (Return_value == RETURN_NORMAL)
      {
      if (Door_lock_required_state_G == LOCKED)
         {
         // Turn on red LED, turn off green LED
         GPIO_SetValue(LED3_RED_PORT, LED3_RED_PIN);
         GPIO_ClearValue(LED3_GREEN_PORT, LED3_GREEN_PIN);
         }
      else
         {
         // Turn off red LED, turn on green LED
         GPIO_ClearValue(LED3_RED_PORT, LED3_RED_PIN);
         GPIO_SetValue(LED3_GREEN_PORT, LED3_GREEN_PIN);
         }
      }

   return Return_value;
   }

/*--------------------------------------------------------------------*-
  ---- END OF FILE ---------------------------------------------------
-*--------------------------------------------------------------------*/
