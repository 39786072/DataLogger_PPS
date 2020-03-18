/*
 * ptt.h
 *
 *  Created on: 19 de set. de 2019
 *      Author: Santiago
 */

#ifndef TASKS_PTT_H_
#define TASKS_PTT_H_


#define WAIT_TIMES						6000
#define WAIT_DATA_TIMES					100

void PTT_Init(void);
void PTT_Tx(void);
void PTT_Rx(void);
void PTT_Task(void);


#endif /* TASKS_PTT_H_ */
