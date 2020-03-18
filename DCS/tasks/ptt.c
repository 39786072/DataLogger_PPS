/*
 * ptt.c
 *
 *  Created on: 19 de set. de 2019
 *      Author: Santiago
 */

#include "lpc17xx_uart.h"
#include "ptt.h"
#include "../task_support/uart.h"
#include "../task_support/DCS_buffer.h"
#include "../system/system_1769_003-0_c03a.h"
#include "../port/port_1769_003-0_c03a.h"

typedef enum {
	Init = 0, Test, WaitForACK, Wait,Send_Stage1,Send_Stage2
} Ptt_State;
typedef enum {
	Ready = 0x8A, Busy = 0x85, ACK = 0xAA, NACK = 0x05
} Ptt_Status_Message;
typedef enum {
	Start = 0x10,
	Stop = 0x20,
	StMode = 0x30,
	LdMode = 0x40,
	StData = 0x50,
	LdData = 0x60,
	Nop = 0x7A,
	LdInfo = 0x7D
} Ptt_Commands;
typedef enum {
	Buf1,Buf2
} Ptt_Buffers;
void PTT_Init() {
	UART_Configure(LPC_UART0, 9600);
}

void PTT_Start() {
	BUFFER_Push(Ptt_Tx, Start);
}
void PTT_Stop() {
	BUFFER_Push(Ptt_Tx, Stop);
}
void PTT_StMode(uint8_t buffer, uint8_t autorepeat) {
	BUFFER_Push(Ptt_Tx, StMode | (buffer < 1) | autorepeat);
}
void PTT_LdMode() {
	BUFFER_Push(Ptt_Tx, LdMode);
}
/*uint8_t PTTStoreData(uint8_t buffer, uint8_t dataLength, uint8_t data[]){
 uint8_t ack,i;
 uartWriteByte(UART_2, STDATA | buffer);
 ack=PTTGetAck();
 delay_ms(DELAY_MS_1000);
 uartWriteByte(UART_2, (dataLength+3)/4);
 delay_ms(DELAY_MS_1000);
 for(i=0;i<dataLength;i++){
 uartWriteByte(UART_2, data[i]);
 }
 if(dataLength%4){
 for(i=0;i<(4-(dataLength%4));i++){  // Complete message with 0's
 uartWriteByte(UART_2, 0);
 }
 }

 return ack;
 }*/
void PTT_Init_StData(Ptt_Buffers buffer) {

	//Envio del comando STDATA
	BUFFER_Push(Ptt_Tx, StData | buffer);
}
void PTT_StData( uint8_t * data, uint8_t dataLength) {
	uint8_t i;
	//Envio de la longitud en grupos de 4 bytes de acuerdo al manual de la ptt
	BUFFER_Push(Ptt_Tx, (dataLength + 3) / 4);
	//Envio de los datos
	for (i = 0; i < dataLength; i++) {
		BUFFER_Push(Ptt_Tx, data[i]);
	}
	//Completa el mensaje si el original no llegaba.
	if (dataLength % 4) {
		for (i = 0; i < (4 - (dataLength % 4)); i++) { // Complete message with 0's
			BUFFER_Push(Ptt_Tx, 0);
		}
	}
}

void PTT_LdData(uint8_t buffer) {
	BUFFER_Push(Ptt_Tx, LdData | buffer);
}
void PTT_Nop() {
	BUFFER_Push(Ptt_Tx, Nop);
}
void PTT_LdInfo() {
	BUFFER_Push(Ptt_Tx, LdInfo);
}
static void PTT_Heartbeat() {
	static uint8_t Ptt_Heartbeat = 0;
	if (Ptt_Heartbeat == 1) {
		Ptt_Heartbeat = 0;
		GPIO_ClearValue(LED2B_PORT, LED2B_PIN);
	} else {
		Ptt_Heartbeat = 1;
		GPIO_SetValue(LED2B_PORT, LED2B_PIN);
	}
}

void PTT_Task() {
	static Ptt_State Ptt_Current_State;
	static Ptt_State Ptt_Old_State;
	static uint16_t StateCounter;
	uint8_t aux;
	PTT_Heartbeat();
	switch (Ptt_Current_State) {
	case Init:
		PTT_Nop();
		Ptt_Current_State = WaitForACK;
		Ptt_Old_State = Init;
		break;
	case WaitForACK:
		if (StateCounter++ > WAIT_DATA_TIMES) {
			SYSTEM_Perform_Safe_Shutdown();
		}
			aux = BUFFER_Pop(Ws_Rx);
			if (aux != EMPTY_BUFFER_ERROR) {
				if (aux == ACK) {
					if (Ptt_Old_State == Init)
					{
						Ptt_Current_State = Wait;
						Ptt_Old_State = WaitForACK;
						StateCounter = 0;
					}
					else if (Ptt_Old_State == Send_Stage1)
					{
						Ptt_Current_State = Send_Stage2;
						Ptt_Old_State = WaitForACK;
						StateCounter = 0;

					}
				}
			}
		break;
	case Wait:
		if (StateCounter++ >= WAIT_TIMES)
		{
			Ptt_Current_State = Send_Stage1;
			Ptt_Old_State = Wait;
		}
		break;
	case Send_Stage1:
		PTT_Init_StData(Buf2);
		Ptt_Current_State = WaitForACK;
		Ptt_Old_State = Send_Stage1;
		break;
	case Send_Stage2:
		PTT_StData("s",1);//data y datalenght)
		Ptt_Current_State = Wait;
		Ptt_Old_State = Send_Stage2;
		break;
	default:
		SYSTEM_Perform_Safe_Shutdown();
		break;
	}

}

void PTT_Tx() {
	uint8_t aux = BUFFER_Pop(Ptt_Tx);
	if (aux != (uint8_t) EMPTY_BUFFER_ERROR) {
		UART_SendByte(LPC_UART0, aux);
	}
}
void PTT_Rx() {
	while ((UART_GetLineStatus(LPC_UART0) & 0x1) == 1) {
		BUFFER_Push(Ptt_Rx, UART_ReceiveByte(LPC_UART0));
	}
}
