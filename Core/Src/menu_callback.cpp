/*
 * menu_callback.cpp
 *
 *  Created on: 28 апр. 2020 г.
 *      Author: user
 */
#include "menu_callback.h"
#include "lcd_melt.h"
#include "lcd_menu.h"
#include "GyverFilters.h"
#include "ADC.h"
#include "encoder.h"
#include "eeprom_calls.h"
#include "debug.h"
#include "H-bridge.h"
#include "SD_calls.h"

#define ARRAY_SIZE 10   // размер пространства для аппроксимации
uint8_t attempt_time; //кол-во включений двигателя через Н-мост

uint16_t razbros_kalman = 40;
float speed_kalman_prev, speed_kalman = 0.1;

GKalman testFilter(razbros_kalman, razbros_kalman, speed_kalman);  /////////////////////////////////////// KALMAN !!! ///////////////////////////////////////////////////////////////////////////////////////
GMedian MedianFilter;
GMedianPress PressFilter;
GFilterRA analog0,analog1, PressRandFilter;    // фильтр скользящее среднее
uint16_t setStep_a0 = 15;
float setCoef_a0 = 0.8;
int measure_before_filter=10;
uint8_t kill_read=0;
uint16_t limit = 0;

void read_sensor_with_calibr(void)
{
	static long pressure_sensor_old = 0;
	static long linear_sensor_old = 0;

	unsigned long linear_sensor_raw;
	long linear_sensor = 0x000F0000;
	long pressure_sensor = 0x000F0000;
	long ADC_value;

	//testFilter.setParameters(razbros_kalman,speed_kalman);
		waitDRDY();
		setChannel(0,1);
		waitDRDY();
		pressure_sensor = readCurrentChannel() & 0x000FFFFF;
		pressure_sensor = PressFilter.filtered(pressure_sensor);
		PressRandFilter.setCoef(0.8);
		PressRandFilter.setStep(15);
		pressure_sensor = PressRandFilter.filtered((int32_t)pressure_sensor);


	//----------------------------------------------------калиброква нуля------------------------------------------------
	//	if(ok_button)
	//	{
	//		ok_button=0;
	//		long press_delta = calibr_sens_arr[0][1] - pressure_sensor;
	//		calibr_sens_arr[0][1] = pressure_sensor;
	//
	//		calibr_sens_arr[1][1] = calibr_sens_arr[0][1] - press_delta;
	//	}



	HAL_Delay(4);


	waitDRDY();
	setChannel1(7);
	waitDRDY();
	linear_sensor_raw = readCurrentChannel() & 0x00FFFFFF;
	linear_sensor_raw = linear_sensor_raw >> 6;
	linear_sensor = MedianFilter.filtered(linear_sensor_raw);

	// установка коэффициента фильтрации (0.0... 1.0). Чем меньше, тем плавнее фильтр
	analog0.setCoef(setCoef_a0);
	// установка шага фильтрации (мс). Чем меньше, тем резче фильтр
	analog0.setStep(setStep_a0);
	linear_sensor = analog0.filteredTime((int32_t)linear_sensor);

	if(abs(linear_sensor - linear_sensor_old)<2)
	{
		linear_sensor = linear_sensor_old;
	}
	else
	{
		linear_sensor_old = linear_sensor;
	}
	if(abs(pressure_sensor - pressure_sensor_old)<2)
	{
		pressure_sensor = pressure_sensor_old;
	}
	else
	{
		pressure_sensor_old = pressure_sensor;
	}

	ADC_value = linear_sensor;

	uint8_t lineartab0 = 11; //в таблице калибровке позиция данных линейного датчика
	uint8_t lineartab100 = 12; //в таблице калибровке позиция данных линейного датчика

	if(ADC_value > calibr_sens_arr[lineartab0][1])
	{
		linear_sensor = 0;
	}
	else if(ADC_value < calibr_sens_arr[lineartab0+1][1])
	{
		linear_sensor = 10000;
	}
	else for(int i=lineartab0; i < lineartab100; i++)
	{
		if(ADC_value < calibr_sens_arr[i][1] && ADC_value > calibr_sens_arr[i+1][1])
		{
			uint32_t spread_mv = abs(int(calibr_sens_arr[i][1] - calibr_sens_arr[i+1][1]));
			uint32_t spread_mm = abs(int(calibr_sens_arr[i+1][0] - calibr_sens_arr[i][0]));
			uint32_t mux = spread_mm * 1000000 / spread_mv;
			linear_sensor = calibr_sens_arr[i][0] + abs(int(calibr_sens_arr[i][1] - ADC_value)) * mux;
			linear_sensor /= 10000;
			break;
		}

	}
	char ch[40];
	sprintf(ch,"%03d.%02d",linear_sensor/100,linear_sensor%100);
	LCD_SetPos(14, 0);
	LCD_StringRU(ch);

	//	sprintf(ch,"%04lu",ADC_value);
	//	LCD_SetPos(4, 1);
	//	LCD_StringRU(ch);

	//******************************** KG **********************************************************
	/***********************************************************************************************
	 *
	 */

	ADC_value = pressure_sensor;
	lineartab0 = 0; //в таблице калибровке позиция данных линейного датчика
	lineartab100 = 1; //в таблице калибровке позиция данных линейного датчика

	if(ADC_value > calibr_sens_arr[lineartab0][1])
	{
		pressure_sensor = 0;
	}
	else if(ADC_value < calibr_sens_arr[lineartab0+1][1])
	{
		//pressure_sensor = 10000;
	}
	else for(int i=lineartab0; i < lineartab100; i++)
	{
		if(ADC_value < calibr_sens_arr[i][1] && ADC_value > calibr_sens_arr[i+1][1])
		{
			uint32_t spread_mv = abs(int(calibr_sens_arr[i][1] - calibr_sens_arr[i+1][1]));
			uint32_t spread_mm = abs(int(calibr_sens_arr[i+1][0] - calibr_sens_arr[i][0]));
			uint32_t mux = spread_mm * 10000 / spread_mv;
			pressure_sensor = calibr_sens_arr[i][0] + abs(int(calibr_sens_arr[i][1] - ADC_value)) * mux;
			pressure_sensor /= 100;
			break;
		}

	}

	sprintf(ch,"%03lu.%02d",pressure_sensor/1000,pressure_sensor%100);
	LCD_SetPos(4, 0);
	LCD_StringRU(ch);
}



