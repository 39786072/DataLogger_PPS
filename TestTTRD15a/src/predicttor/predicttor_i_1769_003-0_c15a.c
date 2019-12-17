/*--------------------------------------------------------------------*-

  predicttor_i_1769_003-0_c15a.c (Released 2015-02)

  Check task sequences.

  This version assumes internal representations of task sequences
  (one sequence for each mode).  

  This version supports asynchronous task sets.

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

// Tick list data (TSR)
#include "predicttor_i_1769_tick_list_002-0_c15a.h"

// ------ Public variable declarations -----------------------------

// The current system mode
extern uint32_t System_mode_G;

// ------ Private variable definitions -----------------------------

// Pointer to the active tick list
uint32_t* Tick_list_ptr_G;

// Length of current tick list
uint32_t Length_of_tick_list_G;

// Length of init period (async task sets)
uint32_t Length_TSIP_G;

// Flag set when system starts or mode changes
uint32_t New_list_G;

/*------------------------------------------------------------------*-

  PREDICTTOR_I_Init()

  Prepare internal PredicTTor.

-*------------------------------------------------------------------*/
void PREDICTTOR_I_Init(void)
   {
   // Avoid compiler warnings ...
   TICK_LIST_NORMAL[0] = TICK_LIST_NORMAL[0];
   TICK_LIST_LIMP_HOME[0] = TICK_LIST_LIMP_HOME[0];

   // Set up links to the correct tick list (TSR)
   switch (System_mode_G)
      {
      default:
      case FAIL_SILENT:
         {
         // Nothing to do - PredicTTor not used
         break;
         }

     case LIMP_HOME:
        {
        Tick_list_ptr_G = &TICK_LIST_LIMP_HOME[0];
        Length_of_tick_list_G = LENGTH_OF_TICK_LIST_TASKS_LIMP_HOME;
        Length_TSIP_G = LENGTH_OF_TSIP_TASKS_RELEASES_LIMP_HOME;
        break;
        }

      case NORMAL:
        {
        Tick_list_ptr_G = &TICK_LIST_NORMAL[0];
        Length_of_tick_list_G = LENGTH_OF_TICK_LIST_TASKS_NORMAL;
        Length_TSIP_G = LENGTH_OF_TSIP_TASKS_RELEASES_NORMAL;
        break;
        }
      }

   // Set flag
   New_list_G = 1;
   }

/*------------------------------------------------------------------*-

  PREDICTTOR_I_Check_Task_Sequence()

  Performs the checking.

  This version assumes synchronous task set.

-*------------------------------------------------------------------*/
void PREDICTTOR_I_Check_Task_Sequence(const uint32_t TASK_ID)
   {
   static uint32_t Tick_list_index = 0;

   // Are we in the TSIP?
   if ((New_list_G) && (Tick_list_index < Length_TSIP_G))
      {
      // Here we simply ignore data during the TSIP
      Tick_list_index++;
      }
   else
      {
      if (New_list_G)
         {
         // Completed TSIP (or this is a sync task set)
         New_list_G = 0;

         // Reset the index
         Tick_list_index = 0;
         }
      }

   // Have we completed the TSIP (if there is one)?
   if (!New_list_G)
      {
      // Check the next task ID ...
      if (Tick_list_ptr_G[Tick_list_index] != TASK_ID)
         {
         // Task doesn't match the stored sequence ...
         // => Change mode
         SYSTEM_MODE_Change_Mode_Fault(FAULT_TASK_SEQUENCE);
         }

      // Sequence is correct - update PredicTTor state
      if (++Tick_list_index == Length_of_tick_list_G)
         {
         Tick_list_index = 0;
         }
      }
   }

/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/
