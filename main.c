/*
 * File:   main.c
 * Author: santhoshkumar R
 *
 * Created on December 13, 2023, 10:53 PM
 */


#include <xc.h>
#include "car_black_box.h"

extern unsigned int wait;
void init_config()
{
    init_matrix_keypad();
    init_clcd();
    init_adc();
}

void main(void) {
    init_config();
    unsigned char key;
    while(1)
    {
        //if(wait++ == 1000)
        {
        key = read_switches(EDGE);
        wait = 0;
        }
        display_dashboard(key);
        
    }
    return;
}

