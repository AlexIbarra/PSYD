/*-------------------------------------------------------------------
**
**  Fichero:
**    lab2.c  24/2/2015
**
**    (c) J.M. Mendias
**    Programación de Sistemas y Dispositivos
**    Facultad de Informática. Universidad Complutense de Madrid
**
**  Propósito:
**    Test del laboratorio 2
**
**  Notas de diseño:
**    Presupone que todo el SoC ha sido previamente configurado
**    por el programa residente en ROM que se ejecuta tras reset
**
**-----------------------------------------------------------------*/

#define PCONB		(*(volatile unsigned int *)...)
#define PDATB		(*(volatile unsigned int *)...)

#define PCONG		(*(volatile unsigned int *)...)
#define PDATG		(*(volatile unsigned int *)...)
#define PUPG		(*(volatile unsigned int *)...)
	
void main(void) 
{
	PCONB = ...;	// PB[10] = out, PF[9] = out
	PCONG = ...;	// PG[7] = in, PG[6] = in
    PUPG  = ...;   // Deshabilita pull-up de PG[7] y PG[6]

	while( 1 )
	    PDATB = PDATG << ...;	// PB[10:9] = PG[7:6]
}
