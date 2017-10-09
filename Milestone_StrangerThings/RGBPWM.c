/*
 * softwarePWM.c
 *
 *   Created on:  October 8, 2017
 *  Last Edited:  October 8, 2017
 *       Author:  Nick Gorab
 *        Board:  ALL EXCEPT 2311
 */


#include <msp430.h>
#include <config.h>

#define redLED      BIT2;
#define greenLED    BIT3;
#define blueLED     BIT4;


void ledInitialize(void) {
    P1DIR |= redLED;             // P1.2 to output
    P1SEL |= redLED;             // P1.2 to TA0.1
    P1DIR |= greenLED;
    P1SEL |= greenLED;
    P1DIR |= blueLED;
    P1SEL |= blueLED;
}

int rDuty = 0, gDuty = 0, bDuty = 250;


void main(void) {

    WDTCTL = WDTPW + WDTHOLD;

    ledInitialize();

    TA0CCR0  = 255-1;
    TA0CCTL1 = OUTMOD_7;
    TA0CCR1  = rDuty;
    TA0CCTL2 = OUTMOD_7;
    TA0CCR2  = gDuty;
    TA0CCTL3 = OUTMOD_7;
    TA0CCR3  = bDuty;
    TA0CTL   = TASSEL_2 + MC_1;
}
