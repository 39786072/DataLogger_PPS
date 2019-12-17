/*--------------------------------------------------------------------*-

  ttc_sch_1769_003-0_c02a.c (Released 2015-02)

  --------------------------------------------------------------------

  Time-Triggered Co-operative (TTC) task scheduler for LPC1769.

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

// ------ Private function prototypes ------------------------------

static void SCH_Go_To_Sleep(void);

void SysTick_Handler(void);

/*--------------------------------------------------------------------*-

  SCH_Init()

  Scheduler initialisation function.  Prepares scheduler
  data structures and sets up timer interrupts every TICKms
  milliseconds.

  You must call this function before using the scheduler.  

  [Required_SystemCoreClock frequency can be found in main.h.]

-*--------------------------------------------------------------------*/
void SCH_Init(const uint32_t TICKms)
   {
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
   }


/*--------------------------------------------------------------------*-

  SCH_Start()

  Starts the scheduler, by enabling SysTick interrupt.

  NOTES: 
  * All tasks must be added before starting scheduler.
  * Any other interrupts MUST be synchronised to this tick.  

-*--------------------------------------------------------------------*/
void SCH_Start(void) 
   {
   // Enable SysTick timer
   SysTick->CTRL |= 0x01;

   // Enable SysTick interrupt
   SysTick->CTRL |= 0x02;
   }


/*--------------------------------------------------------------------*-

  SysTick_Handler()

  [Function name determined by CMIS standard.]

  This is the scheduler ISR.  It is called at a rate 
  determined by the timer settings in the SCH_Init() function.

-*--------------------------------------------------------------------*/
void SysTick_Handler(void)
   {
   // Increment tick count (only)
   Tick_count_G++;
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
               (*SCH_tasks_G[Index].pTask)();  // Run the task

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
  Returns the position in the task array at which the task has been 
  added.  If the return value is SCH_MAX_TASKS then the task could 
  not be added to the array (there was insufficient space, or the
  requested task period was 0).
  If the return value is < SCH_MAX_TASKS, then the task was added
  successfully.  

  Note: this return value may be used (in later designs) to
  support the use of backup tasks.
 
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
   }

/*--------------------------------------------------------------------*-
  ------ END OF FILE -------------------------------------------------
-*--------------------------------------------------------------------*/
