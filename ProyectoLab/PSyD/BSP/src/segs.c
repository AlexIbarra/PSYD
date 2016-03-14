#include <s3c44b0x.h>
#include <s3cev40.h>
#include <segs.h>

static const uint8 hex2segs[16] = {0x12, 159, 49, 21, 156, 84, 80, 31, 16, 28, 24, 208, 114, 145, 112, 120};
static uint8 state;

/*
** Inicializa el display 7-segmentos y lo apaga
*/
void segs_init( void ){
	segs_off();
}

/*
** Apaga el display 7-segmentos
*/
void segs_off( void ){
	state = SEGS_OFF;
	SEGS = state;
}

/*
** Visualiza el número indicado en el display 7-segmentos
*/
void segs_putchar( uint8 n ){
	state = n & 0x0f;
	SEGS = hex2segs[state];
}

/*
** Devuelve el número que se está visualizando en el display
** 7-segmentos o SEGS_OFF si está apagado
*/
uint8 segs_status( void ){
	return state;
}
