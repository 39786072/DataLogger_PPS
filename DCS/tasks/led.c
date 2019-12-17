/*
 * led.c
 *
 *  Created on: 28 de nov. de 2019
 *      Author: Santiago
 */
#include "../main/main.h"
void LED_Init(void){


	GPIO_SetDir(MODE0_LED_PORT, MODE0_LED_PIN, 1);
	GPIO_SetDir(MODE1_LED_PORT, MODE1_LED_PIN, 1);
	GPIO_SetDir(ERROR0_LED_PORT, ERROR0_LED_PIN, 1);
	GPIO_SetDir(ERROR1_LED_PORT, ERROR1_LED_PIN, 1);
	GPIO_SetDir(ERROR2_LED_PORT, ERROR2_LED_PIN, 1);
}
