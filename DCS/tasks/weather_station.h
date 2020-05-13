/*
 * weather_station.h
 *
 *  Created on: 18 de dic. de 2019
 *      Author: Santiago
 */

#ifndef TASKS_WEATHER_STATION_H_
#define TASKS_WEATHER_STATION_H_

/*=================={include}================================================*/
#include "lpc_types.h"

/*==================[macros]=================================================*/
/* Model */
#define DAVIS_MODEL                     0x14,0x4d

/* Variables */
#define WCR_INSIDE_TEMPERATURE          0x44,0x30
#define WCR_OUTSIDE_TEMPERATURE         0x44,0x56

#define WCR_OUTSIDE_HUMIDITY            0x24,0x98
#define WCR_INSIDE_HUMIDITY             0x24,0x80

#define WCR_WIND_SPEED                  0x22,0x5e
#define WCR_WIND_DIRECTION              0x44,0xb4
#define WCR_WIND_CHILL					0x42,0xa8

#define WCR_BAROMETER                   0x44,0x00

#define WCR_YEARLY_RAIN                 0x44,0xce
#define WCR_DAILY_RAIN                  0x44,0xd2

#define WCR_DEW_POINT					0x42,0x8a

/* High and Low on Station */
#define WCR_HIGH_INSIDE_TEMPERATURE       		0x44,0x34
#define WCR_HIGH_INSIDE_TEMPERATURE_TIME       	0x44,0x3c
#define WCR_HIGH_INSIDE_TEMPERATURE_DATE       	0x34,0x44

#define WCR_LOW_INSIDE_TEMPERATURE      		0x44,0x38
#define WCR_LOW_INSIDE_TEMPERATURE_TIME       	0x44,0x40
#define WCR_LOW_INSIDE_TEMPERATURE_DATE       	0x34,0x47

#define WCR_HIGH_OUTSIDE_TEMPERATURE      		0x44,0x5a
#define WCR_HIGH_OUTSIDE_TEMPERATURE_TIME      	0x44,0x62
#define WCR_HIGH_OUTSIDE_TEMPERATURE_DATE      	0x34,0x6a

#define WCR_LOW_OUTSIDE_TEMPERATURE     		0x44,0x5e
#define WCR_LOW_OUTSIDE_TEMPERATURE_TIME       	0x44,0x66
#define WCR_LOW_OUTSIDE_TEMPERATURE_DATE       	0x34,0x6d

#define WCR_HIGH_WIND_SPEED                		0x42,0x60
#define WCR_HIGH_WIND_SPEED_TIME      			0x42,0x64
#define WCR_HIGH_WIND_SPEED_DATE		       	0x32,0x68

#define WCR_LOW_WIND_CHILL                 		0x42,0xac
#define WCR_LOW_WIND_CHILL_TIME      			0x42,0xb0
#define WCR_LOW_WIND_CHILL_DATE			       	0x32,0xb4

#define WCR_HIGH_DEW_POINT       				0x42,0x8e
#define WCR_HIGH_DEW_POINT_TIME       			0x42,0x96
#define WCR_HIGH_DEW_POINT_DATE       			0x32,0xa1

#define WCR_LOW_DEW_POINT      					0x42,0x92
#define WCR_LOW_DEW_POINT_TIME       			0x42,0x9a
#define WCR_LOW_DEW_POINT_DATE       			0x32,0x9e

#define WCR_HIGH_INSIDE_HUMIDITY          		0x24,0x82
#define WCR_HIGH_INSIDE_HUMIDITY_TIME       	0x44,0x86
#define WCR_HIGH_INSIDE_HUMIDITY_DATE       	0x34,0x8e

#define WCR_LOW_INSIDE_HUMIDITY         		0x24,0x84
#define WCR_LOW_INSIDE_HUMIDITY_TIME       		0x44,0x8a
#define WCR_LOW_INSIDE_HUMIDITY_DATE       		0x34,0x91

#define WCR_HIGH_OUTSIDE_HUMIDITY         		0x24,0x9a
#define WCR_HIGH_OUTSIDE_HUMIDITY_TIME     		0x44,0x9e
#define WCR_HIGH_OUTSIDE_HUMIDITY_DATE     		0x34,0xa6

#define WCR_LOW_OUTSIDE_HUMIDITY        		0x24,0x9c
#define WCR_LOW_OUTSIDE_HUMIDITY_TIME      		0x44,0xa2
#define WCR_LOW_OUTSIDE_HUMIDITY_DATE      		0x34,0xa9

#define CCR_AVG_INSIDE_TEMPERATURE      		1,0x94,4
#define CCR_AVG_OUTSIDE_TEMPERATURE    		    1,0x98,4
#define CCR_AVG_WIND_SPEED                   	1,0x9c,2

/* Calibration numbers in station*/
#define WCR_CAL_RAIN                    		0x44,0xd6
#define WCR_CAL_RAIN_COMMAND					0x43
#define WCR_CAL_RAIN_ADDRESS					0xd6

#define WCR_CAL_BAROMETER               		0x44,0x2c
#define WCR_CAL_BAROMETER_COMMAND				0x43
#define WCR_CAL_BAROMETER_ADDRESS				0x2c

#define WCR_CAL_INSIDE_TEMPERATURE 		        0x44,0x52
#define WCR_CAL_INSIDE_TEMPERATURE_COMMAND		0x43
#define WCR_CAL_INSIDE_TEMPERATURE_ADDRESS		0x52

