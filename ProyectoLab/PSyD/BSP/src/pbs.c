#include<pbs.h>
#include <s3c44b0x.h>
#include <s3cev40.h>
#include <timers.h>

extern void isr_PB_dummy (void);

/*
** Inicializa los pulsadores
** Inicializa timers
*/
void pbs_init( void ) {
    timers_init(); // únicamente inicializa temporizadores, la configuración de puertos la hace system_init()
}

/*
** Si hay un pulsador presionado devuelve su scancode, en otro caso devuelve PB_FAILURE
*/
uint8 pb_scan( void ) {
    /* Lee secuencialmente los pulsadores para determinar el código a devolver */
    if( !(PDATG & PB_LEFT) )
        return PB_LEFT;
    else if( !(PDATG & PB_RIGHT) )
        return PB_RIGHT;
    else
        return PB_FAILURE; // si ninguno está pulsado devuelve fallo
}

/*
** Devuelve el estado del pulsador indicado
*/
uint8 pb_status( uint8 scancode ) {

    if(scancode == PB_LEFT){
		if(pb_scan() == PB_LEFT)
			return ~(PB_UP);//PB_DOWN;
		else
			return ~(PB_DOWN);//PB_UP;
	} else if(scancode == PB_RIGHT) {
		if(pb_scan() == PB_RIGHT)
			return ~(PB_UP);//PB_DOWN;
		else
			return ~(PB_DOWN);//PB_UP;
	}

	return PB_FAILURE;
}

/*
** Espera a que se presione el pulsador indicado
*/
void pb_wait_keydown( uint8 scancode ) {

}

/*
** Espera a que se presione y despresione el pulsador indicado
*/
void pb_wait_keyup( uint8 scancode ) {

}

/*
** Espera a que se presione cualquier pulsador
*/
void pb_wait_any_keydown( void ) {

}

/*
** Espera a que se presione y depresione cualquier pulsador
*/
void pb_wait_any_keyup( void ) {
    while( (PDATG & PB_LEFT) && (PDATG & PB_RIGHT) ); // espera la presión de cualquier pulsador
    sw_delay_ms( PB_KEYDOWN_DELAY ); // espera SW (el timer 3 está ocupado) fin de rebotes
    while( !(PDATG & PB_LEFT) || !(PDATG & PB_RIGHT) ); // espera la depresión de cualquier pulsador
    sw_delay_ms( PB_KEYUP_DELAY ); // espera SW (el timer 3 está ocupado) fin de rebotes
}

/*
** Espera la presi�n y depresi�n de un pulsador y devuelve su scancode
*/
uint8 pb_getchar( void ) {
    uint8 scancode;

    while( (PDATG & PB_LEFT) && (PDATG & PB_RIGHT) ); // espera la presión de cualquier pulsador
    sw_delay_ms( PB_KEYDOWN_DELAY ); // espera SW (el timer 3 está ocupado) fin de rebotes
    scancode = pb_scan(); // obtiene el código del pulsador presionado
    while( !(PDATG & PB_LEFT) || !(PDATG & PB_RIGHT) ); // espera la depresión de cualquier pulsador
    sw_delay_ms( PB_KEYUP_DELAY ); // espera SW (el timer 3 está ocupado) fin de rebotes

    return scancode; // devuelve el código del pulsador presionado
}

/*
** Espera la presi�n y depresi�n de un pulsador y devuelve su scancode y el intervalo en ms que ha estado pulsado (max. 6553ms)
*/
uint8 pb_getchartime( uint16 *ms ) {
    uint8 scancode;

    while( (PDATG & PB_LEFT) && (PDATG & PB_RIGHT) ); // espera la presión de cualquier pulsador
    timer3_start(); // arranca el timer 3 (0,1 ms de resolución)
    sw_delay_ms( PB_KEYDOWN_DELAY ); // espera SW (el timer 3 está ocupado) fin de rebotes
    scancode = pb_scan(); // obtiene el código del pulsador presionado
    while( !(PDATG & PB_LEFT) || !(PDATG & PB_RIGHT) ); // espera la depresión de cualquier pulsador
    *ms = timer3_stop() / 10; // detiene el timer 3 y calcula los ms
    sw_delay_ms( PB_KEYUP_DELAY ); // espera SW (el timer 3 está ocupado) fin de rebotes

    return scancode; // devuelve el código del pulsador presionado
}

/*
** Espera un m�ximo de ms (no mayor de 6553ms) la presi�n y depresi�n de un pulsador y devuelve su scancode, en caso contrario devuelve PB_TIMEOUT
*/
uint8 pb_timeout_getchar( uint16 ms ) {
    uint8 scancode;

    timer3_start_timeout(ms*10); // arranca el timer 3 (0,1 ms de resolución)
    while( (PDATG & PB_LEFT) && (PDATG & PB_RIGHT)  && !timer3_timeout()); // espera la presión de cualquier pulsador
    timer3_stop();
    if(timer3_timeout())
        return PB_TIMEOUT;
    else {
        timer3_start_timeout(ms*10); // arranca el timer 3 (0,1 ms de resolución)
        sw_delay_ms( PB_KEYDOWN_DELAY ); // espera SW (el timer 3 está ocupado) fin de rebotes
        scancode = pb_scan(); // obtiene el código del pulsador presionado
        while( !(PDATG & PB_LEFT) || !(PDATG & PB_RIGHT) && !timer3_timeout()); // espera la depresión de cualquier pulsador
        timer3_stop();
        if(timer3_timeout())
            return PB_TIMEOUT;
        else
            sw_delay_ms( PB_KEYUP_DELAY ); // espera SW (el timer 3 está ocupado) fin de rebotes
    }

    return scancode; // devuelve el código del pulsador presionado
}

/*
** Instala, en la tabla de vectores de interrupci�n, la funci�n isr como RTI de interrupciones por presi�n de un pulsador
** Borra interrupciones pendientes por presi�n de un pulsador
** Desenmascara globalmente las interrupciones y espec�ficamente las interrupciones por presi�n de un pulsador
*/
void pbs_open( void (*isr)(void) ) {
    pISR_PB = (uint32)isr; // instala la RTI argumento en la tabla virtual de vectores de IRQ
    EXTINTPND = ~(0x0); // borra flag de interrupción pendiente por interrupciones externas
    I_ISPC = BIT_EINT4567; // borra flag de interrupción pendiente por interrupciones por presión de pulsador
    INTMSK &= ~(BIT_GLOBAL | BIT_EINT4567); // desenmascara globalmente interrupciones e interrupciones por presión de pulsador
}

/*
** Enmascara las interrupciones por presi�n de un pulsador
** Desinstala la RTI por presi�n de un pulsador
*/
void pbs_close( void ) {
    INTMSK |= (BIT_GLOBAL | BIT_EINT4567); // enmascara interrupciones por presión de pulsador
    pISR_PB = (uint32)isr_PB_dummy; // instala isr_PB_dummy en la tabla virtual de vectores de interrupción
}
