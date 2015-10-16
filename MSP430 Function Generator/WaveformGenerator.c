//***************************************************************************************
// function generator for sine wave, a square wave, a triangle wave and a saw tooth wave.
// frequency: from 10 Hz to 100Hz
// amplitude: from 1 V to 5 V
//
// DAC      -> MSP pins
// RESET    -> P3.6
// DATA out -> P3.4
// CLOCK    -> P3.0
//
//***************************************************************************************
#include <msp430.h>
#include <stdint.h>

#define SINE     '0'
#define SQUARE   '1'
#define TRAINGLE '2'
#define SAWTOOTH '3'
int sin[50] = {544,576,641,703,762,817,867,911,948,979,1001,1016,1022,1020,1010,991,964,931,890,843,790,733,672,609,544,478,413,350,289,232,179,132,91,58,31,12,2,0,6,21,43,74,111,155,205,260,319,381,446,478};
int square[50] = {1023,1023,1023,1023,1023,1023,1023,1023,1023,1023,1023,1023,1023,1023,1023,1023,1023,1023,1023,1023,1023,1023,1023,1023,1023,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1023};
int sawtooth[50] = {0,20,41,62,83,104,125,146,167,187,208,229,250,271,292,313,334,354,375,396,417,438,459,480,501,521,542,563,584,605,626,647,668,688,709,730,751,772,793,814,835,855,876,897,918,939,960,981,1002,0};
int traingle[50] = {1023,981,939,897,855,814,772,730,688,647,605,563,521,480,438,396,354,313,271,229,187,146,104,62,20,20,62,104,146,187,229,271,313,354,396,438,480,521,563,605,647,688,730,772,814,855,897,939,981,1023};
// SPI stuff
void delay1(void);
void delay2(int);
void SPI_transfer(unsigned int data);
char get_waveform_type(void)
{
    char type;
    char P2_input = P2IN & 0b00000110; //bitmask pin 1 and 2
    if  (P2_input == 0b00000000) { type = SINE;}
    else if (P2_input == 0b00000010) { type = SQUARE;}
    else if (P2_input == 0b00000100) { type = TRAINGLE;}
    else                 { type = SAWTOOTH;}
    return type;
}
void output_waveform(int freq_value, int arr[])
{
          int i;
          for (i = 0; i < 50; i++)
          {
              SPI_transfer(arr[i]);
              delay2(freq_value);
          }
}
// ADC stuff
void ADC_init(void)
{
    ADC10CTL0 = ADC10SHT_2 + ADC10ON + ADC10IE; // ADC10ON, interrupt enabled
    ADC10AE0 |= 0x01;                         // P2.0 ADC option select
}


int get_ADC_value(void)
{
    volatile unsigned int ADC_value;
    ADC10CTL0 |= ENC + ADC10SC;             // conversion start
    __bis_SR_register(CPUOFF + GIE);        // enter the LPM
    ADC_value = ADC10MEM;

    if (ADC_value <120)
    {
        return 8;
    }
    else if (ADC_value > 1010)
    {
        return 235;
    }
    else
    {
        return (ADC_value / 4) -17; //convert from ADC to appropriate value to delay
    }
}

// ADC10 interrupt service routine
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{
  __bic_SR_register_on_exit(CPUOFF);        // Clear CPUOFF bit from 0(SR)
}



int main(void)
{
  volatile unsigned int i;
  //initalizations
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
  P2DIR = 0x00;                         //input port pin 2.0 -> ADC; Pin 2.1 & Pin 2.2 ->select waveform
  P3OUT = 0x40;                             // Set slave reset
  P3DIR |= 0x40;                            //
  P3SEL |= 0x31;                            // P3.0,4,5 USCI_A0 option select
  UCA0CTL0 |= UCCKPL + UCMSB + UCMST + UCSYNC;  // 3-pin, 8-bit SPI master
  UCA0CTL1 |= UCSSEL_3;                     // SMCLK
  UCA0BR0 |= 0x02;                          // /2
  UCA0BR1 = 0;                              //
  UCA0MCTL = 0;                             // No modulation
  UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
  P3OUT &= ~0x40;                           // Now with SPI signals initialized,
  P3OUT |= 0x40;                            // reset slave
  ADC_init();

  int count = 0;
  volatile int freq = get_ADC_value();
  while(1)
  {
      if (count == 24)
      {
         freq = get_ADC_value();
         count = 0;
      }
      count++;

      switch(get_waveform_type())
      {
      case SINE:
          output_waveform(freq,sin);
          break;
      case SQUARE:
          output_waveform(freq,square);
          break;
      case TRAINGLE:
          output_waveform(freq,traingle);
          break;
      case SAWTOOTH:
          output_waveform(freq,sawtooth);
          break;
      }


  }
}
void SPI_transfer( unsigned int data)
{
    uint8_t byte2Transmit = data & 0x003F;
    byte2Transmit = byte2Transmit << 2;
    data = data >> 6 ;
    uint8_t byte1Transmit = data & 0x000F;
    byte1Transmit |= 0xF0;
    P3OUT = 0b10111111 & P3OUT;
    delay1();
    UCA0TXBUF = byte1Transmit;
    while(!(IFG2 & UCA0TXIFG));
    UCA0TXBUF=byte2Transmit;
    while((UCB0STAT&BIT0));
    delay1();
    P3OUT += 0x40;
}

void delay1(void){
    volatile unsigned int i = 1;
    while(i > 0) {
        i--;
    }
}
void delay2(int val){
    // range for 100Hz: 8
    // range for 10 hz: 235

    // ADC max: 1023; ADC min: 93
    volatile unsigned int i = val;
    while(i > 0) {
        i--;
    }
}