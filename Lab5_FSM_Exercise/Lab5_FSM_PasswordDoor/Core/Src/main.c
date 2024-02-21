/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"
#include "fsmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "software_timer.h"
#include "button.h"
#include "lcd.h"
#include "picture.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define INIT_SYSTEM		0
#define LOCK_DOOR		1
#define ENTER_PASSWORD	2
#define UNLOCK_DOOR		3
#define WRONG_PASSWORD	4
#define CHECK_PASSWORD	5
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
unsigned char arrayMapOfNumber[16] = { 1, 2, 3, ' ', 4, 5, 6, ' ', 7, 8, 9, ' ',
		' ', 0, ' ', ' ' };
unsigned char arrayMapOfPassword[5][4] = { { 1, 2, 3, 4 }, { 2, 7, 8, 9 }, { 3,
		3, 3, 3 }, { 4, 8, 6, 8 }, { 5, 1, 0, 0 }, };
unsigned char arrayPassword[4] = { 0, 0, 0, 0 };
uint16_t statusPassword = 0;
uint16_t indexOfNumber = 0;
uint16_t numberValue;
uint16_t timeDelay = 0;
uint8_t flagOpen = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void init_system();

void AppPasswordDoor();
void UnlockDoor();
void LockDoor();
uint8_t IsButtonEnter();
uint16_t IsButtonNumber();
uint16_t CheckPassword();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
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
	MX_SPI1_Init();
	MX_FSMC_Init();
	MX_TIM4_Init();
	/* USER CODE BEGIN 2 */
	init_system();

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		while (timer2_flag == 0)
			;
		timer2_flag = 0;

		// INPUT
		button_scan();

		// PROCESS
		AppPasswordDoor();

		// OUTPPUT

		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Configure the main internal regulator output voltage
	 */
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 4;
	RCC_OscInitStruct.PLL.PLLN = 168;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 4;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV4;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
		Error_Handler();
	}
}

/* USER CODE BEGIN 4 */
void init_system() {
	HAL_GPIO_WritePin(OUTPUT_Y0_GPIO_Port, OUTPUT_Y0_Pin, 0);
	HAL_GPIO_WritePin(OUTPUT_Y1_GPIO_Port, OUTPUT_Y1_Pin, 0);
	HAL_GPIO_WritePin(DEBUG_LED_GPIO_Port, DEBUG_LED_Pin, 0);

	button_init();
	lcd_init();

	timer2_init();
	timer2_set(50);

	HAL_Delay(1000);
}

uint16_t IsButtonNumber() {
	unsigned char i;
	for (i = 0; i <= 15; i++)
		if (button_count[i] == 1) {
			numberValue = arrayMapOfNumber[i];
			if(numberValue == ' ') return 0;
			return 1;
		}
	return 0;
}

uint16_t CheckPassword() {
	unsigned char i, j;
	unsigned char result = 1;
	for (i = 0; i < 5; i++) {
		result = 1;
		for (j = 0; j < 4; j++) {
			if (arrayPassword[j] != arrayMapOfPassword[i][j])
				result = 0;
		}
		if (result == 1)
			return i + 1;
	}

	return result;
}

void UnlockDoor() {
	if (flagOpen == 0) {
		lcd_show_picture(80, 90, 77, 130, image_door_open_1);
		lcd_show_picture(80, 90, 77, 130, image_door_open_2);
		lcd_show_picture(80, 90, 80, 135, image_door_open_3);
		lcd_show_picture(80, 90, 90, 138, image_door_open_4);
		lcd_show_picture(80, 90, 98, 138, image_door_open_5);
		flagOpen = 1;
	}
	lcd_show_picture(80, 90, 112, 138, image_door_open_6);
}

void LockDoor() {
	lcd_show_picture(80, 100, 77, 130, image_door_close);
}

uint8_t IsButtonEnter() {
	if (button_count[12] == 1)
		return 1;
	else
		return 0;
}

void AppPasswordDoor() {
	switch (statusPassword) {
	case INIT_SYSTEM:
		lcd_show_picture(0, 0, 240, 320, image_background);
		statusPassword = LOCK_DOOR;

		break;

	case LOCK_DOOR:
		lcd_show_picture(25, 40, 180, 49, image_press_enter_text);
		lcd_show_picture(107, 265, 30, 32, image_protect_icon);
		lcd_show_string_center(0, 15, "     LOCK       ", RED, WHITE, 16, 0);
		LockDoor();

		if (IsButtonEnter()) {
			indexOfNumber = 0;
			timeDelay = 0;
			statusPassword = ENTER_PASSWORD;

			lcd_fill(30, 25, 210, 80, WHITE);		//clear text
			lcd_fill(30, 265, 210, 300, WHITE);		//clear protect

			lcd_show_picture(17, 40, 206, 48, image_pass_enter_text);
			lcd_show_picture(17, 235, 206, 30, image_pass_enter);
		}
		break;

	case ENTER_PASSWORD:
		lcd_show_string_center(0, 12, "   ENTER PASS   ", RED, WHITE, 16, 0);
		timeDelay++;
		if (IsButtonNumber()) {
			lcd_show_string(70 + indexOfNumber * 30, 242, "*", BLACK, LIGHTGRAY,
					16, 0);
			arrayPassword[indexOfNumber] = numberValue;
			indexOfNumber++;
			timeDelay = 0;
		}

		if (indexOfNumber >= 4) {
			statusPassword = CHECK_PASSWORD;
		}

		if (timeDelay >= 100) {
			statusPassword = LOCK_DOOR;
			lcd_fill(20, 20, 220, 300, WHITE);
		}
		break;

	case CHECK_PASSWORD:

		timeDelay = 0;
		if (CheckPassword()) {
			lcd_fill(30, 40, 210, 80, WHITE);		//clear text
			lcd_fill(30, 265, 210, 300, WHITE);		//clear protect
			statusPassword = UNLOCK_DOOR;
			timeDelay = 0;
		} else {
			statusPassword = WRONG_PASSWORD;
		}

		break;

	case UNLOCK_DOOR:
		lcd_show_string_center(0, 12, "    UNLOCK     ", RED, WHITE, 16, 0);
		timeDelay++;
		UnlockDoor();
		if (timeDelay >= 100) {
			statusPassword = LOCK_DOOR;
			lcd_fill(20, 20, 220, 300, WHITE);
		}
		break;

	case WRONG_PASSWORD:
		timeDelay++;
		lcd_show_picture(17, 40, 206, 53, image_wrong_pass_text);
		lcd_show_picture(17, 235, 206, 30, image_pass_enter);
		lcd_show_picture(17, 265, 206, 28, image_wrong_icon);

		statusPassword = ENTER_PASSWORD;
		indexOfNumber = 0;

		if (timeDelay >= 100) {
			statusPassword = LOCK_DOOR;
		}
		break;

	default:
		break;
	}
}

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
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
