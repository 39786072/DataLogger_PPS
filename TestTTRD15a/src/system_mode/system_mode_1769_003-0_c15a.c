/*--------------------------------------------------------------------*-

  system_mode_1769_003-0_c15a.c (Released 2015-02)
  
  --------------------------------------------------------------------
 
  Part of case study (washing machine controller).
  
  This module controls the system mode.

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

// ------ Public variable definitions ------------------------------

// The current system mode
eSystem_mode System_mode_G;
uint32_t System_mode_iG;       // Inverted copy (uint32_t)

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

// Used for fault-injection model only
extern uint32_t Model_reset_G;
   
/*------------------------------------------------------------------*-

  SYSTEM_MODE_Identify_Required_Mode()

  Try to work out the cause of the system reset.
  Set the system mode accordingly.

-*------------------------------------------------------------------*/
void SYSTEM_MODE_Identify_Required_Mode(void)
   {
   // Try to recover mode and fault information (BEGIN)

   // NOTE:
   // When machine is started for the first time, there 
   // will (clearly) be no data available from the previous 
   // run.  However, in these circumstances we will start
   // from a POR (and the retrieved data won't be used).
   
   // Retrieve any system mode information from NVRAM
   uint32_t Reqd_mode = (uint32_t) LPC_RTC->GPREG0;
   uint32_t Reqd_mode_copy = ~((uint32_t) LPC_RTC->GPREG1);
   
   // Do sanity checks on mode data
   if (!((Reqd_mode == Reqd_mode_copy) &&
        ((Reqd_mode == (uint32_t) FAIL_SILENT) ||
         (Reqd_mode == (uint32_t) LIMP_HOME) ||
         (Reqd_mode == (uint32_t) NORMAL))))
         {
         // There may be a problem with the data
         // => set the system mode to "Fail Silent"
         Reqd_mode = FAIL_SILENT;
         }
   
   // Store FAIL_SILENT as default (for *next* mode):
   // this will be used in absence of any specific mode information
   // in the event of a watchdog-related system reset
   LPC_RTC->GPREG0 = (uint32_t) FAIL_SILENT;
   LPC_RTC->GPREG1 = ~((uint32_t) FAIL_SILENT);  // Inverted copy

   // Now try to retrieve any available fault codes
   
   // Retrieve any fault information from NVRAM
   uint32_t Fault_code = (uint32_t) LPC_RTC->GPREG2;  
   uint32_t Fault_code_copy = ~((uint32_t) LPC_RTC->GPREG3);

   // Do sanity checks on fault data   
   if ((Fault_code != Fault_code_copy) || 
       (Fault_code < FAULT_CODE_MIN) || 
       (Fault_code > FAULT_CODE_MAX))
      {
      // This may be the first time the machine has run
      // or there may be a problem with the data
      // or the NVRAM battery may have failed
      
      // Note: 
      // In some applications, we'd need to monitor
      // the health of the NVRAM battery, but that would 
      // be overkill here.      
      Fault_code = FAULT_UNIDENTIFIED;
      }
   
   // Try to recover mode and fault information (END)

   // If "1", reset was caused by WDT
   uint32_t WDT_flag = (LPC_SC->RSID >> 2) & 1;

   if (WDT_flag == 1)
      {
      // If we are here, reset was caused by WDT
      // (this will occur after every mode change)

      // Need to clear the WDT flag
      // or a future reset may be misinterpreted as a WDT reset
      LPC_SC->RSID &= ~(0x04);

      // Set required mode
      System_mode_G = (eSystem_mode) Reqd_mode;
      System_mode_iG = ~((uint32_t) System_mode_G);
      }
   else   
      {
      // If we are here, reset was *not* caused by WDT
      // => a POR or similar (not an intended mode change)
      // We need to try and work out what has happened

      // First we check the "CFC" jumper

      // It is intended that a Service Technician will
      // have access to the main control board and will be
      // able to insert a jumper (wire) that will control
      // the reset of the stored fault information.

      // This is the "Clear Fault Codes" (CFC) jumper.

      // To clear the fault code:
      // 1. The jumper should be inserted;
      // 2. A Power-On Reset should be performed;
      // 3. The jumper should then be removed.

      // This process will be carried out after any necessary
      // repairs have been completed.

      // We first set up the jumper pin for input
      // - params: Port : Pin : 1 for o/p, 0 for i/p
      GPIO_SetDir(CFC_JUMPER_PORT, CFC_JUMPER_PIN, 0);

      // Read the jumper
      uint32_t Jumper_input = (GPIO_ReadValue(CFC_JUMPER_PORT) & CFC_JUMPER_PIN);

      if (Jumper_input == CFC_JUMPER_INSERTED)
         {
         // Reset stored fault information with inv. copy
         LPC_RTC->GPREG2 = (uint32_t) FAULT_UNIDENTIFIED;
         LPC_RTC->GPREG3 = (uint32_t) ~((uint32_t) LPC_RTC->GPREG2);

         // Enter NORMAL mode
         System_mode_G = NORMAL;
         System_mode_iG = ~((uint32_t) System_mode_G);

         // Used for demo purposes only (resets simple model)
         Model_reset_G = 1;
         }
      else
         {
         // The CFC jumper is not set: 
         // => we need to check the fault codes

         if (Fault_code == FAULT_UNIDENTIFIED)
            {
            // Here we started up because of a POR or similar
            // => "FAULT_UNIDENTIFIED" is interpreted as "No fault"
            System_mode_G = NORMAL;
            System_mode_iG = ~((uint32_t) System_mode_G);
            
            // NOTE:
            // If we are here, the system is starting up.  
            // If the drum is spinning or there is water
            // in the machine (because of a power loss during the
            // wash cycle, for example) there won't necessarily 
            // be a fault code set.  This situation will be detected
            // and handled by the SYSTEM_STATE_x_Update() task
            // when it runs for the first time.
            }
         else
            {
            // Any other fault codes may mean that the power
            // was cycled when a fault was detected
            // => we need a Service Technician
            // => set the system mode to "Fail Silent"
            System_mode_G = FAIL_SILENT;
            System_mode_iG = ~((uint32_t) System_mode_G);
            }
         }
      }
   }

