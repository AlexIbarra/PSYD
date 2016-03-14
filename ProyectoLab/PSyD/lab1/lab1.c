/*-------------------------------------------------------------------
**
**  Fichero:
**    lab1.c  24/2/2015
**
**    (c) J.M. Mendias
**    Programación de Sistemas y Dispositivos
**    Facultad de Informática. Universidad Complutense de Madrid
**
**  Propósito:
**    Test del laboratorio 1
**
**  Notas de diseño:
**    Presupone que todo el SoC ha sido previamente configurado
**    por el programa residente en ROM que se ejecuta tras reset
**
**-----------------------------------------------------------------*/

#define	SEGS	(*(volatile unsigned char *)(0x02140000))

const unsigned char hex2segs[16] = {0x12, 159, 49, 21, 156, 84, 80, 31, 16, 28, 24, 208, 114, 145, 112, 120};

void main(void) {

	unsigned char i;
	unsigned int j;

	SEGS = 0xff;
	while( 1 )
	    for( i=0; i<16; i++){
	        for( j=0; j<300000; j++ );
      	  	SEGS = hex2segs[i];
		}
}