uint32_t read_sensor_press(void)
{
	static long ADC_value_old = 0;

	long ADC_value;

	waitDRDY();
	setChannel(0,1);
	waitDRDY();
	ADC_value = readCurrentChannel() & 0x000FFFFF;
	ADC_value = PressFilter.filtered(ADC_value);
	PressRandFilter.setCoef(0.8);
	PressRandFilter.setStep(15);
	ADC_value = PressRandFilter.filtered((int32_t)ADC_value);
	if(abs(ADC_value - ADC_value_old)<2)
	{
		ADC_value = ADC_value_old;
	}
	else
	{
		ADC_value_old = ADC_value;
	}
	uint8_t lineartab0 = 0; //в таблице калибровке позиция данных линейного датчика
	uint8_t lineartab100 = 1; //в таблице калибровке позиция данных линейного датчика

	if(ADC_value > calibr_sens_arr[lineartab0][1])
	{
		ADC_value = 0;
	}
	else if(ADC_value < calibr_sens_arr[lineartab0+1][1])
	{
		//pressure_sensor = 10000;
	}
	else for(int i=lineartab0; i < lineartab100; i++)
	{
		if(ADC_value < calibr_sens_arr[i][1] && ADC_value > calibr_sens_arr[i+1][1])
		{
			uint32_t spread_mv = abs(calibr_sens_arr[i][1] - calibr_sens_arr[i+1][1]);
			uint32_t spread_mm = abs(calibr_sens_arr[i+1][0] - calibr_sens_arr[i][0]);
			uint32_t mux = spread_mm * 1000 / spread_mv;
			ADC_value = abs(calibr_sens_arr[i][1] - ADC_value);
			ADC_value = ADC_value * mux;
			ADC_value = calibr_sens_arr[i][0] +  ADC_value;
			ADC_value /= 100;
			break;
		}

	}
	return ADC_value;
}

