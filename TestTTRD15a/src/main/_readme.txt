/*--------------------------------------------------------------------*-

   Time-Triggered Reference Design (TTRD15a)

   Targets LPC1769 MCU.

   Designed to run on LPCxpresso 1769 board with EA baseboard.

   [Release 2-0, January 2015]

   ------------------

   Final case study from ERES(1769) book: see Chapter 15.

   This TTRD incorporates:
   
   - A TTC task scheduler

   - Numerous task frameworks (e.g. to control the water pump)

   - A demo of a multi-mode architecture (NORMAL, LIMP_HOME, ...)
   - An illustration of the links between system modes and system states
   
   - Use of (lightweight) version of resource barriers
   - Use of a MoniTTor unit (to ensure that task execution times meet WCET / BCET limits)
   - Use of a PredicTTor unit (to monitor the task execution sequences)  

   ---

   This TTRD includes a simple fault injection framework: see Chapter 15.

   --- 

   This TTRD is accompanied by TTRD15b and TTRD15c.  

   These designs incorporate "Dry Schedulers" and were used to generate the 
   Tick Lists (Task Sequence Representations) used by the PredicTTor unit 
   in "Normal" (TTRD15b) and "Limp Home" (TTRD15c) modes.

   Please see ERES(1769) Chapter 5 for further information about 
   Dry Schedulers, and Chapter 10 (for information about PredicTTor units).

   ---

   This design is documented in the ERES(1769) book, Chapter 15.

   For further (informal) discussions about this TTRD, please visit:
   http://www.safetty.net/news/technical-blog

   ------------------

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
   colleges in courses for which a degree up to and including “MSc” 
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
