/*
 * lcd_melt.h
 *
 *  Created on: 5 февр. 2020 г.
 *      Author: user
 */

#ifndef LCD_MELT_H_
#define LCD_MELT_H_

#include "stm32f4xx_hal.h"
#include <string>



#define d4_set() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET)

#define d5_set() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET)

#define d6_set() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET)

#define d7_set() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET)

#define d4_reset() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET)

#define d5_reset() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET)

#define d6_reset() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET)

#define d7_reset() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET)



#define e1    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET) // установка линии E в 1

#define e0    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET) // установка линии E в 0

#define rs1   HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET) // установка линии RS в 1 (данные)

#define rs0   HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET) // установка линии RS в 0 (команда)

//—————————————-



//------------------------------------------------
void LCD_ini(void);
void LCD_Clear(void);
void LCD_SendChar(char ch);
void LCD_String(char* st);
void LCD_StringRU(char* st);
void LCD_StringRU(std::string str);
void LCD_SetPos(uint8_t x, uint8_t y);

void delay(void);
void LCD_WriteData(uint8_t dt);
void LCD_Command(uint8_t dt);
void LCD_Data(uint8_t dt);

//------------------------------------------------




#endif /* LCD_MELT_H_ */
