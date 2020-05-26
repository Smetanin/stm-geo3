///*
//        ADS1256.h - Arduino Library for communication with Texas Instrument ADS1256 ADC
//        Written by Adien Akhmad, August 2015
//		Modfified  Jan 2019 by Axel Sepulveda for ATMEGA328
// */
//
//#include "ADS1256.h"
//#include "spi.h"
//
////ADS1256::ADS1256(float vref) {
////	// Voltage Reference
////	_VREF = vref;
////
////	// Default conversion factor
////	_conversionFactor = 1.0;
////
////	// Start SPI on a quarter of ADC clock speed
//////	SPI.begin();
//////	SPI.beginTransaction(
//////			SPISettings(clockspdMhz * 1000000 / 4, MSBFIRST, SPI_MODE1));
////}

//void ADS1256::writeRegister(unsigned char reg, unsigned char wdata) {
//
//	CSON();
//	uint8_t temp_reg=WREG | reg;
//	HAL_SPI_Transmit(&hspi2, &temp_reg,1,1);
//	temp_reg = 0;
//	HAL_SPI_Transmit(&hspi2, &temp_reg,1,1);
//	HAL_SPI_Transmit(&hspi2, &wdata,1,1);
//	HAL_Delay(8);
//	CSOFF();  // t11 delay (4*tCLKIN) after WREG command,
//	// 16Mhz avr clock is approximately twice
//	// faster that 7.68 Mhz ADS1256 master clock
//
//}

