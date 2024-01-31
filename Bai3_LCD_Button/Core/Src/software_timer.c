/*
 * software_timer.c
 */

/* Includes */
#include "software_timer.h"
#include "tim.h"

/* Variables */
uint8_t timer2_flag = 0;
uint16_t timer2_counter = 0;
uint16_t timer2_mul = 0;

/**
 * @brief  	Init timer interrupt
 * @param  	None
 * @retval 	None
 */
void timer2_init(void) {
	MX_TIM2_Init();
	HAL_TIM_Base_Start_IT(&htim2);
}

/**
 * @brief	Set duration of software timer interrupt
 * @param	duration Duration of software timer interrupt
 * @retval 	None
 */
void timer2_set(int ms) {
	timer2_mul = ms / TIMER_CYCLE_2;
	timer2_counter = timer2_mul;
	timer2_flag = 0;
}

/**
 * @brief  	Timer interrupt routine
 * @param  	htim TIM Base handle
 * @note	This callback function is called by system
 * @retval 	None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM2) {
		if (timer2_counter > 0) {
			timer2_counter--;
			if (timer2_counter == 0) {
				timer2_flag = 1;
				timer2_counter = timer2_mul;
			}
		}
	}
}

