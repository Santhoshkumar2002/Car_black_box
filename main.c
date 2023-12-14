/*
 * File:   main.c
 * Author: santhoshkumar R
 * Title: Car Black Box Project
 *
 * Created on December 13, 2023, 10:53 PM
 */


#include <xc.h>
#include "car_black_box.h"

void init_config() {
    init_matrix_keypad();
    init_clcd();
    init_adc();
}

void main(void) {
    init_config();
    unsigned char key;
    while (1) {
        key = read_switches(EDGE);
        if(key == 10)
        {
            display_password();
        }
        display_dashboard(key);
    }
    return;
}

