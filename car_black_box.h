/* 
 * File:   car_black_box.h
 * Author: santhoshkumar R
 *
 * Created on December 13, 2023, 10:54 PM
 */

#ifndef CAR_BLACK_BOX_H
#define	CAR_BLACK_BOX_H

void init_config();

#define ADR_EEPROM 0x00

#define SLAVE_READ_eeprom	0xA1
#define SLAVE_WRITE_eeprom	0xA0
void write_external_eeprom(unsigned char address1,  unsigned char data);
unsigned char read_external_eeprom(unsigned char address1);

void read_password(unsigned char key);
void car_menu(unsigned char key);
void view_log(unsigned char key);
void set_time(unsigned char key);
void download_log();
void change_password(unsigned char key);
void clear_log();

void store_event(char *event);

void init_timer0();

void display_dashboard(unsigned char key);
void gear_monitor(unsigned char key);
void get_time(void);
void display_speed(unsigned short speed);

unsigned short read_adc(unsigned char channel);
void init_adc();

#define LEVEL 0
#define EDGE 1
unsigned char read_switches(unsigned char);
unsigned char scan_key();
void init_matrix_keypad();

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef	__cplusplus
}
#endif

#endif	/* CAR_BLACK_BOX_H */