/*------------------------------------------------------------------*-

  SYSTEM_MODE_Configure_Required_Mode()

  Configure the system in the required mode.  

-*------------------------------------------------------------------*/
void SYSTEM_MODE_Configure_Required_Mode(void)
   {
   SCH_Init(1);  // Same tick interval in 'Normal' and 'LH' modes
   
   // Check data integrity
   if ((uint32_t) System_mode_G != ~System_mode_iG)
      {
      System_mode_G = FAIL_SILENT;
      System_mode_iG = ~((uint32_t) System_mode_G);
      }

   switch (System_mode_G)
      {
      default:
      case FAIL_SILENT:
         {
         // Fail as silently as possible
         SYSTEM_Perform_Safe_Shutdown();
         break;
         }

     case LIMP_HOME:
        {
        // We have moved into this state because a fault was detected
        // If the fault was caused by a power issue, we may be able to
        // "tidy up" and return to NORMAL mode.  If we can achieve
        // this, we'll move into FAIL_SILENT mode.

        // Set up WDT (timeout in *microseconds*)
        WATCHDOG_Init(1100);

        // Set up scheduler for 1 ms ticks (tick interval in *ms*)
        SCH_Init(1);

        // Prepare for periodic system temperature check
        SYSTEM_SELF_TEST_Check_MCU_temp_Init();

        // Prepare for main washer (system state) task
        SYSTEM_STATE_LH_Init();

        // Prepare for heartbeat task
        HEARTBEAT_Init();

        // Prepare for low-level washer (sensor) tasks
        DOOR_SENSOR_Init();
        DRUM_SENSOR_Init();
        WATER_LEVEL_SENSOR_Init();

        // Add tasks to schedule.
        // Parameters are:
        // 1. Task name
        // 2. Initial delay / offset (in Ticks)
        // 3. Task period (in Ticks) - set to 0 for "one shot" task
        // 4. Task WCET (in microseconds)
        // 5. Task BCET (in microseconds)

        // Add watchdog task first
        SCH_Add_Task(WATCHDOG_Update, 0, 1, 50, 0);

         // Note: first release of this task initialises 
         // various shared variables: it must run first
        SCH_Add_Task(SYSTEM_STATE_LH_Update, 0, 1000, 500, 0);

        // Add low-level washer tasks
        SCH_Add_Task(DOOR_SENSOR_Update, 1, 100, 100, 0);
        SCH_Add_Task(DRUM_SENSOR_Update, 1, 100, 100, 0);
        SCH_Add_Task(WATER_LEVEL_SENSOR_Update, 1, 100, 100, 0);

        // Perform periodic system checks
        SCH_Add_Task(SYSTEM_SELF_TEST_Check_MCU_temp, 2, 1, 50, 0);
        SCH_Add_Task(SYSTEM_SELF_TEST_Perform_BISTs, 2, 1000, 500, 0);

        // Add Heartbeat task (fast)
        SCH_Add_Task(HEARTBEAT_Update, 2, 250, 20, 0);

        // Feed the watchdog
        WATCHDOG_Update();

        break;
        }

      case NORMAL:
         {
         // Set up WDT (timeout in *microseconds*) 
         WATCHDOG_Init(1100);

         // Set up scheduler for 1 ms ticks (tick interval in *ms*)
         SCH_Init(1);
         
         // Prepare for periodic system temperature check
         SYSTEM_SELF_TEST_Check_MCU_temp_Init();

         // Prepare for main washer (system state) task
         SYSTEM_STATE_N_Init();

         // Prepare for heartbeat task
         HEARTBEAT_Init();

         // Prepare to read START switch
         SWITCH_SW3_Init();

         // Prepare for other tasks
         DETERGENT_HATCH_Init();
         SELECTOR_DIAL_Init();
         
         DOOR_LOCK_Init();
         DOOR_SENSOR_Init();        // Associated sensor

         DRUM_MOTOR_Init();
         DRUM_SENSOR_Init();        // Associated sensor

         WATER_HEATER_Init();
         WATER_TEMP_SENSOR_Init();  // Associated sensor

         WATER_PUMP_Init();
         WATER_VALVE_Init();
         WATER_LEVEL_SENSOR_Init(); // Associated sensor 

         // Add tasks to schedule.
         // Parameters are:
         // 1. Task name
         // 2. Initial delay / offset (in Ticks)
         // 3. Task period (in Ticks) - set to 0 for "one shot" task
         // 4. Task WCET (in microseconds)
         // 5. Task BCET (in microseconds)

         // Add watchdog task first
         SCH_Add_Task(WATCHDOG_Update, 0, 1, 50, 0);

         // Note: first release of this task initialises 
         // various shared variables: it must run first
         SCH_Add_Task(SYSTEM_STATE_N_Update, 0, 1000, 300, 0);

         // Door lock / door sensor released frequently
         SCH_Add_Task(DOOR_LOCK_Update, 1, 10, 100, 0);
         SCH_Add_Task(DOOR_SENSOR_Update, 1, 10, 100, 0);

         // Drum motor / drum sensor released frequently
         SCH_Add_Task(DRUM_MOTOR_Update, 2, 10, 100, 0);
         SCH_Add_Task(DRUM_SENSOR_Update, 2, 10, 100, 0);
         
         // Task to poll SW3 on EA Baseboard (Start switch)
         SCH_Add_Task(SWITCH_SW3_Update, 2, 10, 100, 0);

         // Remaining tasks are released less frequently
         SCH_Add_Task(WATER_HEATER_Update, 3, 100, 100, 0);
         SCH_Add_Task(WATER_TEMP_SENSOR_Update, 3, 100, 100, 0);

         SCH_Add_Task(WATER_PUMP_Update, 4, 100, 100, 0);
         SCH_Add_Task(WATER_VALVE_Update, 4, 100, 100, 0);
         SCH_Add_Task(WATER_LEVEL_SENSOR_Update, 4, 100, 100, 0);

         SCH_Add_Task(SELECTOR_DIAL_Update, 5, 1000, 100, 0);
         SCH_Add_Task(DETERGENT_HATCH_Update, 5, 1000, 100, 0);
         
         // Perform periodic system checks
         SCH_Add_Task(SYSTEM_SELF_TEST_Check_MCU_temp, 6, 1, 50, 0);
         SCH_Add_Task(SYSTEM_SELF_TEST_Perform_BISTs, 6, 1000, 500, 0);

         // Add Heartbeat task
         SCH_Add_Task(HEARTBEAT_Update, 7, 1000, 50, 0);

         // Feed the watchdog
         WATCHDOG_Update();

         break;
         }
      }

   WATCHDOG_Update();
   }

