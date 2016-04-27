#include <s3c44b0x.h>
#include <s3cev40.h>
#include <keypad.h>


/*
** Inicializa el keypad
** Inicializa timers
*/
void keypad_init( void ) {
    EXTINT = (EXTINT & ~(0xf<<4)) | (2<<4); // las interrupciones por EINT1 se generan a flanco de bajada de la señal
    keypad_open( keypad_down_isr );
}

/*
** Si hay una tecla presionada devuelve su scancode, en otro caso devuelve KEYPAD_FAILURE
*/
uint8 keypad_scan( void ) {
    uint8 aux;
    aux = *( KEYPAD_ADDR + 0x1c )
    if( (aux & 0x0f) != 0x0f ) {
        if( (aux & 0x8) == 0 )  return KEYPAD_KEY0;
        else if( (aux & 0x4) == 0 ) return KEYPAD_KEY1;
        else if( (aux & 0x2) == 0 ) return KEYPAD_KEY2;
        else if( (aux & 0x1) == 0 ) return KEYPAD_KEY3;
    }
    aux = *( KEYPAD_ADDR + 0xfb )
    if( (aux & 0x0f) != 0x0f ) {
        if( (aux & 0x8) == 0 )  return KEYPAD_KEY4;
        else if( (aux & 0x4) == 0 ) return KEYPAD_KEY5;
        else if( (aux & 0x2) == 0 ) return KEYPAD_KEY6;
        else if( (aux & 0x1) == 0 ) return KEYPAD_KEY7;
    }
    aux = *( KEYPAD_ADDR + 0xf7 )
    if( (aux & 0x0f) != 0x0f ) {
        if( (aux & 0x8) == 0 )  return KEYPAD_KEY8;
        else if( (aux & 0x4) == 0 ) return KEYPAD_KEY9;
        else if( (aux & 0x2) == 0 ) return KEYPAD_KEYA;
        else if( (aux & 0x1) == 0 ) return KEYPAD_KEYB;
    }
    aux = *( KEYPAD_ADDR + 0xef )
    if( (aux & 0x0f) != 0x0f ) {
        if( (aux & 0x8) == 0 )  return KEYPAD_KEYC;
        else if( (aux & 0x4) == 0 ) return KEYPAD_KEYD;
        else if( (aux & 0x2) == 0 ) return KEYPAD_KEYE;
        else if( (aux & 0x1) == 0 ) return KEYPAD_KEYF;
    }

    return KEYPAD_FAILURE;
}

/*
** Devuelve el estado de la tecla indicada
*/
uint8 keypad_status( uint8 scancode ) {

}

/*
** Espera a que se presione la tecla indicada
*/
void keypad_wait_keydown( uint8 scancode ) {
    while(1) {
        while( keypad_scan()!=scancode ); //Espera presión de
        sw_delay_ms( KEYPAD_KEYDOWN_DELAY );
        if( scancode == keypad_scan() ) //Si la tecla pulsada es la indicada, retorna
            return;
        while( !(PDATG & (1<<1)) ); //Si no lo es, espera depresión y vuelve a empezar
        sw_delay_ms( KEYPAD_KEYUP_DELAY );
    }
}

/*
** Espera a que se presione y depresione la tecla indicada
*/
void keypad_wait_keyup( uint8 scancode ) {

}

/*
** Espera a que se presione cualquier tecla
*/
void keypad_wait_any_keydown( void ) {

}

/*
** Espera a que se presione y depresione cualquier tecla
*/
void keypad_wait_any_keyup( void ) {

}

/*
** Espera la presi�n y depresi�n de una tecla del keypad y devuelve su scancode
*/
uint8 keypad_getchar( void ) {

}

/*
** Espera la presi�n y depresi�n de una tecla del keypad y devuelve su scancode y el intervalo en ms que ha estado pulsada (max. 6553ms)
*/
uint8 keypad_getchartime( uint16 *ms ) {

}

/*
** Espera un m�ximo de ms (no mayor de 6553ms) la presi�n y depresi�n de una tecla del keypad y devuelve su scancode, en caso contrario devuelve KEYPAD_TIMEOUT
*/
uint8 keypad_timeout_getchar( uint16 ms ) {

}

/*
** Instala, en la tabla de vectores de interrupci�n, la funci�n isr como RTI de interrupciones por presi�n del keypad
** Borra interrupciones pendientes por presi�n del keypad
** Desenmascara globalmente las interrupciones y espec�ficamente las interrupciones por presi�n del keypad
*/
void keypad_open( void (*isr)(void) ) {

}

/*
** Enmascara las interrupciones por presi�n del keypad
** Desinstala la RTI por presi�n del keypad
*/
void keypad_close( void ) {

}

void keypad_up_isr( void ) {
    sw_delay_ms( KEYPAD_KEYUP_DELAY );
    EXTINT = (EXTINT & ~(0xf<<4)) | (2<<4);
    keypad_open( keypad_down_isr );
    I_ISPC = BIT_KEYPAD;
}

void keypad_down_isr( void ) {
	sw_delay_ms( KEYPAD_KEYDOWN_DELAY );
	EXTINT = (EXTINT & ~(0xf<<4)) | (4<<4);
	keypad_open( keypad_up_isr );
	I_ISPC = BIT_KEYPAD;
}
