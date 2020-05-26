/*
 * H-bridge.h
 *
 *  Created on: 28 апр. 2020 г.
 *      Author: user
 */

#ifndef H_BRIDGE_H_
#define H_BRIDGE_H_


extern uint8_t rotationTime;


void PositiveTurn();
void NegativeTurn();
void StopDvig();
void motor_move(int speed);



#endif /* H_BRIDGE_H_ */
