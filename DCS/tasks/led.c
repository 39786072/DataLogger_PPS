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

	GPIO_SetDir(LED2R_PORT, LED2R_PIN, 1);
	GPIO_SetDir(LED2G_PORT, LED2G_PIN, 1);
	GPIO_SetDir(LED2B_PORT, LED2B_PIN, 1);

	GPIO_ClearValue(MODE0_LED_PORT, MODE0_LED_PIN);
	GPIO_ClearValue(MODE1_LED_PORT, MODE1_LED_PIN);

	GPIO_ClearValue(LED2R_PORT, LED2R_PIN);
	GPIO_ClearValue(LED2G_PORT, LED2G_PIN);
	GPIO_ClearValue(LED2B_PORT, LED2B_PIN);
}
