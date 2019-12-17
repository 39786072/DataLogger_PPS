/*
===============================================================================
 Name        : Blinky.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>
#define DELAY 12000000UL
#define LPCXpresso_Led (1<<22)
// FUNCION DELAY, RETARDO POR SOFT.
void delay(void){
    unsigned long i; // Declaración de variable entera para uso dentro del retardo.
    for(i=DELAY; i>0; i--);
    }
// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE RESET.
int main(void) {
    // INICIALIZACIONES DE PERIFERICOS Y DECLARACIONES
    LPC_GPIO0->FIODIR |= LPCXpresso_Led; //Seteo la dirección como salida
    // SUPERLOOP
    while(1) {
        LPC_GPIO0->FIOSET = LPCXpresso_Led; // Prendo el led
        delay(); // Retardo por Soft
        LPC_GPIO0->FIOCLR = LPCXpresso_Led; // Apago el led
        delay(); // Retardo por Soft
        }
return 0 ;
}
/* La estructura LPC_GPIO0 (*) esta formada por los siguientes registros:
 * FIODIR: Permite establecer la dirección del pin. 1 para salida, 0 para entrada. (**)
 * FIOSET: Permite poner un 1 lógico en un pin. FIOCLEAR: Permite poner un 0 lógico en un pin.
 * FIOPIN: Permite leer el estado de los pines FIOMAS: Establece una máscara para las acciones de PIN, SET o CLEAR. (*)
 * GPIO = General Purpose Input/Output, definida en el archivo LPC17xx.h (**)
 * FIO = Fast I/O. */
