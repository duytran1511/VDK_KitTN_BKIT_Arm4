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
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"
#include "fsmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "software_timer.h"
#include "lcd.h"
#include "ds3231.h"
#include "button.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define INIT_SYSTEM     0
#define SET_HOUR        1
#define SET_MINUTE      2
#define SET_DAY         3
#define SET_DATE        4
#define SET_MONTH       5
#define SET_YEAR        6
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
int statusSetupTime = INIT_SYSTEM;
int timeBlink = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void system_init();
void DisplayTime();
void UpdateTime();
unsigned char IsButtonMode();
unsigned char IsButtonUp();
unsigned char IsButtonDown();
void SetHour();
void SetMinute();
void SetDay();
void SetDate();
void SetMonth();
void SetYear();
void SetUpTime();
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
	MX_I2C1_Init();
	MX_TIM4_Init();
	/* USER CODE BEGIN 2 */
	system_init();
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	lcd_clear(BLACK);
	UpdateTime();

	while (1) {
		while (!timer2_flag)
			;
		timer2_flag = 0;

		ds3231_read_time();

		DisplayTime();
		SetUpTime();
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
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLM = 8;
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
void system_init() {
	HAL_GPIO_WritePin(OUTPUT_Y0_GPIO_Port, OUTPUT_Y0_Pin, 0);
	HAL_GPIO_WritePin(OUTPUT_Y1_GPIO_Port, OUTPUT_Y1_Pin, 0);
	HAL_GPIO_WritePin(DEBUG_LED_GPIO_Port, DEBUG_LED_Pin, 0);

	lcd_init();
	ds3231_init();

	timer2_init();
	timer2_set(50);
}

void UpdateTime() {
	ds3231_write(ADDRESS_YEAR, 23);
	ds3231_write(ADDRESS_MONTH, 10);
	ds3231_write(ADDRESS_DATE, 20);
	ds3231_write(ADDRESS_DAY, 6);
	ds3231_write(ADDRESS_HOUR, 20);
	ds3231_write(ADDRESS_MIN, 11);
	ds3231_write(ADDRESS_SEC, 23);
}

void DisplayTime()
{
	if(statusSetupTime == INIT_SYSTEM) ds3231_read_time();
	if(statusSetupTime != SET_HOUR || (statusSetupTime == SET_HOUR && timeBlink >= 5)){
		lcd_show_int_num(70, 100, ds3231_hours/10, 1, GREEN, BLACK, 24);
		lcd_show_int_num(83, 100, ds3231_hours%10, 1, GREEN, BLACK, 24);
	}

	lcd_show_char(96, 100, ':', GREEN, BLACK, 24, 0);
	lcd_show_int_num(110, 100, ds3231_min/10, 1, GREEN, BLACK, 24);
	lcd_show_int_num(123, 100, ds3231_min%10, 1, GREEN, BLACK, 24);
	lcd_show_char(136, 100, ':', GREEN, BLACK, 24, 0);
	lcd_show_int_num(150, 100, ds3231_sec/10, 1, GREEN, BLACK, 24);
	lcd_show_int_num(163, 100, ds3231_sec%10, 1, GREEN, BLACK, 24);

    //////day
    switch(ds3231_day)
    {
        case 1:
        	lcd_show_string(20, 130, "SUN", YELLOW, BLACK, 24, 0);
            break;
        case 2:
        	lcd_show_string(20, 130, "MON", YELLOW, BLACK, 24, 0);
            break;
        case 3:
        	lcd_show_string(20, 130, "TUE", YELLOW, BLACK, 24, 0);
            break;
        case 4:
        	lcd_show_string(20, 130, "WED", YELLOW, BLACK, 24, 0);
            break;
        case 5:
        	lcd_show_string(20, 130, "THU", YELLOW, BLACK, 24, 0);
            break;
        case 6:
        	lcd_show_string(20, 130, "FRI", YELLOW, BLACK, 24, 0);
            break;
        case 7:
        	lcd_show_string(20, 130, "SAT", YELLOW, BLACK, 24, 0);
            break;
    }


    switch(ds3231_month)
    {
        case 1:
        	lcd_show_string(105, 130, "JAN", YELLOW, BLACK, 24, 0);
            break;
        case 2:
        	lcd_show_string(105, 130, "FEB", YELLOW, BLACK, 24, 0);
            break;
        case 3:
        	lcd_show_string(105, 130, "MAR", YELLOW, BLACK, 24, 0);
            break;
        case 4:
        	lcd_show_string(105, 130, "APR", YELLOW, BLACK, 24, 0);
            break;
        case 5:
        	lcd_show_string(105, 130, "MAY", YELLOW, BLACK, 24, 0);
            break;
        case 6:
        	lcd_show_string(105, 130, "JUN", YELLOW, BLACK, 24, 0);
            break;
        case 7:
        	lcd_show_string(105, 130, "JUL", YELLOW, BLACK, 24, 0);
            break;
        case 8:
        	lcd_show_string(105, 130, "AUG", YELLOW, BLACK, 24, 0);
            break;
        case 9:
        	lcd_show_string(105, 130, "SEP", YELLOW, BLACK, 24, 0);
            break;
        case 10:
        	lcd_show_string(105, 130, "OCT", YELLOW, BLACK, 24, 0);
            break;
        case 11:
        	lcd_show_string(105, 130, "NOV", YELLOW, BLACK, 24, 0);
            break;
        case 12:
        	lcd_show_string(105, 130, "DEC", YELLOW, BLACK, 24, 0);
            break;
    }

    lcd_show_int_num(70, 130, ds3231_date, 2, YELLOW, BLACK, 24);

	lcd_show_int_num(150, 130, 20, 2, YELLOW, BLACK, 24);
	lcd_show_int_num(176, 130, ds3231_year, 2, YELLOW, BLACK, 24);

}

void SetUpTime()
{
    switch(statusSetupTime)
    {
        case INIT_SYSTEM:
            if(IsButtonMode())
                statusSetupTime = SET_HOUR;
            break;
        case SET_HOUR:
            SetHour();
            if(IsButtonMode())
                statusSetupTime = SET_MINUTE;
            break;
        case SET_MINUTE:
            SetMinute();
            if(IsButtonMode())
                statusSetupTime = SET_DAY;
            break;
        case SET_DAY:
            SetDay();
            if(IsButtonMode())
                statusSetupTime = SET_DATE;
            break;
        case SET_DATE:
            SetDate();
            if(IsButtonMode())
                statusSetupTime = SET_MONTH;
            break;
        case SET_MONTH:
            SetMonth();
            if(IsButtonMode())
                statusSetupTime = SET_YEAR;
            break;
        case SET_YEAR:
            SetYear();
            if(IsButtonMode())
                statusSetupTime = INIT_SYSTEM;
            break;
        default:
            statusSetupTime = INIT_SYSTEM;
            break;


    }
}
//
unsigned char IsButtonMode()
{
    if (button_count[4] == 1)
        return 1;
    else
        return 0;
}

unsigned char IsButtonUp()
{
    if ((button_count[5] == 1) || (button_count[5] >= 10 && button_count[5]%4 == 0))
        return 1;
    else
        return 0;
}

unsigned char IsButtonDown()
{
    if ((button_count[9] == 1) || (button_count[9] >= 10 && button_count[9]%4 == 0))
        return 1;
    else
        return 0;
}

void SetHour()
{
    timeBlink = (timeBlink + 1)%20;
    if(timeBlink < 5)
    	lcd_show_string(70, 100, "  ", GREEN, BLACK, 24, 0);
    if(IsButtonUp())
    {
        ds3231_hours++;
        if(ds3231_hours > 23)
            ds3231_hours = 0;
        ds3231_write(ADDRESS_HOUR, ds3231_hours);
    }
    if(IsButtonDown())
    {
        ds3231_hours--;
        if(ds3231_hours < 0)
            ds3231_hours = 23;
        ds3231_write(ADDRESS_HOUR, ds3231_hours);
    }
}
void SetMinute()
{

}
void SetDay()
{
}
void SetDate()
{
}
void SetMonth()
{
}
void SetYear()
{
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
