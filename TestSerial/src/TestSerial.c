/*
 ===============================================================================
 Name        : TestSerial.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
 ===============================================================================
 */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <lpc17xx_pinsel.h>
#include <lpc17xx_uart.h>
#include <lpc17xx_gpio.h>

#define HEARTBEAT_LED_PORT (0)
#define HEARTBEAT_LED_PIN (0b10000000000000000000000)

#define AT 			(uint8_t*)"AT\r\n"            //Test
#define ATRST 		"AT+RST\r\n"        //Reset
#define ATGMR		"AT+GMR\r\n"        //Check Version
#define ATCWLAP		(uint8_t*)"AT+CWLAP\r\n"      //List Avialable APs
#define ATCWJAP		"AT+CWJAP="         //Connect to AP
#define ATCWQAP		"AT+CWQAP\r\n"      //Disconects from the Ap
#define ATCIFSR		"AT+CIFSR\r\n"      //Gets the local IP address
#define ATCWLIF		"AT+CWLIF\r\n"      //Gets the Station IP to which it's connected
#define ATCWMODE	"AT+CWMODE=0\r\n"        //Sets Wi-Fi Mode
#define ATCIPMUX	"AT+CIPMUX="        //Configure multiple connections mode
#define ATCIPSTART	"AT+CIPSTART="      //Establishes connection (TCP, UDP, SSL)
#define ATCIPSTATUS	"AT+CIPSTATUS\r\n"  //Gets connection status
#define ATCIPCLOSE	"AT+CIPCLOSE"       //Closes connection (TCP,UDP,SSL)
#define ATCIPSTO	"AT+CIPSTO="        //Sets timeouts then it runs as TCP Server
#define ATCIPSERVER "AT+CIPSERVER="     //Delete/Creates a TCP server
#define ATCWSAP		"AT+CWSAP="         //Configures the ESP8266 SoftAP
#define ATCIPSEND	"AT+CIPSEND="       //Sends Data
#define IPD 		"+IPD"              //ESP Receives network data
// TODO: insert other include files here

// TODO: insert other definitions and declarations here
void Configurar_UART3() {
	// Configuro la UART3 del LPCXpresso 1769.
	// Primero se setea el comportamiento de los Pines de las lineas de Tx y Rx
	// Variable de estructura de configuracion de pin
	PINSEL_CFG_Type PinUartCfg;
	// Relleno los campos de la variable de estructura
	PinUartCfg.Portnum = PINSEL_PORT_0;			// 0 a 3
	PinUartCfg.Pinnum = PINSEL_PIN_0;			// 0 a 31
	PinUartCfg.Funcnum = PINSEL_FUNC_2;			// 0 a 3
	PinUartCfg.Pinmode = PINSEL_PINMODE_PULLUP;	// PULLUP, PULLDOWN o TRISTATE
	PinUartCfg.OpenDrain = PINSEL_PINMODE_NORMAL;	// NORMAL u OPENDRAIN

	// Configuro el pin de Tx pasandole la estructura como referencia
	PINSEL_ConfigPin(&PinUartCfg);
	// Entre la configuracion de Tx y Rx varia unicamente el numero de pin
	// por lo que reutilizando la estructura anterior se puede configurar
	PinUartCfg.Pinnum = PINSEL_PIN_1;			// 0 a 31
	// Configuro el pin de Rx pasandole la estructura como referencia
	PINSEL_ConfigPin(&PinUartCfg);
	// Luego hay que configurar el periferico UART
	// Variable de estructura de configuracion de UART
	UART_CFG_Type UartCfg;
	// Relleno los campos de la variable de estructura
	UartCfg.Baud_rate = 9600;
	UartCfg.Databits = UART_DATABIT_8;
	UartCfg.Parity = UART_PARITY_NONE;
	UartCfg.Stopbits = UART_STOPBIT_1;
	// Configuro la UART3 pasandole la estructura como referencia
	UART_Init(LPC_UART3, &UartCfg);
	// Activo el transmisor
	UART_TxCmd(LPC_UART3, ENABLE);
}
void Configurar_UART2() {
	// Configuro la UART3 del LPCXpresso 1769.
	// Primero se setea el comportamiento de los Pines de las lineas de Tx y Rx
	// Variable de estructura de configuracion de pin
	PINSEL_CFG_Type PinUartCfg;
	// Relleno los campos de la variable de estructura
	PinUartCfg.Portnum = PINSEL_PORT_0;			// 0 a 3
	PinUartCfg.Pinnum = PINSEL_PIN_10;			// 0 a 31
	PinUartCfg.Funcnum = PINSEL_FUNC_1;			// 0 a 3
	PinUartCfg.Pinmode = PINSEL_PINMODE_PULLUP;	// PULLUP, PULLDOWN o TRISTATE
	PinUartCfg.OpenDrain = PINSEL_PINMODE_NORMAL;	// NORMAL u OPENDRAIN

	// Configuro el pin de Tx pasandole la estructura como referencia
	PINSEL_ConfigPin(&PinUartCfg);
	// Entre la configuracion de Tx y Rx varia unicamente el numero de pin
	// por lo que reutilizando la estructura anterior se puede configurar
	PinUartCfg.Pinnum = PINSEL_PIN_11;			// 0 a 31
	// Configuro el pin de Rx pasandole la estructura como referencia
	PINSEL_ConfigPin(&PinUartCfg);
	// Luego hay que configurar el periferico UART
	// Variable de estructura de configuracion de UART
	UART_CFG_Type UartCfg;
	// Relleno los campos de la variable de estructura
	UartCfg.Baud_rate = 2048;
	UartCfg.Databits = UART_DATABIT_8;
	UartCfg.Parity = UART_PARITY_NONE;
	UartCfg.Stopbits = UART_STOPBIT_1;
	// Configuro la UART3 pasandole la estructura como referencia
	UART_Init(LPC_UART2, &UartCfg);
	// Activo el transmisor
	UART_TxCmd(LPC_UART2, ENABLE);
}
void Configurar_UART0() {
	// Configuro la UART3 del LPCXpresso 1769.
	// Primero se setea el comportamiento de los Pines de las lineas de Tx y Rx
	// Variable de estructura de configuracion de pin
	PINSEL_CFG_Type PinUartCfg;
	// Relleno los campos de la variable de estructura
	PinUartCfg.Portnum = PINSEL_PORT_0;			// 0 a 3
	PinUartCfg.Pinnum = PINSEL_PIN_2;			// 0 a 31
	PinUartCfg.Funcnum = PINSEL_FUNC_1;			// 0 a 3
	PinUartCfg.Pinmode = PINSEL_PINMODE_PULLUP;	// PULLUP, PULLDOWN o TRISTATE
	PinUartCfg.OpenDrain = PINSEL_PINMODE_NORMAL;	// NORMAL u OPENDRAIN

	// Configuro el pin de Tx pasandole la estructura como referencia
	PINSEL_ConfigPin(&PinUartCfg);
	// Entre la configuracion de Tx y Rx varia unicamente el numero de pin
	// por lo que reutilizando la estructura anterior se puede configurar
	PinUartCfg.Pinnum = PINSEL_PIN_3;			// 0 a 31
	// Configuro el pin de Rx pasandole la estructura como referencia
	PINSEL_ConfigPin(&PinUartCfg);
	// Luego hay que configurar el periferico UART
	// Variable de estructura de configuracion de UART
	UART_CFG_Type UartCfg;
	// Relleno los campos de la variable de estructura
	UartCfg.Baud_rate = 2048;
	UartCfg.Databits = UART_DATABIT_8;
	UartCfg.Parity = UART_PARITY_NONE;
	UartCfg.Stopbits = UART_STOPBIT_1;
	// Configuro la UART3 pasandole la estructura como referencia
	UART_Init(LPC_UART0, &UartCfg);
	// Activo el transmisor
	UART_TxCmd(LPC_UART0, ENABLE);
}

