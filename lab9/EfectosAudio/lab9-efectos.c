/*-------------------------------------------------------------------
**
**  Fichero:
**    lab9-efectos.c  11/5/2014
**
**    (c) J.M. Mendias
**    Programaci�n de Sistemas y Dispositivos
**    Facultad de Inform�tica. Universidad Complutense de Madrid
**
**  Prop�sito:
**    Test del laboratorio 9 (1a. parte)
**
**  Notas de dise�o:
**
**-----------------------------------------------------------------*/

#include <s3c44b0x.h>
#include <common_types.h>
#include <system.h>
#include <segs.h>
#include <pbs.h>
#include <uart.h>
#include <uda1341ts.h>
#include <iis.h>

#define AUDIOBUFFER_SIZE   (16000*10) // 16000sps * 10s = 160000 samples
#define AUDIODELAY_MAXSIZE (16000*2)  // 16000sps * 2s  = 32000 samples
#define LOOKUPTABLE_SIZE   (36)

typedef struct {
	int16 ch0[AUDIOBUFFER_SIZE];
	int16 ch1[AUDIOBUFFER_SIZE];
} audiobuffer_t;

typedef struct {
	int16 ch0[AUDIODELAY_MAXSIZE];
	int16 ch1[AUDIODELAY_MAXSIZE];
	uint16 size;
	uint16 head;
	uint16 tail;
} audiodelay_t;

audiobuffer_t buffer;
audiodelay_t delay_buffer;

int16 sine[LOOKUPTABLE_SIZE] =       // DO central = 440 Hz
{
	  0,  157,  314,  472,  579,  689,  779,  846,  887,  900,  887,  846,  779,  689,  579,  472,  314,  157,
	  0, -157, -314, -472, -579, -689, -779, -846, -887, -900, -887, -846, -779, -689, -579, -472, -314, -157
};

int16 square[LOOKUPTABLE_SIZE] =      // DO central = 440 Hz
{
	900,  900,  900,  900,  900,  900,  900,  900,  900,  900,  900,  900,  900,  900,  900,  900,  900,  900,
   -900, -900, -900, -900, -900, -900, -900, -900, -900, -900, -900, -900, -900, -900, -900, -900, -900, -900
};

int16 triangle[LOOKUPTABLE_SIZE] =    // DO central = 440 Hz
{
      0,  100,  200,  300,  400,  500,  600,  700,  800,  900,  800,  700,  600,  500,  400,  300,  200,  100,
      0, -100, -200, -300, -400, -500, -600, -700, -800, -900, -800, -700, -600, -500, -400, -300, -200, -100
};

void audiodelay_init( audiodelay_t *buffer, uint16 size );
void audiodelay_put( audiodelay_t *buffer, int16 ch0, int16 ch1 );
void audiodelay_get( audiodelay_t *buffer, int16 *ch0, int16 *ch1 );

