void LCD_init(void);
void WriteLCD(int addr, char c);
char readLCD(int addr);
void LCD_puts(char *char_string);
void LCD_putc(char character);
int busyLCD(void);
void LCD_Delay(unsigned int mS);