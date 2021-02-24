#define T2_PRESCALE 1
#define TOGGLES_PER_SEC 1000
#define T2_TICK (FPB/T2_PRESCALE/TOGGLES_PER_SEC)

void system_init(void);
void decode_buttons(unsigned int buttons);
void hw_msDelay(unsigned int debounce);

