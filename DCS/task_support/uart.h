/*
 * uart.h
 *
 *  Created on: 5 de ago. de 2019
 *      Author: Santiago
 */

#ifndef TASK_SUPPORT_UART_H_
#define TASK_SUPPORT_UART_H_
#include <lpc17xx_pinsel.h>
#include <lpc17xx_uart.h>
#include <lpc17xx_gpio.h>
void UART_Configure(LPC_UART_TypeDef* UART, uint32_t BaudRate);
#endif /* TASK_SUPPORT_UART_H_ */
