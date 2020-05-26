/*
 * node_menu.cpp
 *
 *  Created on: 20 апр. 2020 г.
 *      Author: user
 */
#include "lcd_menu.h"
//#include "lcd_melt.h"
//#include "menu_callback.h"

Row* selectedMenuItem;

Row* main_menu 			= new Row({"root"});
Row* process_menu 	= new Row({"root"});
Row* next_step_menu 	= new Row({"root"});
Row* unload_menu 		= new Row({"root"});
Row* setting_menu 	= new Row({"root"});
Row* process_menu_one_curve 	= new Row({"root"});
Row* process_menu_two_curve 	= new Row({"root"});
Row* test_sensor_menu 	= new Row({"root"});
Row* manual_mode_menu 	= new Row({"root"});




Row* auth_menu = new Row({"root"});
Row* calibr_and_check_menu = new Row({"root"});
Row* calibr_select_sens_menu = new Row({"root"});
Row* calibr_pressure_menu = new Row({"root"});
Row* calibr_linear_menu = new Row({"root"});
Row* check_select_sens_menu = new Row({"root"});
Row* check_pressure_menu = new Row({"root"});
Row* check_linear_menu = new Row({"root"});



void menu_init(void)
{


	process_menu_one_curve->addRow({"Схема: одна кривая "});
	process_menu_one_curve->addRow({"N исп.:     [    ] "});
	process_menu_one_curve->addRow({"P1,кг:      [    ] "});
	process_menu_one_curve->addRow({"Pn,кг:      [    ] "});
	process_menu_one_curve->addRow({"Tстаб,мин:  [    ] "});
	process_menu_one_curve->addRow({"Кол-во ступ:[    ] "});
	process_menu_one_curve->addRow({"<Отмена>   <Запуск>"});

	process_menu_two_curve->addRow({"Схема: одна кривая "});
	process_menu_two_curve->addRow({"N исп.:     [    ] "});
	process_menu_two_curve->addRow({"P1,кг:      [    ] "});
	process_menu_two_curve->addRow({"Pn,кг:      [    ] "});
	process_menu_two_curve->addRow({"Tстаб,мин:  [    ] "});
	process_menu_two_curve->addRow({"Nступ:      [    ] "});
	process_menu_two_curve->addRow({"Разг.до N  :[    ] "});
	process_menu_two_curve->addRow({"Циклов     :[    ] "});
	process_menu_two_curve->addRow({"<Отмена>   <Запуск>"});

	setting_menu->addRow({"Схема: одна кривая "});
	setting_menu->addRow({"Инт.сохр,сек:[   ] "});
	setting_menu->addRow({"Инт.ручн,сек:[   ] "});
	setting_menu->addRow({"Sштампа,см2 :[   ] "});
	setting_menu->addRow({"Ед.изм.нагр :[кг]  "});
	setting_menu->addRow({"Отмена   Сохранить "});

	//test_sensor_menu->payload.callback = sensor_check;
	//test_sensor_menu->addRow({"P, кг:             ",&sensor_check});
	test_sensor_menu->addRow({"L, мм:             "});
	test_sensor_menu->addRow({"SD карта:          "});
	test_sensor_menu->addRow({"<Отмена>           "});

	process_menu_one_curve->children.push_back(test_sensor_menu-> next);
	process_menu_two_curve->children.push_back(test_sensor_menu-> next);




	//manual_mode_menu->payload.callback = manual_mode;
	//manual_mode_menu->addRow({"кг:       мм:      ",&manual_mode});
	manual_mode_menu->addRow({"   6 4 2 0 2 4 6   "});
	manual_mode_menu->addRow({"   о о о ф о о о   "});
	manual_mode_menu->addRow({"Нагрузить,кг:[    ]"});


	main_menu->addRow({"Запуск опыта       "})->children.push_back(process_menu_one_curve-> next);
	//row->children.push_back(process_menu_one_curve-> next);
	main_menu->addRow({"Настройка опыта    "})->children.push_back(setting_menu-> next);
	//row->children.push_back(setting_menu-> next);
	main_menu->addRow({"Проверка датчиков  "})->children.push_back(test_sensor_menu-> next);
	//row->children.push_back(test_sensor_menu-> next);
	main_menu->addRow({"Ручной режим       "})->children.push_back(manual_mode_menu-> next);
	//row->children.push_back(manual_mode_menu-> next);


	auth_menu->addRow({"Вход в меню поверки"});
	auth_menu->addRow({"                   "});
	auth_menu->addRow({"Введите ПИН:[0000] "});
	auth_menu->addRow({"Войти    Отмена    "});

	calibr_and_check_menu->addRow({"Действие      дата "});
	calibr_and_check_menu->addRow({"Калибровка dd.mm.yy"});
	calibr_and_check_menu->addRow({"Поверка    dd.mm.yy"});
	calibr_and_check_menu->addRow({"Выход              "});

	calibr_select_sens_menu->addRow({"Калибр  N     дата "});
	calibr_select_sens_menu->addRow({"силы  N000 dd.mm.yy"});
	calibr_select_sens_menu->addRow({"лин   N000 dd.mm.yy"});
	calibr_select_sens_menu->addRow({"Отмена   Сохранить "});

	calibr_pressure_menu->addRow({"Знач.кг  Мв  Статус"});
	calibr_pressure_menu->addRow({"000   [0000000]    "});
	calibr_pressure_menu->addRow({"100   [0000000]    "});
	calibr_pressure_menu->addRow({"200   [0000000]    "});
	calibr_pressure_menu->addRow({"300   [0000000]    "});
	calibr_pressure_menu->addRow({"400   [0000000]    "});
	calibr_pressure_menu->addRow({"500   [0000000]    "});
	calibr_pressure_menu->addRow({"1000  [0000000]    "});
	calibr_pressure_menu->addRow({"2000  [0000000]    "});
	calibr_pressure_menu->addRow({"3000  [0000000]    "});
	calibr_pressure_menu->addRow({"4000  [0000000]    "});
	calibr_pressure_menu->addRow({"5000  [0000000]    "});
	calibr_pressure_menu->addRow({"Отмена   Сохранить "});

	calibr_linear_menu->addRow({"Знач.мм  Мв  Статус"});
	calibr_linear_menu->addRow({"000   [0000000]    "});
	calibr_linear_menu->addRow({"002   [0000000]    "});
	calibr_linear_menu->addRow({"004   [0000000]    "});
	calibr_linear_menu->addRow({"006   [0000000]    "});
	calibr_linear_menu->addRow({"008   [0000000]    "});
	calibr_linear_menu->addRow({"010   [0000000]    "});
	calibr_linear_menu->addRow({"020   [0000000]    "});
	calibr_linear_menu->addRow({"030   [0000000]    "});
	calibr_linear_menu->addRow({"040   [0000000]    "});
	calibr_linear_menu->addRow({"050   [0000000]    "});
	calibr_linear_menu->addRow({"060   [0000000]    "});
	calibr_linear_menu->addRow({"070   [0000000]    "});
	calibr_linear_menu->addRow({"080   [0000000]    "});
	calibr_linear_menu->addRow({"090   [0000000]    "});
	calibr_linear_menu->addRow({"092   [0000000]    "});
	calibr_linear_menu->addRow({"094   [0000000]    "});
	calibr_linear_menu->addRow({"096   [0000000]    "});
	calibr_linear_menu->addRow({"098   [0000000]    "});
	calibr_linear_menu->addRow({"100   [0000000]    "});
	calibr_linear_menu->addRow({"Отмена   Сохранить "});

	check_select_sens_menu->addRow({"Поверка  N    дата "});
	check_select_sens_menu->addRow({"силы  N000 dd.mm.yy"});
	check_select_sens_menu->addRow({"лин   N000 dd.mm.yy"});
	check_select_sens_menu->addRow({"Отмена   Сохранить "});


	check_pressure_menu->addRow({"Знач.кг Показ.кг   "});
	check_pressure_menu->addRow({"000   [0000000]    "});
	check_pressure_menu->addRow({"500   [0000000]    "});
	check_pressure_menu->addRow({"1000  [0000000]    "});
	check_pressure_menu->addRow({"2000  [0000000]    "});
	check_pressure_menu->addRow({"3000  [0000000]    "});
	check_pressure_menu->addRow({"4000  [0000000]    "});
	check_pressure_menu->addRow({"5000  [0000000]    "});
	check_pressure_menu->addRow({"Отмена   Сохранить "});


	check_linear_menu->addRow({"Знач.мм Показ.мм   "});
	check_linear_menu->addRow({"000   [0000000]    "});
	check_linear_menu->addRow({"006   [0000000]    "});
	check_linear_menu->addRow({"010   [0000000]    "});
	check_linear_menu->addRow({"020   [0000000]    "});
	check_linear_menu->addRow({"030   [0000000]    "});
	check_linear_menu->addRow({"040   [0000000]    "});
	check_linear_menu->addRow({"050   [0000000]    "});
	check_linear_menu->addRow({"060   [0000000]    "});
	check_linear_menu->addRow({"070   [0000000]    "});
	check_linear_menu->addRow({"080   [0000000]    "});
	check_linear_menu->addRow({"090   [0000000]    "});
	check_linear_menu->addRow({"096   [0000000]    "});
	check_linear_menu->addRow({"100   [0000000]    "});
	check_linear_menu->addRow({"Отмена   Сохранить "});





}

