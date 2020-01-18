/*
 * buffer.c
 *
 *  Created on: 5 de ago. de 2019
 *      Author: Santiago Corrao
 */
#include "../main/main.h"
#include "DCS_buffer.h"

static uint8_t Wifi_tx_buffer[WIFI_TX_BUFFER_SIZE];
static uint8_t Wifi_rx_buffer[WIFI_RX_BUFFER_SIZE];
static uint8_t Sd_tx_buffer[SD_TX_BUFFER_SIZE];
static uint8_t Sd_rx_buffer[SD_RX_BUFFER_SIZE];
static uint8_t Ptt_tx_buffer[PTT_TX_BUFFER_SIZE];
static uint8_t Ptt_rx_buffer[PTT_RX_BUFFER_SIZE];
static uint8_t Ws_tx_buffer[WS_TX_BUFFER_SIZE];
static uint8_t Ws_rx_buffer[WS_RX_BUFFER_SIZE];

typedef struct {
	 uint8_t* B[8];
	 uint16_t Write_pointer[8];
	 uint16_t Read_pointer[8];
} Buffer_t;

static Buffer_t Buffer;

static const uint16_t Buffers_Sizes[] = {
		 WIFI_TX_BUFFER_SIZE //Wifi_Tx
		,WIFI_RX_BUFFER_SIZE //Wifi_Rx
		,SD_TX_BUFFER_SIZE //Sd_Tx
		,SD_RX_BUFFER_SIZE //Sd_Rx
		,PTT_TX_BUFFER_SIZE //Ptt_Tx
		,PTT_RX_BUFFER_SIZE //Ptt_Rx
		,WS_TX_BUFFER_SIZE //Ws_Tx
		,WS_RX_BUFFER_SIZE //Ws_Rx
};

void Buffer_Init() {
	Buffer.B[Wifi_Tx] = (uint8_t *) &Wifi_tx_buffer;
	Buffer.B[Wifi_Rx] = (uint8_t *)&Wifi_rx_buffer;
	Buffer.B[Sd_Tx] = (uint8_t *)&Sd_tx_buffer;
	Buffer.B[Sd_Rx] = (uint8_t *)&Sd_rx_buffer;
	Buffer.B[Ptt_Tx] = (uint8_t *) &Ptt_tx_buffer;
	Buffer.B[Ptt_Rx] = (uint8_t *)&Ptt_rx_buffer;
	Buffer.B[Ws_Tx] = (uint8_t *)&Ws_tx_buffer;
	Buffer.B[Ws_Rx] = (uint8_t *)&Ws_rx_buffer;

}

uint8_t BUFFER_Push( Buffer_type index,uint8_t data) {
	if (Buffer.Write_pointer[index] < Buffers_Sizes[index])
	{
		Buffer.B[index][Buffer.Write_pointer[index]] = data;
		Buffer.Write_pointer[index] ++;
		return data;
	}
	else
	{
		return FULL_BUFFER_ERROR;
	}
}
uint8_t BUFFER_Push_String( Buffer_type index, uint8_t * data) {
	while((*data) != '\0')
	{
		if (BUFFER_Push(index,(*data)) == FULL_BUFFER_ERROR)
		{
			return FULL_BUFFER_ERROR;
		}
		data ++;
	}
	return (*data);
}


uint8_t BUFFER_LastChar(Buffer_type index)
{
	return Buffer.B[index][Buffer.Write_pointer[index] - 1];
}

uint8_t BUFFER_Pop(Buffer_type index) {
	uint8_t aux;
	if (Buffer.Read_pointer[index] < Buffer.Write_pointer[index])
	{
		aux = Buffer.B[index][Buffer.Read_pointer[index]];
		Buffer.Read_pointer[index] ++;
		if (Buffer.Read_pointer[index] == Buffer.Write_pointer[index])
		{
			Buffer.Read_pointer[index] = 0;
			Buffer.Write_pointer[index] = 0;
		}
		return aux;
	}
	else
	{
		return EMPTY_BUFFER_ERROR;
	}
}
void BUFFER_Flush(Buffer_type index)
{
	Buffer.B[index][0] = '\0';
	Buffer.Read_pointer[index] = 0;
	Buffer.Write_pointer[index] = 0;
}