int main(void) {
	uint32_t i = 0, j = 0, k = 0, instruccion = 0;
	uint8_t rxBuffer1[50];
	uint8_t rxBuffer2[30];
	uint8_t rxBuffer3[30];
	Configurar_UART3();
	Configurar_UART2();
	Configurar_UART0();
	GPIO_SetDir(HEARTBEAT_LED_PORT, HEARTBEAT_LED_PIN, 1);
	printf("Hello World\n");
	UART_Send(LPC_UART3, ATCWMODE, 13, BLOCKING);
	UART_SendByte(LPC_UART2, 0x7A);

	UART_SendByte(LPC_UART0, 'D');
//	printf("EnvioDato");
	while (1) {
		UART_SendByte(LPC_UART2, 0x7A);
		if ((UART_GetLineStatus(LPC_UART3) & 0x1) == 1) {
			rxBuffer1[i] = UART_ReceiveByte(LPC_UART3);
			//printf("%c\r\n", rxBuffer1[i]);
			i = (i + 1) % 50;
			if (rxBuffer1[(i - 2) % 50] == '\r'
					&& rxBuffer1[(i - 1) % 50] == '\n') {
				switch (instruccion) {
				case 0:
					if (rxBuffer1[(i - 4) % 50] == 'O'
							&& rxBuffer1[(i - 3) % 50] == 'K') {
						UART_Send(LPC_UART3, ATCWLAP, 10, BLOCKING);
						instruccion++;
					}
					break;
				case 1:
					//printf("\n");
					break;
				}
			}
		}
		if ((UART_GetLineStatus(LPC_UART2) & 0x1) == 1) {
			rxBuffer2[j] = UART_ReceiveByte(LPC_UART2);
			//printf("\t%d\r\n", rxBuffer2[j]);
			j = (j + 1) % 30;
		}
		if ((UART_GetLineStatus(LPC_UART0) & 0x1) == 1) {
			rxBuffer3[k] = UART_ReceiveByte(LPC_UART0);
			//printf("\t\t%c\r\n", rxBuffer3[k]);
			k = (k + 1) % 30;
		}
	}
	return 0;
}