uint32_t read_sensor_linear(void)
{
	static long ADC_value_old = 0;
	long ADC_value;

	waitDRDY();
	setChannel1(7);
	waitDRDY();
	ADC_value = readCurrentChannel() & 0x00FFFFFF;
	ADC_value = ADC_value >> 6;
	ADC_value = MedianFilter.filtered(ADC_value);

	// установка коэффициента фильтрации (0.0... 1.0). Чем меньше, тем плавнее фильтр
	analog0.setCoef(setCoef_a0);
	// установка шага фильтрации (мс). Чем меньше, тем резче фильтр
	analog0.setStep(setStep_a0);
	ADC_value = analog0.filteredTime((int32_t)ADC_value);

	if(abs(ADC_value - ADC_value_old)<2)
	{
		ADC_value = ADC_value_old;
	}
	else
	{
		ADC_value_old = ADC_value;
	}


	uint8_t lineartab0 = 11; //в таблице калибровке позиция данных линейного датчика
	uint8_t lineartab100 = 12; //в таблице калибровке позиция данных линейного датчика

	if(ADC_value > calibr_sens_arr[lineartab0][1])
	{
		ADC_value = 0;
	}
	else if(ADC_value < calibr_sens_arr[lineartab0+1][1])
	{
		ADC_value = 10000;
	}
	else for(int i=lineartab0; i < lineartab100; i++)
	{
		if(ADC_value < calibr_sens_arr[i][1] && ADC_value > calibr_sens_arr[i+1][1])
		{
			uint32_t spread_mv = abs(int(calibr_sens_arr[i][1] - calibr_sens_arr[i+1][1]));
			uint32_t spread_mm = abs(int(calibr_sens_arr[i+1][0] - calibr_sens_arr[i][0]));
			uint32_t mux = spread_mm * 1000000 / spread_mv;
			ADC_value = calibr_sens_arr[i][0] + abs(int(calibr_sens_arr[i][1] - ADC_value)) * mux;
			ADC_value /= 10000;
			break;
		}

	}

	return ADC_value;
}




long read_sensor_raw(void)
{

	long linear_sensor_old = 0;
	long linear_sensor;
	unsigned long linear_sensor_raw;
	long linear_sensor_kalman_old = 0;
	long linear_sensor_kalman;
	long pressure_sensor;

	//		waitDRDY();
	//		setChannel(0,1);
	//		waitDRDY();
	//		pressure_sensor = readCurrentChannel();
	//		pressure_sensor = PressFilter.filtered(pressure_sensor);
	//		PressRandFilter.setCoef(0.5);
	//		PressRandFilter.setStep(10);
	//		pressure_sensor = PressRandFilter.filtered((int32_t)pressure_sensor);
	waitDRDY();
	setChannel1(7);
	waitDRDY();

	linear_sensor_raw = readCurrentChannel() & 0x00FFFFFF;
	linear_sensor_raw = linear_sensor_raw >> 5;
	linear_sensor = MedianFilter.filtered(linear_sensor_raw);


	//  KALMAN----------------------------------------------------------------------------
	linear_sensor_kalman = testFilter.filtered(linear_sensor);
	if(abs(linear_sensor_kalman_old - linear_sensor_kalman)<5)
	{
		linear_sensor_kalman = linear_sensor_kalman_old;
	}
	linear_sensor_kalman_old = linear_sensor_kalman;


	// установка коэффициента фильтрации (0.0... 1.0). Чем меньше, тем плавнее фильтр
	analog0.setCoef(0.2);
	// установка шага фильтрации (мс). Чем меньше, тем резче фильтр
	analog0.setStep(1000);
	linear_sensor = analog0.filteredTime((int32_t)linear_sensor_kalman);
	return linear_sensor;

}




