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
extern uint8_t flag_timer2;
extern uint8_t flag_timer3;
extern uint8_t flag_timer4;

/* Functions */
void init_timer2(void);
void init_timer3(void);
void init_timer4(void);

void set_timer2(int ms);
void set_timer3(int ms);
void set_timer4(int ms);

#endif /* INC_SOFTWARE_TIMER_H_ */
