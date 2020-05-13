/*
 * ptt.h
 *
 *  Created on: 19 de set. de 2019
 *      Author: Santiago
 */

#ifndef TASKS_PTT_H_
#define TASKS_PTT_H_

/*==================[macros]=================================================*/
#define BUFFER_NUMBER		3
#define BUFFER_LENGTH		32

#define CRC				0	/* Last byte of two's complement sum (1 byte)*/
#define NOB				1	/* Number of Observation (1 byte) */
#define IDUA			2	/* Interval from last sample (2 bytes) */
#define OT 				4	/* Outside Temperature (2 bytes) */
#define AVGT 			6	/* Average Outside Temperature (2 bytes) */
#define HT	 			8	/* High Outside Temperature (2 bytes) */
#define LT	 			10	/* Low Outside Temperature (2 bytes) */
#define DP	 			12	/* Dew Point (2 bytes) */
#define OH	 			14	/* Outside Humidity (1 byte) */
#define HH	 			15	/* High Outside Humidity (1 byte) */
#define LH	 			16	/* Low Outside Humidity (1 byte) */
#define WS	 			17	/* Wind Speed (1 byte) */
#define AVGWS	 		18	/* Average Wind Speed (1 byte) */
#define HWS	 			19	/* High Wind Speed (1 byte) */
#define WC	 			20	/* Wind Chill (2 bytes) */
#define WD	 			22	/* Wind Direction (2 bytes) */
#define BP	 			24	/* Barometric Pressure (2 bytes) */
#define DR	 			26	/* Yearly Rain (2 bytes) */
#define YR	 			28 	/* Daily Rain (2 bytes) */
#define BV	 			30	/* Battery Voltage (2 bytes) */

#define WAIT_TIMES						6000
#define WAIT_DATA_TIMES					100

void PTT_Init(void);
void PTT_TxRx(void);
void PTT_Task(void);
void Ptt_TakeData_Task(void);
void PTT_UpdateBuffers_Task(void);

#endif /* TASKS_PTT_H_ */
