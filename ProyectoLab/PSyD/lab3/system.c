#include <s3c44b0x.h>
#include <s3cev40.h>
#include <system.h>


static void port_init() {

	PDATA = ~0;
	PCONA |= 1<<1 | 1<<2 | 1<<3 | 1<<4 | 1<<5 | 1<<6 | 1<<7;
	PDATB = ~0;
	PCONB = (PCONB | 1<<0 | 1<<1 | 1<<2 | 1<<3 | 1<<6 | 1<<8) & ~(1<<9 | 1<<10);
	PDATC = ~0;
	PCONC = PCONC | 1<<27 | 1<<26 | 1<<25 | 1<<24 | 1<<23 | 1<<22 | 1<<21 | 1<<20;
	PUPC = ;
	PDATD = ~0;
	PCOND = PCOND | 1<<16 | 1<<17 | 1<<18 | 1<<19 | 1<<20 | 1<<21 | 1<<22;
	PUPD = ;
	PDATE = ~0;
	PCONE = PCONE | 1<<16 | 1<<17 | 1<<18 | 1<<19 | 1<<20 | 1<<21 | 1<<22;
	PUPE = ;
	PDATF = ~0;
	PCONF = PCONF | 1<<16 | 1<<17 | 1<<18 | 1<<19 | 1<<20 | 1<<21 | 1<<22;
	PUPF = ;
	PDATG = ~0;
	PCONG = PCONG | 1<<16 | 1<<17 | 1<<18 | 1<<19 | 1<<20 | 1<<21 | 1<<22;
	PUPG = ;
	SPUCR = ;
	EXTINT = ;
}


/*
**  Inicializa el sistema
**    Deshabilita el watchdog
**    Enmascara todas las interrupciones
**    Configura el gestor de reloj
**      Tiempo de estabilización del PLL máximo
**      MCLK = 64 MHz
**      MCLK_SLOW = 500 KHz
**      Modo de funcionamiento normal
**      Reloj distribuido a todos los controladores
**    Arbitro de bus con prioridades por defecto
**    Cache deshabilitada
**    Configura los puertos de E/S
**    Configura el controlador de interrupciones
**      Fija prioridades por defecto
**      Todas las fuentes de interrupción en modo IRQ
**      Instala dummy ISR
**      Borra interrupciones pendientes externas e internas
**      IRQ vectorizadas, linea IRQ activada, linea FIQ desactivada
**  Inicializa el UART0
**  Muestra información del sistema por la UART0
*/
void sys_init( void ){
	port_init();
}

/*
**  Pone el procesador en estado IDLE
*/
inline void sleep( void ){

}
