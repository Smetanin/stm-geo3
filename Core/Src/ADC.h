/*
 * ADC.h
 *
 *  Created on: 28 апр. 2020 г.
 *      Author: user
 */

#ifndef ADC_H_
#define ADC_H_
#include "stm32f4xx_hal.h"
#include "main.h"


//// ADS1256 Register
//uint8_t STATUS= 0x00;
//uint8_t MUX =0x01;
//uint8_t ADCON= 0x02;
//uint8_t DRATE= 0x03;
//uint8_t IO =0x04;
//uint8_t OFC0= 0x05;
//uint8_t OFC1= 0x06;
//uint8_t OFC2= 0x07;
//uint8_t FSC0 =0x08;
//uint8_t FSC1= 0x09;
//uint8_t FSC2 =0x0A;
//
//// ADS1256 Command
//uint8_t WAKEUP= 0x00;
//uint8_t RDATA =0x01;
//uint8_t RDATAC =0x03;
//uint8_t SDATAC =0x0f;
//uint8_t RREG =0x10;
//uint8_t WREG =0x50;
//uint8_t SELFCAL =0xF0;
//uint8_t SELFOCAL =0xF1;
//uint8_t SELFGCAL =0xF2;
//uint8_t SYSOCAL =0xF3;
//uint8_t SYSGCAL =0xF4;
//uint8_t SYNC =0xFC;
//uint8_t STANDBY= 0xFD;
//uint8_t RESET1= 0xFE;
//
//// define multiplexer codes
//uint8_t ADS1256_MUXP_AIN0= 0x00;
//uint8_t ADS1256_MUXP_AIN1 =0x10;
//uint8_t ADS1256_MUXP_AIN2 =0x20;
//uint8_t ADS1256_MUXP_AIN3 =0x30;
//uint8_t ADS1256_MUXP_AIN4 =0x40;
//uint8_t ADS1256_MUXP_AIN5 =0x50;
//uint8_t ADS1256_MUXP_AIN6 =0x60;
//uint8_t ADS1256_MUXP_AIN7 =0x70;
//uint8_t ADS1256_MUXP_AINCOM= 0x80;
//
//uint8_t ADS1256_MUXN_AIN0 =0x00;
//uint8_t ADS1256_MUXN_AIN1 =0x01;
//uint8_t ADS1256_MUXN_AIN2 =0x02;
//uint8_t ADS1256_MUXN_AIN3 =0x03;
//uint8_t ADS1256_MUXN_AIN4 =0x04;
//uint8_t ADS1256_MUXN_AIN5 =0x05;
//uint8_t ADS1256_MUXN_AIN6 =0x06;
//uint8_t ADS1256_MUXN_AIN7 =0x07;
//uint8_t ADS1256_MUXN_AINCOM =0x08;
//
//// define gain codes
//uint8_t ADS1256_GAIN_1 =0x00;
//uint8_t ADS1256_GAIN_2 =0x01;
//uint8_t ADS1256_GAIN_4 =0x02;
//uint8_t ADS1256_GAIN_8 =0x03;
//uint8_t ADS1256_GAIN_16 =0x04;
//uint8_t ADS1256_GAIN_32 =0x05;
//uint8_t ADS1256_GAIN_64 =0x06;
//
//uint8_t ADS1256_DRATE_30000SPS =0xF0;
//uint8_t ADS1256_DRATE_15000SPS =0xE0;
//uint8_t ADS1256_DRATE_7500SPS =0xD0;
//uint8_t ADS1256_DRATE_3750SPS =0xC0;
//uint8_t ADS1256_DRATE_2000SPS =0xB0;
//uint8_t ADS1256_DRATE_1000SPS =0xA1;
//uint8_t ADS1256_DRATE_500SPS =0x92;
//uint8_t ADS1256_DRATE_100SPS =0x82;
//uint8_t ADS1256_DRATE_60SPS =0x72;
//uint8_t ADS1256_DRATE_50SPS =0x63;
//uint8_t ADS1256_DRATE_30SPS =0x53;
//uint8_t ADS1256_DRATE_25SPS =0x43;
//uint8_t ADS1256_DRATE_15SPS =0x33;
//uint8_t ADS1256_DRATE_10SPS =0x23;
//uint8_t ADS1256_DRATE_5SPS =0x13;
//uint8_t ADS1256_DRATE_2_5SPS= 0x03;

