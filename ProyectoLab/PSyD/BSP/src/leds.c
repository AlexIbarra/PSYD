#include <s3c44b0x.h>
#include <s3cev40.h>
#include <leds.h>

/*
** Inicializa los leds y los apaga
*/
void leds_init( void ){
	PCONB &= ~(1<<10) & ~(1<<9);	// PB[10] = out, PF[9] = out
	PDATB |= (RIGHT_LED<<9) | (LEFT_LED<<9);
}

/*
** Enciende el led indicado
*/
void led_on( uint8 led ){
	PDATB &= ~(led<<9);
}

/*
** Apaga el led indicado
*/
void led_off( uint8 led ){
	PDATB |= led<<9;
}

/*
** Conmuta el led indicado
*/
void led_toggle( uint8 led ){
	PDATB ^= (led<<9);
}

/*
** Devuelve el estado (ON/OFF) del led indicado
*/
uint8 led_status( uint8 led ){
	return ((PDATB & led<<9) >> 9);
}
