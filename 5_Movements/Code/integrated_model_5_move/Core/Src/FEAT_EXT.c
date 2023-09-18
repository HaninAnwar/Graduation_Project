#include <stdint.h>
#include <float.h>

#include "FEAT_EXT.h"


uint32_t FEAT_EXT_u32_SlopeSignChange(uint32_t *Copy_int32Data ,uint32_t Copy_int32_Size)
{
	uint32_t Local_int32_NumberOfChanges = 0,
			Local_int32_Slope[Copy_int32_Size],
			Local_int32_Counter;

	/*Compute Slope*/
	Local_int32_Slope[0] = 0;

	for(Local_int32_Counter = 1 ; Local_int32_Counter < Copy_int32_Size ; Local_int32_Counter++)
	{

		if(Copy_int32Data[Local_int32_Counter]  > Copy_int32Data[Local_int32_Counter-1])
			Local_int32_Slope[Local_int32_Counter] = 1;

		else if(Copy_int32Data[Local_int32_Counter]  < Copy_int32Data[Local_int32_Counter-1])

			Local_int32_Slope[Local_int32_Counter] = -1;

		else if(Copy_int32Data[Local_int32_Counter] == Copy_int32Data[Local_int32_Counter-1])
			Local_int32_Slope[Local_int32_Counter] = Local_int32_Slope[Local_int32_Counter-1];

	}

	/*Compute Changes In Sign*/
	for(Local_int32_Counter = 0 ; Local_int32_Counter < Copy_int32_Size-1 ; Local_int32_Counter++)
	{
		if((Local_int32_Slope[Local_int32_Counter] != Local_int32_Slope[Local_int32_Counter+1]))
		{
			Local_int32_NumberOfChanges++;
		}
	}

	return Local_int32_NumberOfChanges;
}

uint32_t FEAT_EXT_u32_ZeroCrossing(uint32_t *Copy_u32Data ,uint32_t Copy_u32_Size)
{
	uint32_t Local_int32_Sign[Copy_u32_Size],
	Local_int32_Counter,
	Local_int32_ZeroCounter;

	/*Compute data sign*/
	for(Local_int32_Counter = 0 ; Local_int32_Counter < Copy_u32_Size ; Local_int32_Counter++)
	{
		if(Copy_u32Data[Local_int32_Counter] > 0)
		{
			Local_int32_Sign[Local_int32_Counter] = 1;
		}
		else if (Copy_u32Data[Local_int32_Counter] <= 0)
		{
			Local_int32_Sign[Local_int32_Counter] = -1;
		}
	}

	/*Compute zero crossing*/
	Local_int32_ZeroCounter = 0;
	for(Local_int32_Counter = 0 ; Local_int32_Counter < Copy_u32_Size-1 ; Local_int32_Counter++)
	{
		if(Local_int32_Sign[Local_int32_Counter] != Local_int32_Sign[Local_int32_Counter+1])
		{
			Local_int32_ZeroCounter++;
		}
	}


	return Local_int32_ZeroCounter;
}

uint32_t FEAT_EXT_u32_WaveformLength(uint32_t *Copy_u32Data ,uint32_t Copy_u32_Size)
{
	uint32_t Local_int32_AbsDiff[Copy_u32_Size-1],
	Local_int32_Counter,
	Local_int32_WaveLength;

	/*Calculate absolute difference of data*/
	for(Local_int32_Counter = 0 ; Local_int32_Counter < Copy_u32_Size-1 ; Local_int32_Counter++)
	{
		if(Copy_u32Data[Local_int32_Counter] > Copy_u32Data[Local_int32_Counter+1])
		{
			Local_int32_AbsDiff[Local_int32_Counter] = Copy_u32Data[Local_int32_Counter] -  Copy_u32Data[Local_int32_Counter+1];
		}
		else
		{
			Local_int32_AbsDiff[Local_int32_Counter] = Copy_u32Data[Local_int32_Counter+1] -  Copy_u32Data[Local_int32_Counter];
		}
	}

	/*Compute wave length*/
	Local_int32_WaveLength = 0;

	for(Local_int32_Counter = 0 ; Local_int32_Counter < Copy_u32_Size-1 ; Local_int32_Counter++)
	{
		Local_int32_WaveLength += Local_int32_AbsDiff[Local_int32_Counter];
	}


	return Local_int32_WaveLength;
}