void main( void )
{
	uint32 i;
	int16 ch0, ch1;
	int16 ch0_aux, ch1_aux;

	sys_init();
    uart0_init();
    segs_init();
    pbs_init();
    uda1341ts_init();
    iis_init( IIS_POLLING );

    /************************************/

    uart0_puts( "\n\nReproducci�n de sonido tabulado (DO central 440 Hz)\n" );
    uart0_puts( "(para finalizar presione el pulsador derecho)\n" );


    /************************************/

    uart0_puts( "  - Sinusoide: escuche... " );
    i = 0;
    while( !pb_status( PB_RIGHT ) )
    {
     	iis_putSample( sine[i], sine[i] );
     	if( ++i > LOOKUPTABLE_SIZE )
     		i = 0;
    }
    pb_wait_keyup( PB_RIGHT );
    uart0_puts( "OK\n" );

    /************************************/

    uart0_puts( "  - Onda cuadrada: escuche... " );
	i = 0;
    while( !pb_status( PB_RIGHT ) )
    {
     	iis_putSample( square[i], square[i] );
     	if( ++i > LOOKUPTABLE_SIZE )
     		i = 0;
    }
    pb_wait_keyup( PB_RIGHT );
    uart0_puts( "OK\n" );

    /************************************/

    uart0_puts( "  - Onda triangular: escuche... " );
    i = 0;
	while( !pb_status( PB_RIGHT ) )
    {
     	iis_putSample( triangle[i], triangle[i] );
     	if( ++i > LOOKUPTABLE_SIZE )
     		i = 0;
    }
    pb_wait_keyup( PB_RIGHT );
	uart0_puts( "OK\n" );

    /************************************/

    uart0_puts( "\nEfectos de audio (dominio temporal)\n" );

    /************************************/

    uart0_puts( "  - Grabacion: hable mientras los 7 segmentos est�n encendidos... " );
    segs_putchar( 8 );
    for( i=0; i<AUDIOBUFFER_SIZE; i++ )
     	iis_getSample( &(buffer.ch0[i]), &(buffer.ch1[i]) );
    segs_off();
    uart0_puts( "OK\n" );

    /************************************/

    uart0_puts( "  - Reproducci�n a velocidad normal: escuche... " );
    for( i=0; i<AUDIOBUFFER_SIZE; i++ )
     	iis_putSample( buffer.ch0[i], buffer.ch1[i] );
    uart0_puts( "OK\n" );

    uart0_puts( "  - Reproducci�n a media velocidad: escuche... " );
    for( i=0; i<AUDIOBUFFER_SIZE; i++ )
    {
     	iis_putSample( buffer.ch0[i], buffer.ch1[i] );	    // cada muestra almacenada se reproduce 2 veces
     	iis_putSample( buffer.ch0[i], buffer.ch1[i] );
     }
    uart0_puts( "OK\n" );

    /************************************/

    uart0_puts( "  - Reproducci�n a doble velocidad: escuche... " );
    for( i=0; i<AUDIOBUFFER_SIZE; i+=2 )		 	// se reproduce 1 de cada 2 muestras almacenadas
     	iis_putSample( buffer.ch0[i], buffer.ch1[i] );
    uart0_puts( "OK\n" );

    /************************************/

    uart0_puts( "  - Reproducci�n inversa: escuche... " );
    for( i=0; i<AUDIOBUFFER_SIZE; i++ )
     	iis_putSample( buffer.ch0[AUDIOBUFFER_SIZE-i], buffer.ch1[AUDIOBUFFER_SIZE-i] );
    uart0_puts( "OK\n" );

    /************************************/

    uart0_puts( "(para finalizar presione el pulsador derecho)\n" );
    uart0_puts( "  - Loopback: hable y escuche... " );
    while( !pb_status( PB_RIGHT ) )
    {
     	iis_getSample( &ch0, &ch1 );
     	iis_putSample( ch0, ch1 );
     }
    pb_wait_keyup( PB_RIGHT );
    uart0_puts( "OK\n" );

    /************************************/

    uart0_puts( "  - Echo (8 ms): hable y escuche... " );
    audiodelay_init( &delay_buffer, 128 );
    while( !pb_status( PB_RIGHT ) )
    {
     	iis_getSample( &ch0, &ch1 );
    	audiodelay_get( &delay_buffer, &ch0_aux, &ch1_aux );
     	ch0_aux = ch0 + (ch0_aux >> 1);
     	ch1_aux = ch1 + (ch1_aux >> 1);
     	iis_putSample( ch0_aux, ch1_aux );
    	audiodelay_put( &delay_buffer, ch0, ch1 );
    }
    pb_wait_keyup( PB_RIGHT );
    uart0_puts( "OK\n" );

    /************************************/

    uart0_puts( "  - Echo (45 ms): hable y escuche... " );
    audiodelay_init( &delay_buffer, 720 );
    while( !pb_status( PB_RIGHT ) )
    {
     	iis_getSample( &ch0, &ch1 );
    	audiodelay_get( &delay_buffer, &ch0_aux, &ch1_aux );
     	ch0_aux = ch0 + (ch0_aux >> 1);
     	ch1_aux = ch1 + (ch1_aux >> 1);
     	iis_putSample( ch0_aux, ch1_aux );
    	audiodelay_put( &delay_buffer, ch0, ch1 );
    }
    pb_wait_keyup( PB_RIGHT );
    uart0_puts( "OK\n" );

    /************************************/

    uart0_puts( "  - Echo (85 ms): hable y escuche... " );
    audiodelay_init( &delay_buffer, 1280 );
    while( !pb_status( PB_RIGHT ) )
    {
     	iis_getSample( &ch0, &ch1 );
    	audiodelay_get( &delay_buffer, &ch0_aux, &ch1_aux );
     	ch0_aux = ch0 + (ch0_aux >> 1);
     	ch1_aux = ch1 + (ch1_aux >> 1);
     	iis_putSample( ch0_aux, ch1_aux );
    	audiodelay_put( &delay_buffer, ch0, ch1 );
    }
    pb_wait_keyup( PB_RIGHT );
    uart0_puts( "OK\n" );

    /************************************/

    uart0_puts( "  - Echo (150 ms): hable y escuche... " );
    audiodelay_init( &delay_buffer, 2400 );
    while( !pb_status( PB_RIGHT ) )
    {
     	iis_getSample( &ch0, &ch1 );
    	audiodelay_get( &delay_buffer, &ch0_aux, &ch1_aux );
     	ch0_aux = ch0 + (ch0_aux >> 1);
     	ch1_aux = ch1 + (ch1_aux >> 1);
     	iis_putSample( ch0_aux, ch1_aux );
    	audiodelay_put( &delay_buffer, ch0, ch1 );
    }
    pb_wait_keyup( PB_RIGHT );
    uart0_puts( "OK\n" );

    /************************************/

    uart0_puts( "  - Echo & fade (1 s): hable y escuche... " );
    audiodelay_init( &delay_buffer, 16000 );
    while( !pb_status( PB_RIGHT ) )
    {
     	iis_getSample( &ch0, &ch1 );
    	audiodelay_get( &delay_buffer, &ch0_aux, &ch1_aux );
      	ch0 = ch0 + (ch0_aux >> 1);
      	ch1 = ch1 + (ch1_aux >> 1);
     	iis_putSample( ch0, ch1 );
    	audiodelay_put( &delay_buffer, ch0, ch1 );
    }
    pb_wait_keyup( PB_RIGHT );
    uart0_puts( "OK\n" );

    /************************************/

    uart0_puts( "  - Reverberaci�n (echo & fade 100 ms): hable y escuche... " );
    audiodelay_init( &delay_buffer, 1600 );
    while( !pb_status( PB_RIGHT ) )
    {
     	iis_getSample( &ch0, &ch1 );
    	audiodelay_get( &delay_buffer, &ch0_aux, &ch1_aux );
      	ch0 = ch0 + (ch0_aux >> 1);
      	ch1 = ch1 + (ch1_aux >> 1);
     	iis_putSample( ch0, ch1 );
    	audiodelay_put( &delay_buffer, ch0, ch1 );
     }
    pb_wait_keyup( PB_RIGHT );
    uart0_puts( "OK\n" );

    /************************************/

    while(1);

}

void audiodelay_init( audiodelay_t *buffer, uint16 size )
{
	uint16 i;

	for( i=0; i<size; i++ )
	{
		buffer->ch0[i] = 0;
		buffer->ch1[i] = 0;
	}
	buffer->head = 0;
	buffer->tail = 0;
	buffer->size = size;
}

void audiodelay_put( audiodelay_t *buffer, int16 ch0, int16 ch1 )
{
	buffer->ch0[buffer->head] = ch0;
	buffer->ch1[buffer->head] = ch1;
	if( ++(buffer->head) == buffer->size )
		buffer->head = 0;
}

void audiodelay_get( audiodelay_t *buffer, int16 *ch0, int16 *ch1 )
{
	*ch0 = buffer->ch0[buffer->tail];
	*ch1 = buffer->ch1[buffer->tail];
	if( ++(buffer->tail) == buffer->size )
		buffer->tail = 0;
}

