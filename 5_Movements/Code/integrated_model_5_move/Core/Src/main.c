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
#include "movements.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

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
float Fest,Gun,OK,Good,Two,Rest;
float arr[4] = {0};
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
	uint32_t Local_u32Counter,Local_u32_RealData[3][250],Local_u32AvgCounter;

	uint8_t Local_u8Avg = 5;
	uint8_t Local_u8Max,Local_u8ModeArr[Local_u8Avg],Local_u8Sum=0;
	float Feat_f32Arr[3][6] , Norm_f32Arr[3][6];

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

	//	SERVO_t Servo1,Servo2,Servo3,Servo4,Servo5;
	//	Servo1.Pin = Servo_PA15;
	//	Servo2.Pin = Servo_PB10;
	//	Servo3.Pin = Servo_PB14;
	//	Servo4.Pin = Servo_PB2;
	//	Servo5.Pin = Servo_PB9;
	init_servo();
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

	uint32_t Move0[SENSOR_NUM][250] = {
			{    0,    0,    0,    0,   42,    0,    0,    0,    0,    0,    0,
					0,    0,  185,    0,    0,    0,    0,   83,    0,  100,  411,
					0,    0,  616,  500,  147,    0,    0,   65,    0,    0,    0,
					0,    0,   21,   78,    0,    0,  790,  128,    0,    0,    0,
					322,  379,    0,    0,    0,    0,  577,  204,    0,    0,    0,
					0,  319,    0,    0,    0,    0,    0,    0,    0,    0,  293,
					0,    0,    0,    0,   53,    0,    0,    0,    0,   34,    0,
					26,    0,    0,  165,  123,    0,    0,    0,   57,  127,    0,
					0,    0,    0,    0,    0,    0,    0,  254,    0,    0,   74,
					0,    0,  216,    0,    0,    0,  272,    0,    0,    0,    0,
					0,    0,    0,    0,  303,  333,    0,    0,    0,    0,    0,
					0,    0,   53,    0,    0,    0,    0,   86,    0,    0,    0,
					0,    0,    0,  243,    0,    0,  149,    0,    0,    0,    0,
					16,    0,    0,  514,  248,   15,    0,    0,  219,   54,    0,
					0,    0,    0,    0,    0,    0,    0,    0,    0,    0,  181,
					155,    0,    0,  402,    0,  103,    0,    0,    0,    0,    0,
					0,    0,   30,  621,    0,    0,    0,  242,  419,   62,    0,
					55,    0,    0, 1211,  128,  571,  363,    0,    0,    0,    0,
					98,  327,    0,    0,    0,    0,    0,    0,    0,    0,    0,
					0,    0,   52,    0,    0,    0,    0,    0,  120,   84,    0,
					0,   25,    0,  552,  494,    0,    0,  410,  299,  419,    0,
					0,    0,    0,  448,  321,    0,    0,    0,    0,   38,    0,
					24,    0,    0,    0,   55,   47,    0,   86},
					{  0.,    0.,  192.,    0.,  309.,  566.,  631.,  238.,    0.,
							0.,  202.,    5.,  175.,    0.,    0.,    0.,  172.,    0.,
							0.,  106.,    0.,   71.,    0.,    0.,    0.,  105.,  650.,
							3.,    0.,   24.,    0.,  134.,    0.,    0.,  306.,    0.,
							81.,   39.,    0.,    0.,   55.,    0.,    0.,    0.,    0.,
							10.,    0.,    0.,    0.,   13.,  582.,  235.,    0.,    0.,
							101.,    0.,    0.,    0.,    0.,  377.,  112.,  312.,    0.,
							0.,    0.,  494.,   34.,    0.,    0.,  228.,    0.,    0.,
							349.,    0.,  128.,   68.,  301.,  178.,    0.,  343.,    0.,
							489.,  581.,    0.,  261.,  229.,  212.,  408.,    0.,  248.,
							504.,    0.,    0.,    0.,    0.,  168.,    0.,    0.,    0.,
							0.,  156.,    0.,    0.,   82.,    9.,   46.,    0.,    0.,
							0.,    0.,   52.,    0.,    0.,    0.,   65.,  168.,  108.,
							0.,   13.,    0.,    0.,   11.,  208.,  145.,    0.,    0.,
							0.,  176.,  427.,  204.,    0.,    0.,   73.,  130.,   34.,
							108.,    0.,  362.,   24.,    0.,    0.,    0.,  225.,   91.,
							0.,    0.,  770.,  301.,    0.,    0.,    0.,  490.,   24.,
							149.,    0.,    0.,  351.,  151.,    0.,  331.,  228.,    0.,
							22.,   16.,    0.,  355.,    0.,    0.,  133.,  242.,   36.,
							0.,    0.,   92.,  408.,   80.,    0.,    0.,  286.,  284.,
							0.,    0.,    0.,  361.,    4.,  364.,    0.,  143.,    0.,
							0.,  139.,  606.,  412.,  277.,  979.,    0.,    0.,   17.,
							0., 1165.,    0.,   97.,   81.,   29.,  250.,    0.,  560.,
							0.,    0.,    0.,    0.,  181.,    0.,    0.,    0.,  112.,
							226.,  657.,    0.,    0.,    0.,  791.,  283., 1183., 1460.,
							0.,    0.,  314., 1105.,  723.,   72.,    0.,    0.,    0.,
							579.,  594.,    0.,    0.,    0.,  103.,    0.,  138.,    0.,
							0.,    0.,  173.,  181.,  368.,   64.,  125.},
							{   2, 2343,  198,  757,  359,  553,  152,    0,    0,  292,  896,
									113,   53,    0,  226,  185,   31,  472,  422,    0,   32,   15,
									172,    0,  463,  535, 1535,   45,   52,   93,   59,  330,   94,
									180,   47,   29,   21,  149,    0,    5,  103,  200,  105,    1,
									70,  346,   54,  179,  782,  162,    0,   37,   10,   52,    0,
									289,   78,   95,    0,   42,   97,  216,  184,   76,  195,  269,
									16,    0,  120,  283,  717,   58,  213,   16,    1,   17,  507,
									1081,   92,  368,  173,    0,  745,    5,   30,   12,  650,  281,
									74,   83,    0,    9,   82,    0,  161,   18,    0,   68,  404,
									18,  392,  243,  454,  456,  147,    4,    0,   10,  362,    0,
									268,  228,    2,   60,    0,   11,  579,  180,  193,    0,    4,
									212,    3,  328,   39,  119,  183,   59,   87,   91,    7,  243,
									190,    0,   10,   85,   12,   37,   50,    5,  536,  116,  439,
									66,  158,   14,  141,   98,  335,  129,  481,   83,  215,   53,
									7,   44,  423,   26,  198,    0,  202,   14,   10,   45,  442,
									325,   33,  265,    6,   90,   11,  850,   64,  386,  188,  196,
									0,   19,   17,   93,    1,    1,  231,    0,   87,  209,   36,
									89,   13,  375,  212,  260,   17,    1,  157,   15,  739,  406,
									211,  250,  357,  149,   90,   32,   27,  146,    1,   44,    0,
									25,  246,    0,    0,    6,  566,   17,    0,  217,  239,    0,
									10,   12,  294,  685,  207,  107,  167,  146,    1,   37,  109,
									408,  283,  144,  310,   40,    2,    0,   81,   20,   57,   60,
									90,    0,  167,  318,  569,   72,    4,   49}};

	FEAT_EXT_f32_AllFeat(Move0 ,*Feat_f32Arr);
	NORM_f32_AllNorm(*Feat_f32Arr, *Norm_f32Arr);

	/*Model Running*/

	float Local_f32KoftaFeat[18],Local_f32KoftaNorm[18];

	//	Local_f32KoftaFeat[VAR_IDX ] = FEAT_EXT_f32_Variance(Move0[0],DATA_SIZE);
	//	Local_f32KoftaFeat[RMS_IDX ] = FEAT_EXT_f32_RMS(Local_f32KoftaFeat[VAR_IDX]);
	//	Local_f32KoftaFeat[MEAN_IDX] = FEAT_EXT_f32_Mean(Move0[0],DATA_SIZE);
	//	Local_f32KoftaFeat[SSC_IDX] = FEAT_EXT_u32_SlopeSignChange(Move0[0],DATA_SIZE);
	//	Local_f32KoftaFeat[ZC_IDX  ] = FEAT_EXT_u32_ZeroCrossing(Move0[0],DATA_SIZE) ;
	//	Local_f32KoftaFeat[WVL_IDX ] = FEAT_EXT_u32_WaveformLength(Move0[0],DATA_SIZE);
	//
	//	Local_f32KoftaFeat[VAR_IDX+6] = FEAT_EXT_f32_Variance(Move0[1],DATA_SIZE);
	//	Local_f32KoftaFeat[RMS_IDX+6] = FEAT_EXT_f32_RMS(Local_f32KoftaFeat[VAR_IDX+6]);
	//	Local_f32KoftaFeat[MEAN_IDX+6] = FEAT_EXT_f32_Mean(Move0[1],DATA_SIZE);
	//	Local_f32KoftaFeat[SSC_IDX+6] = FEAT_EXT_u32_SlopeSignChange(Move0[1],DATA_SIZE);
	//	Local_f32KoftaFeat[ZC_IDX+6] = FEAT_EXT_u32_ZeroCrossing(Move0[1],DATA_SIZE) ;
	//	Local_f32KoftaFeat[WVL_IDX+6] = FEAT_EXT_u32_WaveformLength(Move0[1],DATA_SIZE);
	//
	//
	//	Local_f32KoftaFeat[VAR_IDX+12] = FEAT_EXT_f32_Variance(Move0[2],DATA_SIZE);
	//	Local_f32KoftaFeat[RMS_IDX+12] = FEAT_EXT_f32_RMS(Local_f32KoftaFeat[VAR_IDX+12]);
	//	Local_f32KoftaFeat[MEAN_IDX+12] = FEAT_EXT_f32_Mean(Move0[2],DATA_SIZE);
	//	Local_f32KoftaFeat[SSC_IDX+12] = FEAT_EXT_u32_SlopeSignChange(Move0[2],DATA_SIZE);
	//	Local_f32KoftaFeat[ZC_IDX+12] = FEAT_EXT_u32_ZeroCrossing(Move0[2],DATA_SIZE) ;
	//	Local_f32KoftaFeat[WVL_IDX+12] = FEAT_EXT_u32_WaveformLength(Move0[2],DATA_SIZE);
	//
	//	Local_f32KoftaNorm[VAR_IDX ] = NORM_f32_Variance(SNS1, Local_f32KoftaFeat[VAR_IDX]);
	//	Local_f32KoftaNorm[RMS_IDX ] = NORM_f32_RMS(SNS1, Local_f32KoftaFeat[RMS_IDX]);
	//	Local_f32KoftaNorm[MEAN_IDX] = NORM_f32_Mean(SNS1, Local_f32KoftaFeat[MEAN_IDX]);
	//	Local_f32KoftaNorm[SSC_IDX]  = NORM_f32_SlopeSignChange(SNS1, Local_f32KoftaFeat[SSC_IDX]);
	//	Local_f32KoftaNorm[ZC_IDX  ] = NORM_f32_ZeroCrossing(SNS1, Local_f32KoftaFeat[ZC_IDX]);
	//	Local_f32KoftaNorm[WVL_IDX ] = NORM_f32_WaveformLength(SNS1, Local_f32KoftaFeat[WVL_IDX]);
	//
	//
	//
	//	Local_f32KoftaNorm[VAR_IDX+6] = NORM_f32_Variance(SNS2, Local_f32KoftaFeat[VAR_IDX+6]);
	//	Local_f32KoftaNorm[RMS_IDX+6] = NORM_f32_RMS(SNS2, Local_f32KoftaFeat[RMS_IDX+6]);
	//	Local_f32KoftaNorm[MEAN_IDX+6] = NORM_f32_Mean(SNS2, Local_f32KoftaFeat[MEAN_IDX+6]);
	//	Local_f32KoftaNorm[SSC_IDX+6]  = NORM_f32_SlopeSignChange(SNS2, Local_f32KoftaFeat[SSC_IDX+6]);
	//	Local_f32KoftaNorm[ZC_IDX+6] = NORM_f32_ZeroCrossing(SNS2, Local_f32KoftaFeat[ZC_IDX+6]);
	//	Local_f32KoftaNorm[WVL_IDX+6] = NORM_f32_WaveformLength(SNS2, Local_f32KoftaFeat[WVL_IDX+6]);
	//
	//
	//	Local_f32KoftaNorm[VAR_IDX+12] = NORM_f32_Variance(SNS2, Local_f32KoftaFeat[VAR_IDX+12]);
	//	Local_f32KoftaNorm[RMS_IDX+12] = NORM_f32_RMS(SNS3, Local_f32KoftaFeat[RMS_IDX+12]);
	//	Local_f32KoftaNorm[MEAN_IDX+12] = NORM_f32_Mean(SNS3, Local_f32KoftaFeat[MEAN_IDX+12]);
	//	Local_f32KoftaNorm[SSC_IDX+12]  = NORM_f32_SlopeSignChange(SNS3, Local_f32KoftaFeat[SSC_IDX+12]);
	//	Local_f32KoftaNorm[ZC_IDX+12] = NORM_f32_ZeroCrossing(SNS3, Local_f32KoftaFeat[ZC_IDX+12]);
	//	Local_f32KoftaNorm[WVL_IDX+12] = NORM_f32_WaveformLength(SNS3, Local_f32KoftaFeat[WVL_IDX+12]);
	//
	//	/*Movement 1 Data*/
	//	in_data[RMS_IDX]  	= Local_f32KoftaNorm[RMS_IDX];
	//	in_data[MEAN_IDX] 	= Local_f32KoftaNorm[MEAN_IDX];
	//	in_data[VAR_IDX]  	= Local_f32KoftaNorm[VAR_IDX];
	//	in_data[SSC_IDX] 	= Local_f32KoftaNorm[SSC_IDX];
	//	in_data[ZC_IDX]   	= Local_f32KoftaNorm[ZC_IDX];
	//	in_data[WVL_IDX]  	= Local_f32KoftaNorm[WVL_IDX];
	//
	//	in_data[RMS_IDX+6]  = Local_f32KoftaNorm[RMS_IDX+6];
	//	in_data[VAR_IDX+6]  = Local_f32KoftaNorm[VAR_IDX+6];
	//	in_data[MEAN_IDX+6] = Local_f32KoftaNorm[MEAN_IDX+6];
	//	in_data[SSC_IDX+6] 	= Local_f32KoftaNorm[SSC_IDX+6];
	//	in_data[ZC_IDX+6]   = Local_f32KoftaNorm[ZC_IDX+6];
	//	in_data[WVL_IDX+6]  = Local_f32KoftaNorm[WVL_IDX+6];
	//
	//	in_data[RMS_IDX+12]  = Local_f32KoftaNorm[RMS_IDX+12];
	//	in_data[VAR_IDX+12]  = Local_f32KoftaNorm[VAR_IDX+12];
	//	in_data[MEAN_IDX+12] = Local_f32KoftaNorm[MEAN_IDX+12];
	//	in_data[SSC_IDX+12]  = Local_f32KoftaNorm[SSC_IDX+12];
	//	in_data[ZC_IDX+12]   = Local_f32KoftaNorm[ZC_IDX+12];
	//	in_data[WVL_IDX+12]  = Local_f32KoftaNorm[WVL_IDX+12];



	in_data[RMS_IDX]  	= Norm_f32Arr[0][RMS_IDX];
	in_data[VAR_IDX]  	= Norm_f32Arr[0][VAR_IDX];
	in_data[MEAN_IDX] 	= Norm_f32Arr[0][MEAN_IDX];
	in_data[SSC_IDX] 	= Norm_f32Arr[0][SSC_IDX];
	in_data[ZC_IDX]   	= Norm_f32Arr[0][ZC_IDX];
	in_data[WVL_IDX]  	= Norm_f32Arr[0][WVL_IDX];

	in_data[RMS_IDX+6]  =  Norm_f32Arr[1][RMS_IDX];
	in_data[VAR_IDX+6]  =  Norm_f32Arr[1][VAR_IDX];
	in_data[MEAN_IDX+6] =  Norm_f32Arr[1][MEAN_IDX];
	in_data[SSC_IDX+6] 	=  Norm_f32Arr[1][SSC_IDX];
	in_data[ZC_IDX+6]   =  Norm_f32Arr[1][ZC_IDX];
	in_data[WVL_IDX+6]  =  Norm_f32Arr[1][WVL_IDX];
	//
	//	in_data[RMS_IDX+12]  = Norm_f32Arr[2][RMS_IDX];
	//	in_data[VAR_IDX+12]  = Norm_f32Arr[2][VAR_IDX];

	//	in_data[MEAN_IDX+12] = Norm_f32Arr[2][MEAN_IDX];
	//	in_data[SSC_IDX+12]  = Norm_f32Arr[2][SSC_IDX];
	//	in_data[ZC_IDX+12]   = Norm_f32Arr[2][ZC_IDX];
	//	in_data[WVL_IDX+12]  = Norm_f32Arr[2][WVL_IDX];

	Model_Run(in_data, out_data);
	//
	//	Local_u8Max = 0;
	//	for(int i = 0 ;i < 5; i++)
	//	{
	//		if(out_data[i] > out_data[Local_u8Max])
	//		{
	//			Local_u8Max = i;
	//		}
	//	}


	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
		for(Local_u32Counter = 0 ;Local_u32Counter < DATA_SIZE ;)
		{

			sensor1 =  analogRead(&hadc1);
			sensor2 =  analogRead(&hadc2);
			//			sensor3 =  analogRead(&hadc3);
			//			if((sensor1 != 0) && (sensor2 != 0)/* && (sensor3 != 0)*/)
			//			{

			Local_u32_RealData[SNS1][Local_u32Counter] = sensor1;
			Local_u32_RealData[SNS2][Local_u32Counter] = sensor2;
			Local_u32_RealData[SNS3][Local_u32Counter] = 0;
			Local_u32Counter++;
			//			}

			HAL_Delay(2);
		}

		FEAT_EXT_f32_AllFeat(Local_u32_RealData,*Feat_f32Arr);
		NORM_f32_AllNorm(*Feat_f32Arr, *Norm_f32Arr);

		in_data[RMS_IDX]  	= Norm_f32Arr[0][RMS_IDX];
		in_data[VAR_IDX]  	= Norm_f32Arr[0][VAR_IDX];
		in_data[MEAN_IDX] 	= Norm_f32Arr[0][MEAN_IDX];
		in_data[SSC_IDX] 	= Norm_f32Arr[0][SSC_IDX];
		in_data[ZC_IDX]   	= Norm_f32Arr[0][ZC_IDX];
		in_data[WVL_IDX]  	= Norm_f32Arr[0][WVL_IDX];

		in_data[RMS_IDX+6]  =  Norm_f32Arr[1][RMS_IDX];
		in_data[VAR_IDX+6]  =  Norm_f32Arr[1][VAR_IDX];
		in_data[MEAN_IDX+6] =  Norm_f32Arr[1][MEAN_IDX];
		in_data[SSC_IDX+6] 	=  Norm_f32Arr[1][SSC_IDX];
		in_data[ZC_IDX+6]   =  Norm_f32Arr[1][ZC_IDX];
		in_data[WVL_IDX+6]  =  Norm_f32Arr[1][WVL_IDX];
		//
		//		in_data[RMS_IDX+12]  = Norm_f32Arr[2][RMS_IDX];
		//		in_data[VAR_IDX+12]  = Norm_f32Arr[2][VAR_IDX];
		//		in_data[MEAN_IDX+12] = Norm_f32Arr[2][MEAN_IDX];
		//		in_data[SSC_IDX+12]  = Norm_f32Arr[2][SSC_IDX];
		//		in_data[ZC_IDX+12]   = Norm_f32Arr[2][ZC_IDX];
		//		in_data[WVL_IDX+12]  = Norm_f32Arr[2][WVL_IDX];

		Model_Run(in_data, out_data);
		for(int i = 0 ;i < 4; i++)
		{
			if(out_data[i] > out_data[Local_u8Max])
			{
				Local_u8Max = i;
			}
		}

		Two  = out_data[0];
		OK   = out_data[1];
		Fest = out_data[2];
		Good = out_data[3];
		Rest = out_data[4];

		if(Local_u8Max == 0)
		{
			//			Gun = 100;
			//			Fest = 0;
			//			Two  = 0;
			//			Rest = 0;

			two();
		}
		else if(Local_u8Max == 1)
		{
			//			Fest = 0;
			//			Gun = 0;
			//			OK = 0;
			//			Good = 0;
			//			Two = 100;
			//			Rest = 0;
			ok();
		}

		else if (Local_u8Max == 2)
		{
			//			Fest = 100;
			//			Gun = 0;
			//			OK = 0;
			//			Two = 0;
			//			Rest = 0;
			fest();
		}
		else if (Local_u8Max == 3)
		{
			//			Fest = 0;
			//			Gun  = 0;
			//			Two  = 0;
			//			Rest = 100;
			good();

		}
		else if (Local_u8Max == 4)
		{
			//			Fest = 100;
			//			Gun = 0;
			//			OK = 0;
			//			Good = 0;
			//			Two = 0;
			rest();
		}

		Mode = Local_u8Max;
		HAL_Delay(500);
		Mode = 0;
		Local_u8Max = 0;

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
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLM = 8;
	RCC_OscInitStruct.PLL.PLLN = 180;
	RCC_OscInitStruct.PLL.PLLP = 2;
	RCC_OscInitStruct.PLL.PLLQ = 2;
	RCC_OscInitStruct.PLL.PLLR = 2;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	/** Activate the Over-Drive mode
	 */
	if (HAL_PWREx_EnableOverDrive() != HAL_OK)
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
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
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
