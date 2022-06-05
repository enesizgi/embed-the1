#include <xc.h>
#include <stdint.h>
// CONFIG Oscillator and TURN OFF Watchdog timer
#pragma config OSC = HSPLL
#pragma config WDT = OFF

#define true 1
#define false 0

void tmr_isr();

/*_* GLOBAL DECLERATIONS GO HERE */
typedef enum {TEM, CDM, TSM} game_state_t;
game_state_t game_state = TEM;

uint8_t nOfCustom;      // Number of custom characters
uint8_t sevenSeg3WayCounter;
uint8_t re0Pressed, re1Pressed, re2Pressed, re3Pressed, re4Pressed, re5Pressed;        // flags for input

char predefined[] = {' ', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
uint8_t currPred;

char pred_next()
{
    currPred = (currPred+1)%37;
    return predefined[currPred];
}

char pred_prev()
{
    currPred = (currPred-1)%37;
    return predefined[currPred];
}

/*_* Interrupt Service Routines */
void __interrupt(high_priority) highPriorityISR(void)
{
    if (INTCONbits.TMR0IF)
    {
        tmr_isr();
    }
}

void tmr_isr()
{
    // We increase all sevenSegWayCounter variables and take modulo operation for all of them.
    // Using modulo operation caused some problems on our board, so we used simple if else.

    if (sevenSeg3WayCounter == 2)
        sevenSeg3WayCounter = 0;
    else
        sevenSeg3WayCounter++;

    INTCONbits.TMR0IF = 0; // Reset flag
}

void sevenSeg(uint8_t J, uint8_t D)
{
    // This function changes the 7 segment display to the desired value.
    switch (J)
    {

    // All dps are reset (i.e., bit7 -> 0)
    case 0:           // Also case O
        PORTJ = 0x3f; // abcdef    -> 1111 1100
        break;
    case 1:
        PORTJ = 0x06; // bc         -> 0110 0000
        break;
    case 2:
        PORTJ = 0x5b;
        break;
    case 3:
        PORTJ = 0x4f;
        break;
    case 4:
        PORTJ = 0x66;
        break;
    case 5:
        PORTJ = 0x6d;
        break;
    case 6:
        PORTJ = 0x7d;
        break;
    case 7:
        PORTJ = 0x07;
        break;
    case 8:
        PORTJ = 0x7f;
        break;
    case 9:
        PORTJ = 0x6f;
        break;
    case 10: // L
        PORTJ = 0x38;
        break;
    case 11: // E
        PORTJ = 0x79;
        break;
    case 12: // n
        PORTJ = 0x54;
        break;
    case 13: // d
        PORTJ = 0x5e;
        break;
    }
    switch (D)
    {
    case 0:
        PORTH = 0x01; // RH0 = 1, others = 0
        break;
    case 1:
        PORTH = 0x02; // RH1 = 1, others = 0
        break;
    case 2:
        PORTH = 0x04; // RH2 = 1, others = 0
        break;
    case 3:
        PORTH = 0x08; // RH3 = 1, others = 0
        break;
    }
}

/*_* Initialize variables */
void init_vars()
{
    re0Pressed = re1Pressed = re2Pressed = re3Pressed = re4Pressed = re5Pressed = false;
    game_state = TEM;
    nOfCustom = 0;
    sevenSeg3WayCounter = 0;
    currPred = 0;
}

void init_ports()
{
    /*_* INPUT TRISSES*/
    TRISE = 0x3f;       // 0011 1111 -> RE 0-5
    TRISH = 0X10;       // 0001 0000 -> RH4

    /*_* OUTPUT TRISSES*/
    // LCD BASED TRISSES
    // TRISB = TODO
    // TRISD = TODO 
    TRISA = 0X00;
    TRISC = 0X00;

    // 7-SEG BASED TRISSES
    // PORTH IS EDITED UPWARDS
    PORTJ = 0X00;

}

void init_irq()
{
    // INTCON is configured to use TMR0 interrupts
    INTCON = 0xa0;
}

void tmr_init()
{
    // In order to achieve a 500ms-400ms-300ms delay, we will use Timer0 in 8-bit mode.
    // This setup assumes a 40MHz 18F8722, which corresponds to a 10MHz
    // instruction cycle
    T0CON = 0xc7; // internal clock with 1:256 prescaler and 8-bit MAYBE 16-bit
    TMR0 = 0x00;  // Initialize TMR0 to 0, without a PRELOAD
}

void sev_seg_task()
{

}

void input_task()
{
    if(PORTEbits.RE0)
    {
        re0Pressed = true;
    }
    if(PORTEbits.RE1)
    {
        re1Pressed = true;
    }
    if(PORTEbits.RE2)
    {
        re2Pressed = true;
    }
    if(PORTEbits.RE3)
    {
        re3Pressed = true;
    }
    if(PORTEbits.RE4)
    {
        re4Pressed = true;
    }
    if(PORTEbits.RE5)
    {
        re5Pressed = true;
    }
    
}

void game_task()
{
    switch (game_state)
    {
    case TEM:
        break;

    case CDM:
        break;

    case TSM:
        break;
    
    default:
        break;
    }
}

void lcd_task()
{
    
}

int main()
{
    init_vars();
    init_ports();
    init_irq();
    tmr_init();

    while(1)
    {
        sev_seg_task();
        input_task();
        game_task();
        lcd_task();

    }
    return 0;
}