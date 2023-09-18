/***********************************************************************/
/***********************************************************************/
/************************ AUTHER : Hanin Anwar  ************************/
/************************ LAYER  : LIB          ************************/
/************************ VERSION : 1.00         ************************/
/***********************************************************************/
/***********************************************************************/

#include <stdint.h>
#include "MAPPING.h"

int32_t Mapping(int32_t Copy_int32_InputMin,int32_t Copy_int32_InputMax,int32_t Copy_int32_OutputMin,int32_t Copy_int32_OutputMax,int32_t Copy_int32BiasValue,int32_t Copy_int32_InputValue)
{
	int32_t Local_int32_MappedValue;
	Local_int32_MappedValue = (((Copy_int32_InputValue - Copy_int32_InputMin)*(Copy_int32_OutputMax - Copy_int32_OutputMin))/(Copy_int32_InputMax - Copy_int32_InputMin)) + Copy_int32_InputMin + Copy_int32BiasValue;

	return Local_int32_MappedValue;
}
