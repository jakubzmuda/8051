#include <8051.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;

unsigned char receivedCharacter;
unsigned char receivedFlag;

int getTH1ForFrequency(int frequency) {
    if(frequency == 19200) {
        return 0xfd;
    }
    return -1;
}

void init(int frequency) // todo jakas mape zrobic
{
    SCON = 0x50;
    TMOD &= 0x0f;
    TMOD |= 0x20;

    TCON = 0x40;
    PCON = 0x80;

    TH1 = getTH1ForFrequency(frequency); // th1 obliczamy z tej stronki tak zeby wyszlo 19200 jak w zadaniu
    TR1 = 1;
    TI = 0; // TI mowi o tym czy mozemy pisac czy czytac
    EA = 1;
    ES = 1;
    IE = 0x90;

    receivedFlag=0;
}

void put(unsigned char c)
{
    SBUF = c;
    while(!TI){};
    TI = 0;
}

void put_DEC_U8(uint16_t w) {
    put(w);
    receivedCharacter = '\0';

}

void get_BIN_U8(unsigned char *x)
{
    while (receivedCharacter != ' ')
    {
        if(receivedFlag && (receivedCharacter == '1' || receivedCharacter == '0')) {
            *x = *x << 1;
            *x+= receivedCharacter - '0';
            receivedFlag = 0;
        }
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
        z1 = 0; // po wypisaniu musimy wyzerowac zmienna
    }
}
