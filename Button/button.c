#include <msp430g2553.h> 
#include <intrinsics.h>

#define LED1 BIT0
#define LED2 BIT6
#define BUTTON BIT3


    int main(void)
    {
        WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer
        P1DIR |= (LED1 + LED2); // set leds as outputs
        P1REN |= BUTTON; // enable internal pullup
        P1OUT |= BUTTON; // set pullup
        P1OUT &= ~(LED1 + LED2); // turn off leds
       
        P1IES |= BUTTON; // high to low
        P1IE |= BUTTON; // enable interrupt
        P1IFG &= ~BUTTON; // clear interrupt flag

		__enable_interrupt();

        while(1);
    }

    __attribute__((interrupt(PORT1_VECTOR)))
    void PORT_1(void)
    {
        P1OUT ^= (LED1|LED2);
        P1IFG &= ~BUTTON;
		do P1IFG = 0;
		while( P1IFG != 0 );

    }
