/*
 * ADC.cpp
 *
 *  Created on: 28 апр. 2020 г.
 *      Author: user
 */
#include "ADC.h"
#include "menu_callback.h"

uint32_t linear_sensor_raw;
uint32_t linear_sensor_rand;
uint32_t linear_sensor;
uint32_t pressure_sensor;
uint32_t TimeN;



//
void ADC_init()
{
	begin(ADS1256_DRATE_15SPS,ADS1256_GAIN_1);
	sendCommand(SDATAC);
	waitDRDY();
	setChannel(0,1);

	waitDRDY();
	setChannel1(7);
}


void setChannel1(char channel) { setChannel( -1,channel); }

void setChannel(char AIN_P, char AIN_N)
{
	uint8_t MUX_CHANNEL;
	uint8_t MUXP;
	uint8_t MUXN;

	switch (AIN_P) {
	case 0:
		MUXP = ADS1256_MUXP_AIN0;
		break;
	case 1:
		MUXP = ADS1256_MUXP_AIN1;
		break;
	case 2:
		MUXP = ADS1256_MUXP_AIN2;
		break;
	case 3:
		MUXP = ADS1256_MUXP_AIN3;
		break;
	case 4:
		MUXP = ADS1256_MUXP_AIN4;
		break;
	case 5:
		MUXP = ADS1256_MUXP_AIN5;
		break;
	case 6:
		MUXP = ADS1256_MUXP_AIN6;
		break;
	case 7:
		MUXP = ADS1256_MUXP_AIN7;
		break;
	default:
		MUXP = ADS1256_MUXP_AINCOM;
	}

	switch (AIN_N) {
	case 0:
		MUXN = ADS1256_MUXN_AIN0;
		break;
	case 1:
		MUXN = ADS1256_MUXN_AIN1;
		break;
	case 2:
		MUXN = ADS1256_MUXN_AIN2;
		break;
	case 3:
		MUXN = ADS1256_MUXN_AIN3;
		break;
	case 4:
		MUXN = ADS1256_MUXN_AIN4;
		break;
	case 5:
		MUXN = ADS1256_MUXN_AIN5;
		break;
	case 6:
		MUXN = ADS1256_MUXN_AIN6;
		break;
	case 7:
		MUXN = ADS1256_MUXN_AIN7;
		break;
	default:
		MUXN = ADS1256_MUXN_AINCOM;
	}

	MUX_CHANNEL = MUXP | MUXN;

	CSON();
	writeRegister(MUX, MUX_CHANNEL);
	sendCommand(SYNC);
	sendCommand(WAKEUP);
	CSOFF();
}

void begin(uint8_t drate, uint8_t gain)
{
	sendCommand(SDATAC);
	writeRegister(DRATE, drate);
	uint8_t bytemask = 0b000001111;
	uint8_t adcon = readRegister(ADCON);
	uint8_t byte2send = (adcon & ~bytemask) | gain;
	writeRegister(ADCON, byte2send);
	sendCommand(SELFCAL);
	waitDRDY();
}

void CSON()
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
}

void CSOFF()
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET);
}

void waitDRDY()
{
	while (1) {
		if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_0) == GPIO_PIN_RESET) break;
	}
}

void writeRegister(uint8_t reg, uint8_t wdata)
{
	CSON();
	uint8_t musor=WREG | reg;
	HAL_SPI_Transmit(&hspi2, &musor,1,1);
	musor = 0;
	HAL_SPI_Transmit(&hspi2, &musor,1,1);
	HAL_SPI_Transmit(&hspi2, &wdata,1,1);
	HAL_Delay(8);
	CSOFF();
}

uint8_t readRegister(uint8_t reg)
{
	uint8_t readValue;
	CSON();
	uint8_t musor=(RREG | reg);
	HAL_SPI_Transmit(&hspi2, &musor,1,1);
	musor = 0;
	HAL_SPI_Transmit(&hspi2, &musor,1,1);
	HAL_Delay(8);
	HAL_SPI_Receive(&hspi2, &readValue,1,1);
	HAL_Delay(8);
	CSOFF();
	return readValue;
}

void sendCommand(uint8_t reg)
{
	CSON();
	waitDRDY();
	HAL_SPI_Transmit(&hspi2, &reg,1,1);
	HAL_Delay(8);
	CSOFF();
}



unsigned long readCurrentChannel ()
{
	uint8_t _highByte, _midByte, _lowByte;
	CSON();
	uint8_t musor = RDATA;
	HAL_SPI_Transmit(&hspi2, &musor,1,1);
	HAL_Delay(16);
	HAL_SPI_Receive(&hspi2, &_highByte,1, 1);
	HAL_SPI_Receive(&hspi2,&_midByte,1, 1);
	HAL_SPI_Receive(&hspi2,&_lowByte,1, 1);
	CSOFF();
	unsigned long value = ((long)_highByte << 16) + ((long)_midByte << 8) + ((long)_lowByte);
	if (value & 0x00800000) {   value |= 0xff000000;  }
	return value;
}



