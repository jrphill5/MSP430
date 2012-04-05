#include <msp430.h>

int main()
{

	volatile int i;

	WDTCTL = WDTPW | WDTHOLD;		// disable watchdog timer
	P1DIR = 0x01;					// set P1.0 as output
	P1OUT = 0x00;					// clear P1.0

	while ( 1 )
	{

	    P1OUT ^= 0x01;				// toggle P1.0
	    for (i = 0; i < 8096; i++);	// delay for a bit

	}

}
