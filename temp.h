#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator: High-speed crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config MCLRE = ON       // RA5/MCLR/VPP Pin Function Select bit (RA5/MCLR/VPP pin function is MCLR)
#pragma config BOREN = ON       // Brown-out Detect Enable bit (BOD enabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable bit (RB4/PGM pin has digital I/O function, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EE Memory Code Protection bit (Data memory code protection off)
#pragma config CP = ON          // Flash Program Memory Code Protection bit (0000h to 07FFh code-protected)

#include <xc.h>
#include <stdint.h>

#define     _XTAL_FREQ      8000000
#define     BAUD19200       (_XTAL_FREQ / 16) / 19200 - 1
#define     SH_CP           PORTBbits.RB0
#define     ST_CP           PORTBbits.RB4
#define     DS              PORTBbits.RB5
#define     RS              PORTBbits.RB6
#define     E               PORTBbits.RB7
#define     true            1
#define     false           0
#define     TRISDQ          TRISAbits.TRISA4
#define     PORTDQ          PORTAbits.RA4
#define     SKIP_ROM        0xCC
#define     CONVERT         0x44
#define     READ_TEMP       0xBE

char buffer[5];
__bit neg, online, done;

__bit oneWireInit(void);
uint8_t oneWireRead(void);
char * itoa(int8_t);
char * reverse(char *, uint8_t);
void oneWireWrite(uint8_t);

void delay_40_us(void);
void delay_60_us(void);
void delay_480_us(void);
void delay_1600_us(void);

void shiftByte(uint8_t);
void wr_command(uint8_t);
void wr_data(uint8_t);
void print(char*);
void clear_lcd(void);

