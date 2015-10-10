
#include <msp430.h>
#include "LCD.h"

unsigned int getADCaverage(int times)
{
	unsigned int i;
	volatile unsigned int sum = 0;
	volatile unsigned int value;
	for(i = 0; i < times; i++)
	{
		ADC10CTL0 |= ENC + ADC10SC;             // Start the conversion
		__bis_SR_register(CPUOFF + GIE);        // call ISR
		value = ADC10MEM;
		sum += value;
	}
	return sum / times;
}

void printResistance(unsigned int ADCaverage)
{
	static unsigned short isOutofRange = 0;

	volatile float voltage = ((float)ADCaverage/1023)*3.3;

	float resistance = 1000*(33 / voltage - 10);

	if (ADCaverage > 930 || ADCaverage < 10)
	{
		P1OUT &= ~0x01;                       // turn LED off
		if(isOutofRange == 0)
		{
			lcd_command(CLEAR);
			isOutofRange = 1;
		}
		lcd_string("Out of Range");
		lcd_command(CR);
	}
    else
    {
	   	isOutofRange = 0;
	   	P1OUT |= 0x01;                        // turn LED on
	   	lcd_string("R = ");
		unsigned long resAsInt = resistance;
		int numArray[7];
		int i;
		char numToChar;

		for(i = 6 ;i >= 0; i-- ){
			numArray[i] = resAsInt % 10;
		 	resAsInt /= 10;
		}

		for(i = 0; i < 7; i++ ){
			numToChar = numArray[i] +'0';
			lcd_char(numToChar);
		}
		lcd_string(" Ohms");
		lcd_command(CR);
	}
}


int main(void) {

	WDTCTL = WDTPW | WDTHOLD;		
	P1DIR |= 0x01;					
	// ADC
	ADC10CTL0 = ADC10SHT_2 + ADC10ON + ADC10IE; 
	ADC10AE0 |= 0x01;                         

	P1DIR |= 0x01;                            


	lcd_init();
	lcd_string("Out of range!");
	lcd_command(CR);
	lcd_string("R = XXXXXXX Ohms");
	lcd_command(CR);

	lcd_command(CLEAR);
	volatile unsigned int i;	

	P1OUT = 0x01;				

	while(1)
	{
		unsigned int averageADCvalue;			// variable to store average from the ADC
		averageADCvalue = getADCaverage(20);	// sample 20 times and average out the result
		printResistance(averageADCvalue);		// print out the result to the screen
	}
}


// ADC ISR

#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{
  __bic_SR_register_on_exit(CPUOFF);        
}

