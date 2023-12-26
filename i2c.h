/* 
 * File:   i2c.h
 * Author: santh
 *
 * Created on December 25, 2023, 11:08 AM
 */

#ifndef I2C_H
#define	I2C_H

void init_i2c(void);
void i2c_start(void);
void i2c_rep_start(void);
void i2c_stop(void);
void i2c_write(unsigned char data);
unsigned char i2c_read(void);

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* I2C_H */

