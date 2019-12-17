/*--------------------------------------------------------------------*-

   switch_ea-sw3_1769_003-0_c13a.C (Released 2015-02)
 
  --------------------------------------------------------------------

   Simple switch interface code, with software debounce.

   Global variables used for data transfers have inverted copies
   (lightweight resource barrier).

   [Reads SW3 on LPCxpresso baseboard.]

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
#include "switch_ea-sw3_1769_003-0_c13a.h"

// ------ Public variable definitions ------------------------------

uint32_t Sw_pressed_G; // The current switch status & copy
uint32_t Sw_pressed_iG;


// ------ Private constants ----------------------------------------

// Allows NO or NC switch to be used (or other wiring variations)
#define SW_PRESSED (0)

// SW_THRES must be > 1 for correct debounce behaviour
#define SW_THRES (3)


/*--------------------------------------------------------------------*-

  SWITCH_SW3_Init()

  Initialisation function for the switch library.

  Based on PTTES, Chapter 19.

-*--------------------------------------------------------------------*/
void SWITCH_SW3_Init(void)
   {
   // Set up "SW3" as an input pin
   // Params: Port : Pin : 1 for o/p, 0 for i/p
   GPIO_SetDir(SW_RST_CNT_PORT, SW_RST_CNT_PIN, 0);

   Sw_pressed_G = 0;
   Sw_pressed_iG = ~Sw_pressed_G;
   }

/*--------------------------------------------------------------------*-

  SWITCH_SW3_Update()
  
  This is the main switch function.  
  It should usually be scheduled approx. every 10 ms.
 
-*--------------------------------------------------------------------*/
void SWITCH_SW3_Update(void)
   {
   // Duration of switch press
   static uint32_t Duration = 0;
   uint8_t Sw3_input;

   // Check data integrity
   if (Sw_pressed_G != (~Sw_pressed_iG))
      {
      SYSTEM_Perform_Safe_Shutdown();  
      }

   // ------

   // Read SW3
   Sw3_input = (GPIO_ReadValue(SW_RST_CNT_PORT) & SW_RST_CNT_PIN);

   if (Sw3_input == SW_PRESSED)
      {
      Duration += 1;

      if (Duration > SW_THRES)
         {
         Duration = SW_THRES;

         Sw_pressed_G = 1;
         }
      else
         {
         // Switch pressed, but not yet for long enough
         Sw_pressed_G = 0;
         }
      }
   else
      {
      // Switch not pressed - reset the count
      Duration = 0;

      // Update status
      Sw_pressed_G = 0;
      }

   // Update copy
   Sw_pressed_iG = ~Sw_pressed_G;
   }

/*--------------------------------------------------------------------*-
  ------ END OF FILE -------------------------------------------------
-*--------------------------------------------------------------------*/
