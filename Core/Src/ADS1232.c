/*
  ADS1232.cpp - Library for TI ADS1232
  24-Bit Analog-to-Digital Converter For Bridge Sensors
  Created by Sorin Ciorceri, 2017.
  Released into the public domain.
*/
#include "stm32f4xx_hal.h"
#include "ADS1232.h"

#define HIGH 0x1
#define LOW  0x0

    uint32_t OFFSET = 0;   // used for tare
    float SCALE = 1.0; // used to scale weight to grams/kg/ounces



void ads_init(uint16_t pdwn, uint16_t sclk, uint16_t dout, GPIO_TypeDef* port) {
  PDWN = pdwn;
  SCLK = sclk;
  DOUT = dout;
  PORT = port;
//  pinMode(PDWN, OUTPUT);
//  pinMode(SCLK, OUTPUT);
 // pinMode(DOUT, INPUT);
}


uint8_t ads_is_ready() {
  return HAL_GPIO_ReadPin(PORT, DOUT) == LOW;
}
  
void ads_power_up() {
  //digitalWrite(PDWN, HIGH);
	HAL_GPIO_WritePin(PORT,PDWN,GPIO_PIN_SET);
  while (!ads_is_ready()) {};
}

void ads_power_down() {
  //digitalWrite(PDWN, LOW);
  HAL_GPIO_WritePin(PORT,PDWN,GPIO_PIN_RESET);
}

void ads_set_offset(long offset) {
  OFFSET = offset;
}

void ads_set_scale(float scale) {
  SCALE = scale;
}

uint32_t ads1_raw_read() {
  // From datasheet (page 19): The data must be retrieved before
  // new data are updated or else it will be overwritten
  // This means that I have to wait DOUT to change from LOW to HIGH and back
  // to LOW (that's the signal I have new data) and get it in 12.5 or 100 ms 100ms
  
  //while (ads_is_ready()) {};
  //while (!ads_is_ready()) {};

  uint32_t value = 0;
  uint32_t to_add = 0;
  uint8_t data[3] = { 0 };

  // pulse the clock pin 24 times to read the data
  data[2] = shiftIn(); //shiftIn(DOUT, SCLK, MSBFIRST);
  data[1] = shiftIn();
  data[0] = shiftIn();

  // From datasheet : output 24 bits of data in binary two's complement &
  // The positive full-scale input produces an output code of 7FFFFFh
  // and the negative full-scale input produces an output code of 800000h
  
  // If we have a negative number, this will be translated between 0 -> 0x7FFFFF
  if (data[2] & 0b1000000) {
    data[2] &= 0b01111111;
    to_add = 0x0UL;
  }
  // If we have a positive number, this will be translated between 0x800000 -> 0x1000000
  else {
    to_add = 0x7FFFFFUL;  // 2^23
  }
  value = ( (uint32_t)(data[2]) << 16
          | (uint32_t)(data[1]) << 8
          | (uint32_t)(data[0]) );


  value += (uint32_t)(to_add);
  value -= (uint32_t)(OFFSET);

  return value;
}

uint32_t ads2_raw_read(uint8_t times) {
	uint32_t sum = 0;
  for (uint8_t i=0; i<times; i++) {
    sum += ads1_raw_read();
  }
  return sum/times;
}

float ads_units_read(uint8_t times) {
  return ads2_raw_read(times) / SCALE;
}


uint8_t shiftIn()
{
	uint8_t result=0;
	for(int i=0;i<8;i++)
	{
		HAL_GPIO_WritePin(PORT,SCLK,GPIO_PIN_SET);
		HAL_Delay(1);
		result += HAL_GPIO_ReadPin(PORT, DOUT);
		result = result<<1;
		HAL_GPIO_WritePin(PORT,SCLK,GPIO_PIN_RESET);
	}
	return result;
}




void do_calibration() {
  long t_new_offset = 0;
  long t_raw_read = 0;
  float t_set_scale_value = 0;
  float t_weight = 0;

  // reset to default values
  OFFSET = 0;
  SCALE = 1.0;

  // tare
  t_new_offset = ads2_raw_read(3);
  OFFSET = t_new_offset;

  char ch[100];
  sprintf(ch,"Calibration offset =  %l \r\n",t_new_offset);
  debug(ch);

  //Serial.print("Calibration offset = ");Serial.println(weight.OFFSET);
  //Serial.println("You have 10 seconds to put a 2L CocaCola bottle on scale");
  HAL_Delay(10000);

  // do calibration based on a known weight
  t_raw_read = ads2_raw_read(3);
  sprintf(ch,"Units read =   %l \r\n",t_raw_read);
   debug(ch);

 // Serial.print("Units read = ");Serial.println(t_raw_read);

  t_set_scale_value = t_raw_read / 2.0;  // divide it to the weight of a CocaCola bottle

  SCALE = t_set_scale_value;
  sprintf(ch,"Calibration scale value =  %l \r\n",t_raw_read);
    debug(ch);
 // Serial.print("Calibration scale value = ");Serial.println(weight.SCALE);

  // read weight
  t_weight = ads2_units_read(3);
  sprintf(ch,"Weight =  %l \r\n",t_weight);
    debug(ch);
  //Serial.print("Weight = ");Serial.println(t_weight);
}
