/*
 * node_menu.h
 *
 *  Created on: 20 апр. 2020 г.
 *      Author: user
 */

#ifndef LCD_MENU_H_
#define LCD_MENU_H_

#include "stm32f4xx_hal.h"
#include <list>
#include <string>
#include <iostream>



//здесь указываем какую информацию мы можем хранить
struct Payload
{
	std::string name;
	void (*callback)(void);
	int Data[2];
	int lcd_data_position[2];
};

class Row
{
public:
	Payload payload;

	Row* parent;
	std::list<Row*> children; //list дает возможность сортить подменю
	Row* next;
	Row* prev;

	Row* last; //конец списка
public:
	Row():
		parent(nullptr),children(),next(nullptr),prev(nullptr), last(this), payload()
	{
		//std::cout<<__PRETTY_FUNCTION__<<std::endl;
	}
	Row(const Payload& payload):
		parent(nullptr),children(),next(nullptr),prev(nullptr), last(this), payload(payload)
	{
		//std::cout<<__PRETTY_FUNCTION__<<std::endl;
	}
	Row(const Payload& payload, Row* parent_):
		parent(parent_),children(),next(nullptr),prev(nullptr), last(this), payload(payload)
	{
		//std::cout<<__PRETTY_FUNCTION__<<std::endl;
		if(parent)
			parent->children.push_back(this);
	}
	Row(const Payload& payload, Row* parent_, Row* left, Row* right=nullptr):
		parent(parent_), children(), next(right), prev(left),
		last(prev ? prev->last : (next ? next->last : this)),
		payload(payload)
	{
		//std::cout<<__PRETTY_FUNCTION__<<std::endl;
		if(parent)
			parent->children.push_back(this);

		if(prev)
			prev->next = this;

		if(next)
			next->prev = this;
	}
	//создать подменю
	Row* addChild(const Payload& payload)
	{
		//std::cout<<__PRETTY_FUNCTION__<<std::endl;
		Row* child = new Row(payload,this);
		return child;
	}
	Row* addNextRow(const Payload& payload)
	{
		//std::cout<<__PRETTY_FUNCTION__<<std::endl;
		Row* newRow = new Row(payload,parent,this,next);
		return newRow;
	}
	//создать строку в конце списка
	Row* addRow(const Payload& payload)
	{
		//std::cout<<__PRETTY_FUNCTION__<<std::endl;
		Row* newLast = last->addNextRow(payload); //новый хвост списка
		for(Row* row = this; row!=nullptr; row = row->prev)
		{
			row->last = newLast; //ставим новый хвост всем предыдущим строкам
		}
		for(Row* row = this; row!=nullptr; row = row->next)
		{
			row->last = newLast; //ставим нвоый хвост всем поседующим строкам
		}
		return newLast;
	}

	bool hasChildren() const
	{
		return !children.empty();
	}
	//begin(). Сделано чисто для удобства обхода в цикле
	Row* begin()
	{
		return this;
	}
	Row* end() const
	{
		return nullptr;
	}
};

extern Row* selectedMenuItem;

extern Row* main_menu;
extern Row* process_menu;
extern Row* next_step_menu;
extern Row* unload_menu;
extern Row* setting_menu;
extern Row* process_menu_one_curve;
extern Row* process_menu_two_curve;
extern Row* test_sensor_menu;
extern Row* manual_mode_menu;
//
//
//
//
extern Row* auth_menu;
extern Row* calibr_and_check_menu;
extern Row* calibr_select_sens_menu;
extern Row* calibr_pressure_menu;
extern Row* calibr_linear_menu;
extern Row* check_select_sens_menu;
extern Row* check_pressure_menu;
extern Row* check_linear_menu;




void menu_init(void);
void startMenu(void);
void menu_output(Row* row);
void menuChange(Row* row);


#endif /* LCD_MENU_H_ */
