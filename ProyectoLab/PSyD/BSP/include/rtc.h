/*-------------------------------------------------------------------
**
**  Fichero:
**    rtc.h  29/4/2015
**
**    (c) J.M. Mendias
**    Programaci�n de Sistemas y Dispositivos
**    Facultad de Inform�tica. Universidad Complutense de Madrid
**
**  Prop�sito:
**    Contiene las definiciones de los prototipos de las funciones
**    para la gesti�n b�sica del Real Time Clock del chip S3C44BOX
**
**  Notas de dise�o:
**
**-----------------------------------------------------------------*/

#ifndef __RTC_H__
#define __RTC_H__

#include <common_types.h>
#include <s3c44b0x.h>
#include <s3cev40.h>

typedef struct
{
    uint8 sec;   /* segundo (0-59)*/
    uint8 min;   /* minuto (0-59) */
    uint8 hour;  /* hora (0-23) */
    uint8 mday;  /* d�a del mes (1-31) */
    uint8 wday;  /* d�a de la semana (1-7) comenzado por el domingo */
    uint8 mon;   /* mes (1-12) */
    uint8 year;  /* a�o (0-99)*/
} rtc_time_t;

/*
** Configura el RTC desactivando alarma, round reset y generaci�n de ticks
** Inicializa el RTC a las 00:00:00 horas del martes 1 de enero de 2013
** Inicializa a 0 los registros de alarma
*/
void rtc_init( void );

/*
** Actualiza la fecha y hora mantenida por el RTC
*/
void rtc_puttime( rtc_time_t *rtc_time );

/*
** Recupera en la fecha y hora mantenida por el RTC
*/
void rtc_gettime( rtc_time_t *rtc_time );

/*
** Instala, en la tabla de vectores de interrupci�n, la funci�n isr como RTI de interrupciones por ticks del RTC
** Borra interrupciones pendientes por ticks del RTC
** Desenmascara globalmente las interrupciones y espec�ficamente las interrupciones por ticks del RTC
** Habilita la generaci�n de ticks y fija el valor inicial del contador que los genera: perido = (tick_count+1)/128 segundos
*/
void rtc_open( void (*isr)(void), uint8 tick_count );

/*
** Deshabilita la generaci�n de ticks
** Enmascara las interrupciones por ticks del RTC
** Desinstala la RTI por ticks del RTC
*/
void rtc_close( void );

#endif //__RTC_H__
