/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2012, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************
 * 
 *                       MSP430 CODE EXAMPLE DISCLAIMER
 *
 * MSP430 code examples are self-contained low-level programs that typically
 * demonstrate a single peripheral function or device feature in a highly
 * concise manner. For this the code may rely on the device's power-on default
 * register values and settings such as the clock configuration and care must
 * be taken when combining code from several examples to avoid potential side
 * effects. Also see www.ti.com/grace for a GUI- and www.ti.com/msp430ware
 * for an API functional library-approach to peripheral configuration.
 *
 * --/COPYRIGHT--*/
//******************************************************************************
//  MSP430F22x4 Demo - Timer_A, Toggle P1.1/TA0, Up Mode, HF XTAL ACLK
//
//  Description: Toggle P1.1 using hardware TA0 output. Timer_A is configured
//  for up mode with TACCR0 defining period, TA0 also output on P1.1. In this
//  example, TACCR0 is loaded with 500-1 and TA0 will toggle P1.1 at TACLK/500.
//  Thus the output frequency on P1.1 will be the TACLK/1000. No CPU or
//  software resources required.
//  ACLK = MCLK = TACLK = HF XTAL
//  //* HF XTAL REQUIRED AND NOT INSTALLED ON FET *//
//  //* Min Vcc required varies with MCLK frequency - refer to datasheet *//
//  As coded with TACLK = ACLK, P1.1 output frequency = HF XTAL/1000
//
//           MSP430F22x4
//         -----------------
//     /|\|              XIN|-
//      | |                 | HF XTAL (3 � 16MHz crystal or resonator)
//      --|RST          XOUT|-
//        |                 |
//        |         P1.1/TA0|--> ACLK/1000
//
//  A. Dannenberg
//  Texas Instruments Inc.
//  April 2006
//  Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.41A
//******************************************************************************
#include <msp430.h>

int main(void)
{
  volatile unsigned int i;
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  BCSCTL1 |= XTS;                           // ACLK = LFXT1 = HF XTAL
  BCSCTL3 |= LFXT1S1;                       // LFXT1S1 = 3-16Mhz

  do
  {
    IFG1 &= ~OFIFG;                         // Clear OSCFault flag
    for (i = 0xFF; i > 0; i--);             // Time for flag to set
  }
  while (IFG1 & OFIFG);                     // OSCFault flag still set?
  BCSCTL2 |= SELM_3;                        // MCLK = LFXT1 (safe)

  P1DIR |= 0x02;                            // P1.1 output
  P1SEL |= 0x02;                            // P1.1 option select
  TACCTL0 = OUTMOD_4;                       // TACCR0 toggle mode
  TACCR0 = 500 - 1;
  TACTL = TASSEL_1 + MC_1;                  // ACLK, in upmode

  __bis_SR_register(CPUOFF);                // CPU off
}
