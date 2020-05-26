///*
// * SD_calls.cpp
// *
// *  Created on: 28 апр. 2020 г.
// *      Author: user
// */
#include "stm32f4xx_hal.h"
#include "user_diskio.h"
#include "sd.h"
#include "fatfs.h"
#include <string>

////extern char USER_Path[4];
////char USER_Path[4]=" ";
FATFS SDFatFs;
FIL MyFile;
FRESULT res;
FATFS *fs;

int g=0;
uint32_t byteswritten;
uint8_t musor;
char DanSD[100]={0};
int ZapisSD(int FileName);
//



FRESULT open_append (
    FIL* fp,            /* [OUT] File object to create */
    const char* path    /* [IN]  File name to be opened */
)
{
    FRESULT fr;

    /* Opens an existing file. If not exist, creates a new file. */
    fr = f_open(fp, path, FA_WRITE | FA_OPEN_ALWAYS);
    if (fr == FR_OK) {
        /* Seek to end of the file to append data */
        fr = f_lseek(fp, f_size(fp));
        if (fr != FR_OK)
            f_close(fp);
    }
    return fr;
}




void SD_init()
{

		//ZapisSD(1);
		//	SpisatSD(1);  // инициализация записей с SD
}

int ZapisSD(int FileName)
{

	disk_initialize(SDFatFs.drv);
	if(f_mount(&SDFatFs, (TCHAR const*)USERPath, 0) != FR_OK)
	{
		Error_Handler();
	}
	else
	{
		if (FileName==0)
		{

		    /* Open or create a log file and ready to append */

			res = open_append(&MyFile, "logfile.txt");
		    if (res != FR_OK) return 1;

		    /* Append a line */
		    f_printf(&MyFile, "hello tivk\n");

		    /* Close the file */
		    f_close(&MyFile);


//			if(f_open(&MyFile, "ZhachDat.txt", FA_OPEN_APPEND ) != FR_OK)
//			{
//				Error_Handler();
//				FATFS_UnLinkDriver(USERPath);
//				return 0;
//
//			}
//			else
//			{
//				sprintf (DanSD, "hello\r\n");
//				res = f_write(&MyFile, DanSD, strlen(DanSD), (UINT *)&byteswritten);
//				if((byteswritten == 0) || (res != FR_OK))
//				{
//					Error_Handler();
//				}
//
//				f_close(&MyFile);
//
//			}
		}
		else
		{
			if(f_open(&MyFile, "Param.txt",  FA_WRITE | FA_CREATE_ALWAYS) != FR_OK)
			{
				Error_Handler();
			}
			else
			{
				//timeRecord timeManual S_stamp experimentNumber1 P1_menu1 Pn_menu1 Tstab1 Nstep1
				//experimentNumber2 P1_menu2 Pn_menu2 Tstab2 Nstep2 Unload2 Cycles2
				sprintf (DanSD, "hello\r\n");
				res = f_write(&MyFile, DanSD, strlen(DanSD), (UINT *)&byteswritten);
//				sprintf (DanSD, "%d,", timeRecord -> Data1);
//				res = f_write(&MyFile, DanSD, strlen(DanSD), (UINT *)&byteswritten);
//				sprintf (DanSD, "%d,", timeManual -> Data1);
//				res = f_write(&MyFile, DanSD, strlen(DanSD), (UINT *)&byteswritten);
//				sprintf (DanSD, "%d,", S_stamp -> Data1);
//				res = f_write(&MyFile, DanSD, strlen(DanSD), (UINT *)&byteswritten);
//				sprintf (DanSD, "%d,", Units_measure -> Select);
//				res = f_write(&MyFile, DanSD, strlen(DanSD), (UINT *)&byteswritten);
//
//				sprintf (DanSD, "%d,", experimentNumber1 -> Data1);
//				res = f_write(&MyFile, DanSD, strlen(DanSD), (UINT *)&byteswritten);
//				sprintf (DanSD, "%d,", P1_menu1 -> Data1);
//				res = f_write(&MyFile, DanSD, strlen(DanSD), (UINT *)&byteswritten);
//				sprintf (DanSD, "%d,", Pn_menu1 -> Data1);
//				res = f_write(&MyFile, DanSD, strlen(DanSD), (UINT *)&byteswritten);
//				sprintf (DanSD, "%d,", Tstab1 -> Data1);
//				res = f_write(&MyFile, DanSD, strlen(DanSD), (UINT *)&byteswritten);
//				sprintf (DanSD, "%d,", Nstep1 -> Data1);
//				res = f_write(&MyFile, DanSD, strlen(DanSD), (UINT *)&byteswritten);
//
//				sprintf (DanSD, "%d,", experimentNumber2 -> Data1);
//				res = f_write(&MyFile, DanSD, strlen(DanSD), (UINT *)&byteswritten);
//				sprintf (DanSD, "%d,", P1_menu2 -> Data1);
//				res = f_write(&MyFile, DanSD, strlen(DanSD), (UINT *)&byteswritten);
//				sprintf (DanSD, "%d,", Pn_menu2 -> Data1);
//				res = f_write(&MyFile, DanSD, strlen(DanSD), (UINT *)&byteswritten);
//				sprintf (DanSD, "%d,", Tstab2 -> Data1);
//				res = f_write(&MyFile, DanSD, strlen(DanSD), (UINT *)&byteswritten);
//				sprintf (DanSD, "%d,", Nstep2 -> Data1);
//				res = f_write(&MyFile, DanSD, strlen(DanSD), (UINT *)&byteswritten);
//				sprintf (DanSD, "%d,", Unload2 -> Data1);
//				res = f_write(&MyFile, DanSD, strlen(DanSD), (UINT *)&byteswritten);
//				sprintf (DanSD, "%d,", Cycles2 -> Data1);
//				res = f_write(&MyFile, DanSD, strlen(DanSD), (UINT *)&byteswritten);

				f_close(&MyFile);
			}
		}
	}
	//FATFS_UnLinkDriver(USERPath);
	return 1;
}



