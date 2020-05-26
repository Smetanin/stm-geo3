/*
 * eeprom_calls.h
 *
 *  Created on: 28 апр. 2020 г.
 *      Author: user
 */

#ifndef EEPROM_CALLS_H_
#define EEPROM_CALLS_H_


#define calibr_size 30

void flash_init(void);
void flash_write_var(void);
extern long calibr_sens_arr[calibr_size][2];


#endif /* EEPROM_CALLS_H_ */
