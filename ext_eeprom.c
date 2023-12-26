/*
 * File:   ext_eeprom.c
 * Author: santh
 *
 * Created on December 25, 2023, 11:11 AM
 */


#include <xc.h>
#include "i2c.h"
#include "car_black_box.h"

void write_external_eeprom(unsigned char address, unsigned char data)
{
	i2c_start();
	i2c_write(SLAVE_WRITE_eeprom);
	i2c_write(address);
	i2c_write(data);
	i2c_stop();
	for(unsigned int i = 3000;i--;);
}

unsigned char read_external_eeprom(unsigned char address)
{
	unsigned char data;

	i2c_start();
	i2c_write(SLAVE_WRITE_eeprom);
	i2c_write(address);
	i2c_rep_start();
	i2c_write(SLAVE_READ_eeprom);
	data = i2c_read();
	i2c_stop();

	return data;
}