// ADS1256 Register
#define STATUS 0x00
#define MUX 0x01
#define ADCON 0x02
#define DRATE 0x03
#define IO 0x04
#define OFC0 0x05
#define OFC1 0x06
#define OFC2 0x07
#define FSC0 0x08
#define FSC1 0x09
#define FSC2 0x0A

// ADS1256 Command
#define WAKEUP 0x00
#define RDATA 0x01
#define RDATAC 0x03
#define SDATAC 0x0f
#define RREG 0x10
#define WREG 0x50
#define SELFCAL 0xF0
#define SELFOCAL 0xF1
#define SELFGCAL 0xF2
#define SYSOCAL 0xF3
#define SYSGCAL 0xF4
#define SYNC 0xFC
#define STANDBY 0xFD
#define RESET 0xFE

// define multiplexer codes
#define ADS1256_MUXP_AIN0 0x00
#define ADS1256_MUXP_AIN1 0x10
#define ADS1256_MUXP_AIN2 0x20
#define ADS1256_MUXP_AIN3 0x30
#define ADS1256_MUXP_AIN4 0x40
#define ADS1256_MUXP_AIN5 0x50
#define ADS1256_MUXP_AIN6 0x60
#define ADS1256_MUXP_AIN7 0x70
#define ADS1256_MUXP_AINCOM 0x80

#define ADS1256_MUXN_AIN0 0x00
#define ADS1256_MUXN_AIN1 0x01
#define ADS1256_MUXN_AIN2 0x02
#define ADS1256_MUXN_AIN3 0x03
#define ADS1256_MUXN_AIN4 0x04
#define ADS1256_MUXN_AIN5 0x05
#define ADS1256_MUXN_AIN6 0x06
#define ADS1256_MUXN_AIN7 0x07
#define ADS1256_MUXN_AINCOM 0x08

// define gain codes
#define ADS1256_GAIN_1 0x00
#define ADS1256_GAIN_2 0x01
#define ADS1256_GAIN_4 0x02
#define ADS1256_GAIN_8 0x03
#define ADS1256_GAIN_16 0x04
#define ADS1256_GAIN_32 0x05
#define ADS1256_GAIN_64 0x06

// define drate codes
/*
        NOTE : 	Data Rate vary depending on crystal frequency. Data rates
   listed below assumes the crystal frequency is 7.68Mhz
                for other frequency consult the datasheet.
*/

#define ADS1256_DRATE_30000SPS 0xF0
#define ADS1256_DRATE_15000SPS 0xE0
#define ADS1256_DRATE_7500SPS 0xD0
#define ADS1256_DRATE_3750SPS 0xC0
#define ADS1256_DRATE_2000SPS 0xB0
#define ADS1256_DRATE_1000SPS 0xA1
#define ADS1256_DRATE_500SPS 0x92
#define ADS1256_DRATE_100SPS 0x82
#define ADS1256_DRATE_60SPS 0x72
#define ADS1256_DRATE_50SPS 0x63
#define ADS1256_DRATE_30SPS 0x53
#define ADS1256_DRATE_25SPS 0x43
#define ADS1256_DRATE_15SPS 0x33
#define ADS1256_DRATE_10SPS 0x23
#define ADS1256_DRATE_5SPS 0x13
#define ADS1256_DRATE_2_5SPS 0x03

void ADC_init();
void setChannel(char AIN_P, char AIN_N);
void setChannel1(char channel) ;
void waitDRDY();
void begin(uint8_t drate, uint8_t gain);
void writeRegister(uint8_t reg, uint8_t wdata);
uint8_t readRegister(uint8_t reg);
void sendCommand(uint8_t reg);
void CSON();
void CSOFF();
unsigned long readCurrentChannel ();

#endif /* ADC_H_ */
