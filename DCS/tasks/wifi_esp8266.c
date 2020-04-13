/*
 * wifi_esp8266.c
 *
 *  Created on: 5 de ago. de 2019
 *      Author: Santiago
 */
#include "lpc17xx_uart.h"
#include "wifi_esp8266.h"
#include "../task_support/uart.h"
#include "../task_support/DCS_buffer.h"
#include "../port/port_1769_003-0_c03a.h"
#include "lpc17xx_gpio.h"
#include "../system/system_1769_003-0_c03a.h"
#include "../task_support/rtc.h"
#define HEADER_LENGHT 160
typedef enum {
	Init = 0,
	Echo,
	Test,
	Set_Mode,
	Set_Connection_Type,
	JoinAP,
	GetIP,
	Running,
	Waiting,
	WaitingForString,
	SendData,
	Close,
	UpdateRTC,
	StartConnection
} Wifi_State;

typedef enum {
	Station = 1, SoftAP, Both
} Wifi_Mode;
typedef enum {
	Disable = 0, Enable
} Wifi_Echo_Mode;
typedef enum {
	Single = 0, Multiple
} Wifi_Conection_Type;
typedef enum {
	TCP = 0, UDP
} Wifi_Protocol;
const uint8_t Wifi_String_OK[5] = { 'O', 'K', '\r', '\n', '\0' };
const uint8_t Wifi_String_ready[6] = { 'r', 'e', 'a', 'd', 'y', '\0' };
const uint8_t Wifi_String_Send[2] = { '>', '\0' };
const uint8_t Wifi_String_SendOK[8] =
		{ 'S', 'E', 'N', 'D', ' ', 'O', 'K', '\0' };
const uint8_t Wifi_String_Date[7] =
		{ 'D', 'a', 't', 'e', ':', ' ', '\0' };
static void WIFI_AT() {
	BUFFER_Push_String(Wifi_Tx, (uint8_t *) "AT\r\n");
}
static void WIFI_Echo(Wifi_Echo_Mode state) {
	BUFFER_Push_String(Wifi_Tx, (uint8_t *) "ATE");
	BUFFER_Push(Wifi_Tx,state + '0');
	BUFFER_Push_String(Wifi_Tx,(uint8_t *)"\r\n");
}
// TODO AT+CIFSR Obtener direcciones IP Termina con OK asi que puede estar y esperar el OK si no interesa la direccion
static void WIFI_Get_IP() {
	BUFFER_Push_String(Wifi_Tx, (uint8_t *) "AT+CIFSR\r\n");
}

