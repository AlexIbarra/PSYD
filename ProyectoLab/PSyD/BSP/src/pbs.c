/*
 * pbs.c
 *
 *  Created on: 08/05/2015
 *      Author: usuario_local
 */
#include <pbs.h>
#include <s3c44b0x.h>
#include <s3cev40.h>
#include <timers.h>



void pb_wait_keyup(uint8 pb);
void pb_wait_keydown(uint8 pb);
uint8 pb_status (uint8 scancode);

extern void isr_PB_dummy( void );

 void timers_init();
 void timer3_start();

void pbs_init( void )
{
	timers_init();
}
void pbs_open( void (* isr)(void) )
{
	pISR_PB = (uint32)isr;
	EXTINTPND = ~0;
	I_ISPC = BIT_EINT4567;
	INTMSK &= ~(BIT_GLOBAL | BIT_EINT4567);
}
void pbs_close( void )
{
	INTMSK |= (BIT_GLOBAL | BIT_EINT4567);
	pISR_PB = (uint32)isr_PB_dummy;
}

uint8 pb_scan( void )
{
	if( !(PDATG & (1<<6)))
		return PB_LEFT;
	else if( !(PDATG & (1 << 7)) )
		return PB_RIGHT;
	else
		return PB_FAILURE;
}

uint8 pb_getchar( void ){
	uint8 scancode;
	while( (PDATG & (1<<6)) != 0 && (PDATG & (1 << 7)) != 0 );
	sw_delay_ms( PB_KEYDOWN_DELAY );
	scancode = pb_scan();

	while(!(PDATG & (1<<6)) || !(PDATG & (1 << 7)) );
	sw_delay_ms( PB_KEYUP_DELAY );

	return scancode;

}
uint8 pb_getchartime( uint16 *ms )
{
	uint8 scancode;
	while( (PDATG & (1<<6)) != 0 && (PDATG & (1 << 7)) != 0 );
	timer3_start();
	sw_delay_ms( PB_KEYDOWN_DELAY );
	scancode = pb_scan();

	while(!(PDATG & (1<<6)) || !(PDATG & (1 << 7)) );
	*ms = timer3_stop() / 10;
	sw_delay_ms( PB_KEYUP_DELAY );

    return scancode;
}

void pb_wait_keyup(uint8 pb) {

	//EXTINT = (EXTINT & ~(0xFF << 24)) | (0xFF << 24);
	if (pb == PB_RIGHT) {
		pb_wait_keydown(pb);
		uint32 value = (PDATG & (1 << 7));
		while (value != 128)
			value = (PDATG & (1 << 7));
		sw_delay_ms(PB_KEYUP_DELAY);
	} else if (pb == PB_LEFT) {
		pb_wait_keydown(pb);
		uint32 value = (PDATG & (1 << 6));
		while (value != 64)
			value = (PDATG & (1 << 6));
		sw_delay_ms(PB_KEYUP_DELAY);
	}
}

void pb_wait_keydown(uint8 pb) {
//POR PULLDOWN = FALLING EDGE
	//EXTINT = (EXTINT & ~(0xFF << 24)) | (0x22 << 24);
	if (pb == PB_RIGHT) {
		uint32 value = (PDATG & (1 << 7));
		while (value == 128)
			value = (PDATG & (1 << 7));
	} else if (pb == PB_LEFT) {
		uint32 value = (PDATG & (1 << 6));
		while (value == 64)
			value = (PDATG & (1 << 6));
	}
	sw_delay_ms(PB_KEYDOWN_DELAY);
}

uint8 pb_status (uint8 scancode){

	if(scancode == (1 <<6)){
		if(pb_scan() == (1 <<6)){
			return PB_DOWN;
		}
		else{
			return PB_UP;
		}
	}else if(scancode == (1 << 7)){
		if(pb_scan() == (1 <<7))
			return PB_DOWN;
		else
			return PB_UP;

	}

	return PB_FAILURE;


}

uint8 pb_timeout_getchar(uint16 n) {
	uint8 scancode;
	timer3_start_timeout(n*10);
	while (pb_scan() == PB_FAILURE && timer3_timeout())
		;
	timer3_stop();
	if(!timer3_timeout()){
		sw_delay_ms(PB_KEYDOWN_DELAY);
		scancode = pb_scan();
		timer3_start_timeout(n*10);
		while (pb_scan() != PB_FAILURE && timer3_timeout())
			;
		timer3_stop();
		if(!timer3_timeout()){
			sw_delay_ms(PB_KEYUP_DELAY);
			return scancode;
		}
		else
			return PB_TIMEOUT;
	}else
		return PB_TIMEOUT;
}

void pb_wait_any_keydown( void ) {
    while( ((PDATG & PB_LEFT) == PB_LEFT) && ((PDATG & PB_RIGHT) == PB_RIGHT));
    sw_delay_ms( PB_KEYDOWN_DELAY );
}

void pb_wait_any_keyup( void ) {
	pb_wait_any_keydown();
    while( ((PDATG & PB_LEFT) != 0) && ((PDATG & PB_RIGHT) != 0));
    sw_delay_ms( PB_KEYUP_DELAY );
}




