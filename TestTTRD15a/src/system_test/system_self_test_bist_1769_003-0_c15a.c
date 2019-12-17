/*--------------------------------------------------------------------*-

  system_self_test_bist_1769_003-0_c15a.c (Released 2015-02)
  
  --------------------------------------------------------------------
 
  Part of case study (washing machine controller).
  
  This module provides a framework for various (low level)
  "built in self tests".

  See "ERES(1769)" book, Chapter 13 and Chapter 15 (details below).
  
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

// Fault-injection support (for demo purposes only)
#include "../fault_injection/fault_injection_1769_003-0_c15a.h"


/*------------------------------------------------------------------*-

  SYSTEM_SELF_TEST_Perform_BISTs()
  
  Placeholder function.

  In the final design, this function will perform a series of
  what are sometimes called "Built In Self Tests" to ensure that 
  the processor (MCU) is (still) operating correctly.
  
  These tests are performed periodically (e.g. once per second).
  
  See ERES(1769) Chapter 13 for further information.
  
-*------------------------------------------------------------------*/
uint32_t SYSTEM_SELF_TEST_Perform_BISTs(void)
   {
   static uint32_t Call_count = 0;

   // NXP Application Note AN10918 describes in detail how to 
   // perform BISTs that are in compliance with IEC 60335 on 
   // an LPC1769 MCU. 
   
   // The Application Note is accompanied by a code library.
   
   // It is expected that an implementation of this code library 
   // will be included in the final version of this design.
   
   // We will attempt to fail silently if any BISTs fail. 

   // --- Simple simulation follows ---

   // Simple fault-injection test / demo framework
   // Task called once per second
   // => injected fault after 10 seconds (if used)
   if (INJ_BIST_FAULT && (++Call_count == 10))
      {
      // Injected fault
   //   SYSTEM_MODE_Change_Mode_Fault(FAULT_BIST);
      }

   return RETURN_NORMAL;
   }

/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/
