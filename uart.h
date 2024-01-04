/* 
 * File:   uart.h
 * Author: santh
 *
 * Created on January 4, 2024, 12:50 PM
 */

#ifndef UART_H
#define	UART_H

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef	__cplusplus
}
#endif

void init_uart();
void putch(unsigned char data);
void puts(unsigned char *data);
unsigned char getch();
unsigned char getche();

#endif	/* UART_H */

