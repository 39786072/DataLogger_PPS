﻿/*--------------------------------------------------------------------*-
  
   predicttor_i_1769_tick_list_002-0_c15a.h (Released 2015-02)

  --------------------------------------------------------------------

   See main PredicTTor module for details.

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

#ifndef _TICK_LIST_H
#define _TICK_LIST_H 1

// Length of the Tick Lists (in task releases) for each mode
// (No Tick List for Fail Silent mode)
#define LENGTH_OF_TICK_LIST_TASKS_NORMAL 2555
#define LENGTH_OF_TICK_LIST_TASKS_LIMP_HOME 2036

// Length of "Task Sequence Initialisation Period"
// for each mode (in task releases)
// See ERES(1769), Chapter 10, for details.
#define LENGTH_OF_TSIP_TASKS_RELEASES_NORMAL 22
#define LENGTH_OF_TSIP_TASKS_RELEASES_LIMP_HOME 6

// Tick lists
static uint32_t TICK_LIST_NORMAL[LENGTH_OF_TICK_LIST_TASKS_NORMAL] =
  {0, 14, 16, 0, 14, 0, 14, 0, 14, 0, 2, 3, 14, 0, 4, 5, 6, 14, 0, 14,
   0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 2, 3, 14, 0, 4,
   5, 6, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0,
   2, 3, 14, 0, 4, 5, 6, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14,
   0, 14, 0, 14, 0, 2, 3, 14, 0, 4, 5, 6, 14, 0, 14, 0, 14, 0, 14, 0,
   14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 2, 3, 14, 0, 4, 5, 6, 14, 0, 14,
   0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 2, 3, 14, 0, 4,
   5, 6, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0,
   2, 3, 14, 0, 4, 5, 6, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14,
   0, 14, 0, 14, 0, 2, 3, 14, 0, 4, 5, 6, 14, 0, 14, 0, 14, 0, 14, 0,
   14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 2, 3, 14, 0, 4, 5, 6, 14, 0, 14,
   0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 2, 3, 14, 0, 4,
   5, 6, 14, 0, 7, 8, 14, 0, 9, 10, 11, 14, 0, 14, 0, 14, 0, 14, 0, 14,
   0, 14, 0, 14, 0, 2, 3, 14, 0, 4, 5, 6, 14, 0, 14, 0, 14, 0, 14, 0,
   14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 2, 3, 14, 0, 4, 5, 6, 14, 0, 14,
   0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 2, 3, 14, 0, 4,
   5, 6, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0,
   2, 3, 14, 0, 4, 5, 6, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14,
   0, 14, 0, 14, 0, 2, 3, 14, 0, 4, 5, 6, 14, 0, 14, 0, 14, 0, 14, 0,
   14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 2, 3, 14, 0, 4, 5, 6, 14, 0, 14,
   0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 2, 3, 14, 0, 4,
   5, 6, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0,
   2, 3, 14, 0, 4, 5, 6, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14,
   0, 14, 0, 14, 0, 2, 3, 14, 0, 4, 5, 6, 14, 0, 14, 0, 14, 0, 14, 0,
   14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 2, 3, 14, 0, 4, 5, 6, 14, 0, 7,
   8, 14, 0, 9, 10, 11, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0,
   2, 3, 14, 0, 4, 5, 6, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14,
   0, 14, 0, 14, 0, 2, 3, 14, 0, 4, 5, 6, 14, 0, 14, 0, 14, 0, 14, 0,
   14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 2, 3, 14, 0, 4, 5, 6, 14, 0, 14,
   0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 2, 3, 14, 0, 4,
   5, 6, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0,
   2, 3, 14, 0, 4, 5, 6, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14,
   0, 14, 0, 14, 0, 2, 3, 14, 0, 4, 5, 6, 14, 0, 14, 0, 14, 0, 14, 0,
   14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 2, 3, 14, 0, 4, 5, 6, 14, 0, 14,
   0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 2, 3, 14, 0, 4,
   5, 6, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0,
   2, 3, 14, 0, 4, 5, 6, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14,
   0, 14, 0, 14, 0, 2, 3, 14, 0, 4, 5, 6, 14, 0, 7, 8, 14, 0, 9, 10,
   11, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 2, 3, 14, 0, 4,
   5, 6, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0,
   2, 3, 14, 0, 4, 5, 6, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14,
   0, 14, 0, 14, 0, 2, 3, 14, 0, 4, 5, 6, 14, 0, 14, 0, 14, 0, 14, 0,
   14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 2, 3, 14, 0, 4, 5, 6, 14, 0, 14,
   0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 2, 3, 14, 0, 4,
   5, 6, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0,
   2, 3, 14, 0, 4, 5, 6, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14,
   0, 14, 0, 14, 0, 2, 3, 14, 0, 4, 5, 6, 14, 0, 14, 0, 14, 0, 14, 0,
   14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 2, 3, 14, 0, 4, 5, 6, 14, 0, 14,
   0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 2, 3, 14, 0, 4,
   5, 6, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0,
   2, 3, 14, 0, 4, 5, 6, 14, 0, 7, 8, 14, 0, 9, 10, 11, 14, 0, 14, 0,
   14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 2, 3, 14, 0, 4, 5, 6, 14, 0, 14,
   0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 2, 3, 14, 0, 4,
   5, 6, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0,
   2, 3, 14, 0, 4, 5, 6, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14,
   0, 14, 0, 14, 0, 2, 3, 14, 0, 4, 5, 6, 14, 0, 14, 0, 14, 0, 14, 0,
   14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 2, 3, 14, 0, 4, 5, 6, 14, 0, 14,
   0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 2, 3, 14, 0, 4,
   5, 6, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0,
   2, 3, 14, 0, 4, 5, 6, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14,
   0, 14, 0, 14, 0, 2, 3, 14, 0, 4, 5, 6, 14, 0, 14, 0, 14, 0, 14, 0,
   14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 2, 3, 14, 0, 4, 5, 6, 14, 0, 14,
   0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 2, 3, 14, 0, 4,
   5, 6, 14, 0, 7, 8, 14, 0, 9, 10, 11, 14, 0, 14, 0, 14, 0, 14, 0, 14,
   0, 14, 0, 14, 0, 2, 3, 14, 0, 4, 5, 6, 14, 0, 14, 0, 14, 0, 14, 0,
   14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 2, 3, 14, 0, 4, 5, 6, 14, 0, 14,
   0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 2, 3, 14, 0, 4,
   5, 6, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0,
   2, 3, 14, 0, 4, 5, 6, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14,
   0, 14, 0, 14, 0, 2, 3, 14, 0, 4, 5, 6, 14, 0, 14, 0, 14, 0, 14, 0,
   14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 2, 3, 14, 0, 4, 5, 6, 14, 0, 14,
   0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 2, 3, 14, 0, 4,
   5, 6, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0,
   2, 3, 14, 0, 4, 5, 6, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14,
   0, 14, 0, 14, 0, 2, 3, 14, 0, 4, 5, 6, 14, 0, 14, 0, 14, 0, 14, 0,
   14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 2, 3, 14, 0, 4, 5, 6, 14, 0, 7,
   8, 14, 0, 9, 10, 11, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0,
   2, 3, 14, 0, 4, 5, 6, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14,
   0, 14, 0, 14, 0, 2, 3, 14, 0, 4, 5, 6, 14, 0, 14, 0, 14, 0, 14, 0,
   14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 2, 3, 14, 0, 4, 5, 6, 14, 0, 14,
   0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 2, 3, 14, 0, 4,
   5, 6, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0,
   2, 3, 14, 0, 4, 5, 6, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14,
   0, 14, 0, 14, 0, 2, 3, 14, 0, 4, 5, 6, 14, 0, 14, 0, 14, 0, 14, 0,
   14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 2, 3, 14, 0, 4, 5, 6, 14, 0, 14,
   0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 2, 3, 14, 0, 4,
   5, 6, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0,
   2, 3, 14, 0, 4, 5, 6, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14,
   0, 14, 0, 14, 0, 2, 3, 14, 0, 4, 5, 6, 14, 0, 7, 8, 14, 0, 9, 10,
   11, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 2, 3, 14, 0, 4,
   5, 6, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0,
   2, 3, 14, 0, 4, 5, 6, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14,
   0, 14, 0, 14, 0, 2, 3, 14, 0, 4, 5, 6, 14, 0, 14, 0, 14, 0, 14, 0,
   14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 2, 3, 14, 0, 4, 5, 6, 14, 0, 14,
   0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 2, 3, 14, 0, 4,
   5, 6, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0,
   2, 3, 14, 0, 4, 5, 6, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14,
   0, 14, 0, 14, 0, 2, 3, 14, 0, 4, 5, 6, 14, 0, 14, 0, 14, 0, 14, 0,
   14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 2, 3, 14, 0, 4, 5, 6, 14, 0, 14,
   0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 2, 3, 14, 0, 4,
   5, 6, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0,
   2, 3, 14, 0, 4, 5, 6, 14, 0, 7, 8, 14, 0, 9, 10, 11, 14, 0, 14, 0,
   14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 2, 3, 14, 0, 4, 5, 6, 14, 0, 14,
   0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 2, 3, 14, 0, 4,
   5, 6, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0,
   2, 3, 14, 0, 4, 5, 6, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14,
   0, 14, 0, 14, 0, 2, 3, 14, 0, 4, 5, 6, 14, 0, 14, 0, 14, 0, 14, 0,
   14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 2, 3, 14, 0, 4, 5, 6, 14, 0, 14,
   0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 2, 3, 14, 0, 4,
   5, 6, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0,
   2, 3, 14, 0, 4, 5, 6, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14,
   0, 14, 0, 14, 0, 2, 3, 14, 0, 4, 5, 6, 14, 0, 14, 0, 14, 0, 14, 0,
   14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 2, 3, 14, 0, 4, 5, 6, 14, 0, 14,
   0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 2, 3, 14, 0, 4,
   5, 6, 14, 0, 7, 8, 14, 0, 9, 10, 11, 14, 0, 14, 0, 14, 0, 14, 0, 14,
   0, 14, 0, 14, 0, 2, 3, 14, 0, 4, 5, 6, 14, 0, 14, 0, 14, 0, 14, 0,
   14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 2, 3, 14, 0, 4, 5, 6, 14, 0, 14,
   0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 2, 3, 14, 0, 4,
   5, 6, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0,
   2, 3, 14, 0, 4, 5, 6, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14,
   0, 14, 0, 14, 0, 2, 3, 14, 0, 4, 5, 6, 14, 0, 14, 0, 14, 0, 14, 0,
   14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 2, 3, 14, 0, 4, 5, 6, 14, 0, 14,
   0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 2, 3, 14, 0, 4,
   5, 6, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0,
   2, 3, 14, 0, 4, 5, 6, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14, 0, 14,
   0, 14, 0, 14, 0, 2, 3, 14, 0, 4, 5, 6, 14, 0, 14, 0, 14, 0, 14, 0,
   14, 0, 14, 0, 14, 0, 14, 0, 1, 14, 0, 2, 3, 14, 0, 4, 5, 6, 14, 0,
   7, 8, 14, 0, 9, 10, 11, 14, 0, 12, 13, 14, 0, 14, 15};

// Tick lists
static uint32_t TICK_LIST_LIMP_HOME[LENGTH_OF_TICK_LIST_TASKS_LIMP_HOME] =
  {0, 5, 6, 7, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5,
   0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5,
   0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5,
   0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5,
   0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5,
   0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5,
   0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5,
   0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5,
   0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5,
   0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5,
   0, 2, 3, 4, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0,
   5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0,
   5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0,
   5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0,
   5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0,
   5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0,
   5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0,
   5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0,
   5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0,
   5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0,
   5, 0, 5, 0, 2, 3, 4, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5,
   0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5,
   0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5,
   0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5,
   0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5,
   0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 7, 0, 5, 0, 5, 0, 5, 0, 5, 0,
   5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0,
   5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0,
   5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0,
   5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0,
   5, 0, 5, 0, 5, 0, 5, 0, 2, 3, 4, 5, 0, 5, 0, 5, 0, 5, 0, 5,
   0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5,
   0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5,
   0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5,
   0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5,
   0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5,
   0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5,
   0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5,
   0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5,
   0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5,
   0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 2, 3, 4, 5, 0, 5, 0, 5, 0,
   5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0,
   5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0,
   5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0,
   5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0,
   5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0,
   5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0,
   5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0,
   5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0,
   5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0,
   5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 2, 3, 4, 5, 0, 5,
   7, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0,
   5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0,
   5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0,
   5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0,
   5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0,
   5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0,
   5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0,
   5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0,
   5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0,
   5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 2, 3,
   4, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5,
   0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5,
   0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5,
   0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5,
   0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5,
   0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5,
   0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5,
   0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5,
   0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5,
   0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5,
   0, 2, 3, 4, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0,
   5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0,
   5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0,
   5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0,
   5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0,
   5, 0, 5, 0, 5, 0, 5, 7, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5,
   0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5,
   0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5,
   0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5,
   0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5,
   0, 5, 0, 5, 0, 2, 3, 4, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0,
   5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0,
   5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0,
   5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0,
   5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0,
   5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0,
   5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0,
   5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0,
   5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0,
   5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0,
   5, 0, 5, 0, 5, 0, 5, 0, 2, 3, 4, 5, 0, 5, 0, 5, 0, 5, 0, 5,
   0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5,
   0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5,
   0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5,
   0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5,
   0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5,
   0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5,
   0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5,
   0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5,
   0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5,
   0, 5, 0, 5, 0, 5, 0, 5, 0, 1, 5, 0, 2, 3, 4, 5};

#endif

/*--------------------------------------------------------------------*-
  ------ END OF FILE -------------------------------------------------
-*--------------------------------------------------------------------*/
