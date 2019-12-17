/*--------------------------------------------------------------------*-

   main.h (Released 2015-02)

  --------------------------------------------------------------------
   
   This is the Project Header file.

   This file needs to be adapted for each project.

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

#ifndef _MAIN_H
#define _MAIN_H 1

// Links to target libraries
#include <lpc17xx.h>
#include <cr_section_macros.h>
#include <NXP/crp.h>
#include <lpc17xx_gpio.h>

// Required system operating frequency (in Hz)
// Will be checked in the scheduler initialisation file
#define Required_SystemCoreClock (100000000)

// System header
#include "../system/system_1769_003-0_c02c.h"

// Scheduler header
#include "../scheduler/ttc_sch_od_t0_1769_003-0_c02c.h"

// Port header
#include "../port/port_1769_003-0_c02a.h"

//------------------------------------------------------------------
// System fault codes
//------------------------------------------------------------------

// Scheduler fault codes
#define FAULT_SCH_TOO_MANY_TASKS (1)
#define FAULT_SCH_ONE_SHOT_TASK (2)
#define FAULT_SCH_SYSTEM_OVERLOAD (3)

//------------------------------------------------------------------
// Project constants
//------------------------------------------------------------------

#define RETURN_NORMAL 0
#define RETURN_FAULT 1

#endif

/*--------------------------------------------------------------------*-
  ------ END OF FILE -------------------------------------------------
-*--------------------------------------------------------------------*/

