/*
 * software_timer.h
 */

#ifndef INC_SOFTWARE_TIMER_H_
#define INC_SOFTWARE_TIMER_H_

/* Includes */
#include "stdint.h"

/* Includes */
#define TIMER_CYCLE_2 1
#define TIMER_CYCLE_4 1

/* Variables */
extern uint8_t timer2_flag;
extern uint8_t timer4_flag;

/* Functions */
extern void timer2_init(void);
extern void timer4_init(void);

extern void timer2_set(int ms);
extern void timer4_set(int ms);

#endif /* INC_SOFTWARE_TIMER_H_ */
