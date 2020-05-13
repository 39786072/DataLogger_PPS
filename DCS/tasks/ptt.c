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
#include "weather_station.h"

typedef enum {
	Init = 0, Test, WaitForACK, Wait, Send_Stage1, Send_Stage2
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
	Buf1, Buf2
} Ptt_Buffers;

/*==================[internal data declaration]==============================*/
static uint8_t buffer[BUFFER_NUMBER][BUFFER_LENGTH]; /* Private data structure that represents buffers to be sent */
static uint32_t samples; /* Private number of samples taken */
static uint8_t bufferNumber; /* Private current buffer number */
static uint32_t avgtemp, avgws; /* Private auxiliar variables to calculate averages */

static void initBuffers() {
	/* Internal variables */
	uint8_t i, j;
	/* For each buffer */
	for (i = 0; i < BUFFER_NUMBER; i++) {
		/* For each data value */
		for (j = 0; j < BUFFER_LENGTH; j++) {
			buffer[i][j] = 0;
		}
		/* Special Values */
		buffer[i][LT] = 0x7f;
		buffer[i][LT + 1] = 0xff;
		buffer[i][LH] = 0x7f;
	}

	avgtemp = 0;
	avgws = 0;
	samples = 0;
	bufferNumber = 0;
}
void PTT_Init() {
	UART_Configure(LPC_UART0, 9600);
	initBuffers();
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
void PTT_StData(uint8_t * data, uint8_t dataLength) {
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
				if (Ptt_Old_State == Init) {
					Ptt_Current_State = Wait;
					Ptt_Old_State = WaitForACK;
					StateCounter = 0;
				} else if (Ptt_Old_State == Send_Stage1) {
					Ptt_Current_State = Send_Stage2;
					Ptt_Old_State = WaitForACK;
					StateCounter = 0;

				}
			}
		}
		break;
	case Wait:
		if (StateCounter++ >= WAIT_TIMES) {
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
		PTT_StData((uint8_t*) "s", 1); //data y datalenght)
		Ptt_Current_State = Wait;
		Ptt_Old_State = Send_Stage2;
		break;
	default:
		SYSTEM_Perform_Safe_Shutdown();
		break;
	}

}

void PTT_TxRx(void) {
	uint8_t aux = BUFFER_Pop(Ptt_Tx);
	if (aux != (uint8_t) EMPTY_BUFFER_ERROR) {
		UART_SendByte(LPC_UART0, aux);
	}
	while ((UART_GetLineStatus(LPC_UART0) & 0x1) == 1) {
		BUFFER_Push(Ptt_Rx, UART_ReceiveByte(LPC_UART0));
	}
}
void Ptt_TakeData_Task(void) {
	/* Internal variables */
	uint16_t data;
	/* 1 more sample */
	samples++;
	/* Update High Outside Temperature */
	getCurrentOutsideTemperature((int16_t*) &data);
	if (data
			> (int16_t) ((buffer[bufferNumber][HT] << 8)
					+ buffer[bufferNumber][HT + 1])) {
		buffer[bufferNumber][HT] = (uint8_t) (data >> 8);
		buffer[bufferNumber][HT + 1] = (uint8_t) data;
	}
	/* Update Low Outside Temperature */
	if (data
			< (int16_t) ((buffer[bufferNumber][LT] << 8)
					+ buffer[bufferNumber][LT + 1])) {
		buffer[bufferNumber][LT] = (uint8_t) (data >> 8);
		buffer[bufferNumber][LT + 1] = (uint8_t) data;
	}
	/* Update Average Outside Temperature */
	avgtemp += data;
	/* Update High Outside Humidity */
	getCurrentOutsideHumidity(&data);
	if (data > buffer[bufferNumber][HH]) {
		buffer[bufferNumber][HH] = (uint8_t) data;
	}
	/* Update Low Outside Humidity */
	if (data < buffer[bufferNumber][LH]) {
		buffer[bufferNumber][LH] = (uint8_t) data;
	}
	/* Update High Wind Speed */
	getCurrentWindSpeed(&data);
	if (data > buffer[bufferNumber][HWS]) {
		buffer[bufferNumber][HWS] = (uint8_t) data;
	}
	/* Update Average Wind Speed */
	avgws += data;
}
static void restartPeriodicValues() {
	/* Restart High Outside Temperature */
	buffer[bufferNumber][HT] = 0;
	buffer[bufferNumber][HT + 1] = 0;

	/* Restart Low Outside Temperature */
	buffer[bufferNumber][LT] = 0x7f;
	buffer[bufferNumber][LT + 1] = 0xff;
	/* Restart Average Outside Temperature */
	avgtemp = 0;
	/* Restart High Outside Humidity */
	buffer[bufferNumber][HH] = 0;

	/* Restart Low Outside Humidity */
	buffer[bufferNumber][LH] = 0x7f;

	/* Restart High Wind Speed */
	buffer[bufferNumber][HWS] = 0;

	/* Restart Average Wind Speed */
	avgws = 0;
}

