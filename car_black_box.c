/*
 * File:   car_black_box.c
 * Author: santhoshkumar R
 *
 * Created on December 13, 2023, 10:54 PM
 */


#include <xc.h>
#include "car_black_box.h"

extern unsigned int sec;
unsigned char *gear_data[8] = {" ON ", " GN ", " G1 ", " G2 ", " G3 ", " G4 ", " GR ", " C  "};
unsigned short gear_index = 0, speed;

unsigned char pass_key, attempt = '3';
unsigned short index = 0, wait = 0, menu_flag = 0;
unsigned char *original_password = "0011", enter_password[5] = {' ', ' ', ' ', ' ', '\0'};
extern unsigned char enter_flag;

unsigned char *menu[5] = {"View Log        ", "Set Time        ", "Download Log    ", "Clear log       ", "Reset Password "};
unsigned short previous_key, key, index_1 = 0, index_2 = 1, star_flag = 1, enter_index = 0;
unsigned short wait1 = 0;

void display_dashboard(unsigned char key) {
    clcd_print("  TIME    EV SP ", LINE1(0));
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
    clcd_print(gear_data[gear_index], LINE2(9));

}

void display_speed(unsigned short speed) {
    clcd_putch((speed / 10) + 48, LINE2(13));
    clcd_putch((speed % 10) + 48, LINE2(14));
}

void read_password(unsigned char key) {
    if (menu_flag == 0) {
        if (key == 11) {
            enter_password[index] = '1';
            clcd_putch('*', LINE2(index + 3));
            sec = 0;
            index++;
        } else if (key == 12) {
            enter_password[index] = '0';
            clcd_putch('*', LINE2(index + 3));
            sec = 0;
            index++;
        }

        if (sec == 5) {
            enter_flag = 0;
            index = 0;
            attempt = '3';
            return;
        }

        if (wait++ < 800) {
            clcd_putch('_', LINE2(index + 3));
        } else if (wait > 800) {
            clcd_putch(' ', LINE2(index + 3));
            if (wait == 1600)
                wait = 0;
        }

        clcd_print(" Enter password", LINE1(0));
        if (index == 4) {
            __delay_ms(300);
            attempt--;
            index = 0;
            unsigned short ind_compare = 0, count = 0;
            while (original_password[ind_compare]) {
                if (original_password[ind_compare] != enter_password[ind_compare]) {
                    count = 1;
                    __delay_ms(500);
                    clcd_print(" Wrong Password", LINE1(0));
                    clcd_putch(attempt, LINE2(0));
                    clcd_print("-Attempt Remain", LINE2(1));
                    __delay_ms(500);
                    clcd_print("                ", LINE2(0));
                    break;
                }
                ind_compare++;
            }
            if (count == 0) {
                menu_flag = 1;
                index = 0;
                attempt = '3';
                clcd_print("Correct Password", LINE1(0));
                clcd_print("   Menu Page    ", LINE2(0));
                __delay_ms(700);
            } else if (attempt == '0') {
                clcd_print("  Attempt Over  ", LINE1(0));
                clcd_print(" Wait For    Sec", LINE2(0));
                sec = 0;
                while (sec != 60) {
                    clcd_putch((59 - sec) / 10 + 48, LINE2(10));
                    clcd_putch(((59 - sec) % 10) + 48, LINE2(11));
                }
                enter_flag = 0;
                attempt = '3';
                return;
            }
        }
    } else if (menu_flag == 1) {
        menu_flag = 0;
        enter_flag = 2;
        return;
    }
}

int validate_password(char *original_password, char *enter_password) {
    index = 0;
    unsigned short ind_compare = 0;
    while (original_password[ind_compare]) {
        if (original_password[ind_compare] - enter_password[ind_compare] != 0) {
            return original_password[ind_compare] - enter_password[ind_compare];
        }
        ind_compare++;
    }
    return original_password[ind_compare] - enter_password[ind_compare];
}

void car_menu(unsigned char key) {
    if (menu_flag == 0) {
        if (key == 11) {
            sec = 0;
            previous_key = key;
            if (wait1++ > 400) {
                menu_flag = 1;
                wait1 = 0;
            }
        } else if (wait1 != 0 && (wait1 < 400) && previous_key == 11 && key == 0xFF && index_2 > 0) {
            if (star_flag == 0) {
                star_flag = 1;
            } else if (index_2 > 1) {
                index_1--;
                index_2--;
            }
            if (enter_index > 0) {
                enter_index--;
            }
            wait1 = 0;
        }
        if (star_flag == 1) {
            clcd_putch('*', LINE1(0));
            clcd_putch(' ', LINE2(0));
        } else {
            clcd_putch(' ', LINE1(0));
            clcd_putch('*', LINE2(0));
        }
        clcd_print(menu[index_1], LINE1(1));
        clcd_print(menu[index_2], LINE2(1));
    } else {
        clcd_print("Entered MENU : ", LINE1(0));
        clcd_print(menu[enter_index], LINE2(0));
    }
    if (sec == 5) {
        index_1 = 0;
        index_2 = 1;
        star_flag = 1;
        enter_index = 0;
        wait1 = 0;
        enter_flag = 0;
        return;
    }

    if (key == 12) {
        sec = 0;
        previous_key = key;
        if (wait1++ > 400) {
            index_1 = 0;
            index_2 = 1;
            star_flag = 1;
            enter_index = 0;
            enter_flag = 0;
            wait1 = 0;
            return;
        }
    } else if (wait1 != 0 && wait1 < 400 && previous_key == 12 && key == 0xFF && index_1 < 4) {
        if (menu_flag == 0) {
            if (star_flag == 1) {
                star_flag = 0;
            } else if (index_1 < 3) {
                index_1++;
                index_2++;
            }
            if (enter_index < 4) {
                enter_index++;
            }
            wait1 = 0;
        } else {
            wait1 = 0;
        }
    }
}