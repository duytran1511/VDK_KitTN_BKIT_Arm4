/*
 * button.c
 */

/* Includes */
#include "button.h"

#include "spi.h"
#include "gpio.h"

/* Variables */
uint16_t button_count[16] = {0};
static uint16_t button_spi_buffer = 0x0000;

/* Functions */
/**
 * @brief  	Init matrix button
 * @param  	None
 * @retval 	None
 */
void button_init() {
	HAL_GPIO_WritePin(BTN_LOAD_GPIO_Port, BTN_LOAD_Pin, 1);
}

/**
 * @brief  	Scan matrix button
 * @param  	None
 * @note  	Call every 50ms
 * @retval 	None
 */
void button_scan() {
	HAL_GPIO_WritePin(BTN_LOAD_GPIO_Port, BTN_LOAD_Pin, 0);
	HAL_GPIO_WritePin(BTN_LOAD_GPIO_Port, BTN_LOAD_Pin, 1);
	HAL_SPI_Receive(&hspi1, (void*) &button_spi_buffer, 2, 10);

	int button_index = 0;
	uint16_t mask = 0x8000;
	for (int i = 0; i < 16; i++) {
		if (i >= 0 && i <= 3) {
			button_index = i + 4;
		} else if (i >= 4 && i <= 7) {
			button_index = 7 - i;
		} else if (i >= 8 && i <= 11) {
			button_index = i + 4;
		} else {
			button_index = 23 - i;
		}
		if (button_spi_buffer & mask)
			button_count[button_index] = 0;
		else
			button_count[button_index]++;
		mask = mask >> 1;
	}
}

