/*--------------------------------------------------------------------*-

  report_number_7seg_1769_003-0_c15a.c (Released 2015-02)

  ------------------------------------------------------------------

  Support functions.

  Used to report number (0 to 9) plus '-' and 'F' on 7-seg display.

  For LPC1769.  EA baseboard assumed.

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
#include "report_number_7seg_1769_003-0_c15a.h"

// ------ Private constants ----------------------------------------

static const uint8_t segmentLUT[12] =
   {
   // FCPBAGED
   (uint8_t) ~0b11011011, // 0  '0' => "Wash State 0"
   (uint8_t) ~0b01010000, // 1  '1' => ... 
   (uint8_t) ~0b00011111, // 2  '2'
   (uint8_t) ~0b01011101, // 3  '3'
   (uint8_t) ~0b11010100, // 4  '4'
   (uint8_t) ~0b11001101, // 5  '5'
   (uint8_t) ~0b11001111, // 6  '6'
   (uint8_t) ~0b01011000, // 7  '7'
   (uint8_t) ~0b11011111, // 8  '8'
   (uint8_t) ~0b11011101, // 9  '9'
   (uint8_t) ~0b00000100, // 10 '-' => "Wash finished"
   (uint8_t) ~0b10001110  // 11 'F' => Fault
   };

// ------ Private variable definitions ------------------------------

static uint8_t rxBuff[5]; 
static uint8_t txBuff[5];

static SSP_CFG_Type sspChannelConfig;
static SSP_DATA_SETUP_Type sspDataConfig;

/*------------------------------------------------------------------*/

void REPORT_NUMBER_7SEG_Init(void)
   {
   // Configure 7-segment display on EA baseboard

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

/*------------------------------------------------------------------*/

void REPORT_NUMBER_7SEG_Update(uint32_t DATA)
   {
   // Simple range checks
   if (DATA < 0 || DATA > 11)
      {
      return;
      }

   // Data are in range

   // Clear CS pin
   GPIO_ClearValue(LED_DISPLAY_PORT_CS, LED_DISPLAY_PIN_CS);

   txBuff[0] = segmentLUT[DATA];    // Display data

   // Transfer to 7-Segment Display Driver
   SSP_ReadWrite(SSP_CHANNEL, &sspDataConfig, SSP_TRANSFER_POLLING);

   // Set CS pin to 1
   GPIO_SetValue(LED_DISPLAY_PORT_CS, LED_DISPLAY_PIN_CS);
   }

/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/