static void WIFI_Set_Mode(Wifi_Mode mode) {
	BUFFER_Push_String(Wifi_Tx, (uint8_t *) "AT+CWMODE=");
	BUFFER_Push(Wifi_Tx, mode + '0');
	BUFFER_Push_String(Wifi_Tx, (uint8_t *) "\r\n");
}
static void WIFI_Set_Connection_Type(Wifi_Conection_Type type) {
	BUFFER_Push_String(Wifi_Tx, (uint8_t *) "AT+CIPMUX=");
	BUFFER_Push(Wifi_Tx, type + '0');
	BUFFER_Push_String(Wifi_Tx, (uint8_t *) "\r\n");
}
static void WIFI_Join_AP(uint8_t * ssid, uint8_t * pwd) {
	BUFFER_Push_String(Wifi_Tx, (uint8_t *) "AT+CWJAP=\"");
	BUFFER_Push_String(Wifi_Tx, ssid);
	BUFFER_Push_String(Wifi_Tx, (uint8_t *) "\",\"");
	BUFFER_Push_String(Wifi_Tx, pwd);
	BUFFER_Push_String(Wifi_Tx, (uint8_t *) "\"\r\n");
}
static void WIFI_Start_Transmision(Wifi_Protocol protocol, uint8_t * host,
		uint8_t * port) {
	BUFFER_Push_String(Wifi_Tx, (uint8_t *)"AT+CIPSTART=\"");
	switch (protocol) {
	case TCP:
		BUFFER_Push_String(Wifi_Tx, (uint8_t *)"TCP\",\"");
		break;
	case UDP:
		BUFFER_Push_String(Wifi_Tx, (uint8_t *)"UDP\",\"");
		break;
	default:
		SYSTEM_Perform_Safe_Shutdown();
		break;
	}
	BUFFER_Push_String(Wifi_Tx, host);
	BUFFER_Push_String(Wifi_Tx, (uint8_t *)"\",");
	BUFFER_Push_String(Wifi_Tx, port);
	BUFFER_Push_String(Wifi_Tx, (uint8_t *)"\r\n");
}
static void WIFI_Close()
{
	BUFFER_Push_String(Wifi_Tx, (uint8_t *) "AT+CIPCLOSE\r\n");
}
static void WIFI_RST()
{
	BUFFER_Push_String(Wifi_Tx, (uint8_t *) "AT+RST\r\n");
}
static void WIFI_Send(uint16_t length) {
	BUFFER_Push_String(Wifi_Tx, (uint8_t *) "AT+CIPSEND=");
	uint8_t i = 0;
	uint8_t Result[] = "AAAAA";
	while (length != 0) {
		Result[4 - i++] = (length % 10) + '0';
		length = length / 10;
	}
	i = 0;
	while (Result[i] != '\0') {
		if (Result[i] != 'A') {
			BUFFER_Push(Wifi_Tx, Result[i]);
		}
		i++;
	}
	BUFFER_Push_String(Wifi_Tx, (uint8_t *) "\r\n");
}
void WIFI_Init() {
	GPIO_SetDir(E_WIFI_PORT, E_WIFI_PIN, 1);
	GPIO_SetValue(E_WIFI_PORT, E_WIFI_PIN);
	UART_Configure(LPC_UART3, 115200);
}
static uint8_t WIFI_Make_HTTP(uint8_t * Variable_Name, int16_t Value,
		uint8_t ** String_HTTP) {
	uint8_t i = 0;
	uint8_t Result[] = "00000";
	uint8_t HTTPLength = 0;
	uint8_t DataLenght = 0;
	static uint8_t WIFI_Header[200] =
			"POST /api/v1.6/devices/dcs/?token=BBFF-CtpRFtRXwTq7IYAa9F8nLq44JQs237 HTTP/1.1\r\nHost: things.ubidots.com\r\nContent-Type: application/json\r\nContent-Length: 10\r\n\r\n";
	//A PARTIR DE LA POS HEADER_LENGHT EMPIEZAN LOS DATOS
	WIFI_Header[HEADER_LENGHT + DataLenght++] = '{';
	WIFI_Header[HEADER_LENGHT + DataLenght++] = '"';
	while (Variable_Name[i] != '\0') {
		WIFI_Header[HEADER_LENGHT + DataLenght++] = Variable_Name[i++];
	}
	WIFI_Header[HEADER_LENGHT + DataLenght++] = '"';
	WIFI_Header[HEADER_LENGHT + DataLenght++] = ':';
	i = 0;
	if (Value <= 0) {
		WIFI_Header[HEADER_LENGHT + DataLenght++] = '-';
		Value *= -1;
	}
	while (Value != 0) {
		Result[4 - i++] = (Value % 10) + '0';
		Value = Value / 10;
	}
	i = 0;
	while (Result[i] != '\0') {
		if (Result[i] != '0') {
			WIFI_Header[HEADER_LENGHT + DataLenght++] = Result[i];
		}
		i++;
	}
	WIFI_Header[HEADER_LENGHT + DataLenght++] = '}';
	WIFI_Header[HEADER_LENGHT + DataLenght] = '\0';
	HTTPLength = HEADER_LENGHT + DataLenght;
	i = 0;
	while (DataLenght != 0) {
		WIFI_Header[HEADER_LENGHT - 5 - i] = (DataLenght % 10) + '0';
		DataLenght = DataLenght / 10;
		i++;
	}
	(*String_HTTP) = WIFI_Header;
	return HTTPLength;
}

void WiFi_Heartbeat()
{
	static uint8_t WiFi_Heartbeat = 0;
    if (WiFi_Heartbeat == 1){
    	WiFi_Heartbeat = 0;
       GPIO_ClearValue(LED2R_PORT, LED2R_PIN);
    }else{
    	WiFi_Heartbeat = 1;
    	GPIO_SetValue(LED2R_PORT, LED2R_PIN);
    }
}

