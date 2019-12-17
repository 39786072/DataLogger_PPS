/*--------------------------------------------------------------------*-

  system_1769_003-0_c15a.c (Released 2015-02)
  
  --------------------------------------------------------------------
 
  Part of case study (washing machine controller).
  
  This module controls the system initialisation 
  and safe ("fail silent") shutdown.

  See "ERES(1769)" book, Chapter 15 (details below).
  
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

// Task headers
#include "../tasks/detergent_hatch_1769_003-0_c15a.h"
#include "../tasks/door_lock_1769_003-0_c15a.h"
#include "../tasks/door_sensor_1769_003-0_c15a.h"
#include "../tasks/drum_motor_1769_003-0_c15a.h"
#include "../tasks/drum_sensor_1769_003-0_c15a.h"
#include "../tasks/selector_dial_1769_003-0_c15a.h"
#include "../tasks/start_switch_1769_003-0_c15a.h"
#include "../tasks/water_heater_1769_003-0_c15a.h"
#include "../tasks/water_level_1769_003-0_c15a.h"
#include "../tasks/water_pump_1769_003-0_c15a.h"
#include "../tasks/water_temperature_1769_003-0_c15a.h"
#include "../tasks/water_valve_1769_003-0_c15a.h"

// Support functions
#include "../task_support_fns/report_number_7seg_1769_003-0_c15a.h"
#include "../task_support_fns/hardware_delay_t0_1769_003-0_c15a.h"

// ------ Public variable declarations -----------------------------

// Actuators
extern uint32_t Drum_motor_required_speed_G;
extern uint32_t Door_lock_required_state_G;
extern uint32_t Detergent_hatch_required_state_G;
extern uint32_t Water_heater_required_state_G;
extern uint32_t Water_pump_required_state_G;
extern uint32_t Water_valve_required_state_G;

// Actuators (inverted copies)
extern uint32_t Drum_motor_required_speed_iG;
extern uint32_t Door_lock_required_state_iG;
extern uint32_t Detergent_hatch_required_state_iG;
extern uint32_t Water_heater_required_state_iG;
extern uint32_t Water_pump_required_state_iG;
extern uint32_t Water_valve_required_state_iG;

/*------------------------------------------------------------------*-

  SYSTEM_Init()
  
  Perform initial checks and configure the system in the reqd mode.

-*------------------------------------------------------------------*/
void SYSTEM_Init(void)
   {
   // Perform initial system checks  
   SYSTEM_SELF_TEST_Check_Configuration();
   SYSTEM_SELF_TEST_Perform_POSTs();

   // If we are here, we appear to have an operational system
   // Now we identify and configure the required operating mode
   SYSTEM_MODE_Identify_Required_Mode();
   SYSTEM_MODE_Configure_Required_Mode();

   // This design relies on the WDT to change modes
   // => we need to check that the WDT is running
   if ((LPC_WDT->WDMOD & 0x01) == 0)
      {
      // Watchdog *not* running
      // => we try to shut down as safely as possible
      SYSTEM_Perform_Safe_Shutdown();
      }
   }
   
