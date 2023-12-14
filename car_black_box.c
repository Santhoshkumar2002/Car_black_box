/*
 * File:   car_black_box.c
 * Author: santhoshkumar R
 *
 * Created on December 13, 2023, 10:54 PM
 */


#include <xc.h>
#include "car_black_box.h"

unsigned char *gear_data[8] = {"ON", "GN", "G1", "G2", "G3", "G4", "GR", "C "};
unsigned short gear_index = 0, speed;
unsigned int wait = 0;

void display_dashboard(unsigned char key) {
    clcd_print("  TIME     E  SP", LINE1(0));
    display_time();
    gear_monitor(key);
    speed = (read_adc(4) / 10.23);
    display_speed(speed);
}

void display_time() {
    clcd_print("  10:00:00 ", LINE2(0));
}

void gear_monitor(unsigned char key) {

    if (key == 1) {
        gear_index = 7;
    }
    else if (key == 2 || key == 3) {
        if (gear_index == 7 && key == 2) {
            gear_index = 0;
        }
        if (key == 2 && gear_index < 6) {
            gear_index++;
        } else if (key == 3 && gear_index > 1 && gear_index < 7) {
            gear_index--;
        }
    }
    clcd_print(gear_data[gear_index], LINE2(11));

}

void display_speed(unsigned short speed) {
    clcd_putch((speed / 10) + 48, LINE2(14));
    clcd_putch((speed % 10) + 48, LINE2(15));
}