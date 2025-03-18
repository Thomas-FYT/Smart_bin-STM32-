/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcd.h"
#include "stdio.h"
#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
 TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;
TIM_HandleTypeDef htim8;

SRAM_HandleTypeDef hsram1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
static void MX_FSMC_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM8_Init(void);
static void MX_TIM4_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void delay (uint16_t time)
{
	__HAL_TIM_SET_COUNTER(&htim1, 0);
	while (__HAL_TIM_GET_COUNTER (&htim1) < time);
}

int Length_bin = 24;

//DATA for timer1 channel 1
uint32_t TIM1_CH1_IC_Val1 = 0;
uint32_t TIM1_CH1_IC_Val2 = 0;
uint32_t TIM1_CH1_Difference = 0;
uint8_t TIM1_CH1_Is_First_Captured = 0;  // is the first value captured ?
uint8_t TIM1_CH1_Distance  = 0;

//DATA for timer1 channel 4
uint32_t TIM1_CH4_IC_Val1 = 0;
uint32_t TIM1_CH4_IC_Val2 = 0;
uint32_t TIM1_CH4_Difference = 0;
uint8_t TIM1_CH4_Is_First_Captured = 0;  // is the first value captured ?
uint8_t TIM1_CH4_Distance  = 0;

//DATA for timer8 channel 3
uint32_t TIM8_CH3_IC_Val1 = 0;
uint32_t TIM8_CH3_IC_Val2 = 0;
uint32_t TIM8_CH3_Difference = 0;
uint8_t TIM8_CH3_Is_First_Captured = 0;  // is the first value captured ?
uint8_t TIM8_CH3_Distance  = 0;

//DATA for timer8 channel 4
uint32_t TIM8_CH4_IC_Val1 = 0;
uint32_t TIM8_CH4_IC_Val2 = 0;
uint32_t TIM8_CH4_Difference = 0;
uint8_t TIM8_CH4_Is_First_Captured = 0;  // is the first value captured ?
uint8_t TIM8_CH4_Distance  = 0;



