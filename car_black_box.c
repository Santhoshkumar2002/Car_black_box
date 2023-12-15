/*
 * File:   car_black_box.c
 * Author: santhoshkumar R
 *
 * Created on December 13, 2023, 10:54 PM
 */


#include <xc.h>
#include "car_black_box.h"

extern unsigned int sec;
unsigned char *gear_data[8] = {"ON", "GN", "G1", "G2", "G3", "G4", "GR", "C "};
unsigned short gear_index = 0, speed;

void display_dashboard(unsigned char key) {
    clcd_print("  TIME    E  SP ", LINE1(0));
    clcd_print("                ", LINE2(0));
    display_time();
    gear_monitor(key);
    speed = (read_adc(4) / 10.23);
    display_speed(speed);
}

void display_time() {
    clcd_print("10:00:00 ", LINE2(0));
}

void gear_monitor(unsigned char key) {

    if (key == 1) {
        gear_index = 7;
    } else if (key == 2 || key == 3) {
        if (gear_index == 7 && key == 2) {
            gear_index = 0;
        }
        if (key == 2 && gear_index < 6) {
            gear_index++;
        } else if (key == 3 && gear_index > 1 && gear_index < 7) {
            gear_index--;
        }
    }
    clcd_print(gear_data[gear_index], LINE2(10));

}

void display_speed(unsigned short speed) {
    clcd_putch((speed / 10) + 48, LINE2(13));
    clcd_putch((speed % 10) + 48, LINE2(14));
}

void read_password() {
    sec = 0;
    unsigned char pass_key, attempt = '3';
    unsigned short index = 0, wait = 0, ind, flag = 0;
    unsigned char *original_password = "0011", enter_password[5];
    enter_password[4] = '\0';

    clcd_print("                ", LINE2(0));
    while (1) {
        if (flag == 0) {
            pass_key = read_switches(EDGE);
            if (pass_key == 11) {
                enter_password[index] = '1';
                sec = 0;
                index++;
            } else if (pass_key == 12) {
                enter_password[index] = '0';
                sec = 0;
                index++;
            }

            if(sec == 5)
            {
                return;
            }
            
            for (ind = 0; ind < index; ind++) {
                clcd_putch('*', LINE2(ind + 3));
            }

            if (wait++ < 800) {
                clcd_putch('_', LINE2(ind + 3));
            } else if (wait > 800) {
                clcd_putch(' ', LINE2(ind + 3));
                if (wait == 1600)
                    wait = 0;
            }

            clcd_print(" Enter password", LINE1(0));
            if (index == 4) {
                __delay_ms(600);
                attempt--;
                unsigned short ind_compare = 0, count = 0;
                while (original_password[ind_compare]) {
                    if (original_password[ind_compare] != enter_password[ind_compare]) {
                        index = 0;
                        count = 1;
                        __delay_ms(500);
                        clcd_print(" Wrong Password", LINE1(0));
                        clcd_print("                ", LINE2(0));
                        clcd_putch(attempt, LINE2(0));
                        clcd_print("-Attempt Remain", LINE2(1));
                        __delay_ms(500);
                        clcd_print("                ", LINE2(0));
                        break;
                    }
                    ind_compare++;
                }
                if (count == 0)
                {
                    flag = 1;
                    __delay_ms(500);
                    clcd_print("Correct Password", LINE1(0));
                    clcd_print("   Menu Page    ", LINE2(0));
                }
                else if(attempt == '0') {
                    clcd_print("  Attempt Over  ", LINE1(0));
                    clcd_print("                ", LINE2(0));
                    sec = 0;
                    while(sec != 60)
                    {
                    clcd_print("   Wait For     ", LINE1(0));
                    clcd_putch((59-sec)/10+48, LINE2(0));
                    clcd_putch(((59-sec)%10)+48, LINE2(1));
                    clcd_print("Seconds remain",LINE2(2));
                    }
                    return;
                }
                
            }
        } else if (flag == 1) {
            __delay_ms(1000);
            clcd_print("    View Log    ", LINE1(0));
            clcd_print("    Clear Log   ", LINE2(0));
        }

    }

}