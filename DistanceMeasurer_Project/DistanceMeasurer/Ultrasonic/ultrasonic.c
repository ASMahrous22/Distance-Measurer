/*
 * ultrasonic.c
 *
 *  Created on: ٠٢‏/٠٣‏/٢٠٢٤
 *      Author: AS.Mahrous
 */

#include "ultrasonic.h"
#include "../ICU/icu.h"
#include "util/delay.h"

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/
uint8 g_edgeCount = 0;
uint16 g_highTime = 0;

ICU_ConfigType ICU_settings = {F_CPU_8,RISING};
/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

void Ultrasonic_init(void)
{
	/* initializing the ICU configurations */
	ICU_init(&ICU_settings);

	/* setting the trigger pin as output pin */
	GPIO_setupPinDirection(TRIG_PORT,TRIG_PIN,PIN_OUTPUT);

	/* setting ICU call back function */
	ICU_setCallBack(Ultrasonic_edgeProcessing);
}

void Ultrasonic_edgeProcessing(void)
{
	g_edgeCount++;
	if(g_edgeCount == 1)
	{
		ICU_clearTimerValue();
		ICU_setEdgeDetectionType(FALLING);
	}
	if(g_edgeCount == 2)
	{
		g_highTime = ICU_getInputCaptureValue();
		ICU_setEdgeDetectionType(RISING);

		g_edgeCount = 0;
	}
}

void Ultrasonic_Trigger(void)
{
	/* sending trigger pulse to the ultrasonic sensor */
	GPIO_writePin(TRIG_PORT,TRIG_PIN,LOGIC_HIGH);
	_delay_us(10);
	GPIO_writePin(TRIG_PORT,TRIG_PIN,LOGIC_LOW);
}

uint16 Ultrasonic_readDistance(void)
{
	uint16 ultrasonicDistance = 0;
	/* Send trigger pulse */
	Ultrasonic_Trigger();

	/* calculating the distance = (pulse_duration "high time" * SPEED_OF_SOUND)* (10^-6) / (2)
	 * division by 2 so we get one-way distance
	 * */
	ultrasonicDistance = ((g_highTime * SPEED_OF_SOUND * TICK_TIME)/2);

	return ultrasonicDistance;
}
