#include <plib.h>
#include "CerebotMx7cK.h"
#include "project10.h"

int pwm_init(int dutyCycle, int cycleFrequency)
{
    int PWM_CYCLE_COUNT, OC3;
    PWM_CYCLE_COUNT = FPB/(T2_PRESCALE*cycleFrequency);
    OC3 = (dutyCycle*(T2_TICK)/100);
    mOC3ClearIntFlag();
    
    OpenTimer2(T2_ON | T2_SOURCE_INT | T2_PS_1_1, (T2_TICK-1));
    mT2SetIntPriority(2);
    mT2SetIntSubPriority(1);
    mT2IntEnable(1);
    
    OpenOC3(OC_ON | OC_TIMER_MODE16 | OC_TIMER2_SRC | OC_PWM_FAULT_PIN_DISABLE, 
           OC3, OC3);
        
    return 0;
}

int pwm_set(int dutyCycle)
{
    SetDCOC3PWM(dutyCycle*(T2_TICK)/100);
    return 0;
}

void __ISR(_TIMER_2_VECTOR, IPL2) Timer2Handler(void)
{ 
    LATBINV = LEDA;   
    mT2ClearIntFlag();
}