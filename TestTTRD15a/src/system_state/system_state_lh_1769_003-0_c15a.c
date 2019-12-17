/*--------------------------------------------------------------------*-

  system_state_lh_1769_003-0_c15a.c (Released 2015-02)

  --------------------------------------------------------------------
 
  Part of case study (washing machine controller)
  
  This module (and associated task) controls the system state.
  
  This module is employed in LIMP_HOME mode.

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
#include "../tasks/door_lock_1769_003-0_c15a.h"
#include "../tasks/door_sensor_1769_003-0_c15a.h"
#include "../tasks/drum_motor_1769_003-0_c15a.h"
#include "../tasks/drum_sensor_1769_003-0_c15a.h"
#include "../tasks/water_heater_1769_003-0_c15a.h"
#include "../tasks/water_level_1769_003-0_c15a.h"
#include "../tasks/water_pump_1769_003-0_c15a.h"
#include "../tasks/water_temperature_1769_003-0_c15a.h"
#include "../tasks/water_valve_1769_003-0_c15a.h"

// Support functions
#include "../task_support_fns/report_number_7seg_1769_003-0_c15a.h"

// ------ Public variable declarations -------------------------------

// Sensors
extern uint32_t Door_sensor_reading_G;
extern uint32_t Drum_speed_reading_G;
extern uint32_t Water_level_reading_G;

// Sensors (inverted copies)
extern uint32_t Door_sensor_reading_iG;
extern uint32_t Drum_speed_reading_iG;
extern uint32_t Water_level_reading_iG;

// Actuators
extern uint32_t Door_lock_required_state_G;
extern uint32_t Drum_motor_required_speed_G;
extern uint32_t Water_heater_required_state_G;
extern uint32_t Water_pump_required_state_G;
extern uint32_t Water_valve_required_state_G;

// Actuators (inverted copies)
extern uint32_t Door_lock_required_state_iG;
extern uint32_t Drum_motor_required_speed_iG;
extern uint32_t Water_heater_required_state_iG;
extern uint32_t Water_pump_required_state_iG;
extern uint32_t Water_valve_required_state_iG;

// ------ Private variables ------------------------------------------

static eSystem_state_lh System_state_G;
static uint32_t System_state_iG;       // Inverted copy (uint32_t)

static uint32_t Time_in_state_G;
static uint32_t Time_in_state_iG;      // Inverted copy

/*------------------------------------------------------------------*-

  SYSTEM_STATE_LH_Init()

  Prepare for SYSTEM_STATE_Update() task - see below.

-*------------------------------------------------------------------*/
void SYSTEM_STATE_LH_Init(void)
   {
   // Used to report current system state in this demo
   REPORT_NUMBER_7SEG_Init();

   // Configure initial system state
   System_state_G = INIT_LH;
   System_state_iG = ~((uint32_t) System_state_G);

   // Set up other state-related variables
   Time_in_state_G = 0;
   Time_in_state_iG = ~Time_in_state_G;

   // For demo purposes only
   REPORT_NUMBER_7SEG_Update((uint32_t) 11);
   }

