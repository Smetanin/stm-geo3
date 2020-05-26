///*
// * encoder.cpp
// *
// *  Created on: 28 апр. 2020 г.
// *      Author: user
// */
#include "encoder.h"
#include "lcd_melt.h"
#include "lcd_menu.h"
#include "debug.h"
#include "string.h"

#include "ADC.h" // без этого не работает дебаг





Encoder enc1(0,1,2);
uint8_t reset_button = 0;
uint8_t ok_button = 0;
int encoder = 0, encoder_prev = 0;
uint8_t last_string_prev, scroll, last_string, cursor_x;
uint8_t cursor_next_button = 0; //используется для проверки перехода курсора на вторую кнопку в строке типа: "<отмена>  <Запуск>"

void tik_callback(void)
{
	enc1.tick();
	if (enc1.isRight())
	{
		encoder++;     	// если был поворот направо, +1
	}
	if (enc1.isLeft())
	{
		encoder--;	    // если был поворот налево, -1
	}
}
void tik_callback_x2(void)
{
	enc1.tick();
	if (enc1.isRight())
	{
		encoder+=2;     	// если был поворот направо, +1
	}
	if (enc1.isLeft())
	{
		encoder-=2;	    // если был поворот налево, -1
	}
}

uint8_t keyMenu(uint8_t param) {
	switch (param) {
	case 0:
		return 1;
	case KEY_LEFT:
		if(selectedMenuItem) selectedMenuItem = selectedMenuItem -> parent -> next;
		return 1;
	case KEY_UP:
		if(selectedMenuItem) selectedMenuItem = selectedMenuItem -> next;
		return 1;
	case KEY_DOWN:
		if(selectedMenuItem) selectedMenuItem = selectedMenuItem -> prev;
		return 1;
	case KEY_OK:
		//std::string name = selectedMenuItem->payload.name;
		row_debug(selectedMenuItem);
		encoder++;
		if(selectedMenuItem->hasChildren())
		{
			selectedMenuItem = selectedMenuItem->children.front();
		}
		row_debug(selectedMenuItem);
		menu_output(selectedMenuItem);
		selectedMenuItem->payload.callback();
		break;
		//selectedMenuItem->payload.name = name;
	}
	return (1);
}


