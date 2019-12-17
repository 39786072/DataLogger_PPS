/*--------------------------------------------------------------------*-

  spi_7_seg_display.c (Released 2015-02)

  --------------------------------------------------------------------

  Simple task for display number (0-9) n 7-segment LED panel on
  "Embedded Baseboard" (Embedded Artists)

  Assumes LPC1769 Connected to the baseboard.

  (Borrows from NXP example program - thanks due to NXP Vietnam)

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
#include "spi_7_seg_display_1769_003-0_c13a.h"

// ------ Public variable declarations -----------------------------

// see ttc_sch_1769_003-0_c02a.c for definition
extern int Fault_code_G;

// See menu_1769_003-0_c13a.c for definitions
extern uint32_t Display_value_G;
extern uint32_t Display_value_iG;

// ------ Private constants ----------------------------------------

static const uint8_t segmentLUT[10] =
   {
   // FCPBAGED
   (uint8_t) ~0b11011011, // 0
   (uint8_t) ~0b01010000, // 1
   (uint8_t) ~0b00011111, // 2
   (uint8_t) ~0b01011101, // 3
   (uint8_t) ~0b11010100, // 4
   (uint8_t) ~0b11001101, // 5
   (uint8_t) ~0b11001111, // 6
   (uint8_t) ~0b01011000, // 7
   (uint8_t) ~0b11011111, // 8
   (uint8_t) ~0b11011101, // 9
   };

// ------ Private variable definitions ------------------------------

static uint8_t rxBuff[5]; 
static uint8_t txBuff[5];

static SSP_CFG_Type sspChannelConfig;
static SSP_DATA_SETUP_Type sspDataConfig;

/*--------------------------------------------------------------------*-

  SPI_7_SEG_DISPLAY_Init()

  Prepare for SPI_7_SEG_DISPLAY_Update() function - see below.

-*--------------------------------------------------------------------*/
void SPI_7_SEG_DISPLAY_Init(void)
   {
   LPC_PINCON->PINSEL0 |= 0x2<<14; // SCK1
   LPC_PINCON->PINSEL0 |= 0x2<<18; // MOSI1

   // Set up chip select pin for output
   // Params: Port : Pin : 1 for o/p, 0 for i/p
   GPIO_SetDir(LED_DISPLAY_PORT_CS, LED_DISPLAY_PIN_CS, 1);

   sspDataConfig.length = 1;
   sspDataConfig.tx_data = txBuff;
   sspDataConfig.rx_data = rxBuff;

   SSP_ConfigStructInit(&sspChannelConfig);
   SSP_Init(SSP_CHANNEL, &sspChannelConfig);
   SSP_Cmd(SSP_CHANNEL, ENABLE);
   }


/*--------------------------------------------------------------------*-

  SPI_7_SEG_DISPLAY_Update()

  Display number (0-9) on 7-segment module.

  (SPI interface.)

-*--------------------------------------------------------------------*/
void SPI_7_SEG_DISPLAY_Update(void)
   {
   // Check data integrity
   if (Display_value_G != (~Display_value_iG))
      {
      SYSTEM_Perform_Safe_Shutdown();  
      }

   // ------

   // Display data (if in range)
   if ((Display_value_G >= 0) && (Display_value_G <= 9))
      {
      // Clear CS pin
      GPIO_ClearValue(LED_DISPLAY_PORT_CS, LED_DISPLAY_PIN_CS);

      txBuff[0] = segmentLUT[Display_value_G];  // Buffer display value

      // Transfer to 7-Segment Display Driver
      SSP_ReadWrite(SSP_CHANNEL, &sspDataConfig, SSP_TRANSFER_POLLING);

      // Set CS pin to 1
      GPIO_SetValue(LED_DISPLAY_PORT_CS, LED_DISPLAY_PIN_CS);
      }
   }

/*--------------------------------------------------------------------*-
  ------ END OF FILE -------------------------------------------------
-*--------------------------------------------------------------------*/