/*------------------------------------------------------------------*-

  SYSTEM_Perform_Safe_Shutdown()

  Attempts to move the system into a safe state.

  Display 2-digit fault code on 7-segment display on EA baseboard.

  Note: Does not return and may (if watchdog is operational) result
  in a processor reset, after which the function may be called again.

  [The rationale for this behaviour is that - after the reset -
   the system MAY be in a better position to enter a safe state.
   To avoid the possible reset, adapt the code and feed the WDT
   in the loop.]

   *** This function should *NOT* return ***

-*------------------------------------------------------------------*/
void SYSTEM_Perform_Safe_Shutdown(void)
   {
   // Used for simple fault reporting
   uint32_t i;
   uint32_t Fault_code_digit1;
   uint32_t Fault_code_digit2;
   uint32_t Fault_code_digit3;

   // Disable MoniTTor unit (to avoid erroneous fault messages)
   MONITTOR_I_Disable();

   // Used to report fault codes
   REPORT_NUMBER_7SEG_Init();
   
   // Report stored fault code (if there is one)
   if (LPC_RTC->GPREG2 == ~LPC_RTC->GPREG3)
      {
      // Copy and inverted copy match
      
      // Split code into digits
      // (we only have a 1-segment display)
      Fault_code_digit3 = (LPC_RTC->GPREG2 / 100) % 10;
      Fault_code_digit2 = (LPC_RTC->GPREG2 / 10) % 10;
      Fault_code_digit1 = LPC_RTC->GPREG2 % 10;
      }
   else
      {
      // Problem with fault code
      Fault_code_digit3 = 10;  // '-'
      Fault_code_digit2 = 10;
      Fault_code_digit1 = 10;
      }

   // Try to set up the actuators
   DOOR_LOCK_Init();
   DRUM_MOTOR_Init();
   WATER_HEATER_Init();
   WATER_PUMP_Init();
   WATER_VALVE_Init();

   // Drum may still be turning - keep the door locked (for now)
   Door_lock_required_state_G = LOCKED;
   Door_lock_required_state_iG = ~LOCKED;
   DOOR_LOCK_Update();

   // Stop the drum rotating
   Drum_motor_required_speed_G = 0;
   Drum_motor_required_speed_iG = ~0;
   DRUM_MOTOR_Update();   

   // Close the water valve
   Water_valve_required_state_G = CLOSED;
   Water_valve_required_state_iG = ~CLOSED;
   WATER_VALVE_Update();

   // Turn off the water heater
   Water_heater_required_state_G = OFF;
   Water_heater_required_state_iG = ~OFF;
   WATER_HEATER_Update();

   // Turn off the water pump
   Water_pump_required_state_G = OFF;
   Water_pump_required_state_iG = ~OFF;   
   WATER_PUMP_Update();

   // Wait ~ 12 seconds before unlocking the door
   // => drum should have stopped rotating 
   // => we assume that sensor readings may not be correct
   // Display fault code while we wait
   for (i = 0; i < 2; i++)
      {
      REPORT_NUMBER_7SEG_Update((uint32_t) 11); // 'F'
      HARDWARE_DELAY_T0_ms(1000);

      REPORT_NUMBER_7SEG_Update((uint32_t) Fault_code_digit3);
      HARDWARE_DELAY_T0_ms(1000);

      REPORT_NUMBER_7SEG_Update((uint32_t) 10);  // "-" between digits
      HARDWARE_DELAY_T0_ms(1000);

      REPORT_NUMBER_7SEG_Update((uint32_t) Fault_code_digit2);
      HARDWARE_DELAY_T0_ms(1000);

      REPORT_NUMBER_7SEG_Update((uint32_t) 10);
      HARDWARE_DELAY_T0_ms(1000);

      REPORT_NUMBER_7SEG_Update((uint32_t) Fault_code_digit1);
      HARDWARE_DELAY_T0_ms(1000);
      }

   // Unlock the door
   Door_lock_required_state_G = UNLOCKED;
   Door_lock_required_state_iG = ~UNLOCKED;
   DOOR_LOCK_Update();
  
    // Keep reporting fault code
    while(1)
       {
       REPORT_NUMBER_7SEG_Update((uint32_t) 11); // 'F'
       HARDWARE_DELAY_T0_ms(1000);

       REPORT_NUMBER_7SEG_Update((uint32_t) Fault_code_digit3);
       HARDWARE_DELAY_T0_ms(1000);

       REPORT_NUMBER_7SEG_Update((uint32_t) 10);  // "-" between digits
       HARDWARE_DELAY_T0_ms(1000);

       REPORT_NUMBER_7SEG_Update((uint32_t) Fault_code_digit2);
       HARDWARE_DELAY_T0_ms(1000);

       REPORT_NUMBER_7SEG_Update((uint32_t) 10);
       HARDWARE_DELAY_T0_ms(1000);

       REPORT_NUMBER_7SEG_Update((uint32_t) Fault_code_digit1);
       HARDWARE_DELAY_T0_ms(1000);
       }

    // Should not terminate ...
    }

/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/