void SpisatSD(int FileName)
{
	disk_initialize(SDFatFs.drv);
	if(f_mount(&SDFatFs, (TCHAR const*)USERPath, 0) != FR_OK)
	{
		Error_Handler();
	}
	else
	{
		if(f_open(&MyFile, "Param.txt", FA_READ) != FR_OK)
		{
			Error_Handler();
		}
		else
		{
			res = f_read(&MyFile, DanSD, 251, (UINT *)&byteswritten);
			if((byteswritten == 0) || (res != FR_OK))
			{
				Error_Handler();
			}

			f_close(&MyFile);
		}
	}
	FATFS_UnLinkDriver(USERPath);

//	schemeExperiment -> Select = ZapisChisla();
//	obrez();
//	if(schemeExperiment -> Select == 8)
//	{
//		cancel_test2 -> Parent = experimentName2;
//		startExperiment -> Child = experimentName2;
//		strcpy( schemeExperiment -> Text, "Схема: две кривые " );
//	}
//	else
//	{
//		cancel_test2 -> Parent = experimentName1;
//		startExperiment -> Child = experimentName1;
//		strcpy( schemeExperiment -> Text, "Схема: одна кривая " );
//	}
//
//
//
//	timeRecord -> Data1=ZapisChisla();
//	obrez();
//	timeManual -> Data1=ZapisChisla();
//	obrez();
//	S_stamp -> Data1=ZapisChisla();
//	obrez();
//	Units_measure -> Select=ZapisChisla();
//	obrez();
//	if(Units_measure -> Select == 7)
//	{
//		strcpy( Units_measure -> Text, "Ед.изм.нагр :[МПа] " );
//
//	}
//	else
//	{
//		strcpy( Units_measure -> Text, "Ед.изм.нагр :[кг]  " );
//
//	}
//	experimentNumber1 -> Data1=ZapisChisla();
//	obrez();
//	P1_menu1 -> Data1=ZapisChisla();
//	obrez();
//	Pn_menu1 -> Data1=ZapisChisla();
//	obrez();
//	Tstab1 -> Data1=ZapisChisla();
//	obrez();
//	Nstep1 -> Data1=ZapisChisla();
//	obrez();
//	experimentNumber2 -> Data1=ZapisChisla();
//	obrez();
//	P1_menu2 -> Data1=ZapisChisla();
//	obrez();
//	Pn_menu2 -> Data1=ZapisChisla();
//	obrez();
//	Tstab2 -> Data1=ZapisChisla();
//	obrez();
//	Nstep2 -> Data1=ZapisChisla();
//	obrez();
//	Unload2 -> Data1=ZapisChisla();
//	obrez();
//	Cycles2 -> Data1=ZapisChisla();
//	obrez();
}





int pow10 (int i)
{
	int res1=1;
	for (int y=0; y<i; y++)
		res1*=10;
	return res1;
}


int ZapisChisla ()
{
	int y=0;
	char o=0;
	for (int i=0; i<100; i++)
	{
		if (DanSD[i]==',')
		{
			for (int u=0; u<i; u++)
			{
				o=DanSD[u]-'0';
				y+=o*pow10(i-u-1);
			}
			break;
		}
	}
	return y;
}

void obrez (void)
{
	for (int i=0; i<100; i++)
	{
		if (DanSD[i]==',')
		{
			for (int u=0; u<i+1; u++)
			{
				for (int y=0; y<100; y++)
				{
					DanSD[y]=DanSD[y+1];
					if (DanSD[y+1]=='\n') {DanSD[y+1]=0; break;}
				}
			}
			break;
		}
	}
}


