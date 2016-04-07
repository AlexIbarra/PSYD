#include <s3c44b0x.h>
#include <s3cev40.h>
#include <leds.h>

static uint8 state_left;
static uint8 state_right;
static uint8 state;

/*
** Inicializa los leds y los apaga
*/
void leds_init( void ){
	PCONB &= ~(1<<10) & ~(1<<9);	// PB[10] = out, PF[9] = out
	PDATB |= (RIGHT_LED<<9) | (LEFT_LED<<9);
	state_left = OFF;
	state_right = OFF;
	state &= 0;
}

/*
** Enciende el led indicado
*/
void led_on( uint8 led ){
	PDATB &= ~(led<<9);
	state |= led;
	/*if(led == LEFT_LED)
		state_left = ON;
	else
		state_right = ON;*/
}

/*
** Apaga el led indicado
*/
void led_off( uint8 led ){
	PDATB |= led<<9;
	state &= led;
	/*if(led == LEFT_LED)
		state_left = OFF;
	else
		state_right = OFF;*/
}

/*
** Conmuta el led indicado
*/
void led_toggle( uint8 led ){
	PDATB ^= (led<<9);
	state ^= led;
	/*if(led == LEFT_LED){
		if(state_left == ON)
			state_left = OFF;
		else
			state_left = ON;
	}
	else{
		if(state_right == ON)
			state_right = OFF;
		else
			state_right = ON;
	}*/
}

/*
** Devuelve el estado (ON/OFF) del led indicado
*/
uint8 led_status( uint8 led ){
//	uint8 st;
//	if(led == LEFT_LED){
//		st = ((state & led<<9) >> 9);
//		if(st == OFF)
//			return ON;
//		else
//			return OFF;
//	}
//	else{
//		st = ((state & led<<9) >> 10);
//		if(st == OFF)
//			return ON;
//		else
//			return OFF;
//	}

	return ((state & led) >> led-1);
	/*if(led == LEFT_LED)
		return state_left;
	else
		return state_right;*/
}
