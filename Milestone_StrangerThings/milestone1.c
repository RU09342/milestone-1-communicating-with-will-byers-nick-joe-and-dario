/*
 * Milestone1.c
 *
 *   Created on:  October 8, 2017
 *  Last Edited:  October 9, 2017
 *       Author:  Nick Gorab
 *        Board:  MSP430F5529
 */


#include <msp430f5529.h>


/************************\
 *                      *
 *   Code Definitions   *
 *                      *
\************************/

#define redLED      BIT2;
#define greenLED    BIT3;
#define blueLED     BIT4;
#define active      BIT7;
volatile int bit;



/*************************\
 *                       *
 *  UART Initialization  *
 *                       *
\*************************/

void uartInit(void) {
    P3SEL    |=  BIT3;      // UART TX
    P3SEL    |=  BIT4;      // UART RX
    UCA0CTL1 |=  UCSWRST;   // Resets state machine
    UCA0CTL1 |=  UCSSEL_2;  // SMCLK
    UCA0BR0   =  6;         // 9600 Baud Rate
    UCA0BR1   =  0;         // 9600 Baud Rate
    UCA0MCTL |=  UCBRS_0;   // Modulation
    UCA0MCTL |=  UCBRF_13;  // Modulation
    UCA0MCTL |=  UCOS16;    // Modulation
    UCA0CTL1 &= ~UCSWRST;   // Initializes the state machine
    UCA0IE   |=  UCRXIE;    // Enables USCI_A0 RX Interrupt
}



/************************\
 *                      *
 *  LED Initialization  *
 *                      *
\************************/

void ledInit(void) {
    P4DIR |=  active;       // Indicator LED to output
    P4OUT &= ~active;       // Indicator LED off by default
    P1DIR |=  redLED;       // P1.2 to output
    P1SEL |=  redLED;       // P1.2 to TA0.1
    P1DIR |=  greenLED;     // P1.3 to output
    P1SEL |=  greenLED;     // P1.3 to TA0.2
    P1DIR |=  blueLED;      // P1.4 to output
    P1SEL |=  blueLED;      // P1.4 to TA0.3
}



/************************\
 *                      *
 *  PWM Initialization  *
 *                      *
\************************/

void pwmInit(void) {
    TA0CCR0  = 0x00FF;
    TA0CCTL1 = OUTMOD_7;
    TA0CCR1  = 0;
    TA0CCTL2 = OUTMOD_7;
    TA0CCR2  = 0;
    TA0CCTL3 = OUTMOD_7;
    TA0CCR3  = 0;
    TA0CTL   = TASSEL_2 + MC_1 + ID_3;
}



/***********************\
 *                     * 
 *    Main Function    *
 *                     *
\***********************/

void main(void) {

    WDTCTL = WDTPW + WDTHOLD;

    uartInit();
    ledInit();
    pwmInit();

    __bis_SR_register(LPM0_bits + GIE);

}



/************************\
 *                      *
 *    UART Interrupt    *
 *                      *
\************************/

#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void) {
    P4OUT |= active;
    switch(__even_in_range(UCA0IV,USCI_UCTXIFG)) {

        case USCI_NONE: break;

        case USCI_UCRXIFG:
            switch(bit) {
                case 0 :
                    while(!(UCA0IFG & UCTXIFG));
                    UCA0TXBUF = UCA0RXBUF - 3;
                    __no_operation();
                    break;
                case 1 :
                    TA0CCR1 = (UCA0RXBUF);
                    break;
                case 2 :
                    TA0CCR2 = (UCA0RXBUF);
                    break;
                case 3 :
                    TA0CCR3 = (UCA0RXBUF);
                    break;
                default:
                    while(!(UCA0IFG & UCTXIFG));
                    UCA0TXBUF = UCA0RXBUF;
            }
            if(UCA0RXBUF != 0x0D){
                bit += 1;
            } else if (UCA0RXBUF == 0x0D){
                P4OUT &= ~active;
                bit = 0;
            }
            break;
        case USCI_UCTXIFG : break;
        default: break;
    }
}

