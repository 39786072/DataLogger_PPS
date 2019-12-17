/*
 * buffer.h
 *
 *  Created on: 5 de ago. de 2019
 *      Author: Santiago
 */

#ifndef TASK_SUPPORT_BUFFER_H_
#define TASK_SUPPORT_BUFFER_H_

#define FULL_BUFFER_ERROR (uint8_t)-1
#define EMPTY_BUFFER_ERROR (uint8_t)-2
typedef enum {
	Wifi_Tx, Wifi_Rx, Sd_Tx, Sd_Rx, Ptt_Tx, Ptt_Rx, Ws_Tx, Ws_Rx
} Buffer_type;

#define WIFI_TX_BUFFER_SIZE 220
#define WIFI_RX_BUFFER_SIZE 350
#define SD_TX_BUFFER_SIZE 40
#define SD_RX_BUFFER_SIZE 40
#define PTT_TX_BUFFER_SIZE 40
#define PTT_RX_BUFFER_SIZE 40
#define WS_TX_BUFFER_SIZE 40 //Weather Station
#define WS_RX_BUFFER_SIZE 40 //Weather Station

void Buffer_Init();
uint8_t BUFFER_Push( Buffer_type index,uint8_t data);
uint8_t BUFFER_Pop(Buffer_type index);
uint8_t BUFFER_LastChar(Buffer_type index);
uint8_t BUFFER_Push_String( Buffer_type index, uint8_t * data);
void BUFFER_Flush(Buffer_type index);
#endif /* TASK_SUPPORT_BUFFER_H_ */
