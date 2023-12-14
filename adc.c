/*
 * File:   adc.c
 * Author: santhoshkumar R
 *
 * Created on December 13, 2023, 11:49 PM
 */


#include <xc.h>
#include "car_black_box.h"

void init_adc() {
    ADON = 1;
    VCFG1 = 0;
    VCFG0 = 0;
    PCFG3 = 1;
    PCFG2 = 0;
    PCFG1 = 1;
    PCFG0 = 0;
    ACQT2 = 1;
    ACQT1 = 0;
    ACQT0 = 0;
    ADCS2 = 0;
    ADCS1 = 1;
    ADCS0 = 0;
    ADFM = 1;
}


unsigned short read_adc(unsigned char channel)
{
    ADCON0 = (ADCON0 & 0XC3) | (channel << 2);
    GO = 1;
    while(GO);
    return ADRESL | ((ADRESH & 0X03) << 8);
}