//void test_sensor_output()
//{
//	char ch[40];
//	LCD_SetPos(testSensorsP -> Data1_pos, 0);
//	sprintf(ch,"%lu",pressure_sensor);
//	LCD_StringRU(ch);
//
//	sprintf (DanSD, "%lu",linear_sensor);
//	LCD_SetPos(SDCard -> Data2_pos, 2);
//	if(!ZapisSD(0))  //пищем данные на сд-карту
//	{
//		sprintf(ch,"ERR");
//		LCD_String(ch);
//	}
//	else
//	{
//		sprintf(ch," OK");
//		LCD_String(ch);
//	}
//
//	LCD_SetPos(testSensorsL -> Data1_pos, 2);
//	sprintf(ch,"%lu",linear_sensor_rand);
//	LCD_StringRU(ch);
//
//	LCD_SetPos(testSensorsL -> Data1_pos, 3);
//	sprintf(ch,"%lu",linear_sensor_raw);
//	LCD_StringRU(ch);
//
//	LCD_SetPos(1,1);
//	LCD_StringRU(testSensorsL -> Text); //очистка экрана
//	LCD_SetPos(testSensorsL -> Data1_pos, 1);
//	sprintf(ch,"%lu,%lu",linear_sensor/100,linear_sensor%100);
//	LCD_StringRU(ch);
//
//}
//void calibration_sensor_output_linear(int menu_pos)
//{
//	char ch[40];
//	LCD_SetPos(selectedMenuItem -> Data1_pos, menu_pos);
//	sprintf(ch,"          "); //10
//	LCD_String(ch);
//	LCD_SetPos(selectedMenuItem -> Data1_pos, menu_pos);
//	sprintf(ch,"%04lu]",linear_sensor);
//	LCD_String(ch);
//
//}
//void calibration_sensor_output_press(int menu_pos)
//{
//	char ch[40];
//	LCD_SetPos(selectedMenuItem -> Data1_pos, menu_pos);
//	sprintf(ch,"%04lu]",pressure_sensor);
//	LCD_String(ch);
//}
//void manual_sensor_output()
//{
//	char ch[40];
//	LCD_SetPos(manualMode_menu -> Data1_pos, 0);
//	sprintf(ch,"%lu ",pressure_sensor);
//	LCD_String(ch);
//	/*LCD_SetPos(manualMode_menu.Data1_pos+3, 0);
//	sprintf(ch,".");
//	LCD_String(ch);
//	LCD_SetPos(manualMode_menu.Data1_pos+4, 0);
//	sprintf(ch,"%02u",pressure_sensor%100);
//	LCD_String(ch);*/
//	LCD_SetPos(manualMode_menu -> Data2_pos, 0);
//	sprintf(ch,"%lu  ",linear_sensor);
//	LCD_String(ch);
//	/*LCD_SetPos(manualMode_menu.Data2_pos+3, 0);
//	sprintf(ch,".");
//	LCD_String(ch);
//	LCD_SetPos(manualMode_menu.Data2_pos+4, 0);
//	sprintf(ch,"%02u",linear_sensor%100);
//	LCD_String(ch);*/
//}
//void exp_sensor_output()
//{
//	char ch[40];
//	LCD_SetPos(auto_PN -> Data1_pos, 1);
//	sprintf(ch,"%lu",pressure_sensor);
//	LCD_StringRU(ch);
//	/*	LCD_SetPos(auto_PN.Data1_pos+3, 1);
//	sprintf(ch,".");
//	LCD_String(ch);
//	LCD_SetPos(auto_PN.Data1_pos+4, 1);
//	sprintf(ch,"%02u",pressure_sensor%100);
//	LCD_String(ch);*/
//	LCD_SetPos(auto_PN -> Data2_pos, 1);
//	sprintf(ch,"%lu",linear_sensor);
//	LCD_StringRU(ch);
//	/*	LCD_SetPos(auto_PN.Data2_pos+3, 1);
//	sprintf(ch,".");
//	LCD_String(ch);
//	LCD_SetPos(auto_PN.Data2_pos+4, 1);
//	sprintf(ch,"%02u",linear_sensor%100);
//	LCD_String(ch);*/
//}

