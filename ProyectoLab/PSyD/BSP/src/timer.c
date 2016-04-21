#include<timer.h>
#include <s3c44b0x.h>
#include <s3cev40.h>


extern uint32 mclk;
extern void isr_TIMER0_dummy( void );
static uint32 loop_ms = 0; // almacena el número de iteraciones para retardar 1 ms
static uint32 loop_s = 0; // almacena el número de iteraciones para retardar 1 s


static void sw_delay_init( void ) {
    uint32 i;
    /* Mide la duracion de un millon de iteraciones */
    timer3_start(); // la resolución del temporizador es 100us
    for( i=1000000; i; i-- );
    loop_s = ((uint64)1000000*10000)/timer3_stop(); // Regla de tres: si 10^6 iteraciones tardan n*100us, en 1s se haran 10^6 / (n× 0,0001) iteraciones
    loop_ms = loop_s / 1000; // en 1 ms se harán 1000 veces menos iteraciones que en 1 s
}

/*
** Pone a 0 los registros de configuraci�n
** Pone a 0 todos los b�fferes y registros de cuenta y comparaci�n
** Para todos los temporizadores
** Inicializa las variables para retardos software
*/
void timers_init( void ) {
    TCFG0 = 0x0;
    TCFG1 = 0x0;
    TCNTB0 = 0x0;
    TCNTB1 = 0x0;
    TCNTB2 = 0x0;
    TCNTB3 = 0x0;
    TCNTB4 = 0x0;
    TCNTB5 = 0x0;
    TCMPB0 = 0x0;
    TCMPB1 = 0x0;
    TCMPB2 = 0x0;
    TCMPB3 = 0x0;
    TCMPB4 = 0x0;
    TCMPB5 = 0x0;
    TCON = 0x0;
    sw_delay_init();
}

/*
** Realiza una espera de n milisegundos usando el timer3
*/
void timer3_delay_ms( uint16 n ) {
    TCFG0 = (TCFG0 & ~(0xff << 8)) | (31 << 8);	// (31 +1) *2 / 64
	TCFG1 = (TCFG1 & ~(0xf << 12)) | (0 << 12);	// (N,D,C) = (31, 2, 1000)
    for( ; n; n-- ) {
        TCNTB3 = 1000; // T3 count: C = 31250
        TCON = (TCON & ~(0xf << 16)) | (1 << 17); // one shot, carga TCNT3, stop T3
        TCON = (TCON & ~(0xf << 16)) | (1 << 16); // one shot, carga TCNT3, start T3
        while( !TCNTO3 ); // espera a que TCNTO3 se actualice
        while( TCNTO3 ); // espera a que TCNTO3 sea 0
    }
}

/*
** Realiza una espera de n milisegundos sin usar temporizadores
*/
void sw_delay_ms( uint16 n ) {
    uint32 i;
    for( i=loop_ms*n; i; i-- );
}

/*
** Realiza una espera de n segundos usando el timer3
*/
void timer3_delay_s( uint16 n ) {
    TCFG0 = (TCFG0 & ~(0xff << 8)) | (63 << 8); // T2‐T3 prescaler: N=63
    TCFG1 = (TCFG1 & ~(0xf << 12)) | (4 << 12); // T3 divisor (1/32): D=32
    for( ; n; n-- ) {
        TCNTB3 = 31250; // T3 count: C = 31250
        TCON = (TCON & ~(0xf << 16)) | (1 << 17); // one shot, carga TCNT3, stop T3
        TCON = (TCON & ~(0xf << 16)) | (1 << 16); // one shot, carga TCNT3, start T3
        while( !TCNTO3 ); // espera a que TCNTO3 se actualice
        while( TCNTO3 ); // espera a que TCNTO3 sea 0
    }
}

/*
** Realiza una espera de n segundos sin usar temporizadores
*/
void sw_delay_s( uint16 n ) {
    uint32 i;
    for( i=loop_s*n; i; i-- );
}

/*
** Arranca el timer3 a una frecuencia de 0,01 MHz
** Permitir� medir tiempos con una resoluci�n de 0,1 ms (100 us) hasta un m�ximo de 6.55s
*/
void timer3_start( void ) {
    TCFG0 = (TCFG0 & ~(0xff << 8)) | (199 << 8); // T2‐T3 prescaler: N=199
    TCFG1 = (TCFG1 & ~(0xf << 12)) | (4 << 12); // T3 divisor (1/32): D=32
    TCNTB3 = 0xffff; // T3 count: C = máximo
    TCON = (TCON & ~(0xf << 16)) | (1 << 17); // one shot, carga TCNT3, stop T3
    TCON = (TCON & ~(0xf << 16)) | (1 << 16); // one shot, no carga TCNT3, start T3
    while( !TCNTO3 ); // espera a que TCNTO3 se actualice
}

/*
** Detiene el timer3, devolviendo el n�mero de d�cimas de milisegundo transcurridas desde que arranc� hasta un m�ximo de 6.55s
*/
uint16 timer3_stop( void ) {
    TCON &= ~(1 << 16); // detiene el timer
    return 0xffff - TCNTO3; // calcula los ciclos de cuenta transcurridos
}

