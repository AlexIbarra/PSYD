/*-------------------------------------------------------------------
**
**  Fichero:
**    at24c04.h  17/5/2013
**
**    (c) J.M. Mendias
**    Programación de Sistemas y Dispositivos
**    Facultad de Informática. Universidad Complutense de Madrid
**
**  Propósito:
**    Contiene las definiciones de los prototipos de funciones
**    para la lectura/escritura de la EEPROM serie AT24C04
**
**  Notas de diseño:
**
**-----------------------------------------------------------------*/

#include <at24c04.h>
#include <s3c44b0x.h>
#include <s3cev40.h>
#include <iic.h>


#define DEVICE_ADDR ((0xA<<4) | (0<<1))
#define READ (1)
#define WRITE (0)

/*
** Borra al completo el contenido de la memoria
*/
void at24c04_clear( void ){
	uint8 limpia = 0;
	uint16 indice;
	for( indice = 0; indice < AT24C04_DEPTH; ++indice){
		at24c04_bytewrite(indice, limpia); // Escribes un 0 en todos los indices del at24c04
	}
}

/*
** Realiza una escritura aleatoria en la dirección indicada
*/
void at24c04_bytewrite( uint16 addr, uint8 data ){
	uint8 page;
	page = ((addr & 0x100)>>8);
	iic_start(IIC_Tx, DEVICE_ADDR | (page<<1) | WRITE);
	iic_putByte(addr & 0xFF);
	iic_putByte(data);
	iic_stop(5);
}

/*
** Realiza una lectura aleatoria de la dirección indicada
*/
void at24c04_byteread( uint16 addr, uint8 *data ){
	uint8 page;
	page = (addr & 0x100) >> 8;
	iic_start( IIC_Tx, DEVICE_ADDR | (page << 1) | WRITE );
	iic_putByte( addr & 0xFF );
	// Hasta aqui hacemos  lo mismo que en la anterior solo le tenemos que decir la direccion en la que queremos hacer la lectura
	iic_start(IIC_Rx, DEVICE_ADDR | (page << 1) | READ);
	*data = iic_getByte(NO_RxACK);
	iic_stop(5);
}

/*
** Realiza una lectura secuencial de la memoria completa
*/
void at24c04_load( uint8 *buffer ){
	uint16 indice;
	uint8 page = 0;
	uint16 addr = 0;
	iic_start(IIC_Tx, DEVICE_ADDR | (page << 1) | WRITE);
	iic_putByte(addr & 0xFF);
	iic_start(IIC_Rx, DEVICE_ADDR | (page << 1) | READ);
	for(indice = 0; indice < AT24C04_DEPTH-1; indice++){
		buffer[indice] = iic_getByte(RxACK);
	}
	//La ultima posicion diferente
	buffer[AT24C04_DEPTH] = iic_getByte(NO_RxACK);
	iic_stop(5);
}

/*
** Realiza una escritura paginada de la memoria completa
*/
void at24c04_store( uint8 *buffer ){
	uint16 direccion = 0;
	uint16 indice;
	int p;
	uint8 pagina;
	for (p = 0; p < 32; ++p) {
		pagina = (direccion & 0x100) >> 8;
		iic_start(IIC_Tx, DEVICE_ADDR | (pagina << 1) | WRITE);
		iic_putByte(direccion & 0xFF);
		for (indice = 0; indice < 16; indice++, direccion++) {
			iic_putByte(buffer[direccion]);
		}
		iic_stop(5);
	}
}
