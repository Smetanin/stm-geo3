///*
// * encoder.h
// *
// *  Created on: 28 апр. 2020 г.
// *      Author: user
// */
//
#ifndef ENCODER_H_
#define ENCODER_H_

#include "stm32f4xx_hal.h"
#include "GyverEncoder.h"
//
#define FA_SEEKEND	0x20
#define KEY_UP				1
#define KEY_DOWN			2
#define KEY_RIGHT			3
#define KEY_LEFT			4
#define KEY_OK				5



extern int encoder;
extern uint8_t ok_button;
extern uint8_t reset_button;
extern Encoder enc1;

void tik_callback(void);
void read_encoder(void);
uint16_t read_encoder_set_data(uint8_t x, uint8_t y);
void encoder_clear(void);
uint8_t keyMenu(uint8_t param);
void tik_callback_x2(void);
//
#endif /* ENCODER_H_ */
