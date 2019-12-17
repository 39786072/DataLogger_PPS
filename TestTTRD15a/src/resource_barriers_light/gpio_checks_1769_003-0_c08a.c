/*--------------------------------------------------------------------*-

  gpio_checks_1769_003-0_c08a.c (Released 2015-02)

  ------------------------------------------------------------------

  Check functions for GPIO on LPC1769.

  [Low-level code was adapted from NXP examples.]

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

// Module header
#include "gpio_checks_1769_003-0_c08a.h"


// ------ Private function prototypes ------------------------------

static LPC_GPIO_TypeDef* GPIO_Get_Port_Ptr(const uint8_t);

/*--------------------------------------------------------------------*-

  GPIO_Get_Port_Ptr()

  Translate port number into port pointer (for use with GPIO fns).

  Port must in the range 0-4.

-*--------------------------------------------------------------------*/

static LPC_GPIO_TypeDef* GPIO_Get_Port_Ptr(const uint8_t PORT_NUMBER)
   {
   LPC_GPIO_TypeDef* pGPIO = NULL;

   switch (PORT_NUMBER) 
      {
      case 0:
         {
         pGPIO = LPC_GPIO0;
         break;
         }
      case 1:
         {
         pGPIO = LPC_GPIO1;
         break;
         }
      case 2:
         {
         pGPIO = LPC_GPIO2;
         break;
         }
      case 3:
         {
         pGPIO = LPC_GPIO3;
         break;
         }
      case 4:
         {
         pGPIO = LPC_GPIO4;
         break;
         }
      default:
      break;
      }

   return pGPIO;
   }

/*--------------------------------------------------------------------*-

  GPIO_Check_Output()

  Check that port pin(s) is/are correctly configured for output.

-*--------------------------------------------------------------------*/
uint32_t GPIO_Check_Output(const uint8_t PORT_NUMBER, 
                           const uint32_t REQD_BIT_VALUE)
   {
   uint32_t Current_bit_value;

   LPC_GPIO_TypeDef* pGPIO = GPIO_Get_Port_Ptr(PORT_NUMBER);

   if (pGPIO == NULL)
      {
      // Invalid port
      return RETURN_FAULT;
      }

   // Valid port - check pins are configured for output

   // Read current settings
   Current_bit_value = pGPIO->FIODIR;

   if ((REQD_BIT_VALUE & Current_bit_value) != REQD_BIT_VALUE)
      {
      return RETURN_FAULT;
      }

   return RETURN_NORMAL;
   }

/*--------------------------------------------------------------------*-

  GPIO_Check_Input()

  Check that port pin(s) is/are correctly configured for input.

-*--------------------------------------------------------------------*/
uint32_t GPIO_Check_Input(const uint8_t PORT_NUMBER,
                          const uint32_t REQD_BIT_VALUE)
   {
   uint32_t Current_bit_value;

   LPC_GPIO_TypeDef* pGPIO = GPIO_Get_Port_Ptr(PORT_NUMBER);

   if (pGPIO == NULL)
      {
      // Invalid port
      return RETURN_FAULT;
      }

   // Valid port - check pins are configured for input

   // Read current settings
   Current_bit_value = pGPIO->FIODIR;

   // All reqd bits should set to 0
   if (REQD_BIT_VALUE & Current_bit_value)
      {
      return RETURN_FAULT;
      }

   return RETURN_NORMAL;
   }


/*--------------------------------------------------------------------*-
  ------ END OF FILE -------------------------------------------------
-*--------------------------------------------------------------------*/
