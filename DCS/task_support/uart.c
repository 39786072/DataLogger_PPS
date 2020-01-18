/*
 * uart.c
 *
 *  Created on: 5 de ago. de 2019
 *      Author: Santiago
 */

#include "uart.h"
#include <lpc17xx_pinsel.h>
#include <lpc17xx_uart.h>
#include <lpc17xx_gpio.h>

static void UART_Set_pin(LPC_UART_TypeDef* UART) {
	PINSEL_CFG_Type PinUartCfg;
	PinUartCfg.Pinmode = PINSEL_PINMODE_PULLUP;	// PULLUP, PULLDOWN o TRISTATE
	PinUartCfg.OpenDrain = PINSEL_PINMODE_NORMAL;	// NORMAL u OPENDRAIN

	if (UART == (LPC_UART_TypeDef*)LPC_UART0) {
		PinUartCfg.Portnum = PINSEL_PORT_0;			// 0 a 3
		PinUartCfg.Pinnum = PINSEL_PIN_2;			// 0 a 31
		PinUartCfg.Funcnum = PINSEL_FUNC_1;			// 0 a 3
		PINSEL_ConfigPin(&PinUartCfg);
		PinUartCfg.Pinnum = PINSEL_PIN_3;			// 0 a 31
	}
	if (UART == LPC_UART2) {
		PinUartCfg.Portnum = PINSEL_PORT_0;			// 0 a 3
		PinUartCfg.Pinnum = PINSEL_PIN_10;			// 0 a 31
		PinUartCfg.Funcnum = PINSEL_FUNC_1;			// 0 a 3
		PINSEL_ConfigPin(&PinUartCfg);
		PinUartCfg.Pinnum = PINSEL_PIN_11;			// 0 a 31
	}
	if (UART == LPC_UART3) {
		PinUartCfg.Portnum = PINSEL_PORT_0;			// 0 a 3
		PinUartCfg.Pinnum = PINSEL_PIN_0;			// 0 a 31
		PinUartCfg.Funcnum = PINSEL_FUNC_2;			// 0 a 3
		PINSEL_ConfigPin(&PinUartCfg);
		PinUartCfg.Pinnum = PINSEL_PIN_1;			// 0 a 31
	}
	PINSEL_ConfigPin(&PinUartCfg);
}
void UART_Configure(LPC_UART_TypeDef* UART, uint32_t BaudRate) {
// Variable de estructura de configuracion de UART
	UART_CFG_Type UartCfg;
// Luego hay que configurar el periferico UART
	UART_Set_pin(UART);
// Relleno los campos de la variable de estructura
	UartCfg.Baud_rate = BaudRate;
	UartCfg.Databits = UART_DATABIT_8;
	UartCfg.Parity = UART_PARITY_NONE;
	UartCfg.Stopbits = UART_STOPBIT_1;
// Configuro la UART3 pasandole la estructura como referencia
	UART_Init(UART, &UartCfg);
// Initialize and configure FIFO for UARTx peripheral
	UART->FCR = (UART_FCR_FIFO_EN);
	UART->FCR = (UART_FCR_FIFO_EN | UART_FCR_RX_RS | UART_FCR_TX_RS | UART_FCR_TRG_LEV2);
// Activo el transmisor
	UART_TxCmd(UART, ENABLE);
}
