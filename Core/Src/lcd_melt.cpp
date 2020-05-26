/*
 * lcd_melt.cpp
 *
 *  Created on: 5 февр. 2020 г.
 *      Author: user
 */
#include "stm32f4xx_hal.h"
#include "lcd_melt.h"
#include <string>


void LCD_ini(void)

{

	HAL_Delay(40);

	rs0;

	LCD_WriteData(3);

	e1;

	HAL_Delay(4);

	e0;

	HAL_Delay(1);

	LCD_WriteData(3);

	e1;

	HAL_Delay(1);

	e0;

	HAL_Delay(1);

	LCD_WriteData(3);

	e1;

	HAL_Delay(1);

	e0;

	HAL_Delay(1);

	LCD_Command(0x28);//режим 4 бит, 2 линии (для нашего большого дисплея это 4 линии), шрифт 5х8

	HAL_Delay(1);

	LCD_Command(0x28);//еще раз для верности

	HAL_Delay(1);

	LCD_Command(0x0F);//дисплей включаем (D=1), также включаем пока все курсоры

	HAL_Delay(1);

	LCD_Command(0x01);//уберем мусор

	HAL_Delay(2);

	LCD_Command(0x06);//пишем влево.

	HAL_Delay(1);

	LCD_Command(0x02);//возврат курсора в нулевое положение

	HAL_Delay(2);
	LCD_Command(0b101010); //переключаем на 1ю страницу знакогенератора
	HAL_Delay(2);




}

//------------------------------------------------
void delay(void)
{
	uint16_t i;
	for(i=0;i<1000;i++)
	{

	}
}
//------------------------------------------------
void LCD_WriteData(uint8_t dt)
{
	if(((dt>>3)&0x01)==1) {d7_set();} else {d7_reset();}
	if(((dt>>2)&0x01)==1) {d6_set();} else {d6_reset();}
	if(((dt>>1)&0x01)==1) {d5_set();} else {d5_reset();}
	if((dt&0x01)==1) {d4_set();} else {d4_reset();}
}
//------------------------------------------------
void LCD_Command(uint8_t dt)
{
	rs0;
	LCD_WriteData(dt>>4);
	e1;
	delay();
	e0;
	LCD_WriteData(dt);
	e1;
	delay();
	e0;
}
//------------------------------------------------
void LCD_Data(uint8_t dt)
{
	rs1;
	LCD_WriteData(dt>>4);
	e1;
	delay();
	e0;
	LCD_WriteData(dt);
	e1;
	delay();
	e0;
}
//------------------------------------------------
void LCD_Clear(void)
{
	LCD_Command(0x01);
	HAL_Delay(2);
}
//------------------------------------------------
void LCD_SendChar(char ch)
{
	LCD_Data((uint8_t)ch);
	delay();
}
//------------------------------------------------
void LCD_StringRU(std::string str)
{
	HAL_Delay(1);
	uint8_t buf,i=0,rus=0;
	while(str[i]!=0)
	{
		buf = (uint8_t)str[i];
		if(buf<0xCF || buf>0xDF) //костыль отбрасывает лишний байт русских символов
		{
			if(buf>=0x80 && rus)
			{
				if(buf<0x90) // 0x20 - пробел, это тоже костыль
				{
					buf+=0x70; //если прибавить это магическое число, то код русского символа, совпадет с кодом в таблице дисплея
				}
				else
				{
					buf+=0x30; // тоже самое только для маленьких букв
				}
			}
			LCD_Data(buf);
			HAL_Delay(1);
			rus = 0;
		}
		else
		{
			rus = 1;
		}


		i++;
	}
}
//------------------------------------------------
//------------------------------------------------
void LCD_StringRU(char* st)
{
	HAL_Delay(1);
	uint8_t buf,i=0,rus=0;
	while(st[i]!=0)
	{
		buf = (uint8_t)st[i];
		if(buf<0xCF || buf>0xDF) //костыль отбрасывает лишний байт русских символов
		{
			if(buf>=0x80 && rus)
			{
				if(buf<0x90) // 0x20 - пробел, это тоже костыль
				{
					buf+=0x70; //если прибавить это магическое число, то код русского символа, совпадет с кодом в таблице дисплея
				}
				else
				{
					buf+=0x30; // тоже самое только для маленьких букв
				}
			}
			LCD_Data(buf);
			HAL_Delay(1);
			rus = 0;
		}
		else
		{
			rus = 1;
		}


		i++;
	}
}
//------------------------------------------------
void LCD_String(char* st)
{
	uint8_t i=0;
	while(st[i]!=0)
	{
		LCD_Data(st[i]);
		HAL_Delay(1);
		i++;
	}
}
//------------------------------------------------
void LCD_SetPos(uint8_t x, uint8_t y)
{
	HAL_Delay(1);
	switch(y)
	{
	case 0:
		LCD_Command(x|0x80);
		HAL_Delay(1);
		break;
	case 1:
		LCD_Command((0x40+x)|0x80);
		HAL_Delay(1);
		break;
	case 2:
		LCD_Command((0x14+x)|0x80);
		HAL_Delay(1);
		break;
	case 3:
		LCD_Command((0x54+x)|0x80);
		HAL_Delay(1);
		break;
	}
}
//------------------------------------------------







