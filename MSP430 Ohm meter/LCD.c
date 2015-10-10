/*
 * LCD.c
 *
 *  Created on: Feb 21, 2015
 *      Author: ben & online LCD example
 */
#include "msp430x22x2.h"
#include "LCD.h"
char temp;
char temp2;

void lcd_command(char input)
{
	P4DIR = 0xFF;
	temp = input;
	P4OUT = 0x00;
	__delay_cycles(2200);
	input = input >> 4;
	input = input & 0x0F;
	input = input | 0x20;
	P4OUT = input;
	__delay_cycles(2200);
	input = input & 0x0F;
	P4OUT = input;
	__delay_cycles(2200);
	P4OUT = 0x00;
	__delay_cycles(2200);
	input = temp;
	input = input & 0x0F;
	input = input | 0x20;
	P4OUT = input;
	__delay_cycles(2200);
	input = input & 0x0F;
	P4OUT = input;
	__delay_cycles(2200);
}

void lcd_init(void)
{
	lcd_command(0x33);
	lcd_command(0x32);
	lcd_command(0x2C);
	lcd_command(0x0C);
	lcd_command(CLEAR);
}

void lcd_char(char input)
{
	P4DIR = 0xFF;			//set port D as an output
	temp = input;
	P4OUT = 0x10;
	__delay_cycles(220);
	input = input >> 4;
	input = input & 0x0F;
	input = input | 0x30;
	P4OUT = input;
	__delay_cycles(220);
	input = input & 0x1F;
	P4OUT = input
	__delay_cycles(220);
	P4OUT = 0x10;
	__delay_cycles(220);
	input = temp;
	input = input & 0x0F;
	input = input | 0x30;
	P4OUT = input;
	__delay_cycles(220);
	input = input & 0x1F;
	P4OUT = input;
	__delay_cycles(220);
}

void lcd_string(char* str)
{
	unsigned int i;
	for (i = 0; str[i] != '\0'; i++)
	{
		lcd_char(str[i]);
	}
}

void lcd_setpos(char position){
	position |= 0x80 | position;	//set BIT7 high
	lcd_command(position);
}
