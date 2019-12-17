/*--------------------------------------------------------------------*-
  
   ttc_sch_systick_mipi_1769_003-0_c15a.h (Released 2015-02)

  --------------------------------------------------------------------

   See ttc_sch_systick_mipi_1769_003-0_c15a.c for details.

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

#ifndef _SCHEDULER_H
#define _SCHEDULER_H 1

#include "../main/main.h"

// ------ Public data type declarations ----------------------------

// User-define type to store required data for each task
typedef struct 
   {
   // Pointer to the task (must be a 'uint32_t (void)' function)
   uint32_t (*pTask)(void);

   // Delay (ticks) until the task will (next) be run
   // - see SCH_Add_Task() for further details
   uint32_t Delay;       

   // Interval (ticks) between subsequent runs.
   // - see SCH_Add_Task() for further details
   uint32_t Period;

   // Worst-case execution time (microseconds)
   uint32_t WCET;

   // Best-case execution time (microseconds)
   uint32_t BCET;
   } sTask; 

// ------ Public function prototypes -------------------------------

void SCH_Init(const uint32_t TICKms);

void SCH_Start(void);

void SCH_Dispatch_Tasks(void);

void SCH_Add_Task(uint32_t (*pTask)(void),
                  const uint32_t DELAY,
                  const uint32_t PERIOD,
                  const uint32_t WCET,
                  const uint32_t BCET);

// ------ Public constants -----------------------------------------

// The maximum number of tasks required at any one time
// during the execution of the program
//
// MUST BE CHECKED FOR EACH PROJECT (*not* dynamic)
#define SCH_MAX_TASKS (20)

// Used to set interrupt priorities

// Pre-emption priority groups for IRQs (2 groups)
// Highest priority listed first
#define GROUP_PRIORITY_MONITTOR (1<<2)
#define GROUP_PRIORITY_TTC (2<<2)

#define GROUP_PRIORITY_DEFAULT ((7<<2) | 3)

#endif

/*--------------------------------------------------------------------*-
  ------ END OF FILE -------------------------------------------------
-*--------------------------------------------------------------------*/
