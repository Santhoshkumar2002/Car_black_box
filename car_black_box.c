/*
 * File:   car_black_box.c
 * Author: santhoshkumar R
 *
 * Created on December 13, 2023, 10:54 PM
 */


#include <xc.h>
#include "car_black_box.h"
#include "clcd.h"
#include "i2c.h"
#include "ds1307.h"
#include "uart.h"

unsigned char event[17] = "          ON     ";

unsigned char clock_reg[3];
unsigned int index_eeprom = 0;

extern unsigned int sec;
unsigned char *gear_data[8] = {"ON", "GN", "G1", "G2", "G3", "G4", "GR", "C "};
unsigned short gear_index = 0;

unsigned char pass_key, attempt = '3';
unsigned short index = 0;
unsigned short wait = 0;
unsigned char original_password[5] = "1100", enter_password[5];
extern unsigned char enter_flag;

unsigned char *menu[5] = {"View Log        ", "Set Time        ", "Download Log    ", "Clear log       ", "Reset Password "};
unsigned short previous_key, key, index_1 = 0, index_2 = 1, star_flag = 1, menu_index = 0;
unsigned int wait1 = 0;

unsigned int count_event = 0, count1_event = 0, view_flag = 0, address;
unsigned char view_event[17], total_index = 0;

void display_dashboard(unsigned char key) {
    clcd_print("  TIME    EV  SP", LINE1(0));
    if (key == 1 || key == 2 || key == 3) {
        gear_monitor(key);
    }
    clcd_print(event, LINE2(0));
}

void get_time(void) {
    clock_reg[0] = read_ds1307(HOUR_ADDR);
    clock_reg[1] = read_ds1307(MIN_ADDR);
    clock_reg[2] = read_ds1307(SEC_ADDR);

    if (clock_reg[0] & 0x40) {
        event[0] = '0' + ((clock_reg[0] >> 4) & 0x01);
        event[1] = '0' + (clock_reg[0] & 0x0F);
    } else {
        event[0] = '0' + ((clock_reg[0] >> 4) & 0x03);
        event[1] = '0' + (clock_reg[0] & 0x0F);
    }
    event[2] = ':';
    event[3] = '0' + ((clock_reg[1] >> 4) & 0x0F);
    event[4] = '0' + (clock_reg[1] & 0x0F);
    event[5] = ':';
    event[6] = '0' + ((clock_reg[2] >> 4) & 0x0F);
    event[7] = '0' + (clock_reg[2] & 0x0F);
}

unsigned char gear_flag = 0;

void gear_monitor(unsigned char key) {
    if (key == 1 && previous_key != 1) {
        gear_flag = 1;
        gear_index = 7;
    } else if (key == 2 || key == 3) {
        if (gear_index == 7 && key == 2 && previous_key != 2) {
            gear_flag = 1;
            gear_index = 0;
        }
        if (key == 2 && gear_index < 6) {
            gear_index++;
            gear_flag = 1;
        } else if (key == 3 && gear_index > 1 && gear_index < 7) {
            gear_index--;
            gear_flag = 1;
        }
    }
    if (gear_flag == 1) {
        gear_flag = 0;
        event[10] = gear_data[gear_index][0];
        event[11] = gear_data[gear_index][1];
        store_event(event);
    }
    previous_key = key;
}

void display_speed(unsigned short speed) {
    event[14] = (speed / 10) + 48;
    event[15] = (speed % 10) + 48;
    event[16] = '\0';
}

void store_event(char *event) {
    int address;
    address = ADR_EEPROM + (16 * index_eeprom);
    for (int ind = 0; ind < 16; ind++) {
        write_external_eeprom((address + ind), event[ind]);
    }
    index_eeprom++;

    if (total_index < 10)
        total_index++;
    else if (count1_event < 9)
        count1_event++;
    else
        count1_event = 0;

    if (index_eeprom == 10)
        index_eeprom = 0;
}

