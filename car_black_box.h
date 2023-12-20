/* 
 * File:   car_black_box.h
 * Author: santhoshkumar R
 *
 * Created on December 13, 2023, 10:54 PM
 */

#ifndef CAR_BLACK_BOX_H
#define	CAR_BLACK_BOX_H

void read_password(unsigned char key);
void car_menu(unsigned char key);

void init_timer0();

void display_dashboard(unsigned char key);
void display_time();
void gear_monitor(unsigned char key);
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

#ifndef LCD_H
#define LCD_H



#define CLCD_PORT			PORTD
#define CLCD_EN				RC2
#define CLCD_RS				RC1
#define CLCD_RW				RC0
#define CLCD_BUSY			RD7
#define PORT_DIR			TRISD7


#define HI												1
#define LO												0

#define INPUT											0xFF
#define OUTPUT											0x00

#define DATA_COMMAND									1
#define INSTRUCTION_COMMAND								0
#define _XTAL_FREQ                  20000000
#define LINE1(x)									(0x80 + (x))
#define LINE2(x)										(0xC0 + (x))

#define TWO_LINE_5x8_MATRIX_8_BIT					clcd_write(0x38, INSTRUCTION_COMMAND)
#define CLEAR_DISP_SCREEN				                clcd_write(0x01, INSTRUCTION_COMMAND)
#define CURSOR_HOME							clcd_write(0x02, INSTRUCTION_COMMAND)
#define DISP_ON_AND_CURSOR_OFF						clcd_write(0x0C, INSTRUCTION_COMMAND)
#define EIGHT_BIT_MODE   0x33
void init_clcd(void);
void clcd_print(const unsigned char *data, unsigned char addr);
void clcd_putch(const unsigned char data, unsigned char addr);
void clcd_write(unsigned char bit_values, unsigned char control_bit);

#endif



#ifdef	__cplusplus
}
#endif

#endif	/* CAR_BLACK_BOX_H */

