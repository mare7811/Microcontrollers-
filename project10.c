#include <plib.h>
#include "CerebotMx7cK.h"
#include "LCDlib.h"
#include "project10.h"
#include "pwm.h"

extern float RPS;

int main(void)
{
    system_init();
  
    char duty_str[] = "PWM = 40%";
    LCD_putc('\t');
    LCD_puts(duty_str);
    while(1)
	{
        hw_msDelay(100);
        char RPS_str[] = "RPS = ";
        char result[6];
        sprintf(result, "%.2f", RPS);
        
        mCNIntEnable(FALSE);
        LCD_putc('\b');
        LCD_puts(RPS_str);
        LCD_puts(result);
        mCNIntEnable(TRUE);
	}
}

void system_init(void)
{
    Cerebot_mx7cK_setup();
    int cfg1 = PMP_ON|PMP_READ_WRITE_EN|PMP_READ_POL_HI|PMP_WRITE_POL_HI;
	int cfg2 = PMP_DATA_BUS_8 | PMP_MODE_MASTER1 |PMP_WAIT_BEG_4 | PMP_WAIT_MID_15 | PMP_WAIT_END_4;
	int cfg3 = PMP_PEN_0;
	int cfg4 = PMP_INT_OFF;
	mPMPOpen(cfg1, cfg2, cfg3, cfg4);
    LCD_init();
    
    PORTSetPinsDigitalIn(IOPORT_G, (BTN1 | BTN2));
    PORTSetPinsDigitalOut(IOPORT_B, SM_LEDS);
    PORTSetPinsDigitalOut(IOPORT_D, BIT_1);
    LATBCLR = SM_LEDS;
    LATBCLR = BIT_1;
    
    int dutyCycle = 40;
    int cycleFrequency = 1000;
    pwm_init(dutyCycle, cycleFrequency);
       
    mCNOpen(CN_ON, (CN8_ENABLE | CN9_ENABLE), 0);
    mCNSetIntPriority(1);
    mCNSetIntSubPriority(0);
	unsigned int x;
    x = PORTReadBits(IOPORT_G, BTN1 | BTN2);
    mCNClearIntFlag();
    mCNIntEnable(1);
	
	OpenTimer3(T3_ON | T3_SOURCE_INT | T3_PS_1_256, 0xFFFF);
    mT3SetIntPriority(2);
    mT3SetIntSubPriority(2);
    mT3IntEnable(1);

	input_capture_init();

    INTEnableSystemMultiVectoredInt();
    INTEnableInterrupts();    
}

void __ISR(_CHANGE_NOTICE_VECTOR, IPL1) CNIntHandler(void)
{
    LATBINV = LEDB;
    unsigned int buttons;
    hw_msDelay(20);
    
    buttons = (PORTG & (BTN1 | BTN2));
    buttons = buttons >> 6;
    decode_buttons(buttons);
    
    LATBINV = LEDB;
    mCNClearIntFlag();
}

void decode_buttons(unsigned int buttons)
{  
    char duty_str1[] = "PWM = 40%";
    char duty_str2[] = "PWM = 65%";
    char duty_str3[] = "PWM = 80%";
    char duty_str4[] = "PWM = 95%";
    switch(buttons)
    {
        case 0:
            pwm_set(40);
            LCD_putc('\t');
            LCD_puts(duty_str1);
            break;
        case 1:
            pwm_set(65);
            LCD_putc('\t');
            LCD_puts(duty_str2);
            break;
        case 2:
            pwm_set(80);
            LCD_putc('\t');
            LCD_puts(duty_str3);
            break;
        case 3:
            pwm_set(95);
            LCD_putc('\t');
            LCD_puts(duty_str4);
            break;
    }
}

void hw_msDelay(unsigned int debounce)
{
    unsigned int tWait, tStart;
    tStart = ReadCoreTimer();
    tWait = (CORE_MS_TICK_RATE * debounce);
    while((ReadCoreTimer() - tStart) < tWait);
}