void PTT_UpdateBuffers_Task(void) {
	/* Internal variables */
	uint16_t data;
	uint8_t i, nob = 0;

	/* Set NOB of all buffers */
	for (i = 0; i < 3; i++) {
		buffer[bufferNumber][NOB] = nob;
		bufferNumber = (bufferNumber + 2) % 3;
		nob++;
	}
	/* Update Outside Temperature */
	getCurrentOutsideTemperature((int16_t*) &data);
	buffer[bufferNumber][OT] = (uint8_t) (data >> 8);
	buffer[bufferNumber][OT + 1] = (uint8_t) (data);
	/* Update Average Outside Temperature */
	avgtemp /= samples;
	buffer[bufferNumber][AVGT] = (uint8_t) (avgtemp >> 8);
	buffer[bufferNumber][AVGT + 1] = (uint8_t) (avgtemp);
	/* Update Dew Point */
	getCurrentDewPoint(&data);
	buffer[bufferNumber][DP] = (uint8_t) (data >> 8);
	buffer[bufferNumber][DP + 1] = (uint8_t) (data);
	/* Update Outside Humidity */
	getCurrentOutsideHumidity(&data);
	buffer[bufferNumber][OH] = (uint8_t) (data);
	/* Update Wind Speed */
	getCurrentWindSpeed(&data);
	buffer[bufferNumber][WS] = (uint8_t) (data);
	/* Update Average Wind Speed */
	buffer[bufferNumber][AVGWS] = (uint8_t) (avgws / samples);
	/* Update Wind Chill */
	getCurrentWindChill(&data);
	buffer[bufferNumber][WC] = (uint8_t) (data >> 8);
	buffer[bufferNumber][WC + 1] = (uint8_t) (data);
	/* Update Wind Direction */
	getCurrentWindDirection(&data);
	buffer[bufferNumber][WD] = (uint8_t) (data >> 8);
	buffer[bufferNumber][WD + 1] = (uint8_t) (data);
	/* Update Daily Rain */
	getCurrentDailyRain(&data);
	buffer[bufferNumber][DR] = (uint8_t) (data >> 8);
	buffer[bufferNumber][DR + 1] = (uint8_t) (data);
	/* Update Yearly Rain */
	getCurrentYearlyRain(&data);
	buffer[bufferNumber][YR] = (uint8_t) (data >> 8);
	buffer[bufferNumber][YR + 1] = (uint8_t) (data);
	/* Update Barometric Pressure */
	getCurrentBarometricPressure(&data);
	buffer[bufferNumber][BP] = (uint8_t) (data >> 8);
	buffer[bufferNumber][BP + 1] = (uint8_t) (data);
	/* Update Battery Voltage */
	getCurrentBatteryVoltage(&data);
	buffer[bufferNumber][BV] = (uint8_t) (data >> 8);
	buffer[bufferNumber][BV + 1] = (uint8_t) (data);
	/* Set CRC */
	buffer[bufferNumber][CRC] = (uint8_t) (buffer[bufferNumber][OT]
			+ buffer[bufferNumber][OT + 1] + buffer[bufferNumber][HT]
			+ buffer[bufferNumber][HT + 1] + buffer[bufferNumber][LT]
			+ buffer[bufferNumber][LT + 1] + buffer[bufferNumber][AVGT]
			+ buffer[bufferNumber][AVGT + 1] + buffer[bufferNumber][BP]
			+ buffer[bufferNumber][BP + 1] + buffer[bufferNumber][OH]
			+ buffer[bufferNumber][HH] + buffer[bufferNumber][LH]
			+ buffer[bufferNumber][WS] + buffer[bufferNumber][HWS]
			+ buffer[bufferNumber][AVGWS] + buffer[bufferNumber][WD]
			+ buffer[bufferNumber][WD + 1] + buffer[bufferNumber][YR]
			+ buffer[bufferNumber][YR + 1] + buffer[bufferNumber][DR]
			+ buffer[bufferNumber][DR + 1] + buffer[bufferNumber][WC]
			+ buffer[bufferNumber][WC + 1] + buffer[bufferNumber][DP]
			+ buffer[bufferNumber][DP + 1] + buffer[bufferNumber][BV]
			+ buffer[bufferNumber][BV + 1]);
	/* Update buffer and it's variables*/
	bufferNumber = (bufferNumber + 1) % BUFFER_NUMBER;
	avgtemp = 0;
	avgws = 0;
	samples = 0;
	/* Restart periodic values */
	restartPeriodicValues();

}

