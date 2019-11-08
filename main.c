#include <8051.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;

unsigned char receivedCharacter;
unsigned char receivedFlag;
unsigned char input[100];
unsigned char counter;

void init(int frequency)
{
    SCON = 0x50;
    TMOD &= 0x0f;
    TMOD |= 0x20;

    TCON = 0x40;
    PCON = 0x80;

    TH1 = 0xfd; // th1 obliczamy z tej stronki tak zeby wyszlo 19200 jak w zadaniu
    TR1 = 1;
    TI = 0; // TI mowi o tym czy mozemy pisac czy czytac
    EA = 1;
    ES = 1;
    IE = 0x90;

    receivedFlag=0;
    counter=0;
}

void put(unsigned char c)
{
    SBUF = c; // wysylanie znaku
    while(!TI){}; // czekanie az znak zostanie wyslany
    TI = 0;
    receivedFlag = 0;
}

void put_DEC_U8(uint16_t w) {
    input[counter]=w;
    if ((receivedCharacter == 0x20 || receivedCharacter == 0x09) && receivedFlag)
        {
            put(' ');
            input[counter] ='\0';
            counter = 0;
            while (input[counter] != 0x0) {
               put(input[counter++]);
            }
            counter = 0;
            put(0x0D);
            put(0x0A);
        }
}

void get_BIN_U8(unsigned char *x)
{
    if (receivedFlag && *x != 0x20)
    { // TODO tutaj lapac znak z interrupta i dopisywac do inputu
        //input[counter]=*x;
        put(*x);
        //counter++;
    }
}

void catchInterrupt() __interrupt 4
{
    if(RI)
    {
        receivedCharacter=SBUF;
        RI=0;
        receivedFlag=1;
    }
}

void main(void)
{
    uint8_t z1;
    uint16_t w;
    init(19200);
    for(;;) {
        get_BIN_U8(&z1);
        w = z1 + 4;
        put_DEC_U8(w);
    }
}
