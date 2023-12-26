/*
 * File:   main.c
 * Author: santhoshkumar R
 * Title: Car Black Box Project
 *
 * Created on December 13, 2023, 10:53 PM
 */


#include <xc.h>
#include "car_black_box.h"
#include "i2c.h"
#include "ds1307.h"

extern unsigned char event[17];
extern unsigned sec;
unsigned char enter_flag = 0;

void init_config() {
    init_matrix_keypad();
    init_clcd();
    init_adc();
    init_timer0();
    init_i2c();
    init_ds1307();
}

void main(void) {
    init_config();
    unsigned char key;
    while (1) {
        if(enter_flag == 2 || enter_flag == 3)
        {
            key = read_switches(LEVEL);
        }
        else
        {
            key = read_switches(EDGE);
        }
        
        if(key == 10 && enter_flag != 2)
        {
            clcd_print("               ", LINE2(0));
            enter_flag = 1;
            sec = 0;
        }
        if(enter_flag == 1)
        {
            read_password(key);
            if(enter_flag == 2)
                sec = 0;
        }
        else if(enter_flag == 2)
        {
            car_menu(key);
            if(enter_flag == 7)
                sec = 0;
        }
        else if(enter_flag == 3)
        {
            view_log(key);
            if(enter_flag == 2)
                sec = 0;
        }
        else if(enter_flag == 7)
        {
            change_password(key);
        }
        else
        {
            display_dashboard(key);
        }
    }
    return;
}