//int test_sensor_before_exp()
//{
//	menuChange(selectedMenuItem->Child);
//	disp_menu(1);
//	encoder = 0;
//	scroll=0;
//	last_string = 0;
//	last_string_prev = 0;
//	HAL_Delay(1);
//	LCD_SetPos(0, 3);
//	HAL_Delay(1);
//	LCD_SendChar(0x90);
//	HAL_Delay(1);
//	while(1)
//	{
//		if (HAL_GetTick()-progtime>1500)
//		{
//			//manual_sensor_output();
//			progtime=HAL_GetTick();
//			read_sensor();
//			test_sensor_output();
//		}
//		enc1.tick();
//		if (enc1.isRight()) encoder--;//debug("Right \r\n");         // если был поворот //
//		if (enc1.isLeft()) encoder++;//debug("Left \r\n");
//		if(encoder > 1) encoder = 1;
//		if(encoder < 0) encoder = 0;
//		if(encoder!=encoder_prev)
//		{
//			HAL_Delay(1);
//			LCD_SetPos(cursor_x, 3);
//			HAL_Delay(1);
//			LCD_SendChar(0x20);
//
//			if(encoder > encoder_prev)
//			{
//				cursor_x = encoder+8;
//			}
//			else
//			{
//				cursor_x=0;
//			}
//
//
//			//вывели курсор на новой строке
//			HAL_Delay(1);
//			LCD_SetPos(cursor_x, 3);
//			HAL_Delay(1);
//			LCD_SendChar(0x90);
//			HAL_Delay(1);
//
//			encoder_prev = encoder;
//		}
//
//		if(ok_button)
//		{
//			ok_button=0;
//			keyMenu(KEY_OK);
//		}
//		if(reset_button)
//		{
//			keyMenu(KEY_LEFT);
//			cursor_x = 0;
//			encoder = 0;
//			reset_button=0;
//			scroll=0;
//			last_string=0;
//			last_string_prev=0;
//			HAL_Delay(1);
//			LCD_SetPos(0, 0);
//			HAL_Delay(1);
//			LCD_SendChar(0x90);
//			HAL_Delay(1);
//			return 0;
//		}
//		if(reset_all)
//		{
//			startMenu();
//			return 0;
//		}
//	}
//
//	return 1;
//}
//
//int next_step_exp()
//{
//	menuChange(selectedMenuItem->Child);
//	disp_menu(1);
//	encoder = 0;
//
//	HAL_Delay(1);
//	LCD_SetPos(0, 3);
//	HAL_Delay(1);
//	LCD_SendChar(0x90);
//	HAL_Delay(1);
//	while(1)
//	{
//		if (HAL_GetTick()-progtime>1500)
//		{
//			//manual_sensor_output();
//			progtime=HAL_GetTick();
//			//	read_sensor();
//			//	test_sensor_output();
//		}
//		enc1.tick();
//		if (enc1.isRight()) encoder--;//debug("Right \r\n");         // если был поворот //
//		if (enc1.isLeft()) encoder++;//debug("Left \r\n");
//		if(encoder > 1) encoder = 1;
//		if(encoder < 0) encoder = 0;
//		if(encoder!=encoder_prev)
//		{
//			HAL_Delay(1);
//			LCD_SetPos(cursor_x, 3);
//			HAL_Delay(1);
//			LCD_SendChar(0x20);
//
//			if(encoder > encoder_prev)
//			{
//				cursor_x = encoder+8;
//			}
//			else
//			{
//				cursor_x=0;
//			}
//
//
//			//вывели курсор на новой строке
//			HAL_Delay(1);
//			LCD_SetPos(cursor_x, 3);
//			HAL_Delay(1);
//			LCD_SendChar(0x90);
//			HAL_Delay(1);
//
//			encoder_prev = encoder;
//		}
//
//		if(ok_button)
//		{
//			ok_button=0;
//			menuChange(selectedMenuItem->Parent);
//			disp_menu(1);
//
//			LCD_SetPos(0, 3);
//			HAL_Delay(1);
//			LCD_SendChar(0x90);
//			HAL_Delay(1);
//			encoder=0;
//			reset_button=0;
//			if(cursor_x == 9) //Переход на N+1
//			{
//				return 1;
//			}
//			else
//			{
//				return 0;
//			}
//		}
//	}
//	return 1;
//}
//
//int unload_exp()
//{
//	menuChange(selectedMenuItem->Child);
//	disp_menu(1);
//	encoder = 0;
//	scroll=0;
//	last_string = 0;
//	last_string_prev = 0;
//	HAL_Delay(1);
//	LCD_SetPos(0, 3);
//	HAL_Delay(1);
//	LCD_SendChar(0x90);
//	HAL_Delay(1);
//	while(1)
//	{
//		if (HAL_GetTick()-progtime>1500)
//		{
//			//manual_sensor_output();
//			progtime=HAL_GetTick();
//			//	read_sensor();
//			//	test_sensor_output();
//		}
//		enc1.tick();
//		if (enc1.isRight()) encoder--;//debug("Right \r\n");         // если был поворот //
//		if (enc1.isLeft()) encoder++;//debug("Left \r\n"); //*/
//		if(encoder > 1) encoder = 1;
//		if(encoder < 0) encoder = 0;
//		if(encoder!=encoder_prev)
//		{
//			HAL_Delay(1);
//			LCD_SetPos(cursor_x, 3);
//			HAL_Delay(1);
//			LCD_SendChar(0x20);
//
//			if(encoder > encoder_prev)
//			{
//				cursor_x = encoder+8;
//			}
//			else
//			{
//				cursor_x=0;
//			}
//
//
//			//вывели курсор на новой строке
//			HAL_Delay(1);
//			LCD_SetPos(cursor_x, 3);
//			HAL_Delay(1);
//			LCD_SendChar(0x90);
//			HAL_Delay(1);
//
//			encoder_prev = encoder;
//		}
//
//		if(ok_button)
//		{
//			ok_button=0;
//			if(cursor_x == 9) //Разгрузка прибора
//			{
//				NegativeTurn(80);
//				HAL_Delay(900);
//				StopDvig();
//				reset_all = 1;
//				return 1;
//			}
//			else
//			{
//				reset_button = 1;
//			}
//
//		}
//		if(reset_button)
//		{
//			menuChange(selectedMenuItem->Parent);
//			disp_menu(1);
//			StopDvig();
//			LCD_SetPos(0, 3);
//			HAL_Delay(1);
//			LCD_SendChar(0x90);
//			HAL_Delay(1);
//			encoder=0;
//			reset_button=0;
//			return 0;
//		}
//	}
//	return 1;
//}
//int test_sensor()
//{
//	menuChange(selectedMenuItem->Child);
//	disp_menu(1);
//	encoder = 0;
//	scroll=0;
//	last_string = 0;
//	last_string_prev = 0;
//	HAL_Delay(1);
//	LCD_SetPos(0, 3);
//	HAL_Delay(1);
//	LCD_SendChar(0x90);
//	HAL_Delay(1);
//
//
//
//
//	while(1)
//	{
//		//if (HAL_GetTick()-progtime>50)
//		{
//			//manual_sensor_output();
//			//progtime=HAL_GetTick();
//			//read_sensor();
//			//pressure_sensor = ADC_pressure;
//			//linear_sensor = ADC_linear;
//			read_sensor_with_calibr();
//			test_sensor_output();
//		}
//
//
//		if(reset_button || ok_button)
//		{
//			keyMenu(KEY_LEFT);
//			cursor_x = 0;
//			encoder = 0;
//			reset_button=0;
//			ok_button = 0;
//			scroll=0;
//			last_string=0;
//			last_string_prev=0;
//			HAL_Delay(1);
//			LCD_SetPos(0, 0);
//			HAL_Delay(1);
//			LCD_SendChar(0x90);
//			HAL_Delay(1);
//			return 0;
//		}
//		if(reset_all)
//		{
//			startMenu();
//			return 0;
//		}
//	}
//
//	return 1;
//}
//
//
//
//void calibration_func()
//{
//	selectedMenuItem = calibration4;
//	disp_menu(0);
//	encoder = 0;
//	scroll=0;
//	last_string = 0;
//	last_string_prev = 0;
//	HAL_Delay(1);
//	LCD_SetPos(0, 0);
//	HAL_Delay(1);
//	LCD_SendChar(0x90);
//	HAL_Delay(1);
//}
//void sequent_input(int* a, int n, int curs_pos)
//{
//	for(int i=0;i<n;)
//	{
//		tik_callback();
//		if(encoder < 0)	encoder=0;
//		if(encoder > 9) encoder=9;
//		if(encoder != encoder_prev)
//		{
//			encoder_prev = encoder;
//			char ch[40];
//			LCD_SetPos(curs_pos + i, 2);
//			sprintf(ch,"%d",encoder);
//			LCD_StringRU(ch);
//		}
//
//		if(ok_button)
//		{
//			a[i] = encoder;
//			ok_button = 0;
//			i++;
//			encoder = 0;
//			encoder_prev = 0;
//
//		}
//		if(reset_button)
//		{
//			ok_button = 0;
//			reset_button = 0;
//			encoder = 3;
//			encoder_prev = 3;
//			break;
//		}
//	}
//}
//
//
//void sequent_input2(uint8_t* a, int n, int curs_pos, int menu_pos)
//{
//	uint8_t j=1, i=0;
//	while(i < n)
//	{
//		tik_callback();
//		if(encoder < 0)	encoder=0;
//		if(encoder > 9) encoder=9;
//		if(encoder != encoder_prev)
//		{
//			encoder_prev = encoder;
//			char ch[40];
//			LCD_SetPos(curs_pos + i, menu_pos);
//			sprintf(ch,"%d",encoder);
//			LCD_StringRU(ch);
//		}
//
//		if(ok_button)
//		{
//			a[j] = encoder;
//			ok_button = 0;
//			i++;
//			j++;
//			if(i==2 || i==5) i++;
//			encoder = 0;
//			encoder_prev = 0;
//
//		}
//	}
//}
//
//
//
//int experiment()
//{
//	menuChange(selectedMenuItem->Child);
//	disp_menu(1);
//	encoder = 0;
//	scroll = 0;
//	last_string = 0;
//	last_string_prev = 0;
//	LCD_SetPos(0, 3);
//	LCD_SendChar(0x90);
//	attempt_time = 0;
//	char str_time[6];
//	//	sprintf(str_time,"%02d:%02d",attempt_time/60,attempt_time%60);
//	HAL_TIM_Base_Start_IT(&htim4); // запуск таймера
//	while(1)
//	{
//		encoder_last_string();
//
//		if (HAL_GetTick()-progtime>1500)
//		{
//			read_sensor();
//			exp_sensor_output();
//			progtime=HAL_GetTick();
//		}
//		LCD_SetPos(4, 2);
//		//LCD_SendChar(0x20); - пробел
//		char str_time[6];
//		sprintf(str_time,"%02d:%02d",attempt_time/60,attempt_time%60);
//		LCD_String(str_time);
//
//
//	}
//
//	return 1;
//}
//int calibration_read_press(int menu_pos)
//{
//	ok_button = 0;
//	while(1)
//	{
//		if (HAL_GetTick()-sens_delay>100)
//		{
//			sens_delay=HAL_GetTick();
//			read_sensor_raw();
//			//pressure_sensor = 1456;
//			calibration_sensor_output_press(menu_pos);
//		}
//
//		if(ok_button)
//		{
//
//
//
//			strncpy( ch_tst1,selectedMenuItem->Text, strlen(selectedMenuItem->Text) - 4);
//			sprintf(ch_tst2,"%s OK",ch_tst1);
//			strncpy(selectedMenuItem->Text, ch_tst2,strlen(ch_tst2));
//
//			LCD_SetPos(1, menu_pos);
//			LCD_StringRU(selectedMenuItem->Text);
//
//			char ch[40];
//			selectedMenuItem->Data1 = pressure_sensor;
//			LCD_SetPos(selectedMenuItem -> Data1_pos, menu_pos);
//			sprintf(ch,"%04lu]", selectedMenuItem->Data1);
//			LCD_String(ch);
//			calibr_sens_arr[selectedMenuItem -> Data2][1] = selectedMenuItem->Data1;
//
//
//			ok_button = 0;
//			break;
//		}
//	}
//	return 0;
//}
//int calibration_read_linear(int menu_pos)
//{
//
//	while(1)			//n = strlen(selectedMenuItem->Text);
//	{
//		if (HAL_GetTick()-sens_delay>100)
//		{
//			sens_delay=HAL_GetTick();
//
//			calibration_sensor_output_linear(menu_pos);
//		}
//		read_sensor_raw();
//		if(ok_button)
//		{
//
//			strncpy( ch_tst1,selectedMenuItem->Text, strlen(selectedMenuItem->Text) - 4);
//			sprintf(ch_tst2,"%s OK",ch_tst1);
//			strncpy(selectedMenuItem->Text, ch_tst2,strlen(ch_tst2));
//
//			LCD_SetPos(1, menu_pos);
//			LCD_StringRU(selectedMenuItem->Text);
//
//			char ch[40];
//			selectedMenuItem->Data1 = linear_sensor;
//			LCD_SetPos(selectedMenuItem -> Data1_pos, menu_pos);
//			sprintf(ch,"%04lu]", selectedMenuItem->Data1);
//			LCD_String(ch);
//			calibr_sens_arr[selectedMenuItem -> Data2 + 11][1] = selectedMenuItem->Data1;
//
//
//			ok_button = 0;
//			break;
//		}
//	}
//	return 0;
//}
//
//
void sensor_check(void)
{
	uint16_t readtime=HAL_GetTick();
	encoder_clear();
	uint16_t time_limit = 300;

//	sprintf(ch,"%04d.%03d",read_sensor_linear()/100,read_sensor_linear()%100);
//	LCD_SetPos(9, 1);
//	LCD_StringRU(ch);
//	read_encoder();
//	sprintf(ch,"%07d",read_sensor_press());
//	LCD_SetPos(9, 0);
//	LCD_StringRU(ch);

	while(1)
	{

		char ch[40];
		read_encoder();
		sprintf(ch,"%04d.%d    ",read_sensor_press()/10,read_sensor_press()%10);
		LCD_SetPos(9, 0);
		LCD_StringRU(ch);
		HAL_Delay(5);

		sprintf(ch,"%03d.%02d   ",read_sensor_linear()/100,read_sensor_linear()%100);
		LCD_SetPos(9, 1);
		LCD_StringRU(ch);

		if(reset_button)
		{
			keyMenu(KEY_LEFT);
			encoder_clear();
			break;
		}
	}
}


