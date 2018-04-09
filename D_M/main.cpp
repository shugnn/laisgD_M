#pragma once
#include <iostream>
#include<Windows.h>
#include<process.h>
#include<math.h>
using namespace std;
#include"StkAna.h"
CAna* pAna;
bool force_run = false;//true;//
void main(void)
{
	cout << __FUNCTION__ << " Welcom StkAna!" << endl;
	unsigned int _stdcall  ThreadAna(void* lpParamter);
	bool TimeToAct = true;
	SYSTEMTIME sys_time;
	while (true)
	{
		if (TimeToAct)
		{
			if (!pAna)
			{
				pAna = new CAna();
				pAna->Load();
				cin.get();
				//return;

				HANDLE handle1 = (HANDLE)_beginthreadex(NULL, 0, ThreadAna, NULL, 0, NULL);
			}
			TimeToAct = false;
		}
		Sleep(5 * 60 * 1000);
		GetLocalTime(&sys_time);
		//if (fmod(sys_time.wHour, 12) == 6 && sys_time.wMinute > 50)TimeToAct = true;
	}
}
unsigned int _stdcall  ThreadAna(void* lpParamter)
{
	pAna->Load();
	delete pAna;
	pAna = NULL;
	return 0;
}