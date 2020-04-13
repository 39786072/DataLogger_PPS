/*
 * adc.c
 *
 *  Created on: 25 de mar. de 2020
 *      Author: Santiago
 */
#include "lpc17xx_adc.h"
#include "lpc17xx_pinsel.h"
static uint16_t adc_value;

void ADC_Task(void)
{
	ADC_StartCmd(LPC_ADC,ADC_START_NOW);
	while (!(ADC_ChannelGetStatus(LPC_ADC,ADC_CHANNEL_0,ADC_DATA_DONE)));
	adc_value = ADC_ChannelGetData(LPC_ADC,ADC_CHANNEL_0);

}

void ADCt_Init(void){
	PINSEL_CFG_Type PinCfg;
	PinCfg.Funcnum = 1;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Pinnum = 23;
	PinCfg.Portnum = 0;
	PINSEL_ConfigPin(&PinCfg);

	ADC_Init(LPC_ADC, 200000);
	ADC_IntConfig(LPC_ADC,ADC_ADINTEN0,DISABLE);
	ADC_ChannelCmd(LPC_ADC,ADC_CHANNEL_0,ENABLE);
}
