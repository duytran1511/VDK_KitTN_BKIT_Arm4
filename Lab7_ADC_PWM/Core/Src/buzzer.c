/*
 * buzzer.c
 */

/* Includes */
#include "buzzer.h"
#include "tim.h"

/* Variables */
uint8_t buzzerduty_cycle = 0;

/* Functions */
void buzzer_init() {
	HAL_TIM_PWM_Start(&htim13, TIM_CHANNEL_1);
}

void buzzer_set_volume(uint8_t duty_cycle) {
	buzzerduty_cycle = duty_cycle;
	__HAL_TIM_SET_COMPARE(&htim13, TIM_CHANNEL_1, buzzerduty_cycle);
}
