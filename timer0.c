/*
 * File:   timer0.c
 * Author: santh
 *
 * Created on December 15, 2023, 12:40 PM
 */


#include <xc.h>

void init_timer0()
{
    //interrupt
    GIE = 1;
    PEIE = 1;
    TMR0IE = 1;
    TMR0IF = 0;
    
    //timer0
    //TMR0 = 6; //For 8 bit timer
    
    TMR0 = 3036;
    TMR0ON = 1;
    T08BIT = 0;
    T0CS = 0;
    PSA = 1;
}

