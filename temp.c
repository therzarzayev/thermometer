#include "trainheader.h"

void main() {
    
    TRISB = 0b00001110;
    wr_command(0b00001100);
    wr_command(0b00111000);
    
    SPBRG = BAUD19200;
    TXSTAbits.BRGH = 1;
    TXSTAbits.TXEN = 1;
    RCSTAbits.SPEN = 1;
    
    done = false;
    
    uint8_t tl, th;
    int8_t temp;
    
    while(true) {
        if(oneWireInit()) {
            oneWireWrite(SKIP_ROM);
            if(!done) {
                oneWireWrite(CONVERT);
                done = true;
                __delay_ms(750);
            }
            else {
                oneWireWrite(READ_TEMP);
                tl = oneWireRead();
                th = oneWireRead();
                tl &= 0xF0;
                th &= 0x0F;
                temp = th | tl;
                temp = (temp >> 4) | (temp << 4);
                done = false;
                
                clear_lcd();
                print("Temperature:");
                print(itoa(temp));
                wr_data(223);
                
            }
        }
        else {
            clear_lcd();
            print("ERROR");
            __delay_ms(750);
            
        }
    }
}

char * itoa(int8_t n) {
    neg = false;
    uint8_t i = 0;
    if(n < 0) {
        n = -n;
        neg = true;
    }
    do {
        *(buffer + i++) = n % 10 + 48;
        n /= 10;
    } while(n);
    if(neg) *(buffer + i++) = 45;
    *(buffer + i) = 0;
    return reverse(buffer, i - 1);
}
char * reverse(char *s, uint8_t j) {
    char c;
    uint8_t i = 0;
    while(i < j) {
        c = *(s + i);
        *(s + i++) = *(s + j);
        *(s + j--) = c;
    }
    return s;
}

//----------------------DS18b20------------------------------------------//
__bit oneWireInit() {
    online = false;
    TRISDQ = 0;
    PORTDQ = 0;
    delay_480_us();
    TRISDQ = 1;
    delay_60_us();
    if(!PORTDQ) online = true;
    delay_480_us();
    return online;
}
void oneWireWrite(uint8_t byte) {
    uint8_t t=1;
    for(uint8_t i = 0; i < 8; i++) {
        uint8_t b = byte & t;
        TRISDQ = 0;
        PORTDQ = 0;
        if(b) TRISDQ = 1;        
        delay_60_us();
        TRISDQ = 1;
        t<<=1;
    }
}
uint8_t oneWireRead() {
    uint8_t byte = 0,t=1;
    for(uint8_t i = 0; i < 8; i++) {
        TRISDQ = 0;
        PORTDQ = 0;
        TRISDQ = 1;
        if(PORTDQ) byte |= t;
        delay_60_us();
        t<<=1;
    }
    return byte;
}

void delay_40_us() {
    __delay_us(40);
}
void delay_60_us() {
    __delay_us(60);
}
void delay_480_us() {
    __delay_us(480);
}
void delay_1600_us(){
    __delay_us(1600);
}

//-----------------------LCD---------------------------------------------//
void shiftByte(uint8_t byte){
    uint8_t tmp = 0b10000000;
    SH_CP = 0;
    ST_CP = 0;
    for(uint8_t i=0; i<8; i++){
        if(byte & tmp) DS=1;
        else DS=0;
        SH_CP = 1;
        SH_CP = 0;
        tmp >>= 1;
    }
    ST_CP = 1;
    ST_CP = 0;
}
void wr_command(uint8_t comm){
    RS = 0;
    shiftByte(comm);
    E = 1;
    E = 0;
    delay_40_us();
}
void wr_data(uint8_t data){
    RS = 1;
    shiftByte(data);
    E = 1;
    E = 0;
    delay_40_us();
}
void clear_lcd(){
    wr_command(0b00000001);
    delay_1600_us();
}
void print(char* s){
    while(*s) wr_data(*s++);
}
//-----------------------LCD---------------------------------------------//