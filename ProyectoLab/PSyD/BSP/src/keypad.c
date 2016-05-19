/*
 * keypad.c
 *
 *  Created on: 08/05/2015
 *      Author: usuario_local
 */
#include <keypad.h>
#include <s3c44b0x.h>
#include <s3cev40.h>
#include <timers.h>


void keypad_down_isr( void );
extern void isr_KEYPAD_dummy(void);

uint8 keypad_scan( void )
{
	uint8 aux;
	aux = *( KEYPAD_ADDR + 0x1c );
	if( (aux & 0x0f) != 0x0f )
	{
		if( (aux & 0x8) == 0 ) return KEYPAD_KEY0;
		else if( (aux & 0x4) == 0 ) return KEYPAD_KEY1;
		else if( (aux & 0x2) == 0 ) return KEYPAD_KEY2;
		else if( (aux & 0x1) == 0 ) return KEYPAD_KEY3;
	}

	aux = *(KEYPAD_ADDR + 0xFB);
	aux = (aux & 0xF); //nos quitamos la quinta fila y devolvemos un -1 si no hay datos
	switch (aux){
	case 7: return KEYPAD_KEY4;
			break;
	case 11:return KEYPAD_KEY5;
			break;
	case 13:return KEYPAD_KEY6;
			break;
	case 14:return KEYPAD_KEY7;
			break;
	}

	aux = *(KEYPAD_ADDR + 0xF7);
	aux = (aux & 0xF); //nos quitamos la quinta fila y devolvemos un -1 si no hay datos
	switch (aux){
	case 7:return KEYPAD_KEY8;
			break;
	case 11:return KEYPAD_KEY9;
			break;
	case 13:return KEYPAD_KEYA;
			break;
	case 14:return KEYPAD_KEYB;
			break;
	}

	aux = *(KEYPAD_ADDR + 0xEF);
	aux = (aux & 0xF); //nos quitamos la quinta fila y devolvemos un -1 si no hay datos
	switch (aux){
	case 7:return KEYPAD_KEYC;
			break;
	case 11:return KEYPAD_KEYD;
			break;
	case 13:return KEYPAD_KEYE;
			break;
	case 14:return KEYPAD_KEYF;
			break;
	}
	return KEYPAD_FAILURE;
}


void keypad_wait_keydown( uint8 scancode )
{
	while(1)
	{
	while(keypad_scan()!=scancode);

	sw_delay_ms( KEYPAD_KEYDOWN_DELAY );
	if( scancode == keypad_scan() )
		return;

	while(!(PDATG & (1<<1)));
	sw_delay_ms( KEYPAD_KEYUP_DELAY );

	}

}

void keypad_wait_keyup(uint8 scancode){
	keypad_wait_keydown(scancode);
	while(!(PDATG & (1<<1)));		//mientras sea =0 es decir ESTE PULSADA!!!!!!
		sw_delay_ms(KEYPAD_KEYUP_DELAY);


}
void keypad_wait_any_keydown(void){
	while((PDATG & (1<<1)));
	sw_delay_ms(KEYPAD_KEYDOWN_DELAY);

}

/*
 ** Espera a que se pulse y despulse cualquier tecla
 */
void keypad_wait_any_keyup(void){

	keypad_wait_any_keydown();
	while(!(PDATG & (1<<1)));
		sw_delay_ms(KEYPAD_KEYUP_DELAY);


}

uint8 keypad_getchar(void){
	keypad_wait_any_keydown();
	return keypad_scan();

}

/*
 ** Espera la pulsaci�n de una tecla del keypad y devuelve su scancode y el intervalo en ms que ha estado pulsada (max. 65535ms)
 */
uint8 keypad_getchartime(uint16 *ms){
	uint8 value = KEYPAD_FAILURE;
	while((PDATG & (1<<1)));
	timer3_start();
	sw_delay_ms(PB_KEYDOWN_DELAY);
	int temp=keypad_getchar();
	while(!(PDATG & (1<<1)));
	*ms = timer3_stop() / 10; //Paro temporizador
	sw_delay_ms(PB_KEYUP_DELAY);
	return temp;
}

/*
 ** Espera hasta un m�ximo de n segundos la pulsaci�n de una tecla del keypad y devuelve su scancode, en caso contrario devuelve KEYPAD_TIMEOUT
 */
uint8 keypad_timeout_getchar(uint16 n){
	timer3_start_timeout(n*10000);
	while ((!(timer3_timeout()))|| (!(PDATG & (1<<1))));
	timer3_start();
	sw_delay_ms(PB_KEYDOWN_DELAY);
	return keypad_getchar();


}

void keypad_open(void (* isr)(void)){
	pISR_KEYPAD= ((uint32)isr);
	EXTINTPND = ~0;
	I_ISPC = BIT_KEYPAD;
	INTMSK &= ~(BIT_GLOBAL | BIT_KEYPAD);

}

/*
 ** Deshabilita las interrupciones por pulsaci�n del keypad
 ** Desinstala la RTI por pulsaci�n del keypad
 */
void keypad_close(void){
	INTMSK |= (BIT_KEYPAD);
	pISR_KEYPAD= (uint32)isr_KEYPAD_dummy;
}

void keypad_up_isr( void )
{
sw_delay_ms( KEYPAD_KEYUP_DELAY );
EXTINT = (EXTINT & ~(0xf<<4)) | (2<<4);
keypad_open( keypad_down_isr );
I_ISPC = BIT_KEYPAD;
}
void keypad_down_isr( void )
{
	sw_delay_ms( KEYPAD_KEYDOWN_DELAY );

	EXTINT = (EXTINT & ~(0xf<<4)) | (4<<4);
	keypad_open( keypad_up_isr );
	I_ISPC = BIT_KEYPAD;
}

void keypad_init(void) {
	timers_init();

}


