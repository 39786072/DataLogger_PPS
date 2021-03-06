﻿/*--------------------------------------------------------------------*-

  watchdog_1769_003-0_c02a.c (Released 2015-02)

  --------------------------------------------------------------------
   
  'Watchdog' library for LPC1769.

  ** Jumper controlled (see below) **

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
#include "watchdog_1769_003-0_c02a.h"

/*--------------------------------------------------------------------*-

  WATCHDOG_Init()

  Set up watchdog timer on LPC1769.

  *****************************************************************
  * Handle with care - if WDT is running, debug links may be lost *
  * In this design, WDT is enable only when jumper is inserted.   *
  *****************************************************************
  
  The watchdog timer is driven by the Internal RC Oscillator:
  the minimum available timeout is 256 usec.

-*--------------------------------------------------------------------*/

void WATCHDOG_Init(const uint32_t WDT_TIMEOUTus)
   {
   uint32_t wdt_ticks = 0;
   uint32_t Jumper_input;

   // *If* WDT jumper is in place, we start the WDT

   // Read WDT jumper setting
   // - set up jumper pin for input
   // - params: Port : Pin : 1 for o/p, 0 for i/p
   GPIO_SetDir(WDT_JUMPER_PORT, WDT_JUMPER_PIN, 0);

   // Note: we only read the jumper during system init phase
   Jumper_input = (GPIO_ReadValue(WDT_JUMPER_PORT) & WDT_JUMPER_PIN);

   if (Jumper_input != WDT_JUMPER_INSERTED)
      {
      // Jumper not inserted - don't enable WDT
      return;
      }

   // If we are here, we are setting up the WDT

   // Drive WDT from internal RC timer (IRC)
   LPC_WDT->WDCLKSEL = 0x02;

   // Calculate required tick count for WDT timeout
   wdt_ticks  = (((WDT_PCLK) * WDT_TIMEOUTus) / ( WDT_US_INDEX * 4));

   // Check if tick count is within allowed range
   if ((wdt_ticks >= WDT_TIMEOUT_MIN) && (wdt_ticks <= WDT_TIMEOUT_MAX))
      {
      LPC_WDT->WDTC = wdt_ticks;
      }
   else
      {
      // We simply "stop" if WDT values are wrong
      // - other solutions may make sense for your application
      // - for example, use closest available timeout.
      while(1);
      }

   // Reset if WDT overflows
   LPC_WDT->WDMOD |= 0x02;

   // Start WDT
   LPC_WDT->WDMOD |= 0x01;

   // Feed watchdog
   WATCHDOG_Update();
   }


/*--------------------------------------------------------------------*-

  WATCHDOG_Update()

  Feed the watchdog timer.

  See Watchdog_Init() for further information.

-*--------------------------------------------------------------------*/
void WATCHDOG_Update(void)
   {
   // Feed the watchdog
   __disable_irq();  // Avoid possible interruption
   LPC_WDT->WDFEED = 0xAA;
   LPC_WDT->WDFEED = 0x55;
   __enable_irq();
   }

/*--------------------------------------------------------------------*-
  ------ END OF FILE -------------------------------------------------
-*--------------------------------------------------------------------*/
