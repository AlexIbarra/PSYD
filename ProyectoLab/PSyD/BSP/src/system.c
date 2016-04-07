#include <s3c44b0x.h>
#include <s3cev40.h>
#include <system.h>


static void port_init() {

	/* Esta en la pagina 56 del Tema 2 */

	PDATA = ~0;
	PCONA = 0xFE; //(00.1111.1110)
	PDATB = ~0;
	PCONB = 0x14F; // (001.0100.1111)
	PDATC = ~0;
	PCONC = 0x5FF555FF; // (0101.1111.1111.0101.0101.0101.1111.1111) 
	PUPC = 0x94FB; // (1001.0100.1111.1011) 
	PDATD = ~0;
	PCOND = 0xAAAA; // (1010.1010.1010.1010)
	PUPD = 0xFF; // (1111.1111)
	PDATE = ~0;
	PCONE = 0x25569; // (10.0101.0101.0110.1001)
	PUPE = 0x1FB; // (1.1111.1011) 
	PDATF = ~0;
	PCONF = 0x251A; // (00.0000.0010.0101.0001.1010)
	PUPF = 0x74; // (0.0111.0100)  
	PDATG = ~0;
	PCONG = 0xF5FF; // (1111.0101.1111.1111)
	PUPG = 0x30; // (0011.0000)
	SPUCR = 0x7; // (111)
	EXTINT = 0x22000220; // (X010.X010.X000.X000.X000.X010.X010.X000)
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

	/* Mirar pagina 7 y 8 del tema 2 */
	WTCON = 0;
	INTMSK = ~0;
	LOCKTIME = 0xFFF; //Estabilización del PLL: 512 us --> (1 / 8MHz)×4095
	PLLCON = 0x38021; //Frecuencia del MCLK_SLOW: 500 KHz
	CLKSLOW = 0x8; //Frecuencia del MCLK: 64 MHz 
	CLCKCON = 0x7FF8; //Modo de funcionamiento normal y Reloj distribuido a todos lo controladores
	 
	

	/* Mirar pagina 63 del tema 2 */	
	SBUSCON = 0x8000001B; //Prioridades de bus del sistema fijas: LCD > ZDMA > BDMA > IRQ (por defecto)
	SYSCFG = 0x0; //Cache deshabilitada
	port_init();
}

/*
**  Pone el procesador en estado IDLE
*/
inline void sleep( void ){

}
