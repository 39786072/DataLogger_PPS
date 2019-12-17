/*--------------------------------------------------------------------*-

  ttc_sch_systick_mipi_1769_003-0_c15a.c (Released 2015-02)

  --------------------------------------------------------------------

  Time-Triggered Co-operative (TTC) task scheduler for LPC1769.

  Supports Tasks Contracts.
  Supports Resource Barriers (lightweight version here)
  Supports MoniTTor (internal).
  Supports PredicTTor (internal).

  Scheduler data are duplicated / inverted (generic solution).
  
  *** Supports fault-injection mechanism - for test / demo only ***

  See "ERES (LPC1769)" (Chapter 15) for further information.

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
#include <lpc17xx_clkpwr.h>
// Fault-injection support (for demo purposes only)
#include "../fault_injection/fault_injection_1769_003-0_c15a.h"

// ------ Public variable definitions ------------------------------

// No global fault code employed in this scheduler

// ------ Private variable definitions -----------------------------

// The array of tasks (and inverted copy)
// Check array size in scheduler header file
sTask SCH_tasks_G[SCH_MAX_TASKS];
sTask SCH_tasks_iG[SCH_MAX_TASKS];

// The current tick count (and inverted copy)
static volatile uint32_t Tick_count_G = 0;
static volatile uint32_t Tick_count_iG = ~0;

// Flag indicating whether the scheduler is running (& inv copy)
static volatile uint32_t Sch_running_G = 0;
static volatile uint32_t Sch_running_iG = ~0;

// ------ Private function prototypes ------------------------------

static void SCH_Go_To_Sleep(void);
void SysTick_Handler(void);

/*------------------------------------------------------------------*-

  SCH_Init()

  Scheduler initialisation function.  Prepares scheduler
  data structures and sets up timer interrupts every TICKms
  milliseconds.

  You must call this function before using the scheduler.  

  [Required_SystemCoreClock frequency can be found in main.h.]

-*------------------------------------------------------------------*/
void SCH_Init(const uint32_t TICKms)
   {
   uint32_t i;

   // Initialise tick count
   Tick_count_G = 0;
   Tick_count_iG = ~Tick_count_G;

   // Clear task array
   for (i = 0; i < SCH_MAX_TASKS; i++) 
      {
      SCH_tasks_G[i].pTask = 0;
      SCH_tasks_iG[i].pTask = (uint32_t (*) (void)) (~0);
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

   // Now to set up SysTick timer for "ticks" at interval TICKms
   if (SysTick_Config(TICKms * SystemCoreClock / 1000))
      {
      // Fatal fault
      SYSTEM_Perform_Safe_Shutdown();
      }

   // Timer is started by SysTick_Config():
   // we need to disable SysTick timer and SysTick interrupt until
   // all tasks have been added to the schedule.
   SysTick->CTRL &= 0xFFFFFFFC;

   // Configure the internal MoniTTor 
   MONITTOR_I_Init();

   // Configure the internal PredicTTor 
   PREDICTTOR_I_Init();
   
   // Configure the interrupt priorities (BEGIN)

   // We have interrupts for both scheduler and MoniTTor
   // => we must set up interrupt priorities

   // Two groups of interrupts
   NVIC_SetPriorityGrouping(2);

   // Set default priorities for all interrupt sources
   for (i = 0; i < 35; i++)
      {
      NVIC_SetPriority(i, GROUP_PRIORITY_DEFAULT);
      }

   // Ensure that MoniTTor (T0) has higher priority than sch. tick (SysTick)
   NVIC_SetPriority(TIMER0_IRQn, GROUP_PRIORITY_MONITTOR);
   NVIC_SetPriority(SysTick_IRQn, GROUP_PRIORITY_TTC);
 
   // Set up interrupt priorities (END)
   }

/*------------------------------------------------------------------*-

  SCH_Start()

  Starts the scheduler, by enabling SysTick interrupt.

  NOTES: 
  * All tasks must be added before starting scheduler.
  * Any other interrupts MUST be synchronised to this tick.  

-*------------------------------------------------------------------*/
void SCH_Start(void) 
   {
   // Enable SysTick timer
   SysTick->CTRL |= 0x01;

   // Enable SysTick interrupt
   SysTick->CTRL |= 0x02;
   
   // Set flag to indicate scheduler is running
   Sch_running_G = 1;
   Sch_running_iG = ~Sch_running_G;
   }

/*------------------------------------------------------------------*-

  SysTick_Handler()

  [Function name determined by CMIS standard.]

  This is the scheduler ISR.  It is called at a rate 
  determined by the timer settings in the SCH_Init() function.

-*------------------------------------------------------------------*/
void SysTick_Handler(void)
   {
   // Check integrity of scheduler variables
   // Note: task arrays are checked in dispatcher
   if ((Tick_count_G != (~Tick_count_iG)) ||
       (Sch_running_G != (~Sch_running_iG)))
      {
      SYSTEM_MODE_Change_Mode_Fault(FAULT_RESOURCE);
      }

	// ---

   // Increment tick count (only)
   Tick_count_G++;
   Tick_count_iG = ~Tick_count_G;

   // No checks for system overloads
   // => MoniTTor assumed to deal with this.
   } 

/*------------------------------------------------------------------*-

  SCH_Dispatch_Tasks()

  This is the 'dispatcher' function.  When a task (function)
  is due to run, SCH_Dispatch_Tasks() will run it.
  This function must be called (repeatedly) from the main loop.

  [Checks data integrity throughout the function.]

-*------------------------------------------------------------------*/
void SCH_Dispatch_Tasks(void) 
   {
   uint32_t Index;
   uint32_t Update_required = 0;
   uint32_t Status;

   __disable_irq();  // Protect shared resource (Tick_count_G)
   
   // Note: Integrity of Tick_count_G is checked in scheduler ISR
   if (Tick_count_G > 0)
      {
      Tick_count_G--;
      Tick_count_iG = ~Tick_count_G;
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
            // Check data integrity
            if (((uint32_t) SCH_tasks_G[Index].pTask 
                 != (~ (uint32_t) SCH_tasks_iG[Index].pTask)) ||
                 (SCH_tasks_G[Index].Delay  
                 != (~(SCH_tasks_iG[Index].Delay))) ||
                 (SCH_tasks_G[Index].Period 
                 != (~(SCH_tasks_iG[Index].Period))) ||
                 (SCH_tasks_G[Index].WCET   
                 != (~(SCH_tasks_iG[Index].WCET))) ||
                 (SCH_tasks_G[Index].BCET   
                 != (~(SCH_tasks_iG[Index].BCET))))
               {
               SYSTEM_MODE_Change_Mode_Fault(FAULT_RESOURCE);
               }

            // Decrement the delay value
            SCH_tasks_G[Index].Delay--;
            SCH_tasks_iG[Index].Delay = ~(SCH_tasks_G[Index].Delay);

            if (SCH_tasks_G[Index].Delay == 0)
               {
               // Update the PredicTTor (about to start task)
               PREDICTTOR_I_Check_Task_Sequence(Index);
 
               // Start the internal MoniTTor
               MONITTOR_I_Start(Index,
                                SCH_tasks_G[Index].WCET,
                                SCH_tasks_G[Index].BCET,
                                50); // 50 microsec margin

               // Run the task
               Status = (*SCH_tasks_G[Index].pTask)();

               // Stop the MoniTTor: this version triggers 
               // a mode change in the event of a fault
               // => no return value.
               MONITTOR_I_Stop();

               if (Status == RETURN_FAULT)
                  {
                  // Force mode change
                  SYSTEM_MODE_Change_Mode_Fault(Index);  
                  }

               // All tasks are periodic in this design
               // - schedule task to run again
               SCH_tasks_G[Index].Delay = SCH_tasks_G[Index].Period;
               SCH_tasks_iG[Index].Delay = ~(SCH_tasks_G[Index].Delay);
               }
            }         
         } 

      __disable_irq();
      if (Tick_count_G > 0)
         {
         Tick_count_G--;
         Tick_count_iG = ~Tick_count_G;
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

/*------------------------------------------------------------------*-

  SCH_Add_Task()

  Causes a task (function) to be executed periodically.
  
  *** Instrumented to support fault injection ***
  *** FOR TEST / DEMO PURPOSES ONLY ***
  *** Not intended for use in finished system ***

  pTask  - The name of the task (function) to be scheduled.
           NOTE: All scheduled functions must be
           'uint32_t (void)'
                   
  DELAY  - The interval (TICKS) before the task is first executed

  PERIOD - Task period (in ticks).  Must be > 0.

  WCET   - Worst-Case Execution Time (microseconds)

  BCET   - Best-Case Execution Time (microseconds)

  ---
  
  FAULT RESPONSE:
  
  If an attempt is made to add too many tasks to the schedule, 
  or to add a "one-shot" task, or to add tasks while the scheduler 
  is already running, then a change to FAIL_SILENT mode is forced.

-*------------------------------------------------------------------*/
void SCH_Add_Task(uint32_t (*pTask)(void),
                  const uint32_t DELAY,
                  const uint32_t PERIOD,
                  const uint32_t WCET,
                  const uint32_t BCET)
   {
   uint32_t Index = 0;

   // Check data integrity
   if (Sch_running_G != (~Sch_running_iG))
      {
      SYSTEM_MODE_Change_Mode_Fault(FAULT_RESOURCE);
      }

   // This check is required *only* to support initial test / demo
   if (!INJ_TASK_SEQUENCE_FAULT) 
      {   
      if (Sch_running_G == 1)  
         {
         // Cannot add tasks while scheduler is running
         SYSTEM_MODE_Change_Mode_Fault(FAULT_SCH_RUNNING);
         }
      }
      
   // First find a gap in the array (if there is one)
   while ((SCH_tasks_G[Index].pTask != 0) && (Index < SCH_MAX_TASKS))
      {
      Index++;
      } 
   
   // Have we reached the end of the list?   
   if (Index == SCH_MAX_TASKS)
      {
      // Task list is full
      // => can't add the task
      SYSTEM_MODE_Change_Mode_Fault(FAULT_SCH_TOO_MANY_TASKS);
      }

   // Check for "one shot" tasks
   if (PERIOD == 0)
      {
      // Attenpt to add one-shot task
      // => don't add the task
      SYSTEM_MODE_Change_Mode_Fault(FAULT_SCH_ONE_SHOT_TASK);
      }
      
    // If we're here, there is a space in the task array
    // and we are adding a periodic task
    SCH_tasks_G[Index].pTask  = pTask;
    SCH_tasks_G[Index].Delay  = DELAY + 1;
    SCH_tasks_G[Index].Period = PERIOD;
    SCH_tasks_G[Index].WCET   = WCET;
    SCH_tasks_G[Index].BCET   = BCET;

   // Store the inverted copies
   SCH_tasks_iG[Index].pTask
   = (uint32_t (*) (void))(~ (uint32_t)(SCH_tasks_G[Index].pTask));

   SCH_tasks_iG[Index].Delay  = ~(SCH_tasks_G[Index].Delay);
   SCH_tasks_iG[Index].Period = ~(SCH_tasks_G[Index].Period);
   SCH_tasks_iG[Index].WCET   = ~(SCH_tasks_G[Index].WCET);
   SCH_tasks_iG[Index].BCET   = ~(SCH_tasks_G[Index].BCET);
   }

/*------------------------------------------------------------------*-

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

-*------------------------------------------------------------------*/
void SCH_Go_To_Sleep()
   {
   // Enter sleep mode = "Wait For Interrupt"
	//__WFI();
	//CLKPWR_Sleep();
	  //CLKPWR_DeepSleep();
	CLKPWR_PowerDown();
   }

/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/
