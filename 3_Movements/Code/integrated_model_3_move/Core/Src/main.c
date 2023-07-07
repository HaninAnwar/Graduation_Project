/* USER CODE BEGIN Header */
/**
 **************************
 * @file           : main.c
 * @brief          : Main program body
 **************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 **************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "crc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "PWM.h"
#include "MAPPING.h"
#include "servo.h"
#include "FEAT_EXT.h"
#include "NORM_interface.h"

#include "sensor_readings.h"
#include "app_x-cube-ai.h"

#include <stdio.h>

#include "network.h"
#include "network_data.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum
{
	RMS_IDX,
	VAR_IDX,
	MEAN_IDX,
	SSC_INDX,
	ZC_IDX,
	WVL_IDX
}Model_Idx;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define Vth 				40

#define MODEL_THRESHOLD		0.5

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint32_t sensor1;
uint32_t sensor2;
uint32_t sensor3;
uint32_t sensor;
uint32_t Fest,Gun,Rest;

int32_t Mode = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM10)
	{
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
	}
	else if (htim->Instance == TIM11)
	{
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
	}

	else if (htim->Instance == TIM13)
	{
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
	}
	else if (htim->Instance == TIM14)
	{
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
	}
}

/********	Global Variables	********/
static ai_handle network = AI_HANDLE_NULL;

AI_ALIGNED(32)
static ai_u8 activations[AI_NETWORK_DATA_ACTIVATIONS_SIZE];

AI_ALIGNED(32)
static ai_float in_data[AI_NETWORK_IN_1_SIZE];

AI_ALIGNED(32)
static ai_float out_data[AI_NETWORK_OUT_1_SIZE];

static ai_buffer *ai_input;
static ai_buffer *ai_output;

int Model_Init(void)
{
	ai_error err;
	const ai_handle acts[] = { activations };
	err = ai_network_create_and_init(&network, acts, NULL);

	ai_input = ai_network_inputs_get(network, NULL);
	ai_output = ai_network_outputs_get(network, NULL);

	return 0;
}