//float FEAT_EXT_float_RMS(int32_t *Copy_int32Data ,int32_t Copy_int32_Size)
float FEAT_EXT_f32_RMS(float Copy_f32_Variance)
{
	float Local_f32_RMS;
	Local_f32_RMS = sqrtf(Copy_f32_Variance);

	return Local_f32_RMS;

}

float FEAT_EXT_f32_Variance(uint32_t *Copy_u32Data ,uint32_t Copy_u32_Size)
{

	uint32_t Local_int32_Counter;

	float 	Local_float_Mean = 0,
			Local_float_Var  = 0;

	/*Compute Mean*/
	Local_float_Mean = FEAT_EXT_f32_Mean(Copy_u32Data, Copy_u32_Size);

	/*Compute Variance*/
	for (Local_int32_Counter = 0 ; Local_int32_Counter < Copy_u32_Size ; Local_int32_Counter++)
	{
		Local_float_Var += pow(Copy_u32Data[Local_int32_Counter] - Local_float_Mean,2);
	}
	Local_float_Var /= (Copy_u32_Size-1);

	return Local_float_Var;
}

float FEAT_EXT_f32_Mean(uint32_t *Copy_u32Data ,uint32_t Copy_u32_Size)
{
	float Local_float_Mean = 0;
	uint32_t Local_int32_Counter;

	for(Local_int32_Counter = 0 ; Local_int32_Counter < Copy_u32_Size ; Local_int32_Counter++)
	{
		Local_float_Mean += Copy_u32Data[Local_int32_Counter];
	}

	Local_float_Mean /= (float)Copy_u32_Size;

	return Local_float_Mean;

}
float FEAT_EXT_f32_MeanAbsolute(uint32_t *Copy_u32Data ,uint32_t Copy_u32_Size)
{
	uint32_t Local_int32_Counter;
	float Local_float_Mean = 0;

	for(Local_int32_Counter = 0 ; Local_int32_Counter < Copy_u32_Size ; Local_int32_Counter++)
	{
		Local_float_Mean += abs(Copy_u32Data[Local_int32_Counter]);
	}

	Local_float_Mean /= Copy_u32_Size;

	return Local_float_Mean;

}

uint8_t FEAT_EXT_f32_AllFeat(uint32_t Copy_u32Data[][250] , float Copy_f32_FeatArr[][6])
{
	uint8_t Local_u8ErrorState = 0 ,Local_u8Counter;

	for(Local_u8Counter = 0 ; Local_u8Counter < 3 ; Local_u8Counter++)
	{
		Copy_f32_FeatArr[Local_u8Counter][VAR_IDX ] = FEAT_EXT_f32_Variance(Copy_u32Data[Local_u8Counter],DATA_SIZE);
		Copy_f32_FeatArr[Local_u8Counter][RMS_IDX ] = FEAT_EXT_f32_RMS(Copy_f32_FeatArr[Local_u8Counter][VAR_IDX]);
		Copy_f32_FeatArr[Local_u8Counter][MEAN_IDX] = FEAT_EXT_f32_Mean(Copy_u32Data[Local_u8Counter],DATA_SIZE);
		Copy_f32_FeatArr[Local_u8Counter][SSC_IDX] = FEAT_EXT_u32_SlopeSignChange(Copy_u32Data[Local_u8Counter],DATA_SIZE);
		Copy_f32_FeatArr[Local_u8Counter][ZC_IDX  ] = FEAT_EXT_u32_ZeroCrossing(Copy_u32Data[Local_u8Counter],DATA_SIZE) ;
		Copy_f32_FeatArr[Local_u8Counter][WVL_IDX ] = FEAT_EXT_u32_WaveformLength(Copy_u32Data[Local_u8Counter],DATA_SIZE);

	}
	return Local_u8ErrorState;
}

