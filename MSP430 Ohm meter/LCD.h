/*
 *	LCD.h
 *
 * 	LCD HEADER FILE
 *
 *	Intro: the LCD screen has 80 addresses (DDRAM addresses)
 *	addresses numbers 0-15 will appear on the first line (16 chars in the line)
 *	the next 24 addresses are outside of the range of the screen.
 *	addresses numbers 40-55 will appear on the second line (16 chars in the line)
 *	the next 24 addresses are out of the range of the screen.
 *	TOTAL of 80 CHARS can be stored in the LCD.
 *
 *	HARDWARE circuitry:
 *	MCU used: MSP430F2274
 *
 *	MCU					LCD
 *	P4.0 (PIN 17)  ->	DB4 (PIN 11)
 *	P4.1 (PIN 18)  ->	DB5 (PIN 12)
 *	P4.2 (PIN 19)  ->	DB6 (PIN 13)
 *	P4.3 (PIN 20)  ->	DB7 (PIN 14)
 *	P4.4 (PIN 21)  ->	RS  (PIN  4)
 *	P4.5 (PIN 22)  ->	E   (PIN  6)
 *	P4.7 (PIN 24)  ->	R/W (PIN  5)
 *
 *	For the rest of the LCD pins:
 *	PIN 1 -> GND
 *	PIN 2 -> 5V
 *	PIN 3 -> center of potentiameter
 *
 *  Created on: Feb 21, 2015
 *      Author: Benjamin Babtsov
 */

#ifndef LCD_H_
#define LCD_H_
/**
 * LCD commands
 */
#define CR			0x02	// Carriage Return (return home)
#define CLEAR		0x01	// Clear screen and CR

void lcd_command(char);

/**
 * lcd_setpos sets the position of the DDRAM address counter.
 * valid values are from 0 to 79
 */
void lcd_setpos(char);

/**
* output a single character to the screen
*/
void lcd_char(char);

/**
* Initialize the screen with various configurations
*/
void lcd_init(void);

/**
* output a string to the screen
*/
void lcd_string(char*);

#endif /* LCD_H_ */
