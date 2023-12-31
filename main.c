/*
 * File:   main.c
 * Author: santhoshkumar R
 * Title: Car Black Box Project
 *
 * Created on December 13, 2023, 10:53 PM
 */


#include <xc.h>
#include "car_black_box.h"
#include "clcd.h"
#include "i2c.h"
#include "ds1307.h"
#include "uart.h"

extern unsigned int wait1;
extern unsigned char event[17], time[8];
extern unsigned sec;
extern unsigned char original_password[5];
unsigned char enter_flag = 0;
unsigned short speed;

void init_config() {
    init_matrix_keypad();
    init_clcd();
    init_adc();
    //init_timer0();
    init_i2c();
    init_ds1307();
    //init_uart();
}

void main(void) {
    init_config();
    unsigned char key;

    for(int i = 0; i < 4; i++)
    {
        write_external_eeprom(0x46+i, original_password[i]);
    }
    for(int i = 0; i < 4; i++)
    {
        original_password[i] = read_external_eeprom(0x46+i);
    }
    original_password[4] = '\0';
    
    speed = (read_adc(4) / 10.23);
    display_speed(speed);
    get_time();
    store_event(event);
    
    while (1) {
        if(enter_flag == 2 || enter_flag == 3 || enter_flag == 4)
        {
            key = read_switches(LEVEL);
        }
        else
        {
            key = read_switches(EDGE);
        }
        if(key == 10 && enter_flag == 0)
        {
            clcd_print("                ", LINE2(0));
            enter_flag = 1;
            sec = 0;
        }
        
        switch(enter_flag)
        {
            case 0:
            {
                display_dashboard(key);
                break;
            }
            case 1:
            {
                read_password(key);
                if(enter_flag == 2)
                    sec = 0;
                break;
            }
            case 2:
            {
                car_menu(key);
                if(enter_flag == 7)
                {
                    clcd_print("                ", LINE2(0));
                    sec = 0;
                }
                else if(enter_flag == 4)
                {
                    clcd_print("  Set Time      ", LINE1(0));
                    clcd_print("                ", LINE2(0));
                    for(int i = 0; i < 8; i++)
                    {
                        time[i] = event[i];
                    }
                    __delay_ms(500);
                }
                break;
            }
            case 3:
            {
                view_log(key);
                if(enter_flag == 2)
                    sec = 0; 
                break;
            }
            case 4:
            {
                set_time(key);
                if(enter_flag == 2)
                    sec = 0;
                break;
            }
            case 5:
            {
                download_log();
                if(enter_flag == 2)
                    sec = 0;
                break;
            }
            case 6:
            {
                clear_log();
                enter_flag = 2;
                sec = 0;
                break;
            }
            case 7:
            {
                change_password(key);
                if(enter_flag == 2)
                    sec = 0;
                break;
            }
        }
        get_time();
        speed = (read_adc(4) / 10.23);
        display_speed(speed);
    }
    return;
}

