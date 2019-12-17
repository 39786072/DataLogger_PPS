/*--------------------------------------------------------------------*-

   start_switch_1769_003-0_c15a.c (Released 2015-02)
 
  --------------------------------------------------------------------
  
  Placeholder module.

  See "ERES(1769)" book, Chapter 15 (details below).

  Part of final case study (washing machine controller)
  
  In the finished system, this will read the "start" switch.

  This version reads SW3 on EA Baseboard (with software debounce).

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
#include "start_switch_1769_003-0_c15a.h"

// ------ Private constants ----------------------------------------

// Allows NO or NC switch to be used (or other wiring variations)
#define SW_PRESSED (0)

// SW_THRES must be > 1 for correct debounce behaviour
#define SW_THRES (3)

// ------ Public variable definitions --------------------------------

int32_t Start_switch_pressed_G;
int32_t Start_switch_pressed_iG;  // Inverted copy

/*------------------------------------------------------------------*-

  SWITCH_SW3_Init()

  Initialisation function for the switch library.

-*------------------------------------------------------------------*/
void SWITCH_SW3_Init(void)
   {
   // Set up "SW3" as an input pin
   // Params: Port : Pin : 1 for o/p, 0 for i/p
   GPIO_SetDir(START_SWITCH_PORT, START_SWITCH_PIN, 0);

   // Initial values
   Start_switch_pressed_G = 0;
   Start_switch_pressed_iG = ~0;
   }

/*------------------------------------------------------------------*-

  SWITCH_SW3_Update()
  
  This is the main switch function.  

  It should usually be scheduled approx. every 10 ms.
  
  This task framework incorporates lightweight Resource Barriers.
 
-*------------------------------------------------------------------*/
uint32_t SWITCH_SW3_Update(void)
   {
   static uint32_t Duration = 0;

   uint32_t Return_value = RETURN_NORMAL;
   uint32_t Precond_gpio;
   uint32_t sw3_input;

   // Precond check
   // Check that Switch pin is set for input
   Precond_gpio = GPIO_Check_Input(START_SWITCH_PORT, START_SWITCH_PIN);

   if (Precond_gpio == RETURN_FAULT)
      {
      Return_value = RETURN_FAULT;
      }

   // Check data integrity (lightweight resource barriers)
   if (Start_switch_pressed_G != ~Start_switch_pressed_iG)
      {
      Return_value = RETURN_FAULT;
      }

   // ---

   // If we have met the preconditions, we read the switch
   if (Return_value == RETURN_NORMAL)
      {
      // Read "reset count" switch input (SW3)
      sw3_input = (GPIO_ReadValue(START_SWITCH_PORT) & START_SWITCH_PIN);

      if (sw3_input == SW_PRESSED)
         {
         Duration += 1;

         if (Duration > SW_THRES)
            {
            Duration = SW_THRES;

            // Update the variables
            Start_switch_pressed_G = 1;
            }
         else
            {
            // Update the variables
            Start_switch_pressed_G = 0;
            }
         }
      else
         {
         // Switch not pressed - reset the count
         Duration = 0;

         // Update the variables
         Start_switch_pressed_G = 0;
         }
      }

   // Set inverted copy
   Start_switch_pressed_iG = ~Start_switch_pressed_G;

   // Single return point
   return Return_value;
   }

/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/
