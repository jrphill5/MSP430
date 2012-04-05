// Nokia 5110 48x84 LCD Screen

#include <msp430.h>
#include "font8x6.h"

#define F_CPU 1000000UL

#define CMD		0
#define CHR		1

#define LCDDIR	P2DIR
#define LCDOUT	P2OUT

#define LCDSCE	BIT3	// pin 3
#define LCDRST	BIT4	// pin 4
#define LCDDC	BIT5	// pin 5
#define LCDDIN	BIT2	// pin 6
#define LCDCLK	BIT1	// pin 7
#define LCDBL	BIT0	// pin 8

#define BUTTON		BIT3
#define BUTTON_OUT	P1OUT
#define BUTTON_DIR	P1DIR
#define BUTTON_IN	P1IN
#define BUTTON_IE	P1IE
#define BUTTON_IES	P1IES
#define BUTTON_IFG	P1IFG
#define BUTTON_REN	P1REN

const char LCDROW = 48;
const char LCDCOL = 84;

void delay_us( unsigned int delay );
void delay_ms( unsigned int delay );

void LCD_send_byte( unsigned char command, unsigned char data );
void LCD_send_char( unsigned char character );
void LCD_set_coord( unsigned char X, unsigned char Y );
void LCD_init();
void LCD_clear();
void LCD_fill();
void LCD_backlight( unsigned char enabled );

int main()
{

	WDTCTL = WDTPW | WDTHOLD;	// Disable watchdog timer.

	LCD_init();

	BUTTON_DIR &=~BUTTON;
	BUTTON_OUT |= BUTTON;
	BUTTON_REN |= BUTTON;
	BUTTON_IES |= BUTTON;
	BUTTON_IFG &=~BUTTON;
	BUTTON_IE  |= BUTTON;
	__enable_interrupt();

	int i, j;
	while( 1 ) for( j = 0x00 - LCDCOL/6; j < 0x80; j++ )
	{
		LCD_set_coord( 0, 0 );
		for( i = j; i < j + LCDCOL/6; i++ )
			if( i < 0x00 || i > 0x7F ) LCD_send_char( ' ' );
			else                       LCD_send_char( i );
		delay_ms(100);
	}

}

__attribute__((interrupt(PORT1_VECTOR)))
void PORT1_ISR()
{

	BUTTON_IFG = 0;
	delay_ms( 15 );
	if( (BUTTON_IN & BUTTON) == 0 ) LCDOUT ^= LCDBL;

}

void delay_us( unsigned int delay )
{

	while( delay-- ) __delay_cycles(1);		// MCLK(1MHZ) / 1000000

}

void delay_ms( unsigned int delay )
{

    while( delay-- ) __delay_cycles(1000); // MCLK(1MHz) / 1000

}

void LCD_init()
{

	LCDDIR = LCDSCE | LCDRST | LCDDC | LCDDIN | LCDCLK | LCDBL;

	LCD_backlight( 0 );			// Disable LCD backlight.

	LCDOUT &=~LCDRST;	// RST = 0
	delay_us( 1 );
	LCDOUT |= LCDRST;	// RST = 1

	LCD_send_byte( CMD, 0x20 );	// Active, Horizontal Addressing, Basic Instruction Set
	LCD_send_byte( CMD, 0x0C );	// Normal LCD Mode
	LCD_clear();				// Clear LCD Screen

}

void LCD_send_byte( unsigned char command, unsigned char data )
{

    unsigned char bit;

    LCDOUT &=~LCDSCE;						// SCE = 0

    if(command) LCDOUT |= LCDDC;			// DC = 1
    else        LCDOUT &=~LCDDC;			// DC = 0

    for( bit = 0; bit < 8; bit++ )
    {

        if( data & 0x80) LCDOUT |= LCDDIN;	// DIN = 1
        else             LCDOUT &=~LCDDIN;	// DIN = 0

        LCDOUT &=~LCDCLK;					// CLK = 0
        data = data << 1;					// Rotate to next bit.
        LCDOUT |= LCDCLK;					// CLK = 1

    }

    LCDOUT |= LCDSCE;							// SCE = 1

}

void LCD_send_char( unsigned char character )
{

	unsigned int line;

	for( line = 0; line < 6; line++ )
		LCD_send_byte( CHR, ascii[character][line] );

}

void LCD_set_coord( unsigned char row, unsigned char col )
{

	LCD_send_byte( CMD, 0x80 | col );	// Set X RAM address.
	LCD_send_byte( CMD, 0x40 | row );	// Set Y RAM address.

}

void LCD_clear()
{

	unsigned int i;
	LCD_set_coord( 0, 0 );	// Reset X and Y RAM addresses.
	for( i = 0; i < LCDROW/8*LCDCOL; i++) LCD_send_byte( CHR, 0x00 );

}

void LCD_fill()
{

	unsigned int i;
	LCD_set_coord( 0, 0 );	// Reset X and Y RAM addresses.
	for( i = 0; i < LCDROW/8*LCDCOL; i++) LCD_send_byte( CHR, 0xFF );

}

void LCD_backlight( unsigned char enabled )
{

	if(enabled) LCDOUT |= LCDBL;	// BL = 1
	else        LCDOUT &=~LCDBL;	// BL = 0

}

