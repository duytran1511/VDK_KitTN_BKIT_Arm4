/*
 * sensor.c
 */

/* Includes */
#include "sensor.h"
#include "adc.h"

/* Variables */
uint16_t adc_receive[5];

/* Functions */
void sensor_init() {
	HAL_ADC_Start_DMA(&hadc1, (uint16_t*) adc_receive, 5);
}

void sensor_read() {
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*) adc_receive, 5);
}

uint16_t sensor_get_light() {
	return adc_receive[2];
}

uint16_t sensor_get_potentiometer() {
	return adc_receive[3];
}

float sensor_get_voltage() {
	return ((float) adc_receive[0] * 3.3 * 12) / (4095 * 1.565);
}

float sensor_get_current() {
	return (((float) adc_receive[1] * 3.3 * 1000) / (4095 * 0.647) - 2.5) * 5
			/ 2.5;
}

float sensor_get_temperature() {
	return ((float) adc_receive[4] * 330) / (4095);
}

