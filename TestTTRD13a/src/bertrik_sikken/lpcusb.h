/*--------------------------------------------------------------------*-

  lpcusb.h (Released 2015-02)

  Header file / wrapper for Bertrik Sikken USB library.

  Intended for use with TT architectures

  Created by SafeTTy Systems Ltd.

-*--------------------------------------------------------------------*/

void VCOM_init(void);
int  VCOM_putchar(int c);
int  VCOM_getchar(void);

void USB_SERIAL_Init(void);
void USB_SERIAL_Update(void);

/*--------------------------------------------------------------------*-
  ------ END OF FILE -------------------------------------------------
-*--------------------------------------------------------------------*/