//  uint8_t keyMenu(uint8_t param) {
//	switch (param) {
//	case 0:
//		return 1;
//
//	case KEY_UP:
//		menuChange(selectedMenuItem->Next);
//		break;
//
//	case KEY_DOWN:
//		menuChange(selectedMenuItem->Previous);
//		break;
//
//	case KEY_OK:
//	{
//		char ch[40];
//		int n = strlen(ch);
//		int pin_ok = 1;
//		uint8_t menu_pos=encoder;
//		uint16_t sel=0;
//		sel = selectedMenuItem->Select;
//		switch (sel)
//		{
//		case 0:
//			if(selectedMenuItem->Child != NULL)
//			{
//				menuChange(selectedMenuItem->Child);
//				disp_menu(0);
//				encoder = 0;
//				scroll=0;
//				last_string = 0;
//				last_string_prev = 0;
//				HAL_Delay(1);
//				LCD_SetPos(0, 0);
//				HAL_Delay(1);
//				LCD_SendChar(0x90);
//				HAL_Delay(1);
//			}
//			return(1);
//
//		case 1:
//			encoder = selectedMenuItem->Data1;
//			while(1)
//			{
//				read_encoder_set_data(menu_pos);
//				if(ok_button || reset_button)
//				{
//					ok_button = 0;
//					reset_button = 0;
//					selectedMenuItem->Data1 = encoder;
//					encoder = menu_pos;
//					encoder_prev = menu_pos;
//					return(1);
//				}
//			}
//
//			return (1);
//		case 2:
//			if(cursor_x == 9) //сохранить
//			{
//
//				ZapisSD(1); //пишем данные на SD карту
//			}
//			else // отмена
//			{
//				reset_button = 1;
//			}
//			return (1);
//
//		case 3: //проверка датчиков - главное меню
//
//			test_sensor();
//			return (1);
//
//		case 5: //5 - ромбики <> в меню ручной режим
//			manual_driver_control = !manual_driver_control;
//			encoder = 10;
//			encoder_prev = encoder;
//			return (1);
//		case 10:
//			if(cursor_x == 9) //след.ступень в эксперименте
//			{
//				auto_unload -> Child = next_step_cancel;
//				next_step_exp();
//			}
//			else // разрузка ч эксперименте
//			{
//				auto_unload -> Child = unload_cancel;
//				unload_exp();
//			}
//			return(1);
//		case 11:
//			if(cursor_x == 9) //Переход на N+1
//			{
//				//experiment();
//				//next_step = 1;
//			}
//			else
//			{
//				reset_button = 1;
//			}
//			return(1);
//		case 12:
//			if(cursor_x == 9) //Разгрузка прибора
//			{
//				NegativeTurn(80);
//				HAL_Delay(9000);
//				StopDvig();
//				reset_all = 1;
//			}
//			else
//			{
//				reset_button = 1;
//			}
//			return(1);
//		case 14:
//			if(cursor_x == 9) //проверка датчиков перед запуском
//			{
//				test_sensor_before_exp();
//			}
//			else // отмена
//			{
//				reset_button = 1;
//			}
//			return(1);
//
//		case 15:
//			if(cursor_x == 9) //запуск эксперимента
//			{
//				experiment();
//			}
//			else // отмена
//			{
//				reset_button = 1;
//			}
//			return 1;
//		case 16: //ручной режим
//			manual_mode();
//			return 1;
//		case 17:
//			encoder = selectedMenuItem->Data1;
//			while(1)
//			{
//				read_encoder_set_data(menu_pos);
//				if(ok_button || reset_button)
//				{
//					ok_button = 0;
//					reset_button = 0;
//					selectedMenuItem->Data1 = encoder;
//					encoder = menu_pos;
//					encoder_prev = menu_pos;
//					auto_load_sw=1;
//					return(1);
//				}
//			}
//		case 18: //ввод пинкода
//			encoder = 0;
//			int pin_input[4];
//			sequent_input(pin_input, 4, selectedMenuItem->Data1_pos);
//			for(int ii=0;ii<4;ii++)
//			{
//				if(pin_input[ii] != pin[ii])
//				{
//					pin_ok=0;
//				}
//			}
//
//			return(1);
//		case 19: //ввод пинкода
//
//			encoder = selectedMenuItem->Data1;
//			while(!ok_button)
//			{
//				read_encoder_set_data(menu_pos);
//			}
//
//			selectedMenuItem->Data1 = encoder;
//			date_calibr[0] = encoder;
//			ok_button = 0;
//			encoder = 0;
//
//
//			sequent_input2(date_calibr, 8, selectedMenuItem->Data2_pos, menu_pos);
//
//			strncpy( ch_tst1,selectedMenuItem->Text,selectedMenuItem->Data2_pos-1);
//
//			sprintf(ch_tst2,"%s%03d %d%d.%d%d.%d%d",ch_tst1,date_calibr[0],date_calibr[1],date_calibr[2],date_calibr[3],date_calibr[4],date_calibr[5],date_calibr[6]);
//
//			n = strlen(ch_tst2);
//
//			strncpy(selectedMenuItem->Text, ch_tst2,n);
//			LCD_SetPos(1, menu_pos);
//			LCD_StringRU(selectedMenuItem->Text);
//			//selectedMenuItem = calibration4;
//			//disp_menu(0);
//
//			ok_button = 0;
//			//HAL_Delay(5000);
//			while(!ok_button)
//			{
//				HAL_Delay(100);
//			}
//			ok_button = 0;
//			selectedMenuItem = selectedMenuItem->Child;
//			disp_menu(0);
//			encoder = 0;
//			scroll=0;
//			last_string = 0;
//			last_string_prev = 0;
//			HAL_Delay(1);
//			LCD_SetPos(0, 0);
//			HAL_Delay(1);
//			LCD_SendChar(0x90);
//			HAL_Delay(1);
//
//
//			break;
//
//		case 20:
//			calibration_read_press(menu_pos);
//			return(1);
//		case 21:
//			calibration_read_linear(menu_pos);
//			return(1);
//		case 23:
//			if(pin_ok)
//			{
//				//calibration_func(); //калибровка датчиков
//				if(selectedMenuItem->Child != NULL)
//				{
//					menuChange(selectedMenuItem->Child);
//					disp_menu(0);
//					encoder = 0;
//					scroll=0;
//					last_string = 0;
//					last_string_prev = 0;
//					HAL_Delay(1);
//					LCD_SetPos(0, 0);
//					HAL_Delay(1);
//					LCD_SendChar(0x90);
//					HAL_Delay(1);
//				}
//			}
//			else
//			{
//				sprintf(ch,"неправильный ПИН");
//				n = strlen(ch);
//				//menu_ref -> Text = "Запуск опыта      ";
//				strncpy(calibration3->Text, ch,n);
//				selectedMenuItem = calibration0;
//				disp_menu(0);
//				encoder = 0;
//				scroll=0;
//				last_string = 0;
//				last_string_prev = 0;
//				HAL_Delay(1);
//				LCD_SetPos(0, 0);
//				HAL_Delay(1);
//				LCD_SendChar(0x90);
//				HAL_Delay(1);
//			}
//			return(1);
//		case 24:
//
//			return(1);
//		case 25:
//			save_calibr_data();
//			return(1);
//		case 26:
//			save_calibr_data();
//			return(1);
//		case 9:
//			cancel_test2 -> Parent = experimentName2;
//			startExperiment -> Child = experimentName2;
//			selectedMenuItem->Select=8;
//			strcpy( selectedMenuItem->Text, "Схема: две кривые " );
//			break;
//
//		case 8:
//			cancel_test2 -> Parent = experimentName1;
//			startExperiment -> Child = experimentName1;
//			selectedMenuItem->Select=9;
//			strcpy( selectedMenuItem->Text, "Схема: одна кривая " );
//			break;
//
//		case 7:
//
//			selectedMenuItem->Select=6;
//			strcpy( selectedMenuItem->Text, "Ед.изм.нагр :[кг]  " );
//			break;
//
//		case 6:
//
//			selectedMenuItem->Select=7;
//			strcpy( selectedMenuItem->Text, "Ед.изм.нагр :[МПа] " );
//			break;
//		}
//		LCD_SetPos(1, encoder); //какого хера это тут делает?
//		LCD_StringRU((char*)&selectedMenuItem->Text);
//
//		break;
//	}
//	case KEY_LEFT:  // отмена выбора (возврат)
//		menuChange(selectedMenuItem->Parent);
//		disp_menu(0);
//	}
//
//	return (1);
//}
//
//
//
//void read_encoder(void)
//{
//	tik_callback();
//
//	if(encoder!=encoder_prev)
//	{
//
//		//HAL_Delay(1);
//		LCD_SetPos(cursor_x, encoder_prev);
//		//HAL_Delay(1);
//		LCD_SendChar(0x20);
//
//		if(last_string && encoder == 2)
//		{//на последней строке два пункта меню, и иначе курсор сразу уходит
//			encoder = 3;
//			cursor_x = 0;
//			last_string = 0;
//		}
//		else
//		{
//			if(encoder<encoder_prev)
//			{
//
//				keyMenu(KEY_DOWN);
//			}
//			else
//			{
//				keyMenu(KEY_UP);
//			}
//		}
//
//		if(encoder < 0)
//		{
//			encoder=0;
//			if(scroll>0)
//			{
//				scroll--;
//				//disp_menu(0);//перерисовываем меню на строку выше
//			}
//		}
//
//		if(encoder > 3)
//		{
//			encoder = 3;
//			//			if(selectedMenuItem->Have_Scroll)
//			//				if(last_string)
//			//				{
//			//					cursor_x = 9; //на последней строке меню ставим курсор на вторую кнопку "сохранить >выход"
//			//				}
//			//				else
//			//				{
//			//					if(selectedMenuItem->Next == NULL && selectedMenuItem != SensorCalibration)
//			//					{// проверяю когда дойдем до последней строки при скроллинге
//			//						last_string=1;
//			//					}
//			//					scroll++;
//			//					disp_menu(1);//перерисовываем меню на строку ниже
//			//				}
//		}
//		//вывели курсор на новой строке
//		LCD_SetPos(cursor_x, encoder);
//		LCD_SendChar(0x90);
//		encoder_prev = encoder;
//	}
//
//	if(reset_button)
//	{
//		keyMenu(KEY_LEFT);
//		cursor_x = 0;
//		encoder = 0;
//		reset_button=0;
//		scroll=0;
//		last_string=0;
//		last_string_prev=0;
//		HAL_Delay(1);
//		LCD_SetPos(0, 0);
//		HAL_Delay(1);
//		LCD_SendChar(0x90);
//		HAL_Delay(1);
//
//
//	}
//	if(ok_button)
//	{
//		ok_button=0;
//		keyMenu(KEY_OK);
//	}
//}
//
//
//
//
uint16_t read_encoder_set_data(uint8_t x, uint8_t y)
{
	enc1.tick();
	if (enc1.isRight()) encoder++;//debug("Right \r\n");         // если был поворот //
	if (enc1.isLeft()) encoder--;//debug("Left \r\n"); //*/
	if(enc1.isFastR())
		encoder+=10;
	if(enc1.isFastL()) encoder -=10;
	if(encoder < 0)	encoder=0;
	if(encoder > 9999) encoder=9999;
	if(encoder != encoder_prev)
	{
		encoder_prev = encoder;
		char ch[40];
		LCD_SetPos(x, y);
		sprintf(ch,"%04d",encoder);
		LCD_StringRU(ch);
	}
	return encoder;
}
//
//
//
//
//int encoder_last_string()
//{
//	enc1.tick();
//	if (enc1.isRight()) encoder--;//debug("Right \r\n");         // если был поворот //
//	if (enc1.isLeft()) encoder++;//debug("Left \r\n"); */
//	if(encoder > 1) encoder = 1;
//	if(encoder < 0) encoder = 0;
//	if(encoder!=encoder_prev)
//	{
//		HAL_Delay(1);
//		LCD_SetPos(cursor_x, 3);
//		HAL_Delay(1);
//		LCD_SendChar(0x20);
//
//		if(encoder > encoder_prev)
//		{
//			cursor_x = encoder+8;
//		}
//		else
//		{
//			cursor_x=0;
//		}
//
//
//		//вывели курсор на новой строке
//		HAL_Delay(1);
//		LCD_SetPos(cursor_x, 3);
//		HAL_Delay(1);
//		LCD_SendChar(0x90);
//		HAL_Delay(1);
//
//		encoder_prev = encoder;
//	}
//
//	if(ok_button)
//	{
//		ok_button=0;
//		if(cursor_x == 9) //след.ступень в эксперименте
//		{
//			auto_unload -> Child = next_step_cancel;
//			next_step_exp();
//		}
//		else // разрузка ч эксперименте
//		{
//			auto_unload -> Child = unload_cancel;
//			unload_exp();
//		}
//	}
//	if(reset_button)
//	{
//		keyMenu(KEY_LEFT);
//		cursor_x = 0;
//		encoder = 0;
//		reset_button=0;
//		scroll=0;
//		last_string=0;
//		last_string_prev=0;
//		HAL_Delay(1);
//		LCD_SetPos(0, 0);
//		HAL_Delay(1);
//		LCD_SendChar(0x90);
//		HAL_Delay(1);
//		return 0;
//	}
//	if(reset_all)
//	{
//		return 1;
//	}
//}
//
void encoder_clear(void)
{
	encoder = 0;
	encoder_prev = 0;
	ok_button = 0;
	reset_button = 0;
	cursor_next_button = 0;
	cursor_x = 0;
	encoder = 0;
	scroll=0;
	last_string=0;
	last_string_prev=0;
	HAL_Delay(1);
	LCD_SetPos(0, 0);
	HAL_Delay(1);
	LCD_SendChar(0x90);
	HAL_Delay(1);
}






void read_encoder(void)
{
	tik_callback();

	if(encoder!=encoder_prev)
	{

		LCD_SetPos(0, encoder_prev);
		LCD_SendChar(0x20);

		if(encoder < 0)
		{
			encoder=0;
		}
		else if(encoder > 3)
		{
			encoder = 3;
		}
		else if(encoder > encoder_prev)
		{
			keyMenu(KEY_UP);
		}
		else if(encoder < encoder_prev)
		{
			keyMenu(KEY_DOWN);
		}
		//вывели курсор на новой строке
		LCD_SetPos(0, encoder);
		LCD_SendChar(0x90);
		encoder_prev = encoder;
	}

	if(reset_button)
	{
		keyMenu(KEY_LEFT);
		encoder_clear();
	}
	if(ok_button)
	{
		encoder_clear();
		keyMenu(KEY_OK);
	}
}


