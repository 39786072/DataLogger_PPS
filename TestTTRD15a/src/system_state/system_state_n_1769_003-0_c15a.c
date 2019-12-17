/*--------------------------------------------------------------------*-

  system_state_n_1769_003-0_c15a.c (Released 2015-02)

  --------------------------------------------------------------------
 
  Part of case study (washing machine controller)
  
  This module (and associated task) controls the system state.
  
  This module is employed in NORMAL mode.

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

// Support functions
#include "../task_support_fns/report_number_7seg_1769_003-0_c15a.h"

// Access to MAX_DRUM_MOTOR_SPEED
#include "../tasks/drum_motor_1769_003-0_c15a.h"

// Access to functions / tasks used during init process
#include "../tasks/drum_sensor_1769_003-0_c15a.h"
#include "../tasks/water_heater_1769_003-0_c15a.h"
#include "../tasks/water_level_1769_003-0_c15a.h"
#include "../tasks/water_pump_1769_003-0_c15a.h"
#include "../tasks/water_valve_1769_003-0_c15a.h"

// ------ Public variable declarations -------------------------------

// Sensors
extern uint32_t Start_switch_pressed_G;
extern uint32_t Selector_dial_reading_G;
extern uint32_t Water_level_reading_G;
extern uint32_t Door_sensor_reading_G;
extern uint32_t Drum_speed_reading_G;
extern uint32_t Water_temperature_reading_G;

// Sensors (inverted copies)
extern uint32_t Start_switch_pressed_iG;
extern uint32_t Selector_dial_reading_iG;
extern uint32_t Water_level_reading_iG;
extern uint32_t Door_sensor_reading_iG;
extern uint32_t Drum_speed_reading_iG;
extern uint32_t Water_temperature_reading_iG;

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

// ------ Private constants ------------------------------------------

// All durations are in seconds (short times here for demo)
#define PRE_WASH_DURATION (20)
#define MAIN_WASH_DURATION (20)
#define SPIN_DURATION (10)

// ------ Private variables ------------------------------------------

static eSystem_state_n System_state_G;
static uint32_t System_state_iG;       // Inverted copy (uint32_t)

static uint32_t Time_in_state_G;
static uint32_t Time_in_state_iG;      // Inverted copy

static uint32_t Program_G;
static uint32_t Program_iG;            // Inverted copy

// Ten different programs are supported
// Each program may (or may not) use detergent
static uint32_t Detergent_G[10] = {1,1,1,0,0,1,0,1,1,0};
static uint32_t Detergent_iG[10];  // Inverted copies (set up below)

// Each program may (or may not) use hot water
static uint32_t Hot_Water_G[10] = {1,1,1,0,0,1,0,1,1,0};
static uint32_t Hot_Water_iG[10];   // Inverted copies (set up below)

/*------------------------------------------------------------------*-

  SYSTEM_STATE_N_Init()

  Prepare for SYSTEM_STATE_N_Update() task - see below.

-*------------------------------------------------------------------*/
void SYSTEM_STATE_N_Init(void)
   {
   uint32_t i;

   // Used to report current system state in this demo
   REPORT_NUMBER_7SEG_Init();

   // Configure initial system state
   System_state_G = INIT;
   System_state_iG = ~((uint32_t) System_state_G);

   // Set up other state-related variables
   Time_in_state_G = 0;
   Time_in_state_iG = ~Time_in_state_G;
   Program_G = 0;
   Program_iG = ~Program_G;

   // Set up inverted copies of arrays
   for (i = 0; i < 10; i++)
      {
      Detergent_iG[i] = ~Detergent_G[i];
      Hot_Water_iG[i] = ~Hot_Water_G[i];
      }

   // For demo purposes only
   REPORT_NUMBER_7SEG_Update((uint32_t) 11);
   }