#define WCR_CAL_OUTSIDE_TEMPERATURE     		0x44,0x78
#define WCR_CAL_OUTSIDE_TEMPERATURE_COMMAND		0x43
#define WCR_CAL_OUTSIDE_TEMPERATURE_ADDRESS		0x78

#define WCR_CAL_HUMIDITY  		        		0x44,0xda
#define WCR_CAL_HUMIDITY_COMMAND				0x43
#define WCR_CAL_HUMIDITY_ADDRESS				0xda

/* Time */
#define WCR_DATE_COMMAND				0x43
#define WCR_DATE_ADDRESS				0xc8
#define WCR_TIME_COMMAND				0x63
#define WCR_TIME_ADDRESS				0xbe

#define WCR_MONTH 						0x14,0xca
#define WCR_DAY 						0x24,0xc8
#define WCR_HOUR						0x24,0xbe
#define WCR_MINUTES						0x24,0xc0
#define WCR_SECONDS						0x24,0xc2



/* User defined calibration numbers */
#define SOFTWARE_CALIBRATION				 1

#define SOFTWARE_CAL_RAIN               	 0
#define SOFTWARE_CAL_BAROMETER               0
#define SOFTWARE_CAL_INSIDE_TEMPERATURE      0
#define SOFTWARE_CAL_OUTSIDE_TEMPERATURE     0
#define SOFTWARE_CAL_HUMIDITY			     0

/* Errors */
#define POSITIVE_SIGNED_4NIBBLE_ERROR		 0x7fff
#define NEGATIVE_SIGNED_4NIBBLE_ERROR		 0x8000
#define POSITIVE_SIGNED_2NIBBLE_ERROR		 0x7f
#define NEGATIVE_SIGNED_2NIBBLE_ERROR		 0x80

#define NO_ERROR							0
#define HEADER_ERROR						1
#define CRC_ERROR							2

/* Loop constants */
#define HEADER 							 	0x01
#define SENSOR_IMAGE_LENGTH					18




#define WAIT_TIMES						6000
#define WAIT_DATA_TIMES					100
/*==================[macros]=================================================*/
/* Weather Macros */

/*==================[typedef]================================================*/

typedef enum {
	INSIDE_TEMPERATURE	=	0,  /* INT Value */
	OUTSIDE_TEMPERATURE =	1,	/* INT Value */
	DEW_POINT  			= 	2,
	INSIDE_HUMIDITY		= 	3,
	OUTSIDE_HUMIDITY    = 	4,
	WIND_SPEED			= 	5,
	WIND_CHILL			= 	6,
	WIND_DIRECTION		=	7, 	/* Only current data */
	BAROMETRIC_PRESSURE	= 	8,
	DAILY_RAIN			= 	9, 	/* Only current data */
	YEARLY_RAIN			=	10, /* Only current data */
	BATTERY_VOLTAGE		=	11, /* Only current data */
	WEATHER_DATA		=	12	/*Field Count*/
} WS_Fields;

typedef enum {
	CURRENT_DATA		=	0,
	MAX_DATA			=	1,
	MIN_DATA			=	2,
	AVG_DATA_HIGH		=	3,
	AVG_DATA_LOW		=	4,
	WEATHER_PARAMETERS	=	5	/*Parameter Count*/
} WS_Parameters;
/* Weather Parameters */

/*===========================================================================*/
void WS_Init(); /*Initialize structure*/

void WS_UpdateData(); /*Read from WS and store in structure*/

/* Inside Temperature */
int16_t getCurrentInsideTemperature();
int16_t getAverageInsideTemperature();
int16_t getMaxInsideTemperature();
int16_t getMinInsideTemperature();

/* Outside Temperature */
int16_t getCurrentOutsideTemperature();
int16_t getAverageOutsideTemperature();
int16_t getMaxOutsideTemperature();
int16_t getMinOutsideTemperature();

/* Dew Point */
uint16_t getCurrentDewPoint();
uint16_t getAverageDewPoint();
uint16_t getMaxDewPoint();
uint16_t getMinDewPoint();

/* Inside Humidity */
uint16_t getCurrentInsideHumidity();
uint16_t getAverageInsideHumidity();
uint16_t getMaxInsideHumidity();
uint16_t getMinInsideHumidity();

/* Outside Humidity */
uint16_t getCurrentOutsideHumidity();
uint16_t getAverageOutsideHumidity();
uint16_t getMaxOutsideHumidity();
uint16_t getMinOutsideHumidity();

/* Wind Speed */
uint16_t getCurrentWindSpeed();
uint16_t getAverageWindSpeed();
uint16_t getMaxWindSpeed();
uint16_t getMinWindSpeed();

/* Wind Chill */
uint16_t getCurrentWindChill();
uint16_t getAverageWindChill();
uint16_t getMaxWindChill();
uint16_t getMinWindChill();

/* Wind Direction*/
uint16_t getCurrentWindDirection();

/* Barometric Pressure */
uint16_t getCurrentBarometricPressure();
uint16_t getAverageBarometricPressure();
uint16_t getMaxBarometricPressure();
uint16_t getMinBarometricPressure();

/* Daily Rain*/
uint16_t getCurrentDailyRain();

/* Yearly Rain*/
uint16_t getCurrentYearlyRain();

/* Battery Voltage */
uint16_t getCurrentBatteryVoltage();

/* Cardinal Point */
char* cardinalPoint(uint16_t point);

uint32_t WS_getSamples(void);
void WS_TxRx(void);

#endif /* TASKS_WEATHER_STATION_H_ */