// the callback function

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == htim1.Instance && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)  // if the interrupt source is timer 1 channel 1
	{
		if (htim->Instance == htim1.Instance && TIM1_CH1_Is_First_Captured==0) // if the first value is not captured
		{
			TIM1_CH1_IC_Val1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1); // read the first value
			TIM1_CH1_Is_First_Captured = 1;  // set the first captured as true
			// Now change the polarity to falling edge
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);
		}

		else if (TIM1_CH1_Is_First_Captured==1)   // if the first is already captured
		{
			TIM1_CH1_IC_Val2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);  // read second value
			__HAL_TIM_SET_COUNTER(htim, 0);  // reset the counter

			if (TIM1_CH1_IC_Val2 > TIM1_CH1_IC_Val1)
			{
				TIM1_CH1_Difference = TIM1_CH1_IC_Val2-TIM1_CH1_IC_Val1;
			}

			else if (TIM1_CH1_IC_Val1 > TIM1_CH1_IC_Val2)
			{
				TIM1_CH1_Difference = (0xffff - TIM1_CH1_IC_Val1) + TIM1_CH1_IC_Val2;
			}

			TIM1_CH1_Distance = TIM1_CH1_Difference * .034/2;
			TIM1_CH1_Is_First_Captured = 0; // set it back to false

			// set polarity to rising edge
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
			__HAL_TIM_DISABLE_IT(&htim1, TIM_IT_CC1);
		}
	}
	else if (htim->Instance == htim1.Instance && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)  // if the interrupt source is timer 4 channel 1
			{
				if (TIM1_CH4_Is_First_Captured==0) // if the first value is not captured
				{
					TIM1_CH4_IC_Val1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4); // read the first value
					TIM1_CH4_Is_First_Captured = 1;  // set the first captured as true
					// Now change the polarity to falling edge
					__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_4, TIM_INPUTCHANNELPOLARITY_FALLING);
				}

				else if (TIM1_CH4_Is_First_Captured==1)   // if the first is already captured
				{
					TIM1_CH4_IC_Val2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4);  // read second value
					__HAL_TIM_SET_COUNTER(htim, 0);  // reset the counter

					if (TIM1_CH4_IC_Val2 > TIM1_CH4_IC_Val1)
					{
						TIM1_CH4_Difference = TIM1_CH4_IC_Val2-TIM1_CH4_IC_Val1;
					}

					else if (TIM1_CH4_IC_Val1 > TIM1_CH4_IC_Val2)
					{
						TIM1_CH4_Difference = (0xffff - TIM1_CH4_IC_Val1) + TIM1_CH4_IC_Val2;
					}

					TIM1_CH4_Distance = TIM1_CH4_Difference * .034/2;
					TIM1_CH4_Is_First_Captured = 0; // set it back to false

					// set polarity to rising edge
					__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_4, TIM_INPUTCHANNELPOLARITY_RISING);
					__HAL_TIM_DISABLE_IT(&htim1, TIM_IT_CC4);
				}
			}
	else if (htim->Instance == htim8.Instance && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)  // if the interrupt source is channel 4
			{
				if (TIM8_CH3_Is_First_Captured==0) // if the first value is not captured
				{
					TIM8_CH3_IC_Val1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3); // read the first value
					TIM8_CH3_Is_First_Captured = 1;  // set the first captured as true
					// Now change the polarity to falling edge
					__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_3, TIM_INPUTCHANNELPOLARITY_FALLING);
				}

				else if (TIM8_CH3_Is_First_Captured==1)   // if the first is already captured
				{
					TIM8_CH3_IC_Val2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3);  // read second value
					__HAL_TIM_SET_COUNTER(htim, 0);  // reset the counter

					if (TIM8_CH3_IC_Val2 > TIM8_CH3_IC_Val1)
					{
						TIM8_CH3_Difference = TIM8_CH3_IC_Val2-TIM8_CH3_IC_Val1;
					}

					else if (TIM8_CH3_IC_Val1 > TIM8_CH3_IC_Val2)
					{
						TIM8_CH3_Difference = (0xffff - TIM8_CH3_IC_Val1) + TIM8_CH3_IC_Val2;
					}

					TIM8_CH3_Distance = TIM8_CH3_Difference * .034/2;
					TIM8_CH3_Is_First_Captured = 0; // set it back to false

					// set polarity to rising edge
					__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_3, TIM_INPUTCHANNELPOLARITY_RISING);
					__HAL_TIM_DISABLE_IT(&htim8, TIM_IT_CC3);
				}
			}
	else if (htim->Instance == htim8.Instance && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)  // if the interrupt source is channel 4
			{
				if (TIM8_CH4_Is_First_Captured==0) // if the first value is not captured
				{
					TIM8_CH4_IC_Val1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4); // read the first value
					TIM8_CH4_Is_First_Captured = 1;  // set the first captured as true
					// Now change the polarity to falling edge
					__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_4, TIM_INPUTCHANNELPOLARITY_FALLING);
				}

				else if (TIM8_CH4_Is_First_Captured==1)   // if the first is already captured
				{
					TIM8_CH4_IC_Val2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4);  // read second value
					__HAL_TIM_SET_COUNTER(htim, 0);  // reset the counter

					if (TIM8_CH4_IC_Val2 > TIM8_CH4_IC_Val1)
					{
						TIM8_CH4_Difference = TIM8_CH4_IC_Val2-TIM8_CH4_IC_Val1;
					}

					else if (TIM8_CH4_IC_Val1 > TIM8_CH4_IC_Val2)
					{
						TIM8_CH4_Difference = (0xffff - TIM8_CH4_IC_Val1) + TIM8_CH4_IC_Val2;
					}

					TIM8_CH4_Distance = TIM8_CH4_Difference * .034/2;
					TIM8_CH4_Is_First_Captured = 0; // set it back to false

					// set polarity to rising edge
					__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_4, TIM_INPUTCHANNELPOLARITY_RISING);
					__HAL_TIM_DISABLE_IT(&htim8, TIM_IT_CC4);
				}
			}

}

