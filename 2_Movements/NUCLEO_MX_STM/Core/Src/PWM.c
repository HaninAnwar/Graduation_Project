#include <stdint.h>
#include "PWM.h"


int8_t PWM_int32SetPWMValue(TIM_TypeDef *htim,int32_t Copy_int32Channel,int32_t Copy_int32Value)
{
	int8_t Local_int8ErrorState = 0;

	if (htim == TIM2)
	{
		switch(Copy_int32Channel)
		{
		case PWM_Channel1 : TIM2->CCR1 = Copy_int32Value; break;
		case PWM_Channel2 : TIM2->CCR2 = Copy_int32Value; break;
		case PWM_Channel3 : TIM2->CCR3 = Copy_int32Value; break;
		case PWM_Channel4 : TIM2->CCR4 = Copy_int32Value; break;
		default : Local_int8ErrorState = 1;
		}
	}

	else if (htim == TIM12)
	{
		switch(Copy_int32Channel)
		{
		case PWM_Channel1 : TIM12->CCR1 = Copy_int32Value; break;
		case PWM_Channel2 : TIM12->CCR2 = Copy_int32Value; break;
		default : Local_int8ErrorState = 1;
		}
	}

	return Local_int8ErrorState;
}



