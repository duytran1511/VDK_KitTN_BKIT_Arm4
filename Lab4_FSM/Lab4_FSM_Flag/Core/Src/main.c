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
#define	INIT_SYSTEM     0
#define	MOVING_UP       1
#define	MOVING_DOWN     2
#define	STOP_MOVING     3
#define	TOP_POSITION    4
#define	BOTTOM_POSITION 5

#define TOP_POSITION_OF_FLAG 	100
#define BOTTOM_POSITION_OF_FLAG 200
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t statusFlag = INIT_SYSTEM;
uint16_t flag_position = BOTTOM_POSITION_OF_FLAG;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void init_system();

uint8_t IsButtonUp();
uint8_t IsButtonDown();
uint8_t IsButtonStop();
uint8_t IsBottomSwitch();
uint8_t IsTopSwitch();
void FlagMovingDown();
void FlagMovingUp();
void FlagStopMoving();
void BaiTapFlag();
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
		BaiTapFlag();

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

uint8_t IsButtonUp() {
	if (button_count[3] == 1)
		return 1;
	else
		return 0;
}

uint8_t IsButtonDown() {
	if (button_count[7] == 1)
		return 1;
	else
		return 0;
}

uint8_t IsButtonStop() {
	if (button_count[11] == 1)
		return 1;
	else
		return 0;
}

uint8_t IsBottomSwitch() {
	if (flag_position == BOTTOM_POSITION_OF_FLAG)
		return 1;
	else
		return 0;
}

uint8_t IsTopSwitch() {
	if (flag_position == TOP_POSITION_OF_FLAG)
		return 1;
	else
		return 0;
}

void FlagMovingDown() {
	lcd_draw_line(132, flag_position - 1, 222, flag_position - 1, 0x875c);
	lcd_show_picture(132, flag_position, 90, 58, gImage_l_flag);
	flag_position++;
}

void FlagMovingUp() {
	lcd_draw_line(132, flag_position + 58, 222, flag_position + 58, 0x875c);
	lcd_show_picture(132, flag_position, 90, 58, gImage_l_flag);
	flag_position--;
}

void FlagStopMoving() {
	lcd_show_picture(132, flag_position, 90, 58, gImage_l_flag);
}

void BaiTapFlag() {
	switch (statusFlag) {
	case INIT_SYSTEM:
		lcd_clear(0x875c);
		lcd_fill(0, 0, 240, 20, BLUE);
		lcd_show_picture(80, 100, 90, 209, gImage_c_flag);

		statusFlag = BOTTOM_POSITION;
		break;

	case MOVING_UP:
		lcd_show_string_center(0, 2, "   MOVING UP   ", WHITE, BLUE, 16, 0);
		FlagMovingUp();

		if (IsButtonDown()) {
			statusFlag = MOVING_DOWN;
		}

		if (IsTopSwitch()) {
			statusFlag = TOP_POSITION;
		}

		if (IsButtonStop()) {
			statusFlag = STOP_MOVING;
		}
		break;

	case BOTTOM_POSITION:
		lcd_show_string_center(0, 2, "BOTTOM POSITION", WHITE, BLUE, 16, 0);
		FlagStopMoving();

		if (IsButtonUp()) {
			statusFlag = MOVING_UP;
		}
		break;

	case MOVING_DOWN:
		lcd_show_string_center(0, 2, "  MOVING DOWN  ", WHITE, BLUE, 16, 0);
		FlagMovingDown();

		if (IsBottomSwitch()) {
			statusFlag = BOTTOM_POSITION;
		}

		if (IsButtonStop()) {
			statusFlag = STOP_MOVING;
		}
		break;

	case TOP_POSITION:
		lcd_show_string_center(0, 2, "  TOP POSTION  ", WHITE,
		BLUE, 16, 0);
		FlagStopMoving();

		if (IsButtonDown()) {
			statusFlag = MOVING_DOWN;
		}
		break;

	case STOP_MOVING:
		lcd_show_string_center(0, 2, "  STOP MOVING  ", WHITE, BLUE, 16, 0);
		FlagStopMoving();

		if (IsButtonUp()) {
			statusFlag = MOVING_UP;
		}

		if (IsButtonDown()) {
			statusFlag = MOVING_DOWN;
		}
		break;

	default:
		statusFlag = INIT_SYSTEM;
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
