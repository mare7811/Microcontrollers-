#include <plib.h>

#include "CerebotMX7cK.h"
#include "LCDlib.h"

void LCD_init(void)
{
   
    
	LCD_Delay(50);
	PMPSetAddress(0);
	PMPMasterWrite(0x38);
	LCD_Delay(50);
	PMPSetAddress(0);
	PMPMasterWrite(0x0f);
	LCD_Delay(50);
	PMPSetAddress(0);
	PMPMasterWrite(0x01);
	LCD_Delay(5);
}

void writeLCD(int addr, char c)
{
	while(busyLCD());
	PMPSetAddress(addr);
	PMPMasterWrite(c);
}

char readLCD(int addr)
{
	PMPSetAddress(addr);
	mPMPMasterReadByte();
	return mPMPMasterReadByte();
}

void LCD_puts(char *char_string)
{
	while(*char_string)
	{
		LCD_putc(*char_string);
		char_string++;
	}
}

void LCD_putc(char character)
{
	int addr;
	while(busyLCD());
	addr = (readLCD(0) & 0x7f);

	switch(character)
   	{
       	case '\n':
			if((addr >= 0) && (addr < 0x40))
				writeLCD(0,0xC0);
			else
				writeLCD(0,0x80);			
			break;
       	case '\r':
			if((addr >= 0) && (addr < 0x40))
				writeLCD(0,0x80);
       	   	else
				writeLCD(0,0xC0);
			break;
        case '\t':
            writeLCD(0,0x80);
            break;
        case '\b':
            writeLCD(0,0xC0);
            break;            
		default:
			if((addr > 0x10) && (addr < 0x3F))
				writeLCD(0,0xC0);
			if(addr > 0x4f)
				writeLCD(0,0x80);
			writeLCD(1,character);
			break;
	}	
}

int busyLCD(void)
{
	int busyflag;
	busyflag = (readLCD(0) & 0x80);
	return busyflag;
}

void LCD_Delay(unsigned int mS)
{
	unsigned int tWait, tStart;
    tStart = ReadCoreTimer();
    tWait = (CORE_MS_TICK_RATE * mS);
    while((ReadCoreTimer() - tStart) < tWait);
    LATBINV = LEDA;
}