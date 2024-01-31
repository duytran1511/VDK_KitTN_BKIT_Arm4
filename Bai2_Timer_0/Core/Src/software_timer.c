/*
 * software_timer.c
 */

/* Includes */
#include "software_timer.h"
#include "tim.h"

#include "led_7seg.h"

/* Variables */
uint8_t timer2_flag = 0;
uint16_t timer2_counter = 0;
uint16_t timer2_mul = 0;

uint8_t timer3_flag = 0;
uint16_t timer3_counter = 0;
uint16_t timer3_mul = 0;

uint8_t timer4_flag = 0;
uint16_t timer4_counter = 0;
uint16_t timer4_mul = 0;

/**
 * @brief  	Init timer interrupt
 * @param  	None
 * @retval 	None
 */
void timer2_init(void) {
	HAL_TIM_Base_Start_IT(&htim2);
}

void timer3_init(void) {
	HAL_TIM_Base_Start_IT(&htim3);
}

void timer4_init(void) {
	HAL_TIM_Base_Start_IT(&htim4);
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

void timer3_set(int ms) {
	timer3_mul = ms / TIMER_CYCLE_3;
	timer3_counter = timer3_mul;
	timer3_flag = 0;
}

void timer4_set(int ms) {
	timer4_mul = ms / TIMER_CYCLE_4;
	timer4_counter = timer4_mul;
	timer4_flag = 0;
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

	if (htim->Instance == TIM3) {
		if (timer3_counter > 0) {
			timer3_counter--;
			if (timer3_counter == 0) {
				timer3_flag = 1;
				timer3_counter = timer3_mul;
			}
		}
	}

	if (htim->Instance == TIM4) {
		if (timer4_counter > 0) {
			timer4_counter--;
			if (timer4_counter == 0) {
				timer4_flag = 1;
				timer4_counter = timer4_mul;
			}
		}

		led_7seg_display();
	}
}

