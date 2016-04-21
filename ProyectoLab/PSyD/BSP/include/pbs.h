/*-------------------------------------------------------------------
**
**  Fichero:
**    pbs.h  26/2/2015
**
**    (c) J.M. Mendias
**    Programaci�n de Sistemas y Dispositivos
**    Facultad de Inform�tica. Universidad Complutense de Madrid
**
**  Prop�sito:
**    Contiene las definiciones de los prototipos de funciones
**    para el acceso a los push-buttons de la placa de prototipado
**    S3CEV40
**
**  Notas de dise�o:
**
**-----------------------------------------------------------------*/

#ifndef __PBS_H__
#define __PBS_H__

#include <common_types.h>

/*
** Scancodes de los pulsadores
*/
#define PB_RIGHT   (1 << 7)
#define PB_LEFT	   (1 << 6)

/*
** Errorcodes de los pulsadores
*/
#define PB_FAILURE (0xff)  /* Fallo durante el proceso de scan */
#define PB_TIMEOUT (0xfe)  /* Tiempo de espera sobrepasado */

/*
** Status de un pulsador
*/
#define PB_DOWN   (1)
#define PB_UP     (0)

/*
** Inicializa los pulsadores
** Inicializa timers
*/
void pbs_init( void );

/*
** Si hay un pulsador presionado devuelve su scancode, en otro caso devuelve PB_FAILURE
*/
uint8 pb_scan( void );

/*
** Devuelve el estado del pulsador indicado
*/
uint8 pb_status( uint8 scancode );

/*
** Espera a que se presione el pulsador indicado
*/
void pb_wait_keydown( uint8 scancode );

/*
** Espera a que se presione y despresione el pulsador indicado
*/
void pb_wait_keyup( uint8 scancode );

/*
** Espera a que se presione cualquier pulsador
*/
void pb_wait_any_keydown( void );

/*
** Espera a que se presione y depresione cualquier pulsador
*/
void pb_wait_any_keyup( void );

/*
** Espera la presi�n y depresi�n de un pulsador y devuelve su scancode
*/
uint8 pb_getchar( void );

/*
** Espera la presi�n y depresi�n de un pulsador y devuelve su scancode y el intervalo en ms que ha estado pulsado (max. 6553ms)
*/
uint8 pb_getchartime( uint16 *ms );

/*
** Espera un m�ximo de ms (no mayor de 6553ms) la presi�n y depresi�n de un pulsador y devuelve su scancode, en caso contrario devuelve PB_TIMEOUT
*/
uint8 pb_timeout_getchar( uint16 ms );

/*
** Instala, en la tabla de vectores de interrupci�n, la funci�n isr como RTI de interrupciones por presi�n de un pulsador
** Borra interrupciones pendientes por presi�n de un pulsador
** Desenmascara globalmente las interrupciones y espec�ficamente las interrupciones por presi�n de un pulsador
*/
void pbs_open( void (*isr)(void) );

/*
** Enmascara las interrupciones por presi�n de un pulsador
** Desinstala la RTI por presi�n de un pulsador
*/
void pbs_close( void );

#endif