/*------------------------------------------------------------------*-

  SYSTEM_MODE_Change_Mode_Fault()

  Force change in the system mode in the event of a fault

-*------------------------------------------------------------------*/
void SYSTEM_MODE_Change_Mode_Fault(const int32_t FAULT)
   {
   // Disable MoniTTor unit (to avoid erroneous fault messages)
   MONITTOR_I_Disable();

   // Check data integrity
   if ((uint32_t) System_mode_G != ~System_mode_iG)
      {
      // Serious (resource) fault
      // Store fault code in NVRAM (may be reqd in new mode)
      LPC_RTC->GPREG2 = (uint32_t) FAULT_RESOURCE;
      LPC_RTC->GPREG3 = (uint32_t) ~((uint32_t) LPC_RTC->GPREG2);
      SYSTEM_MODE_Change_Mode(FAIL_SILENT);
      }
      
   // Passed data-integrity tests   
   
   // Store fault code in NVRAM (may be reqd in new mode)
   LPC_RTC->GPREG2 = FAULT;
   LPC_RTC->GPREG3 = ~FAULT;  // Inv. copy

   // We try to "tidy up" if fault *may* be caused by a power
   // glitch or by the user cycling the power.
   // These are indicated at startup by the presence of water
   // in the drum and / or the fact that the drum is turning.
   if ((FAULT == FAULT_WATER_IN_DRUM) ||
       (FAULT == FAULT_DRUM_TURNING))
      {
      // We try to "recover via the limp-home mode
      SYSTEM_MODE_Change_Mode(LIMP_HOME);
      }
   else
      {
      // In all other cases we shut down
      // (and display the fault code)
      SYSTEM_MODE_Change_Mode(FAIL_SILENT);
      }
   }