void manual_mode(void)//5 - ромбики <> в меню ручной режим
{

	uint16_t readtime=0, debouncerTime = 0;
	uint8_t encoder_prev=encoder;
	encoder = 3;
	typedef enum
	{
		MOTOR_ENABLE = 1,
		MOTOR_DISABLE = 0
	} Motor_TypeDef;
	Motor_TypeDef motorState = MOTOR_DISABLE;
	int motorSpeed = 0;

	LCD_SetPos(0, 3);
	LCD_SendChar(0x90);
	char ch[40];
	read_encoder();
	uint32_t press  = read_sensor_press()/10;
	sprintf(ch,"%04d   ",press);
	LCD_SetPos(4, 0);
	LCD_StringRU(ch);
	HAL_Delay(5);

	sprintf(ch,"%03d.%02d",read_sensor_linear()/100,read_sensor_linear()%100);
	LCD_SetPos(14, 0);
	LCD_StringRU(ch);
	StopDvig();

	while(1)
	{


		tik_callback();
		if(encoder > 3) encoder = 3;
		if(encoder < 2) encoder = 2;

		if(encoder != encoder_prev)
		{
			StopDvig();
			motorState = MOTOR_DISABLE;
			LCD_SetPos(0, encoder);
			LCD_SendChar(0x90);

			//вывели курсор на новой строке
			LCD_SetPos(0, encoder_prev);
			LCD_SendChar(0x20);
			encoder_prev = encoder;
		}
		if(motorState == MOTOR_ENABLE)
		{
			motor_move(motorSpeed);
			if (HAL_GetTick()-readtime>1500)
			{
				readtime=HAL_GetTick();
				char ch[40];
				read_encoder();
				sprintf(ch,"%04d.%d    ",read_sensor_press()/10,read_sensor_press()%10);
				LCD_SetPos(4, 0);
				LCD_StringRU(ch);
				HAL_Delay(5);

				sprintf(ch,"%03d.%02d   ",read_sensor_linear()/100,read_sensor_linear()%100);
				LCD_SetPos(14, 0);
				LCD_StringRU(ch);

				if(reset_button)
				{
					keyMenu(KEY_LEFT);
					encoder_clear();
					break;
				}
			}
		}
		if(ok_button)
		{
			StopDvig();
			motorState = MOTOR_DISABLE;
			ok_button = 0;
			ZapisSD(0);
			if(encoder == 2)//управление движком
			{

				LCD_SetPos(0, 2);
				LCD_StringRU("    о о о ф о о о   ");
				encoder = 10;
				encoder_prev = encoder;
				while(1)
				{
					tik_callback_x2();
					if(encoder < 4)	encoder=4;
					if(encoder > 16) encoder=16;
					if(encoder != encoder_prev)
					{//передвигаем ромбик
						LCD_SetPos(encoder_prev, 2 );
						LCD_StringRU("о");
						LCD_SetPos(encoder, 2);
						LCD_StringRU("ф");
						encoder_prev = encoder;
					}
					if(ok_button || reset_button)
					{
						ok_button = 0;
						reset_button = 0;
						motorState = MOTOR_ENABLE;
						motorSpeed = encoder - 10;
						rotationTime = 0;
						encoder_prev = 2;
						encoder = 2;
						LCD_SetPos(0, encoder);
						LCD_SendChar(0x90);
						break;
					}
				} // while 1
			}
			else if(encoder == 3)//нагрузить до
			{
				char ch[40];
				press = read_sensor_press()/10;
				sprintf(ch,"%04d ",press );
				LCD_SetPos(4, 0);
				LCD_StringRU(ch);
				encoder = limit;

				while(1)
				{
					limit = read_encoder_set_data(15,3);
					if(ok_button)
					{
						ok_button = 0;
						 press = read_sensor_press()/10;
//
//						while(press < limit / 2)
//						{
//							motor_move(6);
//							char ch[40];
//							sprintf(ch,"%03lu.%02d",press/1000,press%100);
//							LCD_SetPos(4, 0);
//							LCD_StringRU(ch);
//
//							if(reset_button)
//							{
//								StopDvig();
//								encoder_clear();
//								break;
//							}
//						}
						while(press < limit)
						{
							motor_move(2);
							press = read_sensor_press()/10;
							sprintf(ch,"%04d ",press );
							LCD_SetPos(4, 0);
							LCD_StringRU(ch);
							HAL_Delay(5);


							if(reset_button)
							{

								StopDvig();
								encoder_clear();
								break;
							}
						}
						break;
					}

					if(reset_button)
					{
						StopDvig();
						encoder_clear();
						break;
					}
				}
			}//управление движком
		}//ok_button = 1
		if(reset_button)
		{
			StopDvig();
			encoder_clear();
			break;
		}
	}//while 1
}
