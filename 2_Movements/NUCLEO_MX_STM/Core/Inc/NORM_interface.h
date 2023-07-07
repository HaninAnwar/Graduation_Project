#ifndef NORM_INTERFACE_H_
#define NORM_INTERFACE_H_

typedef enum
{
	SNS1,
	SNS2,
	SNS3

}Sensor_Num_T;

float NORM_f32_SlopeSignChange(uint8_t Copy_u8SensorNum , uint32_t Copy_u32SSC);

float NORM_f32_ZeroCrossing(uint8_t Copy_u8SensorNum , uint32_t Copy_u32zeroCross);

float NORM_f32_WaveformLength(uint8_t Copy_u8SensorNum , uint32_t Copy_u3WaveLength);

float NORM_f32_RMS(uint8_t Copy_u8SensorNum , float Copy_f32RMS);

float NORM_f32_Variance(uint8_t Copy_u8SensorNum , float Copy_f32Variance);

float NORM_f32_Mean(uint8_t Copy_u8SensorNum , float Copy_f32Mean);


#endif
