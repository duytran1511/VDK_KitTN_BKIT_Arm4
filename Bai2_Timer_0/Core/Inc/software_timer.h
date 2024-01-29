/*
 * software_timer.h
 */

#ifndef INC_SOFTWARE_TIMER_H_
#define INC_SOFTWARE_TIMER_H_

/* Includes */
#include "stdint.h"

/* Includes */
#define TIMER_CYCLE_2 1
#define TIMER_CYCLE_3 1
#define TIMER_CYCLE_4 1

/* Variables */
extern uint8_t timer2_flag;
extern uint8_t timer3_flag;
extern uint8_t timer4_flag;

/* Functions */
void timer2_init(void);
void timer3_init(void);
void timer4_init(void);

void timer2_set(int ms);
void timer3_set(int ms);
void timer4_set(int ms);

#endif /* INC_SOFTWARE_TIMER_H_ */