/*------------------------------------------------------------------*-

  SYSTEM_STATE_LH_Update().

  Main (system state) task for the washing-machine controller.

-*------------------------------------------------------------------*/
uint32_t SYSTEM_STATE_LH_Update(void)
   {
   uint32_t Return_value = RETURN_NORMAL;

   // Sensors and actuators check their own data integrity
   // Here we check only the system state variables
   // that are the direct responsibility of this module

   if ((uint32_t) System_state_G != ~System_state_iG)
      {
      Return_value = RETURN_FAULT;
      }

   if (Time_in_state_G != ~Time_in_state_iG)
      {
      Return_value = RETURN_FAULT;
      }
	  
   // ---	  

   // If we have met the preconditions we proceed
   if (Return_value == RETURN_NORMAL)
      {
      // Call once per second
      switch (System_state_G)
         {
         default:
         case INIT_LH: // 0
            {
            // Report the state (for demo purposes only)
            REPORT_NUMBER_7SEG_Update((uint32_t) System_state_G);

            // Set up initial state (LIMP HOME mode)

            // Our goal in this mode is to try and ensure that the drum
            // has [i] stopped rotating (if required) and / or that it has
            // been drained (again, if required).

            // We have only 2 low-level (sensing) tasks
            // In this mode, actuator functions are called directly
            // from this task, as required.

            // We first set the key actuators as required

            // Door should be locked
            //(at least until we have checked the drum state)
            DOOR_LOCK_Init();
            Door_lock_required_state_G = LOCKED;
            Door_lock_required_state_iG = ~LOCKED;
            DOOR_LOCK_Update();

            // Drum should not be rotating
            DRUM_MOTOR_Init();
            Drum_motor_required_speed_G = 0;
            Drum_motor_required_speed_iG = ~0;
            DRUM_MOTOR_Update();

            // Pump should be switched off (for now)
            WATER_PUMP_Init();
            Water_pump_required_state_G = OFF;
            Water_pump_required_state_iG = ~OFF;
            WATER_PUMP_Update();

            // Water valve should be closed
            // (if open, we won't be able to drain the drum)
            WATER_VALVE_Init();
            Water_valve_required_state_G = CLOSED;
            Water_valve_required_state_iG = ~CLOSED;
            WATER_VALVE_Update();

            // Water heater should be off
            WATER_HEATER_Init();
            Water_heater_required_state_G = OFF;
            Water_heater_required_state_iG = ~OFF;
            WATER_HEATER_Update();

            // Detergent hatch is ignored

            // ---

            // Check initial conditions (begin)

            // In this state, the drum should not be rotating
            // => we need to check this here

            // Read drum sensor (and check data integrity)
            DRUM_SENSOR_Update();

            if (Drum_speed_reading_G == 1)
               {
               // The drum is turning
               // Change state
               System_state_G = WAIT_FOR_DRUM_TO_STOP_LH;
               System_state_iG = ~((uint32_t) System_state_G);
               Time_in_state_G = 0;
               Time_in_state_iG = ~0;
               }

            if (System_state_G == INIT_LH)
               {
               // If we're here, the drum is stationary.
               // There should not be any water in the drum.
               // We check this next.

               // Read water level (and check data integrity)
               WATER_LEVEL_SENSOR_Update();

               if (Water_level_reading_G > 0)
                  {
                  // There is water in the drum
                  // Change state
                  System_state_G = DRAIN_DRUM_LH;
                  System_state_iG = ~((uint32_t) System_state_G);
                  Time_in_state_G = 0;
                  Time_in_state_iG = ~0;
                  }
               }
            // Check initial conditions (end)

            if (System_state_G == INIT_LH)
               {
               // If we are here, the drum is stationary
               // *and* it contains no water.  We can therefore
               // return to NORMAL mode
               SYSTEM_MODE_Change_Mode(NORMAL);
               }

            break;
            }

         case WAIT_FOR_DRUM_TO_STOP_LH: // 1
            {
            // For demo purposes only
            REPORT_NUMBER_7SEG_Update((uint32_t) System_state_G);

            if (Door_sensor_reading_G == OPEN)
               {
               // Door should not be open (dangerous)
               // => shut down immediately
               // Note: sensor may be wrong, but
               // it's better to play it safe ... 
               SYSTEM_MODE_Change_Mode_Fault(FAULT_DRUM_STOP_OPEN_DOOR_LH);
               }

            // Keep track of time in state
            Time_in_state_iG = ~(++Time_in_state_G);              

            // Give drum time to stop (timing in seconds)
            if (Time_in_state_G >= MAX_DRUM_STOP_DURATION)
               {
               // Drum should have stopped by now ...
               // => We move into FAIL_SILENT mode
               SYSTEM_MODE_Change_Mode_Fault(FAULT_DRUM_STOP_TIMEOUT_LH);
               }

            // Check the drum speed
            if (Drum_speed_reading_G == 0)
               {
               // The drum has stopped turning.
               // We now need to check whether the drum needs to be drained:
               // we do this by returning to INIT state
               System_state_G = INIT_LH;
               System_state_iG = ~((uint32_t) System_state_G);
               }

            break;
            }

         case DRAIN_DRUM_LH: // 2
            {
            // For demo purposes only
            REPORT_NUMBER_7SEG_Update((uint32_t) System_state_G);

            // Ignore door sensor here
            // (the sensor *may* not be operational 
            //  and drum will not be moved)

            // Pump is activated to drain drum
            Water_pump_required_state_G = ON;
            Water_pump_required_state_iG = ~ON;
            WATER_PUMP_Update();

            // Drum is NOT rotated in LIMP_HOME mode

            // Keep track of time in state
            Time_in_state_iG = ~(++Time_in_state_G);    

            // Wait until water has drained (with timeout)
            if (Time_in_state_G >= MAX_DRUM_DRAIN_DURATION)
               {
               // Water should have drained by now...
               // => Will move system into FAIL_SILENT mode
               SYSTEM_MODE_Change_Mode_Fault(FAULT_DRAIN_DRUM_TIMEOUT_LH);
               }

            // Check the water level
            if ((Water_level_reading_G == 0) 
                && (Water_level_reading_iG == ~0))
               {
               // We have emptied the drum successfully
               
               // Turn off the pump
               Water_pump_required_state_G = OFF;
               Water_pump_required_state_iG = ~OFF;

               // Return to INIT_LH state (and then to NORMAL mode)
               System_state_G = INIT_LH;
               System_state_iG = ~((uint32_t) System_state_G);
               }

            break;
            }
         }
      }

   return Return_value;
   }

/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/
