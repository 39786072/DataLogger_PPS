/*
 * weather_station.c
 *
 *  Created on: 18 de dic. de 2019
 *      Author: Santiago
 */

#include "weather_station.h"
#include "lpc17xx_uart.h"
#include "../task_support/uart.h"
#include "../task_support/DCS_buffer.h"
#include "../system/system_1769_003-0_c03a.h"
#include "../port/port_1769_003-0_c03a.h"
typedef enum {
	Wait = 0,
	WaitForACK = 1,
	WaitForData = 2,
	SendCommand = 3
} States;

static uint16_t weather_data[WEATHER_DATA][WEATHER_PARAMETERS];
static uint32_t samples;

static WS_Fields ActualField;
static States ActualState;

static uint8_t StateCounter;
static uint16_t MEFCounter;

static uint16_t data;
uint8_t aux;
static const uint8_t ws_command [WEATHER_DATA - 1][2] = {
											{WCR_INSIDE_TEMPERATURE},
											{WCR_OUTSIDE_TEMPERATURE},
											{WCR_DEW_POINT},
											{WCR_INSIDE_HUMIDITY},
											{WCR_OUTSIDE_HUMIDITY},
											{WCR_WIND_SPEED},
											{WCR_WIND_CHILL},
											{WCR_WIND_DIRECTION},
											{WCR_BAROMETER},
											{WCR_DAILY_RAIN},
											{WCR_YEARLY_RAIN},
											};
static void WS_Heartbeat()
{
	static uint8_t Ws_Heartbeat = 0;
    if (Ws_Heartbeat == 1){
    	Ws_Heartbeat = 0;
       GPIO_ClearValue(LED2G_PORT, LED2G_PIN);
    }else{
    	Ws_Heartbeat = 1;
    	GPIO_SetValue(LED2G_PORT, LED2G_PIN);
    }
}
/**
 * @brief   Inits internal data structure
 * @return  Nothing
 */
void WS_Init(){
	/* Internal variables */
	uint8_t i, j;
	for(i=0;i<WEATHER_DATA;i++){
		for(j=0;j<WEATHER_PARAMETERS;j++){
			if(MIN_DATA==j)
				weather_data[i][j]=0x7fff;
			else
				weather_data[i][j]=0;
		}
	}
	samples=0;
	UART_Configure(LPC_UART2, 2400);
	ActualState = SendCommand;
	ActualField=0;
}


/* Updates internal data structure */
/**
 * @brief   Updates internal data structure
 * @return  Nothing
 */
void WS_UpdateData(){
	WS_Heartbeat();
	switch(ActualState)
	{
	case SendCommand:
		BUFFER_Flush(Ws_Rx);
		BUFFER_Push_String(Ws_Tx,(uint8_t*)"WRD");
		BUFFER_Push(Ws_Tx,ws_command[ActualField][0]);
		BUFFER_Push(Ws_Tx,ws_command[ActualField][1]);
		BUFFER_Push(Ws_Tx,CR);
		ActualState = WaitForACK;
		StateCounter = 0;
		break;
	case WaitForACK:
		/*if (StateCounter++ > ####) {
			SYSTEM_Perform_Safe_Shutdown();
		}*/
			aux = BUFFER_Pop(Ws_Rx);
			if (aux != EMPTY_BUFFER_ERROR) {
				if (aux == ACK) {
					ActualState = WaitForData;
					StateCounter = 0;
					data = 0;
				}
			}
		break;
	case WaitForData:
		/*if (StateCounter++ > ####) {
			SYSTEM_Perform_Safe_Shutdown();
		}*/
			aux = BUFFER_Pop(Ws_Rx);
			if (aux != EMPTY_BUFFER_ERROR) {
				data = data | (aux<<(8*StateCounter));
				if (++StateCounter >= (((ws_command[ActualField][0]>>4) + 1)/2))
				{
					weather_data[ActualField][CURRENT_DATA] = data;
					ActualField++;
					ActualState = SendCommand;
					if (ActualField == WEATHER_DATA-1)
					{
						ActualState = Wait;
					}
				}
			}
		break;
	case Wait:
		break;
	default:
		SYSTEM_Perform_Safe_Shutdown();
		break;
	}

}



