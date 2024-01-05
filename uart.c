/*
 * File:   uart.c
 * Author: santh
 *
 * Created on January 4, 2024, 12:49 PM
 */


#include <xc.h>
#include "uart.h"

void init_uart()
{
    TRISC7 = 1;
    TRISC6 = 0;
    
    CSRC = 0;
    TX9 = 0;
    TXEN = 1;
    SYNC = 0;
    SENDB = 0;
    BRGH = 1;
    TRMT = 0;
    TX9D = 0;
    
    SPEN = 1;
    RX9 = 0;
    SREN = 0;
    CREN = 1;
    ADDEN = 0;
    FERR = 0;
    OERR = 0;
    RX9D = 0;
    
    ABDOVF = 0;
    RCIDL = 1;
    SCKP = 0;
    BRG16 = 0;
    WUE = 0;
    ABDEN = 0;
    
    SPBRG = 64;
    
    TXIE = 1;
    TXIF = 0;
    RCIE = 1;
    RCIF = 0;
    
    
}

void putch(unsigned char data)
{
    while(!TXIF);
    TXREG = data;
    //TXIF = 0;
}

void puts(unsigned char *data)
{
    while (*data)
    {
        putch(*data++);
    }
}

unsigned char getch()
{
    while (!RCIF);
    //RCIF = 0;
    return RCREG;
}

unsigned char getche()
{
    char ch;
    //while (!RCIF);
    //RCIF = 0;
    putch(ch = RCREG);
    return ch;
}
