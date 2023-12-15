/*
 * File:   isr.c
 * Author: santh
 *
 * Created on December 15, 2023, 12:44 PM
 */


#include <xc.h>

unsigned int count = 0, sec = 0;

void __interrupt() isr()
{
    //For 8 bit
    //TMR0 = TMR0 + 8; //here we need to add 6 but two operator having it take 2 instruction cycle so we adding 6 +2 = 8
    
    //For 16 bit
    TMR0 = TMR0 + 3038; //3036 + 2 = 3038
    if(TMR0IF)
    {
        if(count++ == 80) //For 8 bit = 2000 and 16 bit = 80
        {
            count = 0;
            sec++;
        }
        TMR0IF = 0;
    }
}

