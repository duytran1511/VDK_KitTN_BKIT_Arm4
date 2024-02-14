/*
 * uart.h
 */

#ifndef INC_UART_H_
#define INC_UART_H_

/* Includes */
#include "usart.h"
#include <stdio.h>
#include "utils.h"

/* Functions */
void uart_init_rs232();

void uart_rs232_send_string(uint8_t* str);
void uart_rs232_send_bytes(uint8_t* bytes, uint16_t size);
void uart_rs232_send_num(uint32_t num);
void uart_rs232_send_num_percent(uint32_t num);

#endif /* INC_UART_H_ */