/*
** Arranca el timer3 a una frecuencia de 0,01 MHz
** Permitir� contar n d�cimas de milisegundo (0,1 ms = 100 us) hasta un m�ximo de 6.55s
*/
void timer3_start_timeout( uint16 n ) {
    TCFG0 = (TCFG0 & ~(0xff << 8)) | (199 << 8); // T2‐T3 prescaler: N=199
    TCFG1 = (TCFG1 & ~(0xf << 12)) | (4 << 12); // T3 divisor (1/32): D=32
    TCNTB3 = n; // T3 count: C = timeout
    TCON = (TCON & ~(0xf << 16)) | (1 << 17); // one shot, carga TCNT3, stop T3
    TCON = (TCON & ~(0xf << 16)) | (1 << 16); // one shot, no carga TCNT3, start T3
    while( !TCNTO3 ); // espera a que TCNTO3 se actualice
}

/*
** Indica si el timer3 ha finalizado su cuenta
*/
uint16 timer3_timeout( void ) {
    return !TCNTO3;
}

/*
** Instala, en la tabla de vectores de interrupci�n, la funci�n isr como RTI de interrupciones del timer0
** Borra interrupciones pendientes del timer0
** Desenmascara globalmente las interrupciones y espec�ficamente las interrupciones del timer0
** Configura el timer0 para que genere tps interrupciones por segundo
*/
void timer0_open_tick( void (*isr)(void), uint16 tps ) {
    pISR_TIMER0 = (uint32)isr; // instala la RTI argumento en la tabla virtual de vectores de IRQ
    I_ISPC = BIT_TIMER0; // borra flag de interrupción pendiente por interrupciones del timer 0
    INTMSK &= ~( BIT_GLOBAL | BIT_TIMER0 ); // desenmascara globalmente interrupciones e interrupciones del timer 0
    if( tps > 0 && tps <= 10 ) {
        /* programa el T0 con resolución de 25 us (40 KHz) */
        TCFG0 = (TCFG0 & ~(0xff << 0)) | (199 << 0);;
        TCFG1 = (TCFG1 & ~(0xf << 0)) | (2 << 0);
        TCNTB0 = (40000U / tps); // permite obtener el num. de ticks/s indicado
    } else if( tps > 10 && tps <= 100 ) {
        /* programa el T0 con resolución de 2,5 us (400 KHz) */
        TCFG0 = (TCFG0 & ~(0xff << 0))  | (4 << 0);
        TCFG1 = (TCFG1 & ~(0xf << 0)) | (4 << 0);
        TCNTB0 = (400000U / tps); // permite obtener el num. de ticks/s indicado
    } else if( tps > 100 && tps <= 1000 ) {
        /* programa el T0 con resolución de 25 us (40 KHz) */
        TCFG0 = (TCFG0 & ~(0xff << 0)) | (1 << 0);
        TCFG1 = (TCFG1 & ~(0xf << 0)) | (2 << 0);
        TCNTB0 = (4000000U / tps); // permite obtener el num. de ticks/s indicado
    } else if ( tps > 1000 ) {
        /* programa el T0 con resolución de 25 us (40 KHz) */
        TCFG0 = (TCFG0 & ~(0xff << 0)) | (0 << 0);
        TCFG1 = (TCFG1 & ~(0xf << 0)) | (0 << 0);
        TCNTB0 = (32000000U / tps); // permite obtener el num. de ticks/s indicado
    }
    TCON = (TCON & ~(0x7 << 0)) | (1<<3) | (1 << 1); // interval, carga TCNT0, stop T0
    TCON = (TCON & ~(0x7 << 0)) | (1<<3) | (1 << 0); // interval, no carga TCNT0, start T0
}

/*
** Instala, en la tabla de vectores de interrupci�n, la funci�n isr como RTI de interrupciones del timer0
** Borra interrupciones pendientes del timer0
** Desenmascara globalmente las interrupciones y espec�ficamente las interrupciones del timer0
** Configura el timer0 para que genere interrupciones en el modo y con la periodicidad indicadas
** Configura el timer0 para que genere interrupciones en el modo y con la periodicidad indicadas
*/
void timer0_open_ms( void (*isr)(void), uint16 ms, uint8 mode ) {
    pISR_TIMER0 = (uint32)isr; // instala la RTI argumento en la tabla virtual de vectores de IRQ
    I_ISPC = BIT_TIMER0; // borra flag de interrupción pendiente por interrupciones del timer 0
    INTMSK &= ~( BIT_GLOBAL | BIT_TIMER0 ); // desenmascara globalmente interrupciones e interrupciones del timer 0
    /* programa el T0 con resolución de 100us */
    TCFG0 = (TCFG0 & ~(0xff << 8)) | (199 << 8);
    TCFG1 = (TCFG1 & ~(0xf << 12)) | (4 << 12);
    TCNTB0 = 10*ms; // 1 ms = 10 intervalos de 100 us
    TCON = (TCON & ~(0xf << 0)) | (mode << 1); // mode, carga TCNT0, stop T0
    TCON = (TCON & ~(0xf << 0)) & ~(mode << 1); // mode, no carga TCNT0, start T0
}

/*
** Para y pone a 0 todos sus bufferes y registros del timer0
** Deshabilita las interrupciones del timer0
** Desinstala la RTI del timer0
*/
void timer0_close( void ) {
    TCNTB0 = 0x0; // pone a cero el count buffer del timer 0
    TCMPB0 = 0x0; // pone a cero el compare buffer del timer 0
    TCON = (TCON & ~(0xf << 0)) | (1 << 1); // carga TCNT0, stop T0
    TCON = (TCON & ~(0xf << 0)) | (1 << 0); // no carga TCNT0, start T0
    INTMSK |= BIT_TIMER0; // enmascara interrupciones por fin de timer 0
    pISR_TIMER0 = (uint32)isr_TIMER0_dummy;
}
