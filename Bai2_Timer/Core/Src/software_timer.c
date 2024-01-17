/*
 * software_timer.c
 */

/* Includes */
#include "software_timer.h"
#include "tim.h"

/* Variables */
uint8_t flag_timer2 = 0;
uint16_t timer2_counter = 0;
uint16_t timer2_mul = 0;

uint8_t flag_timer3 = 0;
uint16_t timer3_counter = 0;
uint16_t timer3_mul = 0;

uint8_t flag_timer4 = 0;
uint16_t timer4_counter = 0;
uint16_t timer4_mul = 0;

/**
 * @brief  	Init timer interrupt
 * @param  	None
 * @retval 	None
 */
void init_timer2(void) {
	MX_TIM2_Init();
	HAL_TIM_Base_Start_IT(&htim2);
}

void init_timer3(void) {
	MX_TIM3_Init();
	HAL_TIM_Base_Start_IT(&htim3);
}

void init_timer4(void) {
	MX_TIM4_Init();
	HAL_TIM_Base_Start_IT(&htim4);
}

/**
 * @brief	Set duration of software timer interrupt
 * @param	duration Duration of software timer interrupt
 * @retval 	None
 */
void set_timer2(int ms) {
	timer2_mul = ms / TIMER_CYCLE_2;
	timer2_counter = timer2_mul;
	flag_timer2 = 0;
}

void set_timer3(int ms) {
	timer3_mul = ms / TIMER_CYCLE_3;
	timer3_counter = timer3_mul;
	flag_timer3 = 0;
}

void set_timer4(int ms) {
	timer4_mul = ms / TIMER_CYCLE_4;
	timer4_counter = timer4_mul;
	flag_timer4 = 0;
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
				flag_timer2 = 1;
				timer2_counter = timer2_mul;
			}
		}
	}

	if (htim->Instance == TIM3) {
		if (timer3_counter > 0) {
			timer3_counter--;
			if (timer3_counter == 0) {
				flag_timer3 = 1;
				timer3_counter = timer3_mul;
			}
		}
	}

	if (htim->Instance == TIM4) {
		if (timer4_counter > 0) {
			timer4_counter--;
			if (timer4_counter == 0) {
				flag_timer4 = 1;
				timer4_counter = timer4_mul;
			}
		}
	}
}

