/*--------------------------------------------------------------------*-

  monittor_i_t0_1769_003-0_c15a.c (Released 2015-02)

  --------------------------------------------------------------------

  MoniTTor mechanism (internal) for LPC1769.  Uses Timer 0.

  See "ERES: LPC1769" (Chapter 9) for further information. 

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
#include "../monittor/monittor_i_t0_1769_003-0_c15a.h"

// LPC1769 - Timer 0
#include <lpc17xx_timer.h>

// ------ Private variable definitions -----------------------------

// Timer 0 match register
static TIM_MATCHCFG_Type TMR0_Match;

// ID of task that is currently being monitored
static uint32_t Task_ID_G;
static uint32_t Task_BCET_G;
static uint32_t Task_t_variation_G;

/*------------------------------------------------------------------*-

  MONITTOR_I_Init()

  Set up Timer 0 as MoniTTor unit.

-*------------------------------------------------------------------*/
void MONITTOR_I_Init(void)
   {
   TIM_TIMERCFG_Type TMR0_Cfg;

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

   // Start timer configuration
   TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &TMR0_Cfg);
   
   // IMPORTANT:
   // Interrupt priorities must be set in Scheduler init function
   // => see TTRD09a for example

   // [Not yet started timer]
   }

/*------------------------------------------------------------------*-

  MONITTOR_I_Start()

  Start monitoring a task (call just before task is released).

  ALLOWED_TIMING_VARIATION_us gives some flexibility in both
  WCET and BCET measurements. 

-*------------------------------------------------------------------*/
void MONITTOR_I_Start(const uint32_t TASK_ID,
		                const uint32_t TASK_WCET_us,
                      const uint32_t TASK_BCET_us,
                      const uint32_t ALLOWED_TIMING_VARIATION_us)
   {
   Task_ID_G = TASK_ID;
   Task_BCET_G = TASK_BCET_us;
   Task_t_variation_G = ALLOWED_TIMING_VARIATION_us;

   // Timeout value (in microseconds) 
   TMR0_Match.MatchValue = (TASK_WCET_us + ALLOWED_TIMING_VARIATION_us);

   // Complete timer configuration
   TIM_ConfigMatch(LPC_TIM0, &TMR0_Match);

   // Enable interrupt for Timer 0 
   NVIC_EnableIRQ(TIMER0_IRQn);

   LPC_TIM0->TCR = 0x02;   // Reset timer 
   LPC_TIM0->TCR = 0x01;   // Start timer 
   }

/*------------------------------------------------------------------*-

  MONITTOR_I_Stop()

  Stop monitoring a task (call just after task completes).

  Task has not overrun - need to check for possible underrun.

-*------------------------------------------------------------------*/
void MONITTOR_I_Stop(void)
   {
   uint32_t Execution_time_us;

   // Stop the timer
   LPC_TIM0->TCR = 0;

   // Check BCET (read timer value)
   // This value is in microseconds
   Execution_time_us = LPC_TIM0->TC;

   if ((Execution_time_us + Task_t_variation_G) < Task_BCET_G)
      {
      // Task has completed too quickly
      // => Change mode
      SYSTEM_MODE_Change_Mode_Fault(FAULT_TASK_TIMING);

      // Not storing task ID here
      // Not distinguishing BCET / WCET faults 
      // => either / both this can be done here, if required
      }
   }

/*------------------------------------------------------------------*-

  MONITTOR_I_Disable()

  Disable the MoniTTor unit.

  Typically used in the event of an unrelated fault, prior to mode
  change, to avoid MoniTTor ISR being triggered erroneously.

-*------------------------------------------------------------------*/
void MONITTOR_I_Disable(void)
   {
   // Stop the timer
   LPC_TIM0->TCR = 0;

   // Disable interrupt for Timer 0
   NVIC_DisableIRQ(TIMER0_IRQn);
   }

/*------------------------------------------------------------------*-

  TIMER0_IRQHandler()

  [Function name determined by CMIS standard.]

  This is the task overrun ISR.  

  It will be triggered only if the monitored task exceeds its 
  allowed WCET (and allowed overrun period)

-*------------------------------------------------------------------*/
void TIMER0_IRQHandler(void)
   {
   // if MR0 interrupt, proceed
   if ((LPC_TIM0->IR & 0x01) == 0x01) 
      {
      // Clear MR0 interrupt flag 
      LPC_TIM0->IR |= 1 << 0;         

      // Task has completed too slowly
      // => Change mode
      SYSTEM_MODE_Change_Mode_Fault(FAULT_TASK_TIMING);

      // Not storing task ID here
      // Not distinguishing BCET / WCET faults 
      // => either / both this can be done here, if required
      }
   }  

/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/