void TIM1_CH1_HCSR04_Read (void)
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_SET);  // pull the TRIG pin HIGH
	delay(10);  // wait for 10 us
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_RESET);  // pull the TRIG pin low


	__HAL_TIM_ENABLE_IT(&htim1, TIM_IT_CC1);
}
void TIM1_CH4_HCSR04_Read (void)
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_SET);  // pull the TRIG pin HIGH
	delay(10);  // wait for 10 us
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_RESET);  // pull the TRIG pin low


	__HAL_TIM_ENABLE_IT(&htim1, TIM_IT_CC4);
}
void TIM8_CH3_HCSR04_Read (void)
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_SET);  // pull the TRIG pin HIGH
	delay(10);  // wait for 10 us
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_RESET);  // pull the TRIG pin low


	__HAL_TIM_ENABLE_IT(&htim8, TIM_IT_CC3);
}
void TIM8_CH4_HCSR04_Read (void)
{
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);  // pull the TRIG pin HIGH
	delay(10);  // wait for 10 us
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);  // pull the TRIG pin low

	__HAL_TIM_ENABLE_IT(&htim8, TIM_IT_CC4);
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

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
  MX_TIM2_Init();
  MX_FSMC_Init();
  MX_TIM1_Init();
  MX_TIM3_Init();
  MX_TIM8_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
  LCD_INIT();
  LCD_DrawLine(0,160,239,160,BLACK);
  LCD_DrawLine(120,0,120,319,BLACK);
  LCD_DrawBin(20,30,80,120,BLACK); // trash bin
  LCD_DrawBin(140,30,80,120,BLACK); // metal bin
  LCD_DrawBin(20,190,80,120,BLACK); // plastic bin
  LCD_DrawBin(140,190,80,120,BLACK); // paper bin
  char str1[10] = "Trash:";
  char str2[10] = "Metal:";
  char str3[10] = "Plastic:";
  char str4[10] = "Paper:";
  LCD_DrawString(20, 10, str1);
  LCD_DrawString(140, 10, str2);
  LCD_DrawString(20, 170, str3);
  LCD_DrawString(140, 170, str4);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  /*HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);*/
  HAL_TIM_IC_Start_IT(&htim1, TIM_CHANNEL_1);// Start input capture mode with interrupt for Channel 1
  HAL_TIM_IC_Start_IT(&htim1, TIM_CHANNEL_4);// Start input capture mode with interrupt for Channel 2
  HAL_TIM_IC_Start_IT(&htim8, TIM_CHANNEL_3);// Start input capture mode with interrupt for Channel 3
  HAL_TIM_IC_Start_IT(&htim8, TIM_CHANNEL_4);// Start input capture mode with interrupt for Channel 4



  while (1)
  {






	  	  // Trash bin start lcd updating
	      // PC10 : trig
	  	  // PA8 : echo
	  	  TIM1_CH1_HCSR04_Read();
	  	  int Trash_volume = (((double)Length_bin - (double)TIM1_CH1_Distance)/(double)Length_bin*100);
	  	  if(Trash_volume > 100 || Trash_volume < 0 ){
	  		LCD_Clear(21,31,79,119,WHITE);
	  		char Trash_ErrorData[10] = "Error";
		  	int default_volume = 0;
		  	char Trash_bin_volume[4];
		  	sprintf(Trash_bin_volume, "%3u", default_volume);
	  		LCD_DrawString(30, 80, Trash_ErrorData);
	  		LCD_DrawString(70, 10, Trash_bin_volume);
	  	  }
	  	  else{
			  int Trash_page_value = (double)150-(((double)Length_bin - (double)TIM1_CH1_Distance)/(double)Length_bin*120);
			  if(Trash_volume >= 0 && Trash_volume <= 30){
				LCD_Clear(21,31,79,119,WHITE);
				LCD_Clear(21,Trash_page_value,79,150-Trash_page_value,GREEN);
			  }
			  else if(Trash_volume > 30  && Trash_volume <= 90){
				LCD_Clear(21,31,79,119,WHITE);
				LCD_Clear(21,Trash_page_value,79,150-Trash_page_value,YELLOW);
			  }
			  else if(Trash_volume > 90  && Trash_volume <= 100){
				LCD_Clear(21,31,79,119,WHITE);
				LCD_Clear(21,Trash_page_value,79,150-Trash_page_value,RED);
			  }

			  char Trash_bin_volume[4];
			  sprintf(Trash_bin_volume, "%3u", Trash_volume);
			  sprintf(Trash_bin_volume + strlen(Trash_bin_volume), "%c", '%');
			  LCD_DrawString(70, 10, Trash_bin_volume);
	  	  }
		  HAL_Delay(200);	//Trash bin finish lcd updating

		  // Metal bin start lcd updating
		  // PC11 : trig
		  //PA11 : echo
		  TIM1_CH4_HCSR04_Read();
	  	  int Metal_volume = (((double)Length_bin - (double)TIM1_CH4_Distance)/(double)Length_bin*100);
	  	  if(Metal_volume > 100 || Metal_volume < 0){
	  		LCD_Clear(141,31,79,119,WHITE);
	  		char Metal_ErrorData[10] = "Error";
		  	int default_volume = 0;
		  	char Metal_bin_volume[4];
		  	sprintf(Metal_bin_volume, "%3u", default_volume);
	  		LCD_DrawString(150, 80, Metal_ErrorData);
	  		LCD_DrawString(190, 10, Metal_bin_volume);
	  	  }
	  	  else{
			  int Metal_page_value = (double)150-(((double)Length_bin - (double)TIM1_CH4_Distance)/(double)Length_bin*120);
			  if(Metal_volume >= 0 && Metal_volume <= 30){
				LCD_Clear(141,31,79,119,WHITE);
				LCD_Clear(141,Metal_page_value,79,150-Metal_page_value,GREEN);
			  }
			  else if(Metal_volume > 30  && Metal_volume <= 90){
				LCD_Clear(141,31,79,119,WHITE);
				LCD_Clear(141,Metal_page_value,79,150-Metal_page_value,YELLOW);
			  }
			  else if(Metal_volume > 90  && Metal_volume <= 100){
				LCD_Clear(141,31,79,119,WHITE);
				LCD_Clear(141,Metal_page_value,79,150-Metal_page_value,RED);
			  }
			  char Metal_bin_volume[4];
			  sprintf(Metal_bin_volume, "%3u", Metal_volume);
			  sprintf(Metal_bin_volume + strlen(Metal_bin_volume), "%c", '%');
			  LCD_DrawString(190, 10, Metal_bin_volume);
	  	  }
		  HAL_Delay(200);
		  // Metal bin finish lcd updating


		  // Plastic bin start lcd updating
		  // PC12 : trig
		  // PC8 : echo
		  TIM8_CH3_HCSR04_Read();
	  	  int Plastic_volume = (((double)Length_bin - (double)TIM8_CH3_Distance)/(double)Length_bin*100);
	  	  if(Plastic_volume > 100 || Plastic_volume < 0){
	  		LCD_Clear(21,191,79,119,WHITE);
	  		char Plastic_ErrorData[10] = "Error";
		  	int default_volume = 0;
		  	char Plastic_bin_volume[4];
		  	sprintf(Plastic_bin_volume, "%3u", default_volume);
	  		LCD_DrawString(30, 240, Plastic_ErrorData);
	  		LCD_DrawString(75, 170, Plastic_bin_volume);
	  	  }
	  	  else{
			  int Plastic_page_value = (double)310-(((double)Length_bin - (double)TIM8_CH3_Distance)/(double)Length_bin*120);
			  if(Plastic_volume >= 0 && Plastic_volume <= 30){
				LCD_Clear(21,191,79,119,WHITE);
				LCD_Clear(21,Plastic_page_value,79,310-Plastic_page_value,GREEN);
			  }
			  else if(Plastic_volume > 30  && Plastic_volume <= 90){
				LCD_Clear(21,191,79,119,WHITE);
				LCD_Clear(21,Plastic_page_value,79,310-Plastic_page_value,YELLOW);
			  }
			  else if(Plastic_volume > 90  && Plastic_volume <= 100){
				LCD_Clear(21,191,79,119,WHITE);
				LCD_Clear(21,Plastic_page_value,79,310-Plastic_page_value,RED);
			  }
			  char Plastic_bin_volume[4];
			  sprintf(Plastic_bin_volume, "%3u", Plastic_volume);
			  sprintf(Plastic_bin_volume + strlen(Plastic_bin_volume), "%c", '%');
			  LCD_DrawString(75, 170, Plastic_bin_volume);
	  	  }
		  HAL_Delay(200);
		  // Plastic bin finish lcd updating


		  //Paper bin start lcd updating
		  // PD2 : trig
		  // PC9 : echo
		  TIM8_CH4_HCSR04_Read();
	  	  int Paper_volume = (((double)Length_bin - (double)TIM8_CH4_Distance)/(double)Length_bin*100);
	  	  if(Paper_volume > 100 || Paper_volume < 0){
	  		LCD_Clear(141,191,79,119,WHITE);
		  	char Paper_ErrorData[10] = "Error";
		  	int default_volume = 0;
		  	char Paper_bin_volume[4];
		  	sprintf(Paper_bin_volume, "%3u", default_volume);
		  	LCD_DrawString(150, 240, Paper_ErrorData);
		  	LCD_DrawString(190, 170, Paper_bin_volume);
	  	  }
	  	  else{
			  int Paper_page_value = (double)310-(((double)Length_bin - (double)TIM8_CH4_Distance)/(double)Length_bin*120);
			  if(Paper_volume >= 0 && Paper_volume <= 30){
				LCD_Clear(141,191,79,119,WHITE);
				LCD_Clear(141,Paper_page_value,79,310-Paper_page_value,GREEN);
			  }
			  else if(Paper_volume > 30  && Paper_volume <= 90){
				LCD_Clear(141,191,79,119,WHITE);
				LCD_Clear(141,Paper_page_value,79,310-Paper_page_value,YELLOW);
			  }
			  else if(Paper_volume > 90  && Paper_volume <= 100){
				LCD_Clear(141,191,79,119,WHITE);
				LCD_Clear(141,Paper_page_value,79,310-Paper_page_value,RED);
			  }
			  char Paper_bin_volume[4];
			  sprintf(Paper_bin_volume, "%3u", Paper_volume);
			  sprintf(Paper_bin_volume + strlen(Paper_bin_volume), "%c", '%');
			  LCD_DrawString(190, 170, Paper_bin_volume);
	  	  }
		  HAL_Delay(200);
	    }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 71;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 65535;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_IC_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim1, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_ConfigChannel(&htim1, &sConfigIC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 35;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 40000;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 4000;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.Pulse = 2000;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 35;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 40000;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 4000;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.Pulse = 2000;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.Pulse = 4000;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.Pulse = 2000;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 35;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 40000;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 4000;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.Pulse = 2000;
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */
  HAL_TIM_MspPostInit(&htim4);

}