/**
 * @brief   Current Inside Temperature Getter
 * @param   data : Pointer to where to store data
 * @return  Nothing
 */
int16_t getCurrentInsideTemperature(){
	return (int16_t)weather_data[INSIDE_TEMPERATURE][CURRENT_DATA];
}

/**
 * @brief   Average Inside Temperature Getter
 * @param   avg : Pointer to where to store data
 * @return  Nothing
 */
int16_t getAverageInsideTemperature(){
	/* To avoid division by 0 */
	if(samples)
		return (int16_t)(((weather_data[INSIDE_TEMPERATURE][AVG_DATA_HIGH]<<16) + weather_data[INSIDE_TEMPERATURE][AVG_DATA_LOW])/samples);
	else
		return 0;
}

/**
 * @brief   Max Inside Temperature Getter
 * @param   max : Pointer to where to store data
 * @return  Nothing
 */
int16_t getMaxInsideTemperature(){
	return (int16_t)weather_data[INSIDE_TEMPERATURE][MAX_DATA];
}

/**
 * @brief   Min Inside Temperature Getter
 * @param   min : Pointer to where to store data
 * @return  Nothing
 */
int16_t getMinInsideTemperature(){
	return (int16_t)weather_data[INSIDE_TEMPERATURE][MIN_DATA];
}

/**
 * @brief   Current Outside Temperature Getter
 * @param   data : Pointer to where to store data
 * @return  Nothing
 */
int16_t getCurrentOutsideTemperature(){
	return (int16_t)weather_data[OUTSIDE_TEMPERATURE][CURRENT_DATA];
}

/**
 * @brief   Average Outside Temperature Getter
 * @param   avg : Pointer to where to store data
 * @return  Nothing
 */
int16_t getAverageOutsideTemperature(){
	/* To avoid division by 0 */
	if(samples)
		return (int16_t)(((weather_data[OUTSIDE_TEMPERATURE][AVG_DATA_HIGH]<<16) + weather_data[OUTSIDE_TEMPERATURE][AVG_DATA_LOW])/samples);
	else
		return 0;
}

/**
 * @brief   Max Outside Temperature Getter
 * @param   max : Pointer to where to store data
 * @return  Nothing
 */
int16_t getMaxOutsideTemperature(){
	return (int16_t)weather_data[OUTSIDE_TEMPERATURE][MAX_DATA];
}

/**
 * @brief   Min Outside Temperature Getter
 * @param   min : Pointer to where to store data
 * @return  Nothing
 */
int16_t getMinOutsideTemperature(){
	return (int16_t)weather_data[OUTSIDE_TEMPERATURE][MIN_DATA];
}

/**
 * @brief   Current Dew Point Getter
 * @param   data : Pointer to where to store data
 * @return  Nothing
 */
uint16_t getCurrentDewPoint(){
	return weather_data[DEW_POINT][CURRENT_DATA];
}

/**
 * @brief   Average Dew Point Getter
 * @param   avg : Pointer to where to store data
 * @return  Nothing
 */
uint16_t getAverageDewPoint(){
	/* To avoid division by 0 */
	if(samples)
		return (uint16_t)(((weather_data[DEW_POINT][AVG_DATA_HIGH]<<16) + weather_data[DEW_POINT][AVG_DATA_LOW])/samples);
	else
		return 0;
}

/**
 * @brief   Max Dew Point Getter
 * @param   max : Pointer to where to store data
 * @return  Nothing
 */
uint16_t getMaxDewPoint(){
	return weather_data[DEW_POINT][MAX_DATA];
}

/**
 * @brief   Min Dew Point Getter
 * @param   min : Pointer to where to store data
 * @return  Nothing
 */
