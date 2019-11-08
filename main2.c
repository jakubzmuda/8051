#include <8051.h>
#include <stdio.h>
#include <ctype.h>
//#include <stdint.h>

typedef unsigned short int      uint8_t;
typedef unsigned long int       uint32_t;

char getchar(void) {
 char c;
 while (!RI);
 c = SBUF;
 RI = 0;
 return c;
}

void putchar(char c) {
 while (!TI);
 TI = 0;
 SBUF = c;
}

void put_BIN_U8(uint32_t w) {
    int c;
	int k, i;
    static int pows[] = {10000, 1000, 100, 10, 1};
	int zero = 1;
    for (i = 31; i >= 0; i--) {
        k = w >> i;
        if (k & 1)
            putchar('1');
        else
            putchar('0');
    }
    putchar(' ');
}

void get_DEC_U8(uint8_t *z) {
    char character = getchar();
    while (character != ' ') {
        *z*=10;
        *z+=character - '0';
        character = getchar();
    }

}

void init(int bitrate) {
	SCON = 0x50; /* configure serial */
    TMOD&=0x0f; /* configure timer */
	TMOD|=0x20;
    TH1 = 0xfd; /* baud rate 1200 */
    TL1 = 0xfd; /* baud rate 1200 */
    TR1 = 1; /* enable timer */
    TI = 1; /* enable transmitting */
    RI = 0; /* waiting to receive */
	TCON=0x40;
	PCON=0x80;
}

void main(void) {
    uint8_t z1;
	uint32_t w;
    init(19200);
    for (;;) {
        z1 = 0;
        get_DEC_U8(&z1);
        w = z1 * 3;
        put_BIN_U8(w);
    }
}
