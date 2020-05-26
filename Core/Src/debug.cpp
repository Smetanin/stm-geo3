/*
 * debug.cpp
 *
 *  Created on: 28 апр. 2020 г.
 *      Author: user
 */
#include "stm32f4xx_hal.h"
#include "main.h"
#include "lcd_menu.h"
#include <string>
#include <cstring>

void debug(char* output)
{
	//sprintf(output,"enc=%d \r\n",encoder);
	int size=0;
	for(;size<255;size++)
	{
		if(output[size]=='\n')
		{
			size++;
			break;
		}
	}
	//int size = strlen(output);
	HAL_UART_Transmit(&huart2,(uint8_t*)output,size,1);
}

void row_debug(Row* row)
{
	std::string name = row->payload.name + "\n";
	char ch[name.size() + 1];
	strcpy(ch,name.c_str());
	debug(ch);
}
