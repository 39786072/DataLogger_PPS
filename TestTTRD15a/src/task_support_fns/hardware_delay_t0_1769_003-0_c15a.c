/*--------------------------------------------------------------------*-

  hardware_delay_t0_1769_003-0_c15a.c (Released 2015-02)

  --------------------------------------------------------------------

  Hardware delay function for LPC1769.

  This version uses Timer 0.  Delays in milliseconds.

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

// Links to CMSIS library for Timer 0
#include <lpc17xx_timer.h>


/*------------------------------------------------------------------*-

  HARDWARE_DELAY_T0_ms()

  Hardware delay (in milliseconds), based on Timer 0.

-*------------------------------------------------------------------*/
void HARDWARE_DELAY_T0_ms(const uint32_t DELAYms)
   {
   // Used to configure Timer 0
   TIM_TIMERCFG_Type TMR0_Cfg;
   TIM_MATCHCFG_Type TMR0_Match;
   
   // Timer may already be enabled / in use

   // Disable interrupt for Timer 0
   NVIC_DisableIRQ(TIMER0_IRQn);

   // Disable timer
   TIM_Cmd(LPC_TIM0, DISABLE);

   // ---

   // Now configure / reconfigure timer
   
   // Power-on Timer 0
   LPC_SC->PCONP |= 1 << 1;

   // Initialise Timer 0, prescale counter = 1 µs
   TMR0_Cfg.PrescaleOption = TIM_PRESCALE_USVAL;
   TMR0_Cfg.PrescaleValue = 1;

   // Use channel 0, MR0 
   TMR0_Match.MatchChannel = 0;

   // Set timer flag when MR0 matches the value in TC register
   TMR0_Match.IntOnMatch = ENABLE;

   // Enable reset on MR0: timer will reset if MR0 matches it 
   TMR0_Match.ResetOnMatch = TRUE;

   // Stop timer if MR0 matches it
   TMR0_Match.StopOnMatch = TRUE;

   // Do nothing for external output pin if match 
   TMR0_Match.ExtMatchOutputType = TIM_EXTMATCH_NOTHING;

   // Delay value 
   // Set Match value, count value in microseconds in this version.
   TMR0_Match.MatchValue = (DELAYms * 1000);

   // Set configuration for Tim_config and Tim_MatchConfig 
   TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &TMR0_Cfg);
   TIM_ConfigMatch(LPC_TIM0, &TMR0_Match);
   
   // Disable interrupt for Timer 0
   NVIC_DisableIRQ(TIMER0_IRQn);

   // Start Timer 0
   TIM_Cmd(LPC_TIM0, ENABLE);

   // Wait for flag to be set
   while ((LPC_TIM0->IR & 0x01) != 0x01);

   // Clear flag
   LPC_TIM0->IR |= 1;
   }

/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/
