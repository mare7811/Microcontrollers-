#include <plib.h>
#include "CerebotMx7cK.h"
#include "LCDlib.h"
#include "project10.h"
#include "pwm.h"
#include "input_capture.h"

float RPS;

void __ISR(_TIMER_3_VECTOR, IPL2) Timer3Handler(void)
{ 
    LATBINV = LEDC;   
    mT3ClearIntFlag();
}

void input_capture_init(void)
{
	PORTSetPinsDigitalIn(IOPORT_D,(BIT_3|BIT_12));
	mIC5ClearIntFlag();
	
    OpenCapture5(IC_ON|IC_CAP_16BIT|IC_IDLE_STOP|IC_FEDGE_FALL|IC_TIMER3_SRC|IC_INT_1CAPTURE|IC_EVERY_FALL_EDGE);
	
    ConfigIntCapture5(IC_INT_ON|IC_INT_PRIOR_3|IC_INT_SUB_PRIOR_0);	
}

void __ISR(_INPUT_CAPTURE_5_VECTOR, IPL3) Capture5(void)
{
	static unsigned int con_buff[4];
	static unsigned short int t_new;
	static unsigned short int t_old = 0;
	static unsigned short int time_diff;
	
	LATBINV = LEDD;
	ReadCapture5(con_buff);
	t_new = con_buff[0];
	time_diff = t_new - t_old;
	t_old = t_new;
	
    static float avg, sum = 0;
    static int i = 0;
    static unsigned int circ_buff[16];
    
    circ_buff[i++] = time_diff;
    int j;
    sum = 0;
    for(j=0; j<16; j++)
    {
       sum += circ_buff[j]; 
    }
    
    avg = sum/16;
    RPS = 10e6/(avg*256);
	
    if(i == 16)
        i = 0;
	mIC5ClearIntFlag();
}