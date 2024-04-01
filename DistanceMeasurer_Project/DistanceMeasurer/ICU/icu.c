/*
 * icu.c
 *
 *  Created on: ٠٢‏/٠٣‏/٢٠٢٤
 *  Author: AS.Mahrous
 */

#include "../common_macros.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include "icu.h"

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_callBackPtr)(void) = NULL_PTR;

/*******************************************************************************
 *                                   ISRs                                      *
 *******************************************************************************/

ISR(TIMER1_CAPT_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		//Ultrasonic_edgeProcessing();
		/* another method to call the function using pointer to function g_callBackPtr();*/
		(*g_callBackPtr)();
	}
}

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

void ICU_init(const ICU_ConfigType *Config_Ptr)
{
	/* setting PD6 as input pin */
	CLEAR_BIT(DDRD,PD6);

	/* configure the ICU :
	* 1. Non PWM mode FOC1A=1 & FOC1B=1
	* 2. Normal Mode COM1A1=0 & COM1B1=0
	* 3. Setting RISING as the starting edge, ICES1=1
	* 4. Setting Prescaler
	* 5. Putting initial values, TCNT1=0 & ICR1=0
	* 6. Enabling ICU interrupt enable
	*/

	/* selecting mode */
	TCCR1A = (1<<FOC1A) | (1<<FOC1B);

	/* selecting edge */
	TCCR1B = (TCCR1B & 0xBF) | ((Config_Ptr -> edge) << ICES1);

	/* another method:
	 * switch(Config_Ptr -> edge)
	{
	case FALLING:
		CLEAR_BIT(TCCR1B,ICES1);
		break;

	case RISING:
		SET_BIT(TCCR1B,ICES1);
		break;
	}*/

	/* selecting prescaler */
	TCCR1B = (TCCR1B & 0xF8) | (Config_Ptr -> clock);

	/* another method:
	 * switch(Config_Ptr -> clock)
	{
	case NO_CLOCK:
		CLEAR_BIT(TCCR1B,CS10);
		CLEAR_BIT(TCCR1B,CS11);
		CLEAR_BIT(TCCR1B,CS12);
		break;

	case F_CPU_CLOCK:
		SET_BIT(TCCR1B,CS10);
		CLEAR_BIT(TCCR1B,CS11);
		CLEAR_BIT(TCCR1B,CS12);
		break;

	case F_CPU_8:
		CLEAR_BIT(TCCR1B,CS10);
		SET_BIT(TCCR1B,CS11);
		CLEAR_BIT(TCCR1B,CS12);
		break;

	case F_CPU_64:
		SET_BIT(TCCR1B,CS10);
		SET_BIT(TCCR1B,CS11);
		CLEAR_BIT(TCCR1B,CS12);
		break;

	case F_CPU_256:
		CLEAR_BIT(TCCR1B,CS10);
		CLEAR_BIT(TCCR1B,CS11);
		SET_BIT(TCCR1B,CS12);
		break;

	case F_CPU_1024:
		SET_BIT(TCCR1B,CS10);
		CLEAR_BIT(TCCR1B,CS11);
		SET_BIT(TCCR1B,CS12);
		break;
	}*/

	/* setting initial values */
	TCNT1 = 0;
	ICR1 = 0;

	/* enabling ICU interrupt */
	SET_BIT(TIMSK,TICIE1);
}

void ICU_deInit(void)
{
	/* clearing the timer registers */
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1 = 0;
	ICR1 = 0;

	/* disable ICU interrupt */
	CLEAR_BIT(TIMSK,TICIE1);

	/* Reset the global pointer value */
	g_callBackPtr = NULL_PTR;
}

void ICU_clearTimerValue(void)
{
	TCNT1 = 0;
}

uint16 ICU_getInputCaptureValue(void)
{
	return ICR1;
}

void ICU_setCallBack(void(*a_ptr)(void))
{
	g_callBackPtr = a_ptr;
}

void ICU_setEdgeDetectionType(const ICU_EdgeType a_edgeType)
{
	TCCR1B = (TCCR1B & 0xBF) | (a_edgeType << ICES1);

	/*  another method:
	 * switch(a_edgeType)
	{
	case FALLING:
		CLEAR_BIT(TCCR1B,ICES1);
		break;

	case RISING:
		SET_BIT(TCCR1B,ICES1);
		break;
	}*/
}