uint16_t getMinDewPoint(){
	return weather_data[DEW_POINT][MIN_DATA];
}

/**
 * @brief   Current Inside Humidity Getter
 * @param   data : Pointer to where to store data
 * @return  Nothing
 */
uint16_t getCurrentInsideHumidity(){
	return weather_data[INSIDE_HUMIDITY][CURRENT_DATA];
}

/**
 * @brief   Average Inside Humidity Getter
 * @param   avg : Pointer to where to store data
 * @return  Nothing
 */
uint16_t getAverageInsideHumidity(){
	/* To avoid division by 0 */
	if(samples)
		return (uint16_t)(((weather_data[INSIDE_HUMIDITY][AVG_DATA_HIGH]<<16) + weather_data[INSIDE_HUMIDITY][AVG_DATA_LOW])/samples);
	else
		return 0;
}

/**
 * @brief   Max Inside Humidity Getter
 * @param   max : Pointer to where to store data
 * @return  Nothing
 */
uint16_t getMaxInsideHumidity(){
	return weather_data[INSIDE_HUMIDITY][MAX_DATA];
}

/**
 * @brief   Min Inside Humidity Getter
 * @param   min : Pointer to where to store data
 * @return  Nothing
 */
uint16_t getMinInsideHumidity(){
	return weather_data[INSIDE_HUMIDITY][MIN_DATA];
}

/**
 * @brief   Current Outside Humidity Getter
 * @param   data : Pointer to where to store data
 * @return  Nothing
 */
uint16_t getCurrentOutsideHumidity(){
	return weather_data[OUTSIDE_HUMIDITY][CURRENT_DATA];
}

/**
 * @brief   Average Outside Humidity Getter
 * @param   avg : Pointer to where to store data
 * @return  Nothing
 */
uint16_t getAverageOutsideHumidity(){
	/* To avoid division by 0 */
	if(samples)
		return (uint16_t)(((weather_data[OUTSIDE_HUMIDITY][AVG_DATA_HIGH]<<16) + weather_data[OUTSIDE_HUMIDITY][AVG_DATA_LOW])/samples);
	else
		return 0;
}

/**
 * @brief   Max Outside Humidity Getter
 * @param   max : Pointer to where to store data
 * @return  Nothing
 */
uint16_t getMaxOutsideHumidity(){
	return weather_data[OUTSIDE_HUMIDITY][MAX_DATA];
}

/**
 * @brief   Min Outside Humidity Getter
 * @param   min : Pointer to where to store data
 * @return  Nothing
 */
uint16_t getMinOutsideHumidity(){
	return weather_data[OUTSIDE_HUMIDITY][MIN_DATA];
}

/**
 * @brief   Current Wind Speed Getter
 * @param   data : Pointer to where to store data
 * @return  Nothing
 */
uint16_t getCurrentWindSpeed(){
	return weather_data[WIND_SPEED][CURRENT_DATA];
}

/**
 * @brief   Average Wind Speed Getter
 * @param   avg : Pointer to where to store data
 * @return  Nothing
 */
uint16_t getAverageWindSpeed(){
	/* To avoid division by 0 */
	if(samples)
		return (uint16_t)(((weather_data[WIND_SPEED][AVG_DATA_HIGH]<<16) + weather_data[WIND_SPEED][AVG_DATA_LOW])/samples);
	else
		return 0;
}

/**
 * @brief   Max Wind Speed Getter
 * @param   max : Pointer to where to store data
 * @return  Nothing
 */
uint16_t getMaxWindSpeed(uint16_t *max){
	return weather_data[WIND_SPEED][MAX_DATA];
}

/**
 * @brief   Min Wind Speed Getter
 * @param   min : Pointer to where to store data
 * @return  Nothing
 */
uint16_t getMinWindSpeed(uint16_t *min){
	return weather_data[WIND_SPEED][MIN_DATA];
}

/**
 * @brief   Current Wind Chill Getter
 * @param   data : Pointer to where to store data
 * @return  Nothing
 */
