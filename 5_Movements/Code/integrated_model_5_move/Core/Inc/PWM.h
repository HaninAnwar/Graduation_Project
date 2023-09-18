#ifndef PWM_H_
#define PWM_H_

#include "tim.h"

typedef enum
{
	PWM_Channel1,
	PWM_Channel2,
	PWM_Channel3,
	PWM_Channel4

}PWM_Channel;

int8_t PWM_int32SetPWMValue(TIM_TypeDef *htim,int32_t Copy_int32Channel,int32_t Copy_int32Value);
#endif