/*------------------------------------------------------------------*-

  SYSTEM_MODE_Change_Mode()

  Force change in the system mode (change to specific mode).

-*------------------------------------------------------------------*/
void SYSTEM_MODE_Change_Mode(const eSystem_mode NEW_MODE)
   {
   // Avoid any potential problems while we store data
   WATCHDOG_Update();

   // Disable MoniTTor unit (to avoid erroneous fault messages)
   MONITTOR_I_Disable();

   // Store required new system mode in NVRAM
   LPC_RTC->GPREG0 = (uint32_t) NEW_MODE;
   LPC_RTC->GPREG1 = ~((uint32_t) NEW_MODE);  // Inverted copy

   // In this design, we are using the WDT to force a reset
   // => we check (again) that the WDT is running ...
   if ((LPC_WDT->WDMOD & 0x01) == 0)
      {
      // Watchdog *not* running
      // => we try to shut down as safely as possible
      SYSTEM_Perform_Safe_Shutdown();
      }

   if (NEW_MODE == FAIL_SILENT)
      {
      // We are trying to shut down because of a fault
      // If the problems are serious, we may not manage to reset
      // => attempt to perform safe shutdown in current mode
      // Note: system reset should still take place
      // => this is a "safety net"
      SYSTEM_Perform_Safe_Shutdown();
      }

   // Force system reset (if WDT enabled)
   while(1);
   }

/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/
