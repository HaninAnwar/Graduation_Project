#include <stdio.h>


#include "PWM.h"
#include "servo.h"
#include "MAPPING.h"


void Servo_void_SetAngle(SERVO_t Copy_ServoMotor,uint32_t Copy_int32_Angle)
{
	uint32_t Local_int32_MappedValue;

	Local_int32_MappedValue = Mapping(SERVO_MIN_ANGLE, SERVO_MAX_ANGLE, SERVO_MIN_INPUT, SERVO_MAX_INPUT, SERVO_MIN_INPUT,Copy_int32_Angle);

	switch(Copy_ServoMotor.Pin)
	{
	case Servo_PA15 : Copy_ServoMotor.htim = htim2;  Copy_ServoMotor.Channel = PWM_Channel1;break;
	case Servo_PB2  : Copy_ServoMotor.htim = htim2;  Copy_ServoMotor.Channel = PWM_Channel4;break;
	case Servo_PB9  : Copy_ServoMotor.htim = htim2;  Copy_ServoMotor.Channel = PWM_Channel2;break;
	case Servo_PB10 : Copy_ServoMotor.htim = htim2;  Copy_ServoMotor.Channel = PWM_Channel3;break;
	case Servo_PB14 : Copy_ServoMotor.htim = htim12; Copy_ServoMotor.Channel = PWM_Channel1;break;

	}
	PWM_int32SetPWMValue(Copy_ServoMotor.htim.Instance, Copy_ServoMotor.Channel, Local_int32_MappedValue);

}

