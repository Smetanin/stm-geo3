/*
 * eeprom_calls.cpp
 *
 *  Created on: 28 апр. 2020 г.
 *      Author: user
 */

#include "eeprom_calls.h"
#include "stm32f4xx_hal.h"
#include "eeprom_flash.h"
#include "main.h"
#include "lcd_menu.h"
#include "debug.h"






uint8_t date_calibr[8] = {2,1,0,1,0,1,1,0};

long calibr_sens_arr[calibr_size][2] = {
		{ 0, 1048536 },
		{ 4000, 1039709 },
		{ 200 },
		{ 300 },
		{ 400 },
		{ 500 },
		{ 1000 },
		{ 2000 },
		{ 3000 },
		{ 4000 },
		{ 5000 },
		{ 0, 262076 },
		{ 100, 175780 },
		{ 4 },
		{ 6 },
		{ 8 },
		{ 10 },
		{ 20 },
		{ 30 },
		{ 40 },
		{ 50 },
		{ 60 },
		{ 70 },
		{ 80 },
		{ 90 },
		{ 92 },
		{ 94 },
		{ 96 },
		{ 98 },
		{ 100 },
};
/* Virtual address defined by the user: 0xFFFF value is prohibited */
uint16_t VirtAddVarTab[NB_OF_VAR];
uint16_t VarDataTab[NB_OF_VAR][2] = {			//= {'M', 'a', 't', 'e', 'u', 's', 'z', ' ', 'S', 'a', 'l', 'a', 'm', 'o', 'n',
		//' ', 'm', 's', 'a', 'l', 'a', 'm', 'o', 'n', '.', 'p', 'l', 't', 't', 't'};
		{ 0 },
		{ 100 },
		{ 200 },
		{ 300 },
		{ 400 },
		{ 500 },
		{ 1000 },
		{ 2000 },
		{ 3000 },
		{ 4000 },
		{ 5000 },
		{ 0 },
		{ 2 },
		{ 4 },
		{ 6 },
		{ 8 },
		{ 10 },
		{ 20 },
		{ 30 },
		{ 40 },
		{ 50 },
		{ 60 },
		{ 70 },
		{ 80 },
		{ 90 },
		{ 92 },
		{ 94 },
		{ 96 },
		{ 98 },
		{ 100 },
		{ 0 },
		{ 100 },
		{ 200 },
		{ 300 },
		{ 400 },
		{ 500 },
		{ 1000 },
		{ 2000 },
		{ 3000 },
		{ 4000 },
		{ 5000 },
		{ 0 },
		{ 2 },
		{ 4 },
		{ 6 },
		{ 8 },
		{ 10 },
		{ 20 },
		{ 30 },
		{ 40 },
		{ 50 },
		{ 60 },
		{ 70 },
		{ 80 },
		{ 90 },
		{ 92 },
		{ 94 },
		{ 96 },
		{ 98 },
		{ 100 },
		{ 0 },
		{ 100 },
		{ 200 },
		{ 300 },
		{ 400 },
		{ 500 },
		{ 1000 },
		{ 2000 },
		{ 3000 },
		{ 4000 },
		{ 5000 },
		{ 0 },
		{ 2 },
		{ 4 },
		{ 6 },
		{ 8 },
		{ 10 },
		{ 20 },
		{ 30 },
		{ 40 },
		{ 50 },
		{ 60 },
		{ 70 },
		{ 80 },
		{ 90 },
		{ 92 },
		{ 94 },
		{ 96 },
		{ 98 },
		{ 100 },
		{ 0 },
		{ 100 },
		{ 200 },
		{ 300 },
		{ 400 },
		{ 500 },
		{ 1000 },
		{ 2000 },
		{ 3000 },
		{ 4000 },
		{ 5000 },
		{ 0 },
		{ 2 },
		{ 4 },
		{ 6 },
		{ 8 },
		{ 10 },
		{ 20 },
		{ 30 },
		{ 40 },
		{ 50 },
		{ 60 },
		{ 70 },
		{ 80 },
		{ 90 },
		{ 92 },
		{ 94 },
		{ 96 },
		{ 98 },
		{ 100 },
		{ 100 },
		{ 100 },
		{ 100 },
		{ 100 },
		{ 100 },
		{ 100 },
		{ 100 },
		{ 100 },
		{ 100 },
		{ 100 },
		{ 100 },
		{ 100 },
		{ 100 },
		{ 100 },
};
uint8_t VarDataTabRead[NB_OF_VAR];
uint16_t VarIndex,VarDataTmp = 0;