void WIFI_Task() {
	static Wifi_State Wifi_Current_State = Init;
	static Wifi_State Wifi_Old_State = 0xFF;
	static uint8_t const * Wifi_Required_Text;
	static uint32_t Wifi_Waiting_Times;
	static uint8_t Wifi_Waiting_Index;
	uint8_t auxChar;
	uint8_t auxDateString[25];
	static uint8_t * HTTP;
	WiFi_Heartbeat();
	switch (Wifi_Current_State) {
	case Init:
		if (Wifi_Old_State == Waiting) {
			Wifi_Old_State = Init;
			Wifi_Current_State = Test;
		} else {
			BUFFER_Flush(Wifi_Rx);
			//WIFI_RST();
			Wifi_Waiting_Times = 1000;
			Wifi_Old_State = Init;
			Wifi_Current_State = Waiting;
		}
	break;
	case Echo:
		if (Wifi_Old_State != WaitingForString) {
			WIFI_Echo(Disable);
			Wifi_Old_State = Echo;
			Wifi_Required_Text = Wifi_String_OK;
			Wifi_Waiting_Index = 0;
			Wifi_Waiting_Times = 200;
			Wifi_Current_State = WaitingForString;
		} else {
			BUFFER_Flush(Wifi_Rx);
			Wifi_Old_State = Echo;
			Wifi_Current_State = Set_Mode;
		}
		break;
	case Test:
		if (Wifi_Old_State != WaitingForString) {
			WIFI_AT();
			Wifi_Old_State = Test;
			Wifi_Required_Text = Wifi_String_OK;
			Wifi_Waiting_Index = 0;
			Wifi_Waiting_Times = 200;
			Wifi_Current_State = WaitingForString;
		} else {
			BUFFER_Flush(Wifi_Rx);
			Wifi_Old_State = Test;
			Wifi_Current_State = Echo;
		}
		break;


	case Set_Mode:
		if (Wifi_Old_State != WaitingForString) {
			WIFI_Set_Mode(Station);
			Wifi_Old_State = Set_Mode;
			Wifi_Required_Text = Wifi_String_OK;
			Wifi_Waiting_Index = 0;
			Wifi_Waiting_Times = 100;
			Wifi_Current_State = WaitingForString;
		} else {
			BUFFER_Flush(Wifi_Rx);
			Wifi_Old_State = Set_Mode;
			Wifi_Current_State = JoinAP;
		}
		break;

	case Set_Connection_Type:
		if (Wifi_Old_State != WaitingForString) {
			WIFI_Set_Connection_Type(Single);
			Wifi_Old_State = Set_Connection_Type;
			Wifi_Required_Text = Wifi_String_OK;
			Wifi_Waiting_Index = 0;
			Wifi_Waiting_Times = 100;
			Wifi_Current_State = WaitingForString;
		} else {
			BUFFER_Flush(Wifi_Rx);
			Wifi_Old_State = Set_Connection_Type;
			Wifi_Current_State = GetIP;
		}
		break;

	case JoinAP:
		if (Wifi_Old_State != WaitingForString) {
			WIFI_Join_AP((uint8_t*) "Melasoba", (uint8_t*) "penerguido");
			Wifi_Old_State = JoinAP;
			Wifi_Required_Text = Wifi_String_OK;
			Wifi_Waiting_Index = 0;
			Wifi_Waiting_Times = 500;
			Wifi_Current_State = WaitingForString;
		} else {
			BUFFER_Flush(Wifi_Rx);
			Wifi_Old_State = JoinAP;
			Wifi_Current_State = Set_Connection_Type;
		}
		break;
	case GetIP:
		if (Wifi_Old_State != WaitingForString) {
			WIFI_Get_IP();
			Wifi_Old_State = GetIP;
			Wifi_Required_Text = Wifi_String_OK;
			Wifi_Waiting_Index = 0;
			Wifi_Waiting_Times = 100;
			Wifi_Current_State = WaitingForString;
		} else {
			BUFFER_Flush(Wifi_Rx);
			Wifi_Old_State = GetIP;
			Wifi_Current_State = Running;
		}
		break;

	case Running:
		//Verificar si hay nuevos datos que enviar a ubidots y enviarlos
		if (Wifi_Old_State == Waiting) {
			BUFFER_Flush(Wifi_Rx);
			Wifi_Old_State = Running;
			Wifi_Current_State = StartConnection;
		} else {
			BUFFER_Flush(Wifi_Rx);
			Wifi_Waiting_Times = 500;
			Wifi_Old_State = Running;
			Wifi_Current_State = Waiting;
		}
		break;

	case WaitingForString:

		if (--Wifi_Waiting_Times <= 0) {
			SYSTEM_Perform_Safe_Shutdown();
		}
		if (Wifi_Required_Text[Wifi_Waiting_Index] != '\0') {
			auxChar = BUFFER_Pop(Wifi_Rx);
			if (auxChar != EMPTY_BUFFER_ERROR) {
				if (auxChar == Wifi_Required_Text[Wifi_Waiting_Index]) {
					Wifi_Waiting_Index++;
				} else {
					Wifi_Waiting_Index = 0;
				}
			}
		} else {
			Wifi_Current_State = Wifi_Old_State;
			Wifi_Old_State = WaitingForString;
		}
		break;
	case Waiting:
// Verificar que sigan en el buffer los datos de la fecha
		if (--Wifi_Waiting_Times <= 0) {
			Wifi_Current_State = Wifi_Old_State;
			Wifi_Old_State = Waiting;
		}
		break;
	case StartConnection:
		if (Wifi_Old_State != WaitingForString) {
			WIFI_Start_Transmision(TCP, (uint8_t *)"things.ubidots.com", (uint8_t *)"80");
			Wifi_Old_State = StartConnection;
			Wifi_Required_Text = Wifi_String_OK;
			Wifi_Waiting_Index = 0;
			Wifi_Waiting_Times = 200;
			Wifi_Current_State = WaitingForString;
		} else {
			BUFFER_Flush(Wifi_Rx);
			Wifi_Old_State = StartConnection;
			Wifi_Current_State = SendData;
		}
		break;
	case Close:
		if (Wifi_Old_State != WaitingForString) {
			WIFI_Close();
			Wifi_Old_State = Close;
			Wifi_Required_Text = Wifi_String_OK;
			Wifi_Waiting_Index = 0;
			Wifi_Waiting_Times = 200;
			Wifi_Current_State = WaitingForString;
		} else {
			BUFFER_Flush(Wifi_Rx);
			Wifi_Old_State = Close;
			Wifi_Current_State = Running;
		}
		break;
	case UpdateRTC:
		if (Wifi_Old_State != WaitingForString) {
			Wifi_Old_State = UpdateRTC;
			Wifi_Required_Text = Wifi_String_Date;
			Wifi_Waiting_Index = 0;
			Wifi_Waiting_Times = 200;
			Wifi_Current_State = WaitingForString;

		} else {
			for (auxChar = 0; auxChar<25; auxChar++)
			{
				auxDateString[auxChar] = BUFFER_Pop(Wifi_Rx);
			}
			BUFFER_Flush(Wifi_Rx);
			RTCSetTime(auxDateString);
			Wifi_Old_State = UpdateRTC;
			Wifi_Current_State = Close;
		}
		break;
	case SendData:
		switch (Wifi_Old_State) {
		case StartConnection:
			//Send AT+CIPSEND= HTTP_LENGTH
			WIFI_Send(WIFI_Make_HTTP((uint8_t *) "temperature", -152, &HTTP));
			Wifi_Old_State = SendData;
			Wifi_Required_Text = Wifi_String_Send;
			Wifi_Waiting_Index = 0;
			Wifi_Waiting_Times = 100;
			Wifi_Current_State = WaitingForString;
			break;
		case WaitingForString:

			if (Wifi_Required_Text != Wifi_String_SendOK) {
				BUFFER_Flush(Wifi_Rx);
				if(BUFFER_Push_String(Wifi_Tx, HTTP) == FULL_BUFFER_ERROR){
					SYSTEM_Perform_Safe_Shutdown();
				}
				Wifi_Old_State = SendData;
				Wifi_Required_Text = Wifi_String_SendOK;
				Wifi_Waiting_Index = 0;
				Wifi_Waiting_Times = 250;
				Wifi_Current_State = WaitingForString;

			}else
			{
				Wifi_Old_State = SendData;
				Wifi_Current_State = Waiting;
				Wifi_Waiting_Times = 200;
			}

			break;
		case Waiting:
			Wifi_Old_State = SendData;
			Wifi_Current_State = UpdateRTC;
			break;
		default:
			SYSTEM_Perform_Safe_Shutdown();
			break;
		}

		break;
	default:
		SYSTEM_Perform_Safe_Shutdown();
		break;
	}

}
void WIFI_TxRx(void) {
	uint8_t aux = BUFFER_Pop(Wifi_Tx);
	if (aux != (uint8_t) EMPTY_BUFFER_ERROR) {
		UART_SendByte(LPC_UART3, aux);
	}
	while ((UART_GetLineStatus(LPC_UART3) & 0x1) == 1) {
		if (BUFFER_Push(Wifi_Rx, UART_ReceiveByte(LPC_UART3)) == FULL_BUFFER_ERROR)
		{
			SYSTEM_Perform_Safe_Shutdown();
		}
	}
}