/*------------------------------------------------------------------*-

  SYSTEM_STATE_N_Update_N().

  Main (system state) task for the washing-machine controller.

  Used in NORMAL mode.

-*------------------------------------------------------------------*/
uint32_t SYSTEM_STATE_N_Update(void)
   {
   uint32_t i;
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

   if (Program_G != ~Program_iG)
      {
      Return_value = RETURN_FAULT;
      }

   // Check array integrity (arrays)
   for (i = 0; i < 10; i++)
      {
      if (Detergent_iG[i] != ~Detergent_G[i])
         {
         Return_value = RETURN_FAULT;
         }

      if (Hot_Water_iG[i] != ~Hot_Water_G[i])
         {
         Return_value = RETURN_FAULT;
         }
      }
	  
   // ---	  

   // If we have met the preconditions we proceed
   if (Return_value == RETURN_NORMAL)
      {
      // Call once per second
      switch (System_state_G)
         {
         case INIT: // 0
            {
            // Report the state (for demo purposes only)
            REPORT_NUMBER_7SEG_Update((uint32_t) System_state_G);

            // Set up initial state (NORMAL mode)

            // Drum should not be rotating
            Drum_motor_required_speed_G = 0;
            Drum_motor_required_speed_iG = ~0;

            // Pump should be switched off
            Water_pump_required_state_G = OFF;
            Water_pump_required_state_iG = ~OFF;

            // Heater should be off
            Water_heater_required_state_G = OFF;
            Water_heater_required_state_iG = ~OFF;

            // Water valve should be closed
            Water_valve_required_state_G = CLOSED;
            Water_valve_required_state_iG = ~CLOSED;

            // Detergent hatch should be closed
            Detergent_hatch_required_state_G = CLOSED;
            Detergent_hatch_required_state_iG = ~CLOSED;

            // Door should be initially locked 
            // (until we have checked the drum state)
            Door_lock_required_state_G = LOCKED;
            Door_lock_required_state_iG = ~LOCKED; 

            // Check initial conditions (begin)

            // In this state, the drum should not be rotating
            // => we need to check this here

            // Read drum sensor
            DRUM_SENSOR_Update();

            if (Drum_speed_reading_G == 1)
               {
               // The drum is turning
               SYSTEM_MODE_Change_Mode_Fault(FAULT_DRUM_TURNING);
               }
             
            // In this state, there should not be any water in the drum
            // => we need to check this here

            // Read water level
            WATER_LEVEL_SENSOR_Update();

            if (Water_level_reading_G > 0)
               {
               // There is water in the drum
               SYSTEM_MODE_Change_Mode_Fault(FAULT_WATER_IN_DRUM);
               }

            // Check initial conditions (end)

            // We can now safely unlock the door
            Door_lock_required_state_G = UNLOCKED;
            Door_lock_required_state_iG = ~UNLOCKED;

            // We now wait until the START switch is pressed
            if ((Start_switch_pressed_G))
               {
               // Start switch has been pressed
 
               // Read the selector dial
               Program_G = Selector_dial_reading_G;
               Program_iG = ~Selector_dial_reading_G;

               // Change state
               System_state_G = START;
               System_state_iG = ~((uint32_t) System_state_G);
               }

            break;
            }

         case START: // 1
            {
            // For demo purposes only
            REPORT_NUMBER_7SEG_Update((uint32_t) System_state_G);

            // Lock the door
            Door_lock_required_state_G = LOCKED;
            Door_lock_required_state_iG = ~LOCKED;
            
            // We assume that door can still be shut (but not opened)
            // when the lock is engaged
            
            // Wait in this state until the door is securely shut ...
            if (Door_sensor_reading_G == CLOSED)
               {
               // Release the detergent (if any)
               if (Detergent_G[Program_G] == 1)
                  {
                  Detergent_hatch_required_state_G = OPEN;
                  Detergent_hatch_required_state_iG = ~OPEN;
                  }

               // Ready to go to next state
               System_state_G = FILL_DRUM;
               System_state_iG = ~((uint32_t) System_state_G);

               Time_in_state_G = 0;
               Time_in_state_iG = ~0;
               }

            break;
            }

         case FILL_DRUM: // 2
            {
            // For demo purposes only
            REPORT_NUMBER_7SEG_Update((uint32_t) System_state_G);

            // Check the door sensor
            if (Door_sensor_reading_G == OPEN)
               {
               // Door *must* be closed:
               // ** Potentially dangerous fault **
               SYSTEM_MODE_Change_Mode_Fault(FAULT_DOOR);
               }

            // Open the water valve
            Water_valve_required_state_G = OPEN;
            Water_valve_required_state_iG = ~OPEN;
            
            // Keep track of time in state
            Time_in_state_iG = ~(++Time_in_state_G);    
 
            // Wait until drum is full (with timeout)
            if (Time_in_state_G >= MAX_DRUM_FILL_DURATION)
               {
               // Should have filled the drum by now...
               SYSTEM_MODE_Change_Mode_Fault(FAULT_FILL_DRUM);
               }

            // Check the water level
            if (Water_level_reading_G == 2)
               {
               // Drum is full

               // Stop filling the drum
               Water_valve_required_state_G = CLOSED;
               Water_valve_required_state_iG = ~CLOSED;

               // Does the program require hot water?
               if (Hot_Water_G[Program_G] == 1)
                  {
                  // Ready to go to next state
                  System_state_G = HEAT_WATER;
                  System_state_iG = ~((uint32_t) System_state_G);
                  Time_in_state_G = 0;
                  Time_in_state_iG = ~0;
                  }
               else
                  {
                  // Using cold water only
                  // Ready to go to next state
                  System_state_G = PRE_WASH;
                  System_state_iG = ~((uint32_t) System_state_G);
                  Time_in_state_G = 0;
                  Time_in_state_iG = ~0;
                  }
               }
            break;
            }

         case HEAT_WATER: // 3
            {
            // For demo purposes only
            REPORT_NUMBER_7SEG_Update((uint32_t) System_state_G);

            // Check the door sensor
            if (Door_sensor_reading_G == OPEN)
               {
               // Door *must* be closed:
               // ** Potentially dangerous fault **
               SYSTEM_MODE_Change_Mode_Fault(FAULT_DOOR);
               }

            // Turn on water heater
            Water_heater_required_state_G = ON;
            Water_heater_required_state_iG = ~ON;

            // Keep track of time in state
            Time_in_state_iG = ~(++Time_in_state_G);    
            
            // Wait until water is hot (with timeout)
            if (Time_in_state_G >= MAX_WATER_HEAT_DURATION)
               {
               // Water should be hot by now...
               SYSTEM_MODE_Change_Mode_Fault(FAULT_HEAT_WATER);
               }

            // Check the water temperature
            if (Water_temperature_reading_G == 1)
               {
               // Water is at required temperature

               // Turn off the water heater
               Water_heater_required_state_G = OFF;
               Water_heater_required_state_iG = ~OFF;

               // Ready to go to next state
               System_state_G = PRE_WASH;
               System_state_iG = ~((uint32_t) System_state_G);
               Time_in_state_G = 0;
               Time_in_state_iG = ~0;
               }

            break;
            }

         case PRE_WASH: // 4
            {
            // For demo purposes only
            REPORT_NUMBER_7SEG_Update((uint32_t) System_state_G);

            // Check the door sensor
            if (Door_sensor_reading_G == OPEN)
               {
               // Door *must* be closed:
               // ** Potentially dangerous fault **
               SYSTEM_MODE_Change_Mode_Fault(FAULT_DOOR);
               }

            // In this state, the drum is slowly rotated at medium speed
            Drum_motor_required_speed_G = MAX_DRUM_MOTOR_SPEED / 2;
            Drum_motor_required_speed_iG = ~Drum_motor_required_speed_G;

            // Keep track of time in state
            Time_in_state_iG = ~(++Time_in_state_G);    
            
            if (Time_in_state_G >= PRE_WASH_DURATION)
               {
               // Turn off the drum motor
               Drum_motor_required_speed_G = 0;
               Drum_motor_required_speed_iG = ~0;

               System_state_G = MAIN_WASH;
               System_state_iG = ~((uint32_t) System_state_G);
               Time_in_state_G = 0;
               Time_in_state_iG = ~0;
               }

            break;
            }

         case MAIN_WASH: // 5
            {
            // For demo purposes only
            REPORT_NUMBER_7SEG_Update((uint32_t) System_state_G);

            // Check the door sensor
            if (Door_sensor_reading_G == OPEN)
               {
               // Door *must* be closed:
               // ** Potentially dangerous fault **
               SYSTEM_MODE_Change_Mode_Fault(FAULT_DOOR);
               }

            // In this state, the drum is rotated at higher speed
            Drum_motor_required_speed_G = (MAX_DRUM_MOTOR_SPEED * 7) / 10;
            Drum_motor_required_speed_iG = ~Drum_motor_required_speed_G;

            // Keep track of time in state
            Time_in_state_iG = ~(++Time_in_state_G);    
            
            if (Time_in_state_G >= MAIN_WASH_DURATION)
               {
               // Turn off the drum motor
               Drum_motor_required_speed_G = 0;
               Drum_motor_required_speed_iG = ~0;

               System_state_G = DRAIN_DRUM;
               System_state_iG = ~((uint32_t) System_state_G);
               Time_in_state_G = 0;
               Time_in_state_iG = ~0;
               }

            break;
            }

         case DRAIN_DRUM: // 6
            {
            // For demo purposes only
            REPORT_NUMBER_7SEG_Update((uint32_t) System_state_G);

            // Check the door sensor
            if (Door_sensor_reading_G == OPEN)
               {
               // Door *must* be closed:
               // ** Potentially dangerous fault **
               SYSTEM_MODE_Change_Mode_Fault(FAULT_DOOR);
               }

            // Pump is activated to drain drum
            Water_pump_required_state_G = ON;
            Water_pump_required_state_iG = ~ON;

            // Drum is rotated at low speed
            Drum_motor_required_speed_G = MAX_DRUM_MOTOR_SPEED / 10;
            Drum_motor_required_speed_iG = ~Drum_motor_required_speed_G;

            // Keep track of time in state
            Time_in_state_iG = ~(++Time_in_state_G);    
            
            // Wait until water has drained (with timeout)
            if (Time_in_state_G >= MAX_DRUM_DRAIN_DURATION)
               {
               // Water should have drained by now...
               SYSTEM_MODE_Change_Mode_Fault(FAULT_DRAIN_DRUM);
               }           
             
            // Check the water level
            if (Water_level_reading_G == 0)
               {
               // Drum is empty

               // Turn off the pump
               Water_pump_required_state_G = OFF;
               Water_pump_required_state_iG = ~OFF;

               // Turn off the drum motor
               Drum_motor_required_speed_G = 0;
               Drum_motor_required_speed_iG = ~0;

               // Move to next state
               System_state_G = SPIN;
               System_state_iG = ~((uint32_t) System_state_G);
               Time_in_state_G = 0;
               Time_in_state_iG = ~0;
               }

            break;
            }

         case SPIN: // 7
            {
            // For demo purposes only
            REPORT_NUMBER_7SEG_Update((uint32_t) System_state_G);

            // Check the door sensor
            if (Door_sensor_reading_G == OPEN)
               {
               // Door *must* be closed (particularly in SPIN mode)
               // ** Potentially dangerous fault **
               SYSTEM_MODE_Change_Mode_Fault(FAULT_DOOR);
               }

            // Drum is rotated at high speed
            Drum_motor_required_speed_G = MAX_DRUM_MOTOR_SPEED;
            Drum_motor_required_speed_iG = ~Drum_motor_required_speed_G;

            // Keep track of time in state
            Time_in_state_iG = ~(++Time_in_state_G);    
            
            if (Time_in_state_G >= SPIN_DURATION)
               {
               // Turn off the drum motor
               Drum_motor_required_speed_G = 0;
               Drum_motor_required_speed_iG = ~0;

               System_state_G = FINISHED;
               System_state_iG = ~((uint32_t) System_state_G);
               Time_in_state_G = 0;
               Time_in_state_iG = ~0;
               }

            break;
            }

         default:
         case FINISHED: // -
            {
            // For demo purposes only
            REPORT_NUMBER_7SEG_Update((uint32_t) 10);

            // Set up safe state
            Door_lock_required_state_G = LOCKED;
            Door_lock_required_state_iG = ~LOCKED;
            Drum_motor_required_speed_G = 0;
            Drum_motor_required_speed_iG = ~0;
            Water_pump_required_state_G = OFF;
            Water_pump_required_state_iG = ~OFF;
            Water_heater_required_state_G = OFF;
            Water_heater_required_state_iG = ~OFF;
            Water_valve_required_state_G = CLOSED;
            Water_valve_required_state_iG = ~CLOSED;
            Detergent_hatch_required_state_G = CLOSED;
            Detergent_hatch_required_state_iG = ~CLOSED;
            
            // Keep track of time in state
            Time_in_state_iG = ~(++Time_in_state_G);    

            // Don't unlock the door immediately
            // (to ensure drum stopped, etc)
            // Timing in seconds
            if (Time_in_state_G >= MAX_DRUM_STOP_DURATION)
               {
               if (Drum_speed_reading_G == 1)
                  {
                  // The drum is still turning - serious fault
                  SYSTEM_MODE_Change_Mode_Fault(FAULT_DRUM_TURNING);
                  }

               // Unlock the door
               Door_lock_required_state_G = UNLOCKED;
               Door_lock_required_state_iG = ~UNLOCKED;

               // We are in NORMAL mode
               // => return to Init state (ready for next wash)
               System_state_G = INIT;
               System_state_iG = ~((uint32_t) System_state_G);
               Time_in_state_G = 0;
               Time_in_state_iG = ~0;
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