void flash_init()
{
	/* Unlock the Flash Program Erase controller */
	HAL_FLASH_Unlock();
	/* EEPROM Init */
	if( EE_Init() != EE_OK)
	{
		Error_Handler();
	}
	//********************************************************************
	// Fill EEPROM variables addresses
	for(VarIndex = 1; VarIndex <= NB_OF_VAR; VarIndex++)
	{
		VirtAddVarTab[VarIndex-1] = VarIndex;
	}
	//********************************************************************
	// Read Values in EEPROM emulation

	for (VarIndex = 0; VarIndex < NB_OF_VAR; VarIndex++)
	{
		if((EE_ReadVariable(VirtAddVarTab[VarIndex],(uint16_t*)&VarDataTabRead[VarIndex])) != HAL_OK)
		{
			Error_Handler();
		}
	}
	//********************************************************************
	// Заполняем таблицу коэффициентов
	int j=0;
	for(int i=0;i<NB_OF_VAR - 14;)
	{
		calibr_sens_arr[j][1] = ((uint32_t)VarDataTabRead[i] << 24)|((uint32_t)VarDataTabRead[i+1] << 16)|((uint32_t)VarDataTabRead[i+2] << 8)|((uint32_t)VarDataTabRead[i+3]);
		char eeprom_ch[40];
//		sprintf(eeprom_ch,"%d\r\n",calibr_sens_arr[j][1]);
//		debug(eeprom_ch);
		i+=4;
		j++;
	}

	//*******************************************************************
	// Инициализация калибровочных данных для датчиков
	selectedMenuItem = calibr_pressure_menu->next;//инициализация калибровки датчика давления
	for(int i=0;i<10; i++)
	{
		selectedMenuItem = selectedMenuItem ->next;
		selectedMenuItem->payload.Data[0]= calibr_sens_arr[i][1];
	}

	selectedMenuItem = calibr_linear_menu->next; //инициализация калибровки линейного датчика
	for(int i=11;i<29; i++)
	{
		selectedMenuItem = selectedMenuItem ->next;
		selectedMenuItem -> payload.Data[0] = calibr_sens_arr[i][1];
	}
	/** работает******************************************/
//	//********************************************************************
//	// Инициализация даты и времени калибровки
//	j=0;
//	for(int i=120;i<127;i++)
//	{
//		date_calibr[j] = VarDataTabRead[i];
//		char eeprom_ch[40];
//		sprintf(eeprom_ch,"%d\r\n",date_calibr[j]);
//		debug(eeprom_ch);
//		j++;
//	}
//	char ch[50];
//	sprintf(ch,"%s\r\n",calibration5->Text);
//	debug(ch);
//	calibration5->Data1 = date_calibr[0];
//	strncpy( ch_tst1,calibration5->Text,calibration5->Data2_pos-1);
//	sprintf(ch_tst2,"%s%03d %d%d.%d%d.%d%d",ch_tst1,date_calibr[0],date_calibr[1],date_calibr[2],date_calibr[3],date_calibr[4],date_calibr[5],date_calibr[6]);
//	int n = strlen(ch_tst2);
//	strncpy(calibration5->Text, ch_tst2,n);
//	sprintf(ch,"%s\r\n",calibration5->Text);
//	debug(ch);
//	j=0;
//	for(int i=127;i<134;i++)
//	{
//		date_calibr[j] = VarDataTabRead[i];
//		char eeprom_ch[40];
//		sprintf(eeprom_ch,"%d\r\n",date_calibr[j]);
//		debug(eeprom_ch);
//		j++;
//	}
//	sprintf(ch,"%s\r\n",calibration6->Text);
//	debug(ch);
//	calibration6->Data1 = date_calibr[0];
//	strncpy( ch_tst1,calibration6->Text,calibration6->Data2_pos-1);
//	sprintf(ch_tst2,"%s%02d %d%d.%d%d.%d%d",ch_tst1,date_calibr[0],date_calibr[1],date_calibr[2],date_calibr[3],date_calibr[4],date_calibr[5],date_calibr[6]);
//	n = strlen(ch_tst2);
//	strncpy(calibration6->Text, ch_tst2,n);
//	sprintf(ch,"%s\r\n",calibration6->Text);
//	//********************************************************************
//	// Инициализация даты и времени поверки
//	j=0;
//	for(int i=120;i<127;i++)
//	{
//		date_calibr[j] = VarDataTabRead[i];
//		char eeprom_ch[40];
//		sprintf(eeprom_ch,"%d\r\n",date_calibr[j]);
//		debug(eeprom_ch);
//		j++;
//	}
//
//	sprintf(ch,"%s\r\n",calibration45->Text);
//	debug(ch);
//	calibration45->Data1 = date_calibr[0];
//	strncpy( ch_tst1,calibration45->Text,calibration45->Data2_pos-1);
//	sprintf(ch_tst2,"%s%03d %d%d.%d%d.%d%d",ch_tst1,date_calibr[0],date_calibr[1],date_calibr[2],date_calibr[3],date_calibr[4],date_calibr[5],date_calibr[6]);
//	n = strlen(ch_tst2);
//	strncpy(calibration45->Text, ch_tst2,n);
//	sprintf(ch,"%s\r\n",calibration45->Text);
//	debug(ch);
//	j=0;
//	for(int i=127;i<134;i++)
//	{
//		date_calibr[j] = VarDataTabRead[i];
//		char eeprom_ch[40];
//		sprintf(eeprom_ch,"%d\r\n",date_calibr[j]);
//		debug(eeprom_ch);
//		j++;
//	}
//	sprintf(ch,"%s\r\n",calibration46->Text);
//	debug(ch);
//	calibration46->Data1 = date_calibr[0];
//	strncpy( ch_tst1,calibration46->Text,calibration46->Data2_pos-1);
//	sprintf(ch_tst2,"%s%02d %d%d.%d%d.%d%d",ch_tst1,date_calibr[0],date_calibr[1],date_calibr[2],date_calibr[3],date_calibr[4],date_calibr[5],date_calibr[6]);
//	n = strlen(ch_tst2);
//	strncpy(calibration46->Text, ch_tst2,n);
//	sprintf(ch,"%s\r\n",calibration46->Text);
//	LCD_SetPos(0,0);
//	LCD_StringRU(calibration46->Text);
//	//********************************************************************
//	// Инициализация даты и времени меню калибровка-поверка
//	j=0;
//	for(int i=120;i<127;i++)
//	{
//		date_calibr[j] = VarDataTabRead[i];
//		char eeprom_ch[40];
//		sprintf(eeprom_ch,"%d\r\n",date_calibr[j]);
//		debug(eeprom_ch);
//		j++;
//	}
/** работает******************************************/
//	sprintf(ch,"%s\r\n",calibration42->Text);
//	debug(ch);
//	calibration42->Data1 = date_calibr[0];
//	strncpy( ch_tst1,calibration42->Text,calibration42->Data2_pos-1);
//	sprintf(ch_tst2,"%s%d%d.%d%d.%d%d",ch_tst1,date_calibr[1],date_calibr[2],date_calibr[3],date_calibr[4],date_calibr[5],date_calibr[6]);
//	n = strlen(ch_tst2);
//	strncpy(calibration42->Text, ch_tst2,n);
//	sprintf(ch,"%s\r\n",calibration42->Text);
//	LCD_SetPos(0,1);
//	LCD_StringRU(calibration42->Text);
//	debug(ch);
/*********************************************************/
	/*	j=0;
	for(int i=127;i<134;i++)
	{
		date_calibr[j] = VarDataTabRead[i];
		char eeprom_ch[40];
		sprintf(eeprom_ch,"%d\r\n",date_calibr[j]);
		debug(eeprom_ch);
		j++;
	}
	sprintf(ch,"%s\r\n",calibration43->Text);
	debug(ch);
	calibration41->Data1 = date_calibr[0];
	strncpy( ch_tst1,calibration41->Text,calibration41->Data2_pos-1);
	sprintf(ch_tst2,"%s%d%d.%d%d.%d%d",ch_tst1,date_calibr[1],date_calibr[2],date_calibr[3],date_calibr[4],date_calibr[5],date_calibr[6]);
	n = strlen(ch_tst2);
	strncpy(calibration41->Text, ch_tst2,n);
	sprintf(ch,"%s\r\n",calibration41->Text);
	 */
//	debug(ch);
//	debug("eeprom ok\r\n");
}


