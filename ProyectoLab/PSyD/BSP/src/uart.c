/*
 * uart.c
 *
 *  Created on: 05/04/2016
 *      Author: usuario_local
 */

#include <uart.h>
#include <s3c44b0x.h>
#include <s3cev40.h>

/*
** Configura la UART para una comunicaci�n seg�n los siguientes par�metros:
**   E/S programada (por pooling)
**   Control manual de flujo
**   FIFOs: activadas
**   Protocolo: normal, sin paridad, 1 bit de stop, 8 bits de datos
**   Velocidad: 115200 baudios
**   Sin tratamiento de errores
*/
void uart0_init( void ){
	ULCON0 &= ~(1<<6) & ~(1<<5) & ~(1<<4) & ~(1<<3) & ~(1<<2);
	ULCON0 |= 3;
	UBRDIV0 = 34;
	UMCON0 &= ~(1<<4);
	UCON0 |= (1<<2) | (1<<0);
	UCON0 &= ~(1<<7) & ~(1<<6) & ~(1<<5) & ~(1<<4);
	UFCON0 |= (1<<0);
	UFCON0 &= ~(1<<2) & ~(1<<1);
}

/*
** Env�a un caracter por la UART
*/
void uart0_putchar( char ch ){
	while(((UFSTAT0 >> 9) & 1) == 1);
	UTXH0 = ch;
}

/*
** Env�a una cadena de caracteres por la UART
*/
void uart0_puts( char *s ){
	int i=0;
	while(s[i] != '\0'){
		uart0_putchar(s[i]);
		i++;
	}
}

/*
** Env�a una cadena de caracteres por la UART que representa en decimal al entero que toma como argumento
*/
void uart0_putint( int32 i ){
	int num=i/10, dig=i%10, tam=0, numero[11], chorra=0;
	do{
		numero[chorra]=dig;
		num/=10;
		dig=i%10;
		tam++;
	}while(num > 0);

	if(i<0)
		uart0_putchar('-');

	for(chorra=tam-1; chorra>=0; chorra--)
		uart0_putchar(numero[chorra]+48);
}

/*
** Env�a una cadena de caracteres por la UART que representa en hexadecimal al entero que toma como argumento
*/
void uart0_puthex( uint32 i ){

}

/*
** Devuelve un caracter recibido por la UART (espera hasta que llegue)
*/
char uart0_getchar( void ){

}

/*
** Forma una cadena con los caracteres recibidos por la UART hasta la recepci�n de '\n'
*/
void uart0_gets( char *s ){

}

/*
** Forma una cadena con los caracteres recibidos por la UART hasta la recepci�n de '\n' y los interpreta en decimal
*/
int32 uart0_getint( void ){

}

/*
** Forma una cadena con los caracteres recibidos por la UART hasta la recepci�n de '\n' y los interpreta en hexadecimal
*/
uint32 uart0_gethex( void ){

}