//unsigned char ADS1256::readRegister(unsigned char reg) {
//	unsigned char readValue;
//
//
//	CSON();
//	uint8_t temp_reg=(RREG | reg);
//	HAL_SPI_Transmit(&hspi2, &temp_reg,1,1);
//	temp_reg = 0;
//	HAL_SPI_Transmit(&hspi2, &temp_reg,1,1);
//	HAL_Delay(200);
//	HAL_SPI_Receive(&hspi2, &readValue,1,1);
//	HAL_Delay(8);// t11 delay
//	CSOFF();
//	return readValue;  // t6 delay (50*tCLKIN), 16Mhz avr clock is
//	// approximately twice faster that 7.68 Mhz
//	// ADS1256 master clock
//
//}
//
//void ADS1256::sendCommand(unsigned char reg) {
//	CSON();
//	waitDRDY();
//	HAL_SPI_Transmit(&hspi2, &reg,1,1);
//	HAL_Delay(8);
//	CSOFF();
//}
//
//void ADS1256::setConversionFactor(float val) { _conversionFactor = val; }
//
//void ADS1256::readTest() {
//	unsigned char _highByte, _midByte, _lowByte;
//	CSON();
//	uint8_t temp_reg = RDATA;
//	HAL_SPI_Transmit(&hspi2, &temp_reg,1,1);
//	HAL_Delay(200);  // t6 delay
//
//	HAL_SPI_Receive(&hspi2, &_highByte,1, 1);
//	HAL_SPI_Receive(&hspi2,&_midByte,1, 1);
//	HAL_SPI_Receive(&hspi2,&_lowByte,1, 1);
//
//	CSOFF();
//}
//
//float ADS1256::readCurrentChannel() {
//	CSON();
//	uint8_t temp_reg = RDATA;
//	HAL_SPI_Transmit(&hspi2, &temp_reg,1,1);
//	HAL_Delay(200);  // t6 delay
//	float adsCode = read_float32();
//	CSOFF();
//	return ((adsCode / 0x7FFFFF) * ((2 * _VREF) / (float)_pga)) *
//			_conversionFactor;
//}
//
//// Call this ONLY after RDATA command
//unsigned long ADS1256::read_uint24() {
//	uint8_t _highByte, _midByte, _lowByte;
//	CSON();
//	uint8_t temp_reg = RDATA;
//	HAL_SPI_Transmit(&hspi2, &temp_reg,1,1);
//	HAL_Delay(8);
//	HAL_SPI_Receive(&hspi2, &_highByte,1, 1);
//	HAL_SPI_Receive(&hspi2,&_midByte,1, 1);
//	HAL_SPI_Receive(&hspi2,&_lowByte,1, 1);
//	CSOFF();
//	// Combine all 3-bytes to 24-bit data using byte shifting.
//	unsigned long value = ((long)_highByte << 10) + ((long)_midByte << 2) + ((long)_lowByte>>6);
//	if (value & 0x00800000) {   value |= 0xff000000;  }
//	return value;
//}
//
//// Call this ONLY after RDATA command
//long ADS1256::read_int32() {
//	long value = read_uint24();
//
//	if (value & 0x00800000) {
//		value |= 0xff000000;
//	}
//
//	return value;
//}
//
//// Call this ONLY after RDATA command
//float ADS1256::read_float32() {
//	long value = read_int32();
//	return (float)value;
//}
//
//// Channel switching for single ended mode. Negative input channel are
//// automatically set to AINCOM
//void ADS1256::setChannel(uint8_t channel) { setChannel(channel, -1); }
//
//// Channel Switching for differential mode. Use -1 to set input channel to
//// AINCOM
//void ADS1256::setChannel(uint8_t AIN_P, uint8_t AIN_N) {
//	unsigned char MUX_CHANNEL;
//	unsigned char MUXP;
//	unsigned char MUXN;
//
//	switch (AIN_P) {
//	case 0:
//		MUXP = ADS1256_MUXP_AIN0;
//		break;
//	case 1:
//		MUXP = ADS1256_MUXP_AIN1;
//		break;
//	case 2:
//		MUXP = ADS1256_MUXP_AIN2;
//		break;
//	case 3:
//		MUXP = ADS1256_MUXP_AIN3;
//		break;
//	case 4:
//		MUXP = ADS1256_MUXP_AIN4;
//		break;
//	case 5:
//		MUXP = ADS1256_MUXP_AIN5;
//		break;
//	case 6:
//		MUXP = ADS1256_MUXP_AIN6;
//		break;
//	case 7:
//		MUXP = ADS1256_MUXP_AIN7;
//		break;
//	default:
//		MUXP = ADS1256_MUXP_AINCOM;
//	}
//
//	switch (AIN_N) {
//	case 0:
//		MUXN = ADS1256_MUXN_AIN0;
//		break;
//	case 1:
//		MUXN = ADS1256_MUXN_AIN1;
//		break;
//	case 2:
//		MUXN = ADS1256_MUXN_AIN2;
//		break;
//	case 3:
//		MUXN = ADS1256_MUXN_AIN3;
//		break;
//	case 4:
//		MUXN = ADS1256_MUXN_AIN4;
//		break;
//	case 5:
//		MUXN = ADS1256_MUXN_AIN5;
//		break;
//	case 6:
//		MUXN = ADS1256_MUXN_AIN6;
//		break;
//	case 7:
//		MUXN = ADS1256_MUXN_AIN7;
//		break;
//	default:
//		MUXN = ADS1256_MUXN_AINCOM;
//	}
//
//	MUX_CHANNEL = MUXP | MUXN;
//
//	CSON();
//	writeRegister(MUX, MUX_CHANNEL);
//	sendCommand(SYNC);
//	sendCommand(WAKEUP);
//	CSOFF();
//}
//
//void ADS1256::begin(unsigned char drate, unsigned char gain, bool buffenable) {
//	_pga = 1 << gain;
//	sendCommand(
//			SDATAC);  // send out SDATAC command to stop continous reading mode.
//	writeRegister(DRATE, drate);  // write data rate register
//	uint8_t bytemask = 0b00000111;
//	uint8_t adcon = readRegister(ADCON);
//	uint8_t byte2send = (adcon & ~bytemask) | gain;
//	writeRegister(ADCON, byte2send);
//	if (buffenable) {
//		uint8_t status = readRegister(STATUS);
//		status |= 0x10;
//		writeRegister(STATUS, status);
//	}
//	sendCommand(SELFCAL);  // perform self calibration
//	waitDRDY();
//	;  // wait ADS1256 to settle after self calibration
//}
//
//void ADS1256::CSON() {
//	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
//}  // digitalWrite(_CS, LOW); }
//
//void ADS1256::CSOFF() {
//	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET);
//}  // digitalWrite(_CS, HIGH); }

//void ADS1256::waitDRDY() {
//	while (1) {
//		if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_0) == GPIO_PIN_RESET) break;
//	}
//}