void menu_output(Row* row)
{
//	LCD_Clear();
//	char ch[40];
//	for(int i=0;i<4;i++) //количество строк на дисплее
//	{
//		LCD_SetPos(1, i);
//		LCD_StringRU(row->payload.name);
//		if(row->next)
//			row = row->next;
//	}
}


void startMenu(void) {


//	if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11)) //Вход в калибровку и поверку по зажатому "ОК"
//	{
//		selectedMenuItem = main_menu -> next;
//		//selectedMenuItem = manual_mode_menu -> next;
//	}
//	else
//	{
//		//selectedMenuItem = auth_menu;
//		selectedMenuItem = calibr_linear_menu;
//	}
//	menu_output(selectedMenuItem);
//	//установка курсора на 1ю строку
//	LCD_SetPos(0, 0);
//	HAL_Delay(1);
//	LCD_SendChar(0x90);
	//--------------------------------

	//	//формируется строка с ромбиками для ручного режима
	//	char buf[17] = {0x20, 0x20, 0x20, 0xb3, 0x20, 0xb3, 0x20, 0xb3, 0x20, 0xb2,\
	//			0x20, 0xb3, 0x20, 0xb3, 0x20, 0xb3};
	//	uint8_t n = strlen(buf);
	//	strncpy(P1_menu-> Text, buf,n);
	//
}

void menu_debug(Row* row)
{
	//можно и так, но это менее очевидно
//	std::cout<<std::endl<<"Menu rows: "<<std::endl;
//	for(; row/*!=nullptr*/; row = row->next)
//	{
//		std::cout<<"- "<<row->payload.name<<std::endl;
//		if(row->hasChildren())
//		{
//			auto submenu3 = row->children.front();
//			for(auto row2 = submenu3->begin(); row2!=submenu3->end(); row2 = row2->next)
//			{
//				std::cout<<"--- "<<row2->payload.name<<std::endl;
//			}
//		}
//	}

}



void menuChange(Row* row)
{

}

