/*-------------------------------------------------------------------
**
**  Fichero:
**    lab1.c  24/2/2015
**
**    (c) J.M. Mendias
**    Programaci�n de Sistemas y Dispositivos
**    Facultad de Inform�tica. Universidad Complutense de Madrid
**
**  Prop�sito:
**    Test del laboratorio 1
**
**  Notas de dise�o:
**    Presupone que todo el SoC ha sido previamente configurado
**    por el programa residente en ROM que se ejecuta tras reset
**
**-----------------------------------------------------------------*/

#define	SEGS	(*(volatile unsigned char *)(...))

const unsigned char hex2segs[16] = {0x12, ... };

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
