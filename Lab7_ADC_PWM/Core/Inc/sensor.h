/*
 * sensor.h
 */

#ifndef INC_SENSOR_H_
#define INC_SENSOR_H_

/* Includes */
#include <stdint.h>

/* Functions */
void sensor_init();
void sensor_read();

uint16_t sensor_get_light();
uint16_t sensor_get_potentiometer();
float sensor_get_voltage();
float sensor_get_current();
float sensor_get_temperature();

#endif /* INC_SENSOR_H_ */
