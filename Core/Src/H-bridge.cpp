/*
 * H-bridge.cpp
 *
 *  Created on: 28 апр. 2020 г.
 *      Author: user
 */
#include "stm32f4xx_hal.h"
#include "main.h"


uint8_t rotationTime = 0;


void PositiveTurn()
{
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_9, GPIO_PIN_RESET);
}

void NegativeTurn()
{
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_9, GPIO_PIN_SET);
}

void StopDvig()
{
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_9, GPIO_PIN_RESET);
}



void motor_move(int speed)
{

	switch(speed)
	{
	case 0:
		StopDvig();

		rotationTime = 0;

		HAL_TIM_Base_Stop_IT(&htim4); // запуск таймера
		break;

	case 2:

		if(rotationTime == 0)
		{
			HAL_TIM_Base_Start_IT(&htim4);
			PositiveTurn();
		}
		if (rotationTime > 1) //крутим движ 100 тиков, не крутим 1100 тиков
		{
			StopDvig();
			//HAL_TIM_Base_Stop_IT(&htim4);
			//rotationTime = 0;
		}
		if (rotationTime > 9) //крутим движ 100 тиков, не крутим 1100 тиков
		{
			StopDvig();
			HAL_TIM_Base_Stop_IT(&htim4);
			rotationTime = 0;
		}


		break;

	case 4:
		if(rotationTime == 0)
		{
			HAL_TIM_Base_Start_IT(&htim4);
			PositiveTurn();
		}
		if (rotationTime > 20) //крутим движ 100 тиков, не крутим 1100 тиков
		{
			StopDvig();
			//HAL_TIM_Base_Stop_IT(&htim4);
			//rotationTime = 0;
		}
		if (rotationTime > 99) //крутим движ 100 тиков, не крутим 1100 тиков
		{
			StopDvig();
			HAL_TIM_Base_Stop_IT(&htim4);
			rotationTime = 0;
		}

		break;

	case 6:
		PositiveTurn();

		break;

	case -2:
		if(rotationTime == 0)
		{
			HAL_TIM_Base_Start_IT(&htim4);
			NegativeTurn();
		}
		if (rotationTime > 1) //крутим движ 100 тиков, не крутим 1100 тиков
		{
			StopDvig();
			//HAL_TIM_Base_Stop_IT(&htim4);
			//rotationTime = 0;
		}
		if (rotationTime > 9) //крутим движ 100 тиков, не крутим 1100 тиков
		{
			StopDvig();
			HAL_TIM_Base_Stop_IT(&htim4);
			rotationTime = 0;
		}


		break;

	case -4:
		if(rotationTime == 0)
		{
			HAL_TIM_Base_Start_IT(&htim4);
			NegativeTurn();
		}
		if (rotationTime > 20) //крутим движ 100 тиков, не крутим 1100 тиков
		{
			StopDvig();
			//HAL_TIM_Base_Stop_IT(&htim4);
			//rotationTime = 0;
		}
		if (rotationTime > 99) //крутим движ 100 тиков, не крутим 1100 тиков
		{
			StopDvig();
			HAL_TIM_Base_Stop_IT(&htim4);
			rotationTime = 0;
		}


		break;

	case -6:
		NegativeTurn();

		break;
	}
}
