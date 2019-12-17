/*--------------------------------------------------------------------*-

  ttc_sch_od_t0_1769_003-0_c02c.c (Released 2015-02)

  --------------------------------------------------------------------

  Time-Triggered Co-operative (TTC) task scheduler for LPC1769.

  This version uses Timer 0 as the "tick" source.

  See "The Engineering of Reliable Embedded Systems" (Chapter 2)
  for further information about this scheduler - details below.

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
#include <lpc17xx_clkpwr.h>

// ------ Public variable definitions ------------------------------

// May be used (for example) to report faults using Heartbeat LED.
// See Heartbeat task (if used) for basic fault-reporting mechanism
uint32_t Fault_code_G;

// ------ Private variable definitions -----------------------------

// The array of tasks
// Check array size in scheduler header file
sTask SCH_tasks_G[SCH_MAX_TASKS];

// The current tick count
static volatile uint32_t Tick_count_G = 0;

// Flag indicating whether any task is running
static volatile uint32_t Task_running_G = 0;

// ------ Private function prototypes ------------------------------

static void SCH_Go_To_Sleep(void);

void SysTick_Handler(void);

/*--------------------------------------------------------------------*-

  SCH_Init()

  Scheduler initialisation function.  

  Prepares scheduler data structures and sets up timer interrupts 
  (every TICKmicroseconds microseconds).

  You must call this function before using the scheduler.  

  [Required_SystemCoreClock frequency can be found in main.h.]

-*--------------------------------------------------------------------*/
void SCH_Init(const uint32_t TICKmicroseconds)
   {
   // Used to configure Timer 0
   TIM_TIMERCFG_Type TMR0_Cfg;
   TIM_MATCHCFG_Type TMR0_Match;

   uint32_t i;

   // Reset the global fault variable
   Fault_code_G = 0;  

   for (i = 0; i < SCH_MAX_TASKS; i++) 
      {
      SCH_tasks_G[i].pTask = 0;
      }

   // Using CMSIS

   // Must check board oscillator frequency, etc
   // - see "system_lpc17xx.c" (in linked CMSIS project)
   //
   // *If* these values have been set correctly for your hardware
   // SystemCoreClock gives the system operating frequency (in Hz)
   if (SystemCoreClock != Required_SystemCoreClock)
      {
      // Fatal fault
      SYSTEM_Perform_Safe_Shutdown();
      }

   // Power-on Timer 0
   LPC_SC->PCONP |= 1 << 1;

   // Initialise Timer 0, prescale counter = 1 µs
   TMR0_Cfg.PrescaleOption = TIM_PRESCALE_USVAL;
   TMR0_Cfg.PrescaleValue = 1;

   // Use channel 0, MR0 
   TMR0_Match.MatchChannel = 0;

   // Enable interrupt when MR0 matches the value in TC register 
   TMR0_Match.IntOnMatch = ENABLE;

   // Enable reset on MR0: timer will reset if MR0 matches it 
   TMR0_Match.ResetOnMatch = TRUE;

   // Don't stop on MR0 if MR0 matches it
   TMR0_Match.StopOnMatch = FALSE;

   // Do nothing for external output pin if match 
   TMR0_Match.ExtMatchOutputType = TIM_EXTMATCH_NOTHING;

   // Tick value 
   // Set Match value, count value in microseconds in this version.
   TMR0_Match.MatchValue = TICKmicroseconds;

   // Set configuration for Tim_config and Tim_MatchConfig 
   TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &TMR0_Cfg);
   TIM_ConfigMatch(LPC_TIM0, &TMR0_Match);

   // Highest priority = Timer 0
   NVIC_SetPriority(TIMER0_IRQn, 0);
   }

/*--------------------------------------------------------------------*-

  SCH_Start()

  Starts the scheduler, by enabling Timer 0 interrupt.

  NOTES: 
  * All tasks must be added before starting scheduler.
  * Any other interrupts MUST be synchronised to this tick.  

-*--------------------------------------------------------------------*/
void SCH_Start(void) 
   {
   // Enable interrupt for Timer 0
   NVIC_EnableIRQ(TIMER0_IRQn);

   // Start Timer 0
   TIM_Cmd(LPC_TIM0, ENABLE);
   }


/*--------------------------------------------------------------------*-

  TIMER0_IRQHandler()

  [Function name determined by CMIS standard.]

  This is the scheduler ISR.  It is called at a rate 
  determined by the timer settings in the SCH_Init() function.

-*--------------------------------------------------------------------*/
void TIMER0_IRQHandler(void)
   {
   if ((LPC_TIM0->IR & 0x01) == 0x01)   // If MR0 interrupt, proceed
      {
      LPC_TIM0->IR |= 1 << 0;           // Clear MR0 interrupt flag

      // Increment tick count
      Tick_count_G++;

      // As this is a TTC scheduler, we don't usually expect
      // to have a task running when the timer ISR is called
      if (Task_running_G == 1)
         {
         // Simple fault reporting. via heartbeat / fault LED.
         // (This value is *not* reset.)
         Fault_code_G = FAULT_SCH_SYSTEM_OVERLOAD;
         }
      }
   } 


