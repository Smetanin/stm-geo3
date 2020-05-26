/*
  ADS1232.h - Library for TI ADS1232
  24-Bit Analog-to-Digital Converter For Bridge Sensors
  Created by Sorin Ciorceri, 2017.
  Released into the public domain.
*/

#ifndef ADS1232_h
#define ADS1232_h
#include "stm32f4xx_hal.h"


	uint16_t PDWN;         // power down pin //D12 - PA6
    uint16_t SCLK;         // serial clock pin  //D11 - PA7
    uint16_t DOUT;			// serial data out pin //D8 - PA9
    GPIO_TypeDef* PORT;


    void ads_init(uint16_t pdwn, uint16_t sclk, uint16_t dout, GPIO_TypeDef* port);
    uint8_t shiftIn();
    uint8_t is_ready();
    void ads_power_up();
    void ads_power_down();
    void ads_set_offset(long offset);
    void ads_set_scale(float scale);
    uint32_t ads1_raw_read();
    uint32_t ads2_raw_read(uint8_t times);
    float ads_units_read(uint8_t times);

#endif
