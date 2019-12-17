/*--------------------------------------------------------------------*-

   watchdog_1769_003-0_c02a.h (Released 2015-02)

  ------------------------------------------------------------------
   
   - See watchdog_1769_003-0_c02a.c for details.

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

#ifndef _WDT_H
#define _WDT_H 1

#include <lpc17xx_wdt.h>

// ------ Public constants -----------------------------------------

// From NXP

/** Define divider index for microsecond ( us ) */
#define WDT_US_INDEX ((uint32_t)(1000000))

/** WDT Time out minimum value */
#define WDT_TIMEOUT_MIN ((uint32_t)(0xFF))

/** WDT Time out maximum value */
#define WDT_TIMEOUT_MAX ((uint32_t)(0xFFFFFFFF))

// Jumper connections
// WDT will only be enabled if jumper is inserted
// (connecting the jumper pin to ground)
// - see Port Header for jumper pin details.
#define WDT_JUMPER_INSERTED (0)
#define WDT_PCLK (4000000)

// ------ Public function prototypes -------------------------------

void WATCHDOG_Init(const uint32_t);
void WATCHDOG_Update(void);

#endif

/*--------------------------------------------------------------------*-
  ------ END OF FILE -------------------------------------------------
-*--------------------------------------------------------------------*/

