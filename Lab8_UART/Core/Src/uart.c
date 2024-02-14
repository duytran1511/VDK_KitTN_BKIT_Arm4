/*
 * uart.c
 */

/* Includes */
#include "uart.h"

/* Variables */
uint8_t receive_buffer1 = 0;
uint8_t msg[100];

/* Functions */
uint32_t mypow(uint8_t m, uint8_t n) {
	uint32_t result = 1;
	while (n--)
		result *= m;
	return result;
}

void uart_init_rs232() {
	HAL_UART_Receive_IT(&huart1, &receive_buffer1, 1);
}

void uart_rs232_send_string(uint8_t *str) {
	HAL_UART_Transmit(&huart1, (void*) msg, sprintf((void*) msg, "%s", str),
			10);
}

void uart_rs232_send_bytes(uint8_t *bytes, uint16_t size) {
	HAL_UART_Transmit(&huart1, bytes, size, 10);
}

void uart_rs232_send_num(uint32_t num) {
	if (num == 0) {
		uart_rs232_send_string("0");
		return;
	}
	uint8_t num_flag = 0;
	int i;
	if (num < 0)
		uart_rs232_send_string("-");
	for (i = 10; i > 0; i--) {
		if ((num / mypow(10, i - 1)) != 0) {
			num_flag = 1;
			sprintf((void*) msg, "%d", num / mypow(10, i - 1));
			uart_rs232_send_string(msg);
		} else {
			if (num_flag != 0)
				uart_rs232_send_string("0");
		}
		num %= mypow(10, i - 1);
	}
}

void uart_rs232_send_num_percent(uint32_t num) {
	sprintf((void*) msg, "%ld", num / 100);
	uart_rs232_send_string(msg);
	uart_rs232_send_string(".");
	sprintf((void*) msg, "%ld", num % 100);
	uart_rs232_send_string(msg);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart->Instance == USART1) {
		// rs232 isr
		// can be modified
		HAL_UART_Transmit(&huart1, &receive_buffer1, 1, 10);

		// turn on the receice interrupt
		HAL_UART_Receive_IT(&huart1, &receive_buffer1, 1);
	}
}

