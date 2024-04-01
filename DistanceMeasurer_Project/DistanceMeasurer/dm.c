/*
 * dm.c
 *
 *  Created on: ٢٩‏/٠٢‏/٢٠٢٤
 *  Author: AS.Mahrous
 */
#include <avr/io.h>
#include "LCD/lcd.h"
#include "Ultrasonic/ultrasonic.h"

int main(void)
{
	uint16 distance = 0;

	/* enabling global interrupt I-Bit */
	SREG |= (1<<7);

	LCD_init();
	Ultrasonic_init();

	LCD_displayStringRowColumn(1,0,"Distance =");
	for(;;)
	{
		distance = Ultrasonic_readDistance();

		LCD_moveCursor(1,11);
		if(distance >= 100)
		{
			LCD_intgerToString(distance);
			LCD_displayString("cm");
		}
		else
		{
			LCD_intgerToString(distance);
			LCD_displayCharacter(' ');
			LCD_displayString("cm");
		}
	}
}
