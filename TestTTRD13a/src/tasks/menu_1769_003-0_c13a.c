/*--------------------------------------------------------------------*-

   menu_1769_003-0_c13a.c (Released 2015-02)

  --------------------------------------------------------------------

   Simple framework for menu-driven interface over USB serial.

   Assumes LPC1769 Connected to the baseboard.

   Builds on "USB Serial" library (Bertrik Sikken)
   and example code for menu interfaces in "Embedded C" and "PTTES".

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

#include <stdio.h>
#include <math.h>

#include "../main/main.h"
#include "../bertrik_sikken/lpcusb.h"

#include "menu_1769_003-0_c13a.h"

// ------ Public variable definitions ------------------------------

// Value to be displayed on 7-segment LED module
uint32_t Display_value_G;
uint32_t Display_value_iG;

// ------ Public variable declarations -----------------------------

// See switch_ea-sw3_1769_003-0_c13a.c for definitions
extern uint32_t Sw_pressed_G;
extern uint32_t Sw_pressed_iG;

// See system_check_mcu_temperature_1769_003-0_c13a.c for definitions
extern int32_t Current_temperature_x10_G;
extern int32_t Current_temperature_x10_iG;

// ------ Private constants -----------------------------------------

const char CHAR_MAP_G[10]
= {'0','1','2','3','4','5','6','7','8','9'};

// ------ Private variables ----------------------------------------

// If set to 1, menu is displayed
static uint32_t Show_menu_G = 1;

// ------ Private function declarations ----------------------------

void MENU_Write_String_To_Buffer(const char* const);
void MENU_Show_Menu(void);
void MENU_Perform_Task(char);
void MENU_Write_String_To_Buffer(const char* const);

/*--------------------------------------------------------------------*-

  MENU_Init()

  Initialisation.

-*--------------------------------------------------------------------*/
void MENU_Init(void)
   {
   Display_value_G = 0;
   Display_value_iG = ~Display_value_G;
   }

/*--------------------------------------------------------------------*-

  MENU_Update()

  This function is the main menu 'command processor' function.  

  Call this (say) once every 10 ms (approx.).

-*--------------------------------------------------------------------*/
void MENU_Update(void)
   {
   char Ch;

   // Check data integrity
   if (Sw_pressed_G != (~Sw_pressed_iG))
      {
      SYSTEM_Perform_Safe_Shutdown();  
      }

   if (Current_temperature_x10_G != (~Current_temperature_x10_iG))
      {
      SYSTEM_Perform_Safe_Shutdown();  
      }

   if (Display_value_G != (~Display_value_iG))
      {
      SYSTEM_Perform_Safe_Shutdown();  
      }

   // ------

   if (Show_menu_G == 1)
      {
      Show_menu_G = 0;
      MENU_Show_Menu();
      }

   Ch = VCOM_getchar();
      
   if (Ch == 'm')
      {
      Show_menu_G = 1;
      }

   if ((Ch == 'S') || (Ch == 's')
      || (Ch == 'T') || (Ch == 't')
      || ((Ch >= '0') && (Ch <= '9')))
      {
      MENU_Perform_Task(Ch);
      Show_menu_G = 1;
      }
   }


/*--------------------------------------------------------------------*-

  MENU_Show_Menu()

  Display menu options on PC screen (via serial link)
  - edit as required to meet the needs of your application.

-*--------------------------------------------------------------------*/
void MENU_Show_Menu(void)
   {
   MENU_Write_String_To_Buffer("Menu:\n");
   MENU_Write_String_To_Buffer("S   - Read SW3 level\n");
   MENU_Write_String_To_Buffer("T   - Read temperature\n");
   MENU_Write_String_To_Buffer("0-9 - Value to be displayed\n\n");
   MENU_Write_String_To_Buffer("? : ");
   }


/*--------------------------------------------------------------------*-

  MENU_Perform_Task()

  Perform the required user task
  - edit as required to match the needs of your application.

-*--------------------------------------------------------------------*/
void MENU_Perform_Task(char c)
   {
   int32_t Temperature;
   char str[42] = "Current temperature is XX.Y Celsius\n\n";

   // Echo the menu option
   VCOM_putchar(c);
   VCOM_putchar('\n');

   // Perform the task
   switch (c)
      {
      case 's':
      case 'S':
         {
         // Report SW3 level
         if (Sw_pressed_G == 1)
            {
            MENU_Write_String_To_Buffer("Switch is pressed.\n\n");
            }
         else
            {
            MENU_Write_String_To_Buffer("Switch is *NOT* pressed.\n\n");
            }

         break;
         }

      case 't':
      case 'T':
         {
         // Report temperature from board
         Temperature = Current_temperature_x10_G;

         // We ignore sub-zero temperatures in this simple example ...
         if (Temperature < 0)
            {
            Temperature = 0;
            }

         str[23] = CHAR_MAP_G[Temperature / 100];
         str[24] = CHAR_MAP_G[(Temperature / 10) % 10];
         str[26] = CHAR_MAP_G[Temperature % 10];

         MENU_Write_String_To_Buffer(str);

         break;
         }


      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
         {
         Display_value_G = c - '0';
         Display_value_iG = ~Display_value_G;  
         MENU_Write_String_To_Buffer("Display updated.\n\n");
         break;
         }
      } 
   }

/*--------------------------------------------------------------------*-

  MENU_Write_String_To_Buffer()

  Copies a (null terminated) string to the character buffer.
  (The contents of the buffer are then passed over the serial link)

  STR_PTR - Pointer to the NULL-TERMINATED string.

-*--------------------------------------------------------------------*/
void MENU_Write_String_To_Buffer(const char* const STR_PTR)
   {
   uint32_t i = 0;

   while (STR_PTR[i] != '\0')
      {
      VCOM_putchar(STR_PTR[i]);
      i++;
      }
   }

/*--------------------------------------------------------------------*-
  ------ END OF FILE -------------------------------------------------
-*--------------------------------------------------------------------*/