int Model_Run(const void *in_data, void *out_data)
{
	ai_i32 n_batch;
	ai_error err;

	/* 1 - Update IO handlers with the data payload */
	ai_input[0].data = AI_HANDLE_PTR(in_data);
	ai_output[0].data = AI_HANDLE_PTR(out_data);

	/* 2 - Perform the inference */
	n_batch = ai_network_run(network, &ai_input[0], &ai_output[0]);
	if (n_batch != 1) {
		err = ai_network_get_error(network);
	};

	return 0;
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	uint32_t Local_u32Counter,Local_u32_RealData[250],Local_u32AvgCounter;

	uint8_t Local_u8Avg = 5;
	uint8_t Local_u8Max,Local_u8ModeArr[Local_u8Avg],Local_u8Sum=0;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_ADC3_Init();
  MX_CRC_Init();
  MX_TIM2_Init();
  MX_TIM10_Init();
  MX_TIM11_Init();
  MX_TIM12_Init();
  MX_TIM13_Init();
  MX_TIM14_Init();
  /* USER CODE BEGIN 2 */
	Model_Init();

	/* Start GPs Timers */
	HAL_TIM_Base_Start_IT(&htim10);
	HAL_TIM_Base_Start_IT(&htim11);
	HAL_TIM_Base_Start_IT(&htim13);
	HAL_TIM_Base_Start_IT(&htim14);


	/* Initialize PWM Timers*/
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);
	HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_1);

	SERVO_t Servo1,Servo2,Servo3,Servo4,Servo5;
	Servo1.Pin = Servo_PA15;
	Servo2.Pin = Servo_PB10;
	Servo3.Pin = Servo_PB14;
	Servo4.Pin = Servo_PB2;
	Servo5.Pin = Servo_PB9;

	/*********	Testing	*********/
	uint32_t Move2[250] = {0};
	uint32_t Move1[250] = {
			664,  167,   13,   82,  340,  865,  469,  663,  144,  316,   15,
			356,  513,  560,  258,  108,  409,  456,  110,  383,  240,  227,
			314,  866, 1667,  156,  419,  760,   32,  441,  432,  307,  294,
			169,  234,  436,  703,  391,   82,  345,  100,  131,   58,  362,
			222,  323,   47,   41,   40,  273,  141,  405,  805,  471,  468,
			11,  127,  344,  178,  107,    5,  329,   80,   46,  190,    5,
			411,   64,   65,  180,  133,  367,  590,  196,   61,   11,  170,
			144,  454,  460,  188,  431,  112,  375,  208,  198,  139,    2,
			131,   52,   39,  155,   38,  137,  289,  535,  144,  305,  387,
			163, 1396,   43,  178,  219,  563,  268,  846,   62,  224,   37,
			318,  126,  535,  126,  221,  123,   41,    4,  100,  156,  514,
			145,  116,  159,   29,  171,   39,   66,  182,  332,   63,   85,
			7,  112,   58,   81,  130,  192,  198,  609,   57,  108,   15,
			106,  111,   92,   55,  100,  207,   88,  228,  254,   98,  250,
			63,    4,   80,  271,  228,   11,    1,   71,   95,   15,  656,
			275,  164,  150,  729,   85,   54,   71,   44,   34,   93,  220,
			86,  309,  428,   39,  285,  440,   86,  203,  254,   79,  156,
			12,  582,  275,  235,  481,   99,  190,    2,   93,  176,   96,
			250,  333,  269,   94,  346,  757,   43,  475,  179,  474,  198,
			227,  353,  159,   27,  233,   95,  407,  248,  152,  249,   68,
			276,   79,    5,  217,   95,   29,    8,  104,   20,  156,  189,
			146,   93,  125,   96,    5,  160,   51, 1495, 1831,   71,  599,
			181,  250,  219,  246,   47,  213,   23,  225
	};

	uint32_t Move0[250] = {
			4095, 3049, 1888, 3524, 2946, 3305,  531,  212, 1999,   56,  773,
			1270, 1214, 1460,   50,  267,  630,   17, 4095,  881, 1611, 2029,
			1503,  836,  640, 1385,  467,  376, 2873, 1013,  869, 3241,  454,
			344,  726,  139,  907, 3295, 1060,  527,  734, 1853,   52, 1474,
			616, 3453, 1117, 1163, 2207,  707, 1095,  292,  604,  962,  904,
			643,  649,  282,  175,  345,  964,  170,  788, 1128,   34,   88,
			141, 2421,  785,   73, 1578,  584, 1901, 1783, 4095,  718,  109,
			1793, 1538,  460, 4095,  546, 2150,  296, 1203,  199,   52, 1107,
			1417,  929,    2,  211,  254,  549,  961,  823, 3853,  288,  191,
			1833,  262, 1810, 1041,   23,   12, 3060, 1461, 1666,  761,   35,
			432,  208, 1128,  620,  545,  251,  745, 1076,  516,  417,  129,
			1801, 1991,  234,  248,  100, 1589,  121,   25,  341,  525,   95,
			120,    3,  202, 1146, 1117,   99, 1322,  228,  789, 1373, 1299,
			156,   69,   96, 1208,  303,  949,  673,  181,  839, 1386,  414,
			68, 1749,   86,  299,  136,  690,  178, 1502,  524,  506, 1095,
			2389,  133, 1176, 1975, 3978, 2293,  400, 1465,  609, 2374,  736,
			85,   32,  229, 4067, 1148,  256, 4095, 1698,  264,   97, 3905,
			701,  367,  373, 1919, 1249,  717,  680, 1513,  695,  404, 1988,
			625,    4,  245,  526,  509, 2131,  149,  710, 2110, 1908, 2841,
			279,  240,  661,  126,  565,  741,  442,   97, 1925, 2086, 4095,
			543,  641,  755,  385, 1141,   90,  132,   96, 1137, 1127,  696,
			328,  510,  628,   87,  684,  498, 2007,   72, 1383,  149,  681,
			1653, 1915, 1364,  662,  554,  248,  919,   71
	};
	float Feat_Arr[6],Norm_Arr[6];

	/*Feature Extraction*/
	Feat_Arr[VAR_IDX]	= FEAT_EXT_f32_Variance(Move2,250);
	Feat_Arr[RMS_IDX]   = FEAT_EXT_f32_RMS(Feat_Arr[VAR_IDX]);
	Feat_Arr[MEAN_IDX]  = FEAT_EXT_f32_MeanAbsolute(Move2,250);
	Feat_Arr[SSC_INDX]  = FEAT_EXT_u32_SlopeSignChange(Move2,250);
	Feat_Arr[ZC_IDX]    = FEAT_EXT_u32_ZeroCrossing(Move2,250);
	Feat_Arr[WVL_IDX]   = FEAT_EXT_u32_WaveformLength(Move2,250);

	/*Normalization*/
	Norm_Arr[VAR_IDX]  = NORM_f32_Variance(SNS1, Feat_Arr[VAR_IDX]);
	Norm_Arr[RMS_IDX]  = NORM_f32_RMS(SNS1, Feat_Arr[RMS_IDX]);
	Norm_Arr[MEAN_IDX] = NORM_f32_Mean(SNS1, Feat_Arr[MEAN_IDX]);
	Norm_Arr[SSC_INDX] = NORM_f32_SlopeSignChange(SNS1,Feat_Arr[SSC_INDX]);
	Norm_Arr[ZC_IDX]   = NORM_f32_ZeroCrossing(SNS1,Feat_Arr[ZC_IDX]);
	Norm_Arr[WVL_IDX]  = NORM_f32_WaveformLength(SNS1, Feat_Arr[WVL_IDX]);

	/*Model Running*/

	/*Movement 1 Data*/
	in_data[RMS_IDX]  = Norm_Arr[RMS_IDX];
	in_data[VAR_IDX]  = Norm_Arr[VAR_IDX];
	in_data[MEAN_IDX] = Norm_Arr[MEAN_IDX];
	in_data[SSC_INDX] = Norm_Arr[SSC_INDX];
	in_data[ZC_IDX]   = Norm_Arr[ZC_IDX];
	in_data[WVL_IDX]  = Norm_Arr[WVL_IDX];

	Model_Run(in_data, out_data);

	Local_u8Max = 0;
	for(int i = 0 ;i < 3; i++)
	{
		if(out_data[i] > out_data[Local_u8Max])
		{
			Local_u8Max = i;
		}
	}

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1)
	{
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		//		for(Local_u32AvgCounter = 0 ; Local_u32AvgCounter < Local_u8Avg ; Local_u32AvgCounter++)
		//		{
		for(Local_u32Counter = 0 ;Local_u32Counter < 250;)
		{
			//			if(sensor != 0)
			//			{
			sensor = analogRead(&hadc1);
			Local_u32_RealData[Local_u32Counter++]= sensor;
			HAL_Delay(2);
			//			}
		}

		/*Feature Extraction*/
		Feat_Arr[VAR_IDX]	= FEAT_EXT_f32_Variance(Local_u32_RealData,250);
		Feat_Arr[RMS_IDX]   = FEAT_EXT_f32_RMS(Feat_Arr[VAR_IDX]);
		Feat_Arr[MEAN_IDX]  = FEAT_EXT_f32_MeanAbsolute(Local_u32_RealData,250);
		Feat_Arr[SSC_INDX]  = FEAT_EXT_u32_SlopeSignChange(Local_u32_RealData,250);
		Feat_Arr[ZC_IDX]    = FEAT_EXT_u32_ZeroCrossing(Local_u32_RealData,250);
		Feat_Arr[WVL_IDX]   = FEAT_EXT_u32_WaveformLength(Local_u32_RealData,250);

		/*Normalization*/
		Norm_Arr[VAR_IDX]  = NORM_f32_Variance(SNS1, Feat_Arr[VAR_IDX]);
		Norm_Arr[RMS_IDX]  = NORM_f32_RMS(SNS1, Feat_Arr[RMS_IDX]);
		Norm_Arr[MEAN_IDX] = NORM_f32_Mean(SNS1, Feat_Arr[MEAN_IDX]);
		Norm_Arr[SSC_INDX] = NORM_f32_SlopeSignChange(SNS1,Feat_Arr[SSC_INDX]);
		Norm_Arr[ZC_IDX]   = NORM_f32_ZeroCrossing(SNS1,Feat_Arr[ZC_IDX]);
		Norm_Arr[WVL_IDX]  = NORM_f32_WaveformLength(SNS1, Feat_Arr[WVL_IDX]);

		/*Model Running*/

		/*Movement 1 Data*/
		in_data[RMS_IDX]  = Norm_Arr[RMS_IDX];
		in_data[VAR_IDX]  = Norm_Arr[VAR_IDX];
		in_data[MEAN_IDX] = Norm_Arr[MEAN_IDX];
		in_data[SSC_INDX] = Norm_Arr[SSC_INDX];
		in_data[ZC_IDX]   = Norm_Arr[ZC_IDX];
		in_data[WVL_IDX]  = Norm_Arr[WVL_IDX];

		Model_Run(in_data, out_data);
		Local_u8Max = 0;
		for(int i = 0 ;i < 3; i++)
		{
			if(out_data[i] > out_data[Local_u8Max])
			{
				Local_u8Max = i;
			}
		}
		if(Local_u8Max == 0)
		{
			Servo_void_SetAngle(Servo1,100);
			Servo_void_SetAngle(Servo2,120);
			Servo_void_SetAngle(Servo3,170);
			Servo_void_SetAngle(Servo4,20);
			Servo_void_SetAngle(Servo5,120);

			Fest = 100;
			Gun = 0;
			Rest = 0;
		}
		else if(Local_u8Max == 1)
		{
			Servo_void_SetAngle(Servo1,170);
			Servo_void_SetAngle(Servo2,40);
			Servo_void_SetAngle(Servo3,190);
			Servo_void_SetAngle(Servo4,10);
			Servo_void_SetAngle(Servo5,140);

			Fest = 0;
			Gun = 100;
			Rest = 0;
		}
		else
		{
			Servo_void_SetAngle(Servo1,150);
			Servo_void_SetAngle(Servo2,60);
			Servo_void_SetAngle(Servo3,80);
			Servo_void_SetAngle(Servo4,80);
			Servo_void_SetAngle(Servo5,70);
			Fest = 0;
			Gun = 0;
			Rest = 100;
		}

		//			Local_u8ModeArr[Local_u32AvgCounter] = Local_u8Max;
		//			Local_u8Sum +=Local_u8Max;

		//			if(Local_u8Max == 0)
		//			{
		//				Mode +=3;
		//			}
		//			else if(Local_u8Max == 1)
		//			{
		//				Mode+=5;
		//			}
		//			else
		//			{
		//				Mode+=9;
		//			}
		//		}

		//		if( Mode == 15)
		//		{
		//			if(Local_u8Sum == 2)
		//			{
		//				Fest = 100;
		//				Gun = 0;
		//				Rest = 0;
		//			}
		//		}
		//		else if((Mode >= 13) && (Mode <= 19))
		//		{
		//			Fest = 0;
		//			Gun = 100;
		//			Rest = 0;

		//		}
		//		else
		//		{
		//			Fest = 0;
		//			Gun = 0;
		//			Rest = 100;
		//		}
		HAL_Delay(500);
		Mode = 0;
		Local_u8Sum = 0;

	}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 90;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1)
	{
	}
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
