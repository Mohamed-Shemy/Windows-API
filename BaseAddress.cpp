#define PSAPI_VERSION 1
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <psapi.h>
#include <tlhelp32.h>
#include <iostream>


#pragma comment(lib, "psapi.lib")


TCHAR* ProcessNameByID(DWORD processID)
{
	TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
	if (NULL != hProcess)
	{
		HMODULE hMod;
		DWORD cbNeeded;
		if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded))
			GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName) / sizeof(TCHAR));
	}
	_tprintf(TEXT("%s  (PID: %u)\n"), szProcessName, processID);
	CloseHandle(hProcess);
	return szProcessName;
}

void GetBaseAddressByID(DWORD processId, HMODULE&hMod)
{
	TCHAR *processName  = ProcessNameByID(processId);
	TCHAR *szProcessName = processName;
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);

	if (NULL != hProcess)
	{
		DWORD cbNeeded;
		if (EnumProcessModulesEx(hProcess, &hMod, sizeof(hMod),&cbNeeded, LIST_MODULES_32BIT | LIST_MODULES_64BIT))
		{
			GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName) / sizeof(TCHAR));
			if (!_tcsicmp(processName, szProcessName)) 
				_tprintf(TEXT("Base Address : 0x%p\n"), hMod);
		}
	}
	CloseHandle(hProcess);
}

//0x009E0000 base address
//0x009FA000 risze address
//0x0001A000 offsit 106496
int main(void)
{
	DWORD processId = 0;
	float rs = 0;
	std::cout << "Process ID : ";
	std::cin >> processId;
	while (true)
	{
		std::cout << "New Size : ";
		std::cin >> rs;
		HMODULE base = NULL;
		HANDLE hProcess;

		GetBaseAddressByID(processId, base);

		DWORD offset = (DWORD)base + 106496;
		LPVOID NEWADRESS = (LPVOID)offset;

		//hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);
		//BOOL res = ReadProcessMemory(hProcess, offset, &rs, sizeof(rs), NULL);
		//printf("%d   %d\n", rs , res);

		hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_WRITE, FALSE, processId);
		BOOL suc = WriteProcessMemory(hProcess, NEWADRESS, &rs, (DWORD)sizeof(rs), 0);
		printf("%d\n", suc);

		CloseHandle(hProcess);
	}
	system("PAUSE");
	return 0;
}