uint16_t getCurrentWindChill(uint16_t *data){
	return weather_data[WIND_CHILL][CURRENT_DATA];
}

/**
 * @brief   Average Wind Chill Getter
 * @param   avg : Pointer to where to store data
 * @return  Nothing
 */
uint16_t getAverageWindChill(){
	/* To avoid division by 0 */
	if(samples)
		return (uint16_t)(((weather_data[WIND_CHILL][AVG_DATA_HIGH]<<16) + weather_data[WIND_CHILL][AVG_DATA_LOW])/samples);
	else
		return 0;
}

/**
 * @brief   Max Wind Chill Getter
 * @param   max : Pointer to where to store data
 * @return  Nothing
 */
uint16_t getMaxWindChill(){
	return weather_data[WIND_CHILL][MAX_DATA];
}

/**
 * @brief   Min Wind Chill Getter
 * @param   min : Pointer to where to store data
 * @return  Nothing
 */
uint16_t getMinWindChill(){
	return weather_data[WIND_CHILL][MIN_DATA];
}

/**
 * @brief   Current Wind Direction Getter
 * @param   data : Pointer to where to store data
 * @return  Nothing
 */
uint16_t getCurrentWindDirection(){
	return weather_data[WIND_DIRECTION][CURRENT_DATA];
}

/**
 * @brief   Current Barometric Pressure Getter
 * @param   data : Pointer to where to store data
 * @return  Nothing
 */
uint16_t getCurrentBarometricPressure(){
	return weather_data[BAROMETRIC_PRESSURE][CURRENT_DATA];
}

/**
 * @brief   Average Barometric Pressure Getter
 * @param   avg : Pointer to where to store data
 * @return  Nothing
 */
uint16_t getAverageBarometricPressure(){
	/* To avoid division by 0 */
	if(samples)
		return (uint16_t)(((weather_data[BAROMETRIC_PRESSURE][AVG_DATA_HIGH]<<16) + weather_data[BAROMETRIC_PRESSURE][AVG_DATA_LOW])/samples);
	else
		return 0;
}

/**
 * @brief   Max Barometric Pressure Getter
 * @param   max : Pointer to where to store data
 * @return  Nothing
 */
uint16_t getMaxBarometricPressure(){
	return weather_data[BAROMETRIC_PRESSURE][MAX_DATA];
}

/**
 * @brief   Min Barometric Pressure Getter
 * @param   min : Pointer to where to store data
 * @return  Nothing
 */
uint16_t getMinBarometricPressure(){
	return weather_data[BAROMETRIC_PRESSURE][MIN_DATA];
}

/**
 * @brief   Current Daily Rain Getter
 * @param   data : Pointer to where to store data
 * @return  Nothing
 */
uint16_t getCurrentDailyRain(){
	return weather_data[DAILY_RAIN][CURRENT_DATA];
}

/**
 * @brief   Current Yearly Rain Getter
 * @param   data : Pointer to where to store data
 * @return  Nothing
 */
uint16_t getCurrentYearlyRain(){
	return weather_data[YEARLY_RAIN][CURRENT_DATA];
}

/**
 * @brief   Current Battery Voltage Getter
 * @param   data : Pointer to where to store data
 * @return  Nothing
 */
uint16_t getCurrentBatteryVoltage(){
	return weather_data[BATTERY_VOLTAGE][CURRENT_DATA];
}
void WS_Tx() {
	uint8_t aux = BUFFER_Pop(Ws_Tx);
	if (aux != (uint8_t) EMPTY_BUFFER_ERROR) {
		UART_SendByte(LPC_UART2, aux);
	}
}
void WS_Rx() {
	while ((UART_GetLineStatus(LPC_UART2) & 0x1) == 1) {
		if (BUFFER_Push(Ws_Rx, UART_ReceiveByte(LPC_UART2)) == FULL_BUFFER_ERROR)
		{
			SYSTEM_Perform_Safe_Shutdown();
		}
	}
}