/**
  * @brief TIM8 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM8_Init(void)
{

  /* USER CODE BEGIN TIM8_Init 0 */

  /* USER CODE END TIM8_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  /* USER CODE BEGIN TIM8_Init 1 */

  /* USER CODE END TIM8_Init 1 */
  htim8.Instance = TIM8;
  htim8.Init.Prescaler = 71;
  htim8.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim8.Init.Period = 65535;
  htim8.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim8.Init.RepetitionCounter = 0;
  htim8.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_IC_Init(&htim8) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim8, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim8, &sConfigIC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_ConfigChannel(&htim8, &sConfigIC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM8_Init 2 */

  /* USER CODE END TIM8_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12|GPIO_PIN_2, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PA0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB12 PB13 PB14 PB15 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PD12 */
  GPIO_InitStruct.Pin = GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : PC10 PC11 PC12 */
  GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PD2 */
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : PE1 */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

}

/* FSMC initialization function */
static void MX_FSMC_Init(void)
{

  /* USER CODE BEGIN FSMC_Init 0 */

  /* USER CODE END FSMC_Init 0 */

  FSMC_NORSRAM_TimingTypeDef Timing = {0};

  /* USER CODE BEGIN FSMC_Init 1 */

  /* USER CODE END FSMC_Init 1 */

  /** Perform the SRAM1 memory initialization sequence
  */
  hsram1.Instance = FSMC_NORSRAM_DEVICE;
  hsram1.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;
  /* hsram1.Init */
  hsram1.Init.NSBank = FSMC_NORSRAM_BANK1;
  hsram1.Init.DataAddressMux = FSMC_DATA_ADDRESS_MUX_DISABLE;
  hsram1.Init.MemoryType = FSMC_MEMORY_TYPE_SRAM;
  hsram1.Init.MemoryDataWidth = FSMC_NORSRAM_MEM_BUS_WIDTH_16;
  hsram1.Init.BurstAccessMode = FSMC_BURST_ACCESS_MODE_DISABLE;
  hsram1.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
  hsram1.Init.WrapMode = FSMC_WRAP_MODE_DISABLE;
  hsram1.Init.WaitSignalActive = FSMC_WAIT_TIMING_BEFORE_WS;
  hsram1.Init.WriteOperation = FSMC_WRITE_OPERATION_ENABLE;
  hsram1.Init.WaitSignal = FSMC_WAIT_SIGNAL_DISABLE;
  hsram1.Init.ExtendedMode = FSMC_EXTENDED_MODE_DISABLE;
  hsram1.Init.AsynchronousWait = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
  hsram1.Init.WriteBurst = FSMC_WRITE_BURST_DISABLE;
  /* Timing */
  Timing.AddressSetupTime = 15;
  Timing.AddressHoldTime = 15;
  Timing.DataSetupTime = 255;
  Timing.BusTurnAroundDuration = 15;
  Timing.CLKDivision = 16;
  Timing.DataLatency = 17;
  Timing.AccessMode = FSMC_ACCESS_MODE_A;
  /* ExtTiming */

  if (HAL_SRAM_Init(&hsram1, &Timing, NULL) != HAL_OK)
  {
    Error_Handler( );
  }

  /** Disconnect NADV
  */

  __HAL_AFIO_FSMCNADV_DISCONNECTED();

  /* USER CODE BEGIN FSMC_Init 2 */

  /* USER CODE END FSMC_Init 2 */
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