void read_password(unsigned char key) {
    clcd_print(" Enter password ", LINE1(0));
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
    if (index == 4) {
        __delay_ms(300);
        attempt--;
        index = 0;
        if (validate_password(original_password, enter_password) != 0) {
            __delay_ms(500);
            clcd_print(" Wrong Password", LINE1(0));
            clcd_putch(attempt, LINE2(0));
            clcd_print("-Attempt Remain", LINE2(1));
            __delay_ms(500);
            clcd_print("                ", LINE2(0));
        } else {
            index = 0;
            attempt = '3';
            clcd_print("Correct Password", LINE1(0));
            clcd_print("   Menu Page    ", LINE2(0));
            __delay_ms(700);
            enter_flag = 2;
            return;
        }
        if (attempt == '0') {
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
}

int validate_password(char *original_password, char *enter_password) {
    unsigned short ind_compare = 0;
    while (original_password[ind_compare]) {
        if (original_password[ind_compare] - enter_password[ind_compare] != 0) {
            return original_password[ind_compare] - enter_password[ind_compare];
        }
        ind_compare++;
    }
    return original_password[ind_compare] - enter_password[ind_compare];
}

unsigned char *menu_event[4] = {"ST", "DL", "CL", "CP"};

void car_menu(unsigned char key) {
    if (key == 11) {
        sec = 0;
        previous_key = key;
        if (wait1++ > 400) {
            if (menu_index == 4 || menu_index == 3 || menu_index == 1)
                wait1 = 0;
                        
            enter_flag = menu_index + 3;
            if (menu_index > 0) {
                event[10] = menu_event[menu_index - 1][0];
                event[11] = menu_event[menu_index - 1][1];
                store_event(event);
            }
            if (menu_index == 1)
                index = 6;
            else if(menu_index == 0 || menu_index == 2)
                count_event = count1_event;

            return;
        }
    } else if (wait1 != 0 && (wait1 < 400) && previous_key == 11 && key == 0xFF && index_2 > 0) {
        if (star_flag == 0) {
            star_flag = 1;
        } else if (index_2 > 1) {
            index_1--;
            index_2--;
        }
        if (menu_index > 0) {
            menu_index--;
        }
        wait1 = 0;
    }
    if (key == 12) {
        sec = 0;
        previous_key = key;
        if (wait1++ > 400) {
            index_1 = 0;
            index_2 = 1;
            star_flag = 1;
            menu_index = 0;
            enter_flag = 0;
            wait1 = 0;
            return;
        }
    } else if (wait1 != 0 && wait1 < 400 && previous_key == 12 && key == 0xFF && index_1 < 4) {
        if (star_flag == 1) {
            star_flag = 0;
        } else if (index_1 < 3) {
            index_1++;
            index_2++;
        }
        if (menu_index < 4) {
            menu_index++;
        }
        wait1 = 0;
    }
    if (sec == 5) {
        index_1 = 0;
        index_2 = 1;
        star_flag = 1;
        menu_index = 0;
        enter_flag = 0;
        wait1 = 0;
        return;
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
}

void view_log(unsigned char key) {
    clcd_print("Logs:-->[", LINE1(0));
    clcd_putch((view_flag % total_index) + 48, LINE1(9));
    clcd_print("]        ", LINE1(10));
    clcd_putch(count_event + 48, LINE1(14));

    if (key == 11) {
        previous_key = key;
        if (wait1++ > 400) {
        }
    } else if (wait1 != 0 && (wait1 < 400) && previous_key == 11 && key == 0xFF) {
        view_flag--;
        count_event--;
        if (count_event == -1) {
            count_event = total_index-1;
        }
        if (view_flag == -1) {
            view_flag = 9;
        }
        wait1 = 0;
    } else if (key == 12) {
        previous_key = key;
        if (wait1++ > 300) {
            wait1 = 0;
            enter_flag = 2;
            view_flag = 0;
            sec = 0;
            return;
        }
    } else if (wait1 != 0 && wait1 < 300 && previous_key == 12 && key == 0xFF) {
        view_flag++;
        count_event++;
        if (count_event == 10)
            count_event = 0;
        if (view_flag == 10)
            view_flag = 0;
        wait1 = 0;
    } else
        wait1 = 0;

    address = ADR_EEPROM + (16 * (count_event % total_index));
    for (int i = 0; i < 16; i++) {
        view_event[i] = read_external_eeprom(address + i);
    }
    view_event[16] = '\0';
    clcd_print(view_event, LINE2(0));
}

unsigned char time[9] = "00:00:00", wait2 = 0;

void set_time(unsigned char key) {
    if (key == 11) {
        previous_key = key;
        if (wait1++ > 400) {
            wait1 = 0;
            enter_flag = 2;
            index = 0;
            sec = 0;
            clcd_print("Updating Time...", LINE2(0));
            unsigned char ch;
            ch = ((time[6] - 48) << 4) | (time[7] - 48);
            write_ds1307(SEC_ADDR, ch);
            ch = ((time[3] - 48) << 4) | (time[4] - 48);
            write_ds1307(MIN_ADDR, ch);
            ch = ((time[0] - 48) << 4) | (time[1] - 48);
            write_ds1307(HOUR_ADDR, ch);
            __delay_ms(1000);
            return;
        }
    } else if (wait1 != 0 && (wait1 < 400) && previous_key == 11 && key == 0xFF) {
        if (index == 0) {
            if (time[0] != '2') {
                if (time[1]++ == '9') {
                    time[1] = '0';
                    time[0]++;
                }
            } else {
                time[1]++;
                if (time[1] == '4') {
                    time[0] = '0';
                    time[1] = '0';
                }
            }
        } else if (index == 3) {
            if (time[3] != '5') {
                if (time[4]++ == '9') {
                    time[4] = '0';
                    time[3]++;
                }
            } else {
                if (time[4]++ == '9') {
                    time[4] = '0';
                    time[3] = '0';
                }
            }
        } else {
            if (time[6] != '5') {
                if (time[7]++ == '9') {
                    time[7] = '0';
                    time[6]++;
                }
            } else {
                if (time[7]++ == '9') {
                    time[7] = '0';
                    time[6] = '0';
                }
            }
        }
        wait1 = 0;
    } else if (key == 12) {
        previous_key = key;
        if (wait1++ > 400) {
            wait1 = 0;
            enter_flag = 2;
            index = 0;
            sec = 0;
            clcd_print("Time Not Update.", LINE2(0));
            __delay_ms(500);
            return;
        }
    } else if (wait1 != 0 && wait1 < 400 && previous_key == 12 && key == 0xFF) {
        index -= 3;
        if (index == (unsigned) - 3)
            index = 6;
        wait1 = 0;
    }
    if (wait2++ < 100) {
        clcd_print(time, LINE2(0));
    } else if (wait2 > 100) {
        clcd_print("  ", LINE2(index));
        if (wait2 == 200)
            wait2 = 0;
    }
}

void download_log() {
    clcd_print("Download_log :-  ", LINE1(0));
    clcd_print("Downloading... ", LINE2(0));
    clcd_putch(count_event + 48, LINE2(14));
    __delay_ms(4000);
    /*TMR0ON = 0;
    GIE = 0;
    PEIE = 0;
    TMR0IE = 0;
    init_uart();
    puts("#   Time    EV  SP\n\r");*/
    clcd_print("#   Time    EV  SP", LINE2(0));
    __delay_ms(2000);
    for (int i = 0; i < total_index; i++) {
        address = ADR_EEPROM + (16 * (count_event % total_index));
        for (int i = 0; i < 16; i++) {
            view_event[i] = read_external_eeprom(address + i);
        }
        view_event[16] = '\0';
        //puts(view_event);
        clcd_print(view_event, LINE2(0));
        __delay_ms(3000);
        count_event++;
        if(count_event > 9)
            count_event = 0;    
    }
    __delay_ms(2000);
    enter_flag = 2;
    wait1 = 0;
    /*
    TMR0ON = 1;
    GIE = 1;
    PEIE = 1; 
    TMR0IE = 0;*/
}

void clear_log() {
    clcd_print("   Log Cleared   ", LINE1(0));
    clcd_print("  Successfully  ", LINE2(0));
    index_eeprom = 0;
    count_event = 0;
    total_index = 0;
    store_event(event);
    __delay_ms(5000);
}

unsigned char pass_flag = 0, confirm_password[5];

void change_password(unsigned char key) {
    if (pass_flag == 0) {
        clcd_print(" Enter Password ", LINE1(0));
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
        if (index == 4) {
            attempt--;
            index = 0;
            enter_password[4] = '\0';
            if ((validate_password(original_password, enter_password)) != 0) {
                clcd_print(" Wrong Password ", LINE1(0));
                clcd_putch(attempt, LINE2(0));
                clcd_print("-Attempt Remain", LINE2(1));
                __delay_ms(1000);
                clcd_print("                ", LINE2(0));
            } else {
                clcd_print("                ", LINE2(0));
                pass_flag = 1;
            }
            if (attempt == '0') {
                pass_flag = 0;
                enter_flag = 2;
                return;
            }
        }
    } else if (pass_flag == 1) {
        clcd_print("  New Password  ", LINE1(0));
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
        if (index == 4) {
            clcd_print("                ", LINE2(0));
            pass_flag = 2;
            index = 0;
            __delay_ms(500);
        }

    } else if (pass_flag == 2) {
        clcd_print("Confirm Password", LINE1(0));
        if (key == 11) {
            confirm_password[index] = '1';
            clcd_putch('*', LINE2(index + 3));
            sec = 0;
            index++;
        } else if (key == 12) {
            confirm_password[index] = '0';
            clcd_putch('*', LINE2(index + 3));
            sec = 0;
            index++;
        }
        if (index == 4) {
            index = 0;
            confirm_password[4] = '\0';
            if (validate_password(enter_password, confirm_password) == 0) {
                clcd_print("Password changed", LINE1(0));
                clcd_print("  Successfully  ", LINE2(0));
                while (confirm_password[index]) {
                    original_password[index] = confirm_password[index];
                    index++;
                }
                for (int i = 0; i < 4; i++) {
                    write_external_eeprom(0x46 + i, confirm_password[i]);
                }

            } else {
                clcd_print("    Password    ", LINE1(0));
                clcd_print("   Not Matched   ", LINE2(0));
            }
            pass_flag = 0;
            enter_flag = 2;
            index = 0;
            __delay_ms(1000);
        }
    }
    if (sec == 5) {
        pass_flag = 0;
        enter_flag = 2;
        index = 0;
        return;
    }
    if (wait++ < 800) {
        clcd_putch('_', LINE2(index + 3));
    } else if (wait > 800) {
        clcd_putch(' ', LINE2(index + 3));
        if (wait == 1600)
            wait = 0;
    }
}
