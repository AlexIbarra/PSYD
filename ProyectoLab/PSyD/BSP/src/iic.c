/*-------------------------------------------------------------------
**
**  Fichero:
**    iic.h  5/3/2015
**
**    (c) J.M. Mendias
**    Programación de Sistemas y Dispositivos
**    Facultad de Informática. Universidad Complutense de Madrid
**
**  Propósito:
**    Contiene las definiciones de los prototipos de funciones
**    para la comunicación por el bus IIC del chip S3C44BOX
**
**  Notas de diseño:
**
**-----------------------------------------------------------------*/

#include <iic.h>
#include <s3c44b0x.h>
#include <s3cev40.h>
#include <timers.h>

/*
** Configura el controlador de IIC según los siguientes parámetros
**   Interrupciones: habilitadas
**   Transmisión/recepción: activada
**   Frecuencia de comunicación: 250 KHz
**   Generación de ACK: automática
*/
void iic_init( void ){
	IICADD = 0x0;
	IICCON = 0xAF;
	IICSTAT = 0x10;
}

/*
** Inicia un flujo de transmición/recepción en la que el microcontrolador actúa como master:
**   Genenera la START condition
**   Envía la dirección del dispositivo esclavo
**   Espera la recepción de ACK
*/
void iic_start( uint8 mode, uint8 byte ){
	IICDS = byte;
	IICSTAT = (mode << 6)| (1 << 4) | (1 << 5);
	IICCON &= ~(1<<4);
	while(!( IICCON & ( 1 << 4)));
}

/*
** Actuando el microcontrolador como master:
**   Envía el byte indicado
**   Espera la recepción de ACK
*/
void iic_putByte( uint8 byte ){
	IICDS = byte;
	IICCON &=  ~(1<<4);
	while( !(IICCON & (1 << 4)) );
}

/*
** Actuando el microcontrolador como master:
**   Espera la recepción de un byte
**   Genera o no ACK
**   Devuelve el byte recibido
*/
uint8 iic_getByte( uint8 ack ){
	/*IICCON |= (1<<7);
	IICCON &= ~(1<<4);
	while((IICCON & (1<<4)) != (1<<4));
	return IICDS;*/
	if(ack){
		IICCON = IICCON | (1 << 7);
	}
	else{
		IICCON = IICCON & ~(1 << 7);
	}

	IICCON &= ~(1 << 4);

	while(!(IICCON & (1 << 4)));
	return IICDS;
}

/*
** Envía la stop condition y espera los ms indicados para que haga efecto
*/
void iic_stop( uint16 ms ){
	IICSTAT &= ~(1<<5);
	IICCON &= ~(1<<4);
	sw_delay_ms(ms);
}

