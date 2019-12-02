#undef UNICODE

#include "stdafx.h"

#include <stdio.h>
#include <conio.h>
#include <windows.h>

HANDLE mutex, wrt;
int g_ReadCount = 0;
int g_GlobalData = 0;

const int max = 2;
HANDLE reader[max], writer[max];
CRITICAL_SECTION rSect, wSect;

DWORD Readers(LPVOID lpdwThreadParam)
{
	while (true)
	{
		WaitForSingleObject(mutex, INFINITE);
		g_ReadCount++;
		if (g_ReadCount == 1)
			WaitForSingleObject(wrt, INFINITE);
		ReleaseMutex(mutex);

		EnterCriticalSection(&wSect);
		printf("Thread ID : %d  Read data : %d  ReaderCount %d\n", GetCurrentThreadId(), g_GlobalData, g_ReadCount);
		LeaveCriticalSection(&wSect);

		WaitForSingleObject(mutex, INFINITE);
		g_ReadCount--;
		if (g_ReadCount == 0)
		{
			ReleaseMutex(wrt);
			printf("Thread ID : %d Realesed Mutex wrt\n", GetCurrentThreadId());
		}
		printf("Thread ID : %d ReaderCount %d\n", GetCurrentThreadId(), g_ReadCount);
		ReleaseMutex(mutex);
		printf("Reader Thread ID : %d Realesed Mutex mutex\n", g_ReadCount);
		Sleep(0);
	}
	return 0;
}


DWORD Writers(LPVOID lpdwThreadParam)
{
	int n = GetCurrentThreadId();
	int temp = 1;
	while (true)
	{
		printf("Thread ID : %d Waiting for WRT\n", GetCurrentThreadId());
		WaitForSingleObject(wrt, INFINITE);
		printf("WRITER Thread ID : %d    Got  WRT\n", GetCurrentThreadId());
		++n;
		temp++;
		EnterCriticalSection(&wSect);
		printf("Write by Thread ID : %d Data : %d   Temp %d\n", GetCurrentThreadId(), n, temp);
		g_GlobalData = n;
		LeaveCriticalSection(&wSect);
		ReleaseMutex(wrt);
	}

	printf("Exiting Writer Thread\n");
	return 0;
}

void main()
{
	mutex = CreateMutexA(NULL, false, "Writer");
	wrt = CreateMutexA(NULL, false, "wrt");

	InitializeCriticalSection(&rSect);
	InitializeCriticalSection(&wSect);
	DWORD dwThreadId = 0;

	for (int i = 0; i < max; i++)
		reader[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&Readers, (LPVOID)0, 0, &dwThreadId);

	for (int i = 0; i < 1; i++)
		writer[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&Writers, (LPVOID)0, 0, &dwThreadId);

	system("pause");
}