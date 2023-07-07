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
uint32_t Fest,Gun;
float Local_f32Avg = 0;
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
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_ADC3_Init();
  MX_TIM2_Init();
  MX_TIM10_Init();
  MX_TIM11_Init();
  MX_TIM12_Init();
  MX_TIM13_Init();
  MX_TIM14_Init();
  MX_USART2_UART_Init();
  MX_CRC_Init();
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

	SERVO_t Servo1;
	Servo1.Pin = Servo_PB9;

	/*********	Testing	*********/
	uint32_t Data0[250] = {
			0   , 0  , 0  , 0  , 96 , 94 , 0  , 3  , 5   , 20  ,
			0   , 0  , 5  , 0  , 0  , 8  , 0  , 0  , 0   , 0   ,
			0   , 0  , 28 , 0  , 0  , 0  , 13 , 0  , 43  , 1023,
			0   , 0  , 0  , 0  , 0  , 0  , 224, 11 , 0   , 0   ,
			0   , 0  , 232, 411, 0  , 0  , 0  , 0  , 0   , 3   ,
			0   , 0  , 9  , 0  , 0  , 0  , 0  , 0  , 0   , 0   ,
			0   , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 1   , 0   ,
			447 , 343, 0  , 0  , 0  , 182, 27 , 24 , 577 , 0   ,
			0   , 8  , 488, 8  , 0  , 10 , 0  , 0  , 0   , 64  ,
			0   , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0   , 0   ,
			0   , 0  , 88 , 0  , 0  , 0  , 34 , 0  , 0   , 0   ,
			0   , 452, 0  , 769, 0  , 0  , 0  , 0  , 11  , 0   ,
			1023, 0  , 0  , 13 , 19 , 0  , 36 , 172, 0   , 1023,
			0   , 0  , 0  , 0  , 437, 0  , 0  , 0  , 0   , 0   ,
			99  , 183, 0  , 0  , 0  , 0  , 300, 0  , 0   , 0   ,
			0   , 0  , 0  , 0  ,32  , 0  , 0  , 0  , 0   , 156 ,
			139 , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 1023, 11  ,
			0   , 0  , 9  , 0  , 0  , 0  , 0  , 0  , 30  , 188 ,
			33  , 75 , 0  , 13 , 0  , 0  , 0  , 0  , 0   , 35  ,
			233 , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0   , 1   ,
			0   , 138, 0  , 0  , 0  , 0  , 0  , 0  , 0   , 0   ,
			0   , 606, 197, 76 , 0  , 104, 0  , 232, 24  , 0   ,
			1023, 132, 0  , 0  , 0  , 0  , 0  , 0  , 0   , 14  ,
			0   , 0  , 0  , 0  , 89 , 7  , 0  , 32 , 0   , 72  ,
			227 , 12 , 0  , 0  , 0  , 9  , 640, 58 , 0   , 0   };

	uint32_t Data1[250] = {
			0	,0	,0	,0	,0	,0	,1	,0	,0	,0	,0	,0	,0
			,0	,0	,0	,0	,0	,0	,0	,0	,0	,0	,0	,0	,0
			,0	,0	,0	,0	,0	,0	,52	,148,75	,0	,163,17	,70
			,156,37	,0	,59	,184,50	,227,0	,0	,17	,114,0  ,0
			,128,96	,0	,31 ,5	,104,20	,8	,0	,0	,63	,62	,0
			,23	,16	,129,118,13	,162,216,18	,0	,163,81	,246,0
			,52	,5	,82	,0	,44	,60	,180,169,77	,0	,147,149,0
			,22	,192,88 ,16	,0	,49	,0	,73	,45	,122,0	,113,103
			,106,0  ,0  ,19	,0  ,0	,0	,0	,39	,0	,0	,0	,0
			,0	,0	,22 ,0	,0	,30	,64	,0	,0	,74	,0	,0	,0
			,3	,0	,0	,0	,20	,13	,0  ,0	,0	,0	,0	,0	,5
			,0	,0	,0	,0	,0	,0	,0	,0	,0	,0	,0	,0	,0
			,0	,0	,0	,0	,0	,0	,0	,0	,0	,0	,0	,0	,0
			,0  ,0	,0	,0	,0	,0	,0	,0	,0	,0	,0	,0	,0
			,0	,0	,0	,19	,0	,0	,0	,11	,0	,0	,0  ,0	,0
			,0	,0	,0	,0	,0	,0	,0	,0	,0	,0  ,0	,0	,0
			,0	,0	,0	,0	,0	,0	,0	,0	,7	,0	,27	,17	,0
			,0	,51	,0	,90	,95	,0	,98	,0	,5	,0	,79	,0	,55
			,47	,86	,41	,0  ,0	,67	,137,74	,31	,117,0	,33	,0
			,176,2,134
	};
	uint32_t Test[250] = {664,  167,   13,   82,  340,  865,  469,  663,  144,  316,   15,
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
			181,  250,  219,  246,   47,  213,   23,  225};
	float Feat_Arr[6],Norm_Arr[6];

	/*Feature Extraction*/
	Feat_Arr[VAR_IDX]	= FEAT_EXT_f32_Variance(Test,250);
	Feat_Arr[RMS_IDX]   = FEAT_EXT_f32_RMS(Feat_Arr[VAR_IDX]);
	Feat_Arr[MEAN_IDX]  = FEAT_EXT_f32_MeanAbsolute(Test,250);
	Feat_Arr[SSC_INDX]  = FEAT_EXT_u32_SlopeSignChange(Test,250);
	Feat_Arr[ZC_IDX]    = FEAT_EXT_u32_ZeroCrossing(Test,250);
	Feat_Arr[WVL_IDX]   = FEAT_EXT_u32_WaveformLength(Test,250);

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

	if(*out_data < MODEL_THRESHOLD)
	{
		Servo_void_SetAngle(Servo1,180);
		Fest = 100;
		Gun = 0;

	}
	else
	{
		Servo_void_SetAngle(Servo1,90);
		Fest = 0;
		Gun = 100;
	}
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1)
	{
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		for(Local_u32AvgCounter = 0 ; Local_u32AvgCounter < 3; Local_u32AvgCounter++)
		{
			for(Local_u32Counter = 0 ;Local_u32Counter < 250;)
			{
				sensor = analogRead(&hadc1);
				HAL_Delay(2);
				if(sensor != 0)
				{
					Local_u32_RealData[Local_u32Counter++] = sensor;
				}
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

			if(*out_data < MODEL_THRESHOLD)
			{
				Servo_void_SetAngle(Servo1,180);
				Mode--;

			}
			else
			{
				Servo_void_SetAngle(Servo1,90);
				Mode++;
			}
		}

		if(Mode < 0)
		{
			Fest = 100;
			Gun = 0;
		}
		else
		{
			Fest = 0;
			Gun = 100;
		}

		HAL_Delay(500);
		Mode = 0;

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