/*--------------------------------------------------------------------*-

  SCH_Dispatch_Tasks()

  This is the 'dispatcher' function.  When a task (function)
  is due to run, SCH_Dispatch_Tasks() will run it.
  This function must be called (repeatedly) from the main loop.

-*--------------------------------------------------------------------*/
void SCH_Dispatch_Tasks(void) 
   {
   uint32_t Index;
   uint32_t Update_required = 0;

   __disable_irq();  // Protect shared resource (Tick_count_G)
   if (Tick_count_G > 0)
      {
      Tick_count_G--;
      Update_required = 1;
      }
   __enable_irq();

   while (Update_required)
      {
      // Go through the task array
      for (Index = 0; Index < SCH_MAX_TASKS; Index++)
         {
         // Check if there is a task at this location
         if (SCH_tasks_G[Index].pTask)
            {
            if (--SCH_tasks_G[Index].Delay == 0)
               {
               // Set "Task_running" flag
               __disable_irq();
               Task_running_G = 1;
               __enable_irq();

               (*SCH_tasks_G[Index].pTask)();  // Run the task

               // Clear "Task_running" flag
               __disable_irq();
               Task_running_G = 0;
               __enable_irq();

               // All tasks are periodic in this design
               // - schedule task to run again
               SCH_tasks_G[Index].Delay = SCH_tasks_G[Index].Period;
               }
            }         
         } 

      __disable_irq();
      if (Tick_count_G > 0)
         {
         Tick_count_G--;
         Update_required = 1;
         }
      else
         {
         Update_required = 0;
         }
     __enable_irq();
     }

   // The scheduler may enter idle mode at this point (if used) 
   SCH_Go_To_Sleep();        
   }

/*--------------------------------------------------------------------*-

  SCH_Add_Task()

  Causes a task (function) to be executed at regular intervals.

  pTask  - The name of the task (function) to be scheduled.
           NOTE: All scheduled functions must be 'void, void' -
           that is, they must take no parameters, and have 
           a void return type (in this design).
                   
  DELAY  - The interval (ticks) before the task is first executed.

  PERIOD - Task period (in ticks).  Must be > 0.

  WCET   - Worst-Case Execution Time (microseconds)
           [Used only for documentation in this design.]

  BCET   - Best-Case Execution Time (microseconds)
           [Used only for documentation in this design.]

  RETURN VALUE:  

  Returns the position (index) in the task array at which the task 
  has been added.  
  
  If the return value is SCH_MAX_TASKS then the task could not be 
  added to the array (there was insufficient space, or the requested 
  task period was 0).
  
  If the return value is < SCH_MAX_TASKS, then the task was added
  successfully.  

  The return value may also be used to support controlled resource
  access and / or the use of backup tasks.
 
-*--------------------------------------------------------------------*/
uint32_t SCH_Add_Task(void (* pTask)(), 
                      const uint32_t DELAY,
                      const uint32_t PERIOD,
                      const uint32_t WCET,
                      const uint32_t BCET
                      )
   {
   uint32_t Return_value = 0;
   uint32_t Index = 0;
   
   // First find a gap in the array (if there is one)
   while ((SCH_tasks_G[Index].pTask != 0) && (Index < SCH_MAX_TASKS))
      {
      Index++;
      } 
   
   // Have we reached the end of the list?   
   if (Index == SCH_MAX_TASKS)
      {
      // Task list is full
      //
      // Set the global fault variable
      Fault_code_G = FAULT_SCH_TOO_MANY_TASKS;

      // Also return a fault code
      Return_value = SCH_MAX_TASKS;
      }
      
   // Check for "one shot" tasks
   // - not permitted in this design
   if (PERIOD == 0)
      {
      // Set the global fault variable
      Fault_code_G = FAULT_SCH_ONE_SHOT_TASK;

      // Also return a fault code
      Return_value = SCH_MAX_TASKS;
      }

   if (Return_value != SCH_MAX_TASKS)
      {
      // If we're here, there is a space in the task array
      // and the task to be added is periodic
      SCH_tasks_G[Index].pTask  = pTask;

      SCH_tasks_G[Index].Delay  = DELAY + 1;
      SCH_tasks_G[Index].Period = PERIOD;
      SCH_tasks_G[Index].WCET   = WCET;
      SCH_tasks_G[Index].BCET   = BCET;

      Return_value = Index;
      }

   return Return_value;
   }


/*--------------------------------------------------------------------*-

  SCH_Go_To_Sleep()

  This scheduler enters 'sleep mode' between clock ticks
  to [i] reduce tick jitter; and [ii] save power.

  The next clock tick will return the processor
  to the normal operating state.

  Note: a slight performance improvement may be achieved
  if this code is pasted into the 'Dispatch' function
  (this may be at the cost of code readability & portability)

  *** May be able to make further improvements to the jitter
  *** behaviour depending on the target hardware architecture

  *** Various power-saving options can be added
  *** (e.g. shut down unused peripherals)

-*--------------------------------------------------------------------*/
void SCH_Go_To_Sleep()
   {
   // Enter sleep mode = "Wait For Interrupt"
   __WFI();
	//CLKPWR_Sleep();
	//CLKPWR_DeepSleep();
   }

/*--------------------------------------------------------------------*-
  ------ END OF FILE -------------------------------------------------
-*--------------------------------------------------------------------*/