void save_calibr_data(void)
{
	int j = 0;
	int shift = 24;
	for (VarIndex = 0; VarIndex < NB_OF_VAR - 14;VarIndex++ )  //таблица калибровки
	{
		/* Sequence 1 */
		uint16_t VarData_Byte = calibr_sens_arr[j][1] >> shift;
		if((EE_WriteVariable(VirtAddVarTab[VarIndex],  VarData_Byte)) != HAL_OK)
		{
			Error_Handler();
		}
		shift -= 8;
		if(shift < 0)
		{
			j++;
			shift = 24;
		}

	}
	j = 0;
	for (VarIndex = NB_OF_VAR - 14; VarIndex < NB_OF_VAR - 7; j++)//дата калибровки датчик силы
	{
		uint16_t VarData_Byte = date_calibr[j];
		if((EE_WriteVariable(VirtAddVarTab[VarIndex],  VarData_Byte)) != HAL_OK)
		{
			Error_Handler();
		}
		VarIndex++;
	}
	j = 0;
	for (VarIndex = NB_OF_VAR - 7; VarIndex < NB_OF_VAR; j++) //дата калибровки линейный датчик
	{
		uint16_t VarData_Byte = date_calibr[j];
		if((EE_WriteVariable(VirtAddVarTab[VarIndex],  VarData_Byte)) != HAL_OK)
		{
			Error_Handler();
		}
		VarIndex++;
	}
	debug("eeprom write\r\n");
}



void flash_write_var()
{
			/* Unlock the Flash Program Erase controller */
				HAL_FLASH_Unlock();
				/* EEPROM Init */
				if( EE_Init() != EE_OK)
				{
					Error_Handler();
				}
				//********************************************************************
				// Fill EEPROM variables addresses
				for(VarIndex = 1; VarIndex <= NB_OF_VAR; VarIndex++)
				{
					VirtAddVarTab[VarIndex-1] = VarIndex;
				}
				//********************************************************************
			save_calibr_data();
}
