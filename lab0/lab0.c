/*-------------------------------------------------------------------
**
**  Fichero:
**    lab0.c  13/3/2014
**
**    (c) J.M. Mendias
**    Programaci�n de Sistemas y Dispositivos
**    Facultad de Inform�tica. Universidad Complutense de Madrid
**
**  Prop�sito:
**    Test del laboratorio 0
**
**  Notas de dise�o:
**    Presupone que todo el SoC ha sido previamente configurado
**    por el programa residente en ROM que se ejecuta tras reset
**
**-----------------------------------------------------------------*/

void main( void )
{
	int a, b, c;
	long long la, lb, lc;
	float fa, fb, fc;

	c = a + b;
	c = a - b;
	c = a * b;
	c = a / b;
	c = a >> 2;
	c = a >> b;

	lc = la + lb;
	lc = la - lb;
	lc = la * lb;
	lc = la / lb;
	lc = la >> 2;

	fc = fa + fb;
	fc = fa - fb;
	fc = fa * fb;
	fc = fa / fb;

	while( 1 );
}

