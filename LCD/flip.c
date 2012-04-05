#include <stdio.h>
#include "font8x6.h"

unsigned char* reverse( unsigned char a[6] );
void printchar( unsigned char c, unsigned char a[6] );

int main()
{

	unsigned int i, j;
	unsigned char a[6];
	unsigned char c;

	printf("const unsigned char ascii[][6] = {\n\n");

	for( c = 0; c < 0x80; c++ )
	{

		for( i = 0; i < 6; i++ ) a[i] = ascii[c][i];
		printchar( c, reverse( a ) );

	}

	printf("\n};\n");

	return 0;

}

unsigned char* reverse( unsigned char a[6] )
{

	unsigned int i;
	static unsigned char r[6];

	for( i = 0; i < 6; i++ )
		r[5-i] = ((a[i] & 0x01) << 7)
	         | ((a[i] & 0x02) << 5)
	         | ((a[i] & 0x04) << 3)
	         | ((a[i] & 0x08) << 1)
	         | ((a[i] & 0x10) >> 1)
	         | ((a[i] & 0x20) >> 3)
	         | ((a[i] & 0x40) >> 5)
	         | ((a[i] & 0x80) >> 7);

	return r;

}	

void printchar( unsigned char c, unsigned char a[6] )
{

	unsigned int i;

	for( i = 0; i < 6; i++ )
	{

		if( i == 0 ) printf( "	{ " );
		printf( "0x%02X", a[i] );
		if( i == 5 ) printf( " }, // 0x%02X - %c \n", c, c );
		else printf( ", " );

	}

}
