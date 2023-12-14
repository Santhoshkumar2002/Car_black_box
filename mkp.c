/*
 * File:   mkp.c
 * Author: santhoshkumar R
 *
 * Created on December 13, 2023, 10:55 PM
 */


#include <xc.h>
#include "car_black_box.h"

void init_matrix_keypad()
{
    TRISB = 0x1E;
    RBPU = 0;
}

unsigned char read_switches(unsigned char detection) {
    static unsigned char once = 1;
    unsigned char key;
    if (detection == 0) {
        return scan_key();
    } else if (detection == 1) {
        key = scan_key();
        if ((key != 0xFF) && once) {
            once = 0;
            return key;
        } else if (key == 0xFF)
            once = 1;
        return 0xFF;
    }
}

unsigned char scan_key() {
    RB5 = 0;
    RB6 = 1;
    RB7 = 1;

    if (RB1 == 0)
        return 1;
    else if (RB2 == 0)
        return 4;
    else if (RB3 == 0)
        return 7;
    else if (RB4 == 0)
        return 10;

    RB5 = 1;
    RB6 = 0;
    RB7 = 1;

    if (RB1 == 0)
        return 2;
    else if (RB2 == 0)
        return 5;
    else if (RB3 == 0)
        return 8;
    else if (RB4 == 0)
        return 11;

    RB5 = 1;
    RB6 = 1;
    RB7 = 0;
    RB7 = 0;

    if (RB1 == 0)
        return 3;
    else if (RB2 == 0)
        return 6;
    else if (RB3 == 0)
        return 9;
    else if (RB4 == 0)
        return 12;
    
    return 0xFF;
}

