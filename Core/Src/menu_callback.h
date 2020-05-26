/*
 * menu_callback.h
 *
 *  Created on: 28 апр. 2020 г.
 *      Author: user
 */

#ifndef MENU_CALLBACK_H_
#define MENU_CALLBACK_H_
#include "stm32f4xx_hal.h"

extern uint8_t attempt_time;
extern uint8_t kill_read;


void read_sensor_with_calibr(void);
void set_koefficient(void);
void manual_mode(void);
void sensor_check(void);
uint32_t read_sensor_press(void);

#endif /* MENU_CALLBACK_H_ */
