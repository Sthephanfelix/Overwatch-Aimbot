#pragma once
 
#include <Windows.h>
#include <TlHelp32.h>
#include <psapi.h>
//THIS FILE SIMPLY DOES MOST OF THE BACKEND WORK FOR US,
//FROM FINDING THE PROCESS TO SETTING UP CORRECT ACCESS FOR US
//TO EDIT MEMORY
//IN MOST GAMES, A SIMPLER VERSION OF THIS CAN BE USED, or if you're injecting then its often not necessary
//This file has been online for quite a while so credits should be shared but im using this from NubTIK
//So Credits to him and thanks
 




class CHackProcess
{
public:
	HANDLE		_GameHandle;
	DWORD		_GameProcessID;
	CHAR*			_GameProcessName;

	HANDLE Open(DWORD ProcessID)
	{
		_GameProcessID = ProcessID;
		_GameHandle = OpenProcess(PROCESS_ALL_ACCESS, NULL, ProcessID);
		return _GameHandle;
	}
	BOOL Close()
	{
		return CloseHandle(_GameHandle);
	}

    DWORD GetProcess(CHAR *__ProcessName, PROCESSENTRY32 *pEntry)//进程名获取进程信息
    {   
		_GameProcessName = __ProcessName;
        PROCESSENTRY32 __ProcessEntry;
        __ProcessEntry.dwSize = sizeof(PROCESSENTRY32);
        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (hSnapshot == INVALID_HANDLE_VALUE) return 0;      
		if (!Process32First(hSnapshot, &__ProcessEntry))
        {
            CloseHandle(hSnapshot);
            return 0;
        }
		        do{if (!_strcmpi(__ProcessEntry.szExeFile, __ProcessName))
        {
			memcpy((void *)pEntry, (void *)&__ProcessEntry, sizeof(PROCESSENTRY32));
		        CloseHandle(hSnapshot);
				Open(__ProcessEntry.th32ProcessID);
            return __ProcessEntry.th32ProcessID;//返回进程ID
        }} while (Process32Next(hSnapshot, &__ProcessEntry));
        CloseHandle(hSnapshot);
        return 0;
    }
	INT64 GetProcessModuleOffset()
	{
		MODULEENTRY32 lpModuleEntry = { 0 };
		HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, _GameProcessID);
		if (!hSnapShot)
			return NULL;
		lpModuleEntry.dwSize = sizeof(lpModuleEntry);
		BOOL __RunModule = Module32First(hSnapShot, &lpModuleEntry);
		while (__RunModule)
		{
			if (!_strcmpi(lpModuleEntry.szModule, _GameProcessName))
			{
				CloseHandle(hSnapShot);
				return (INT64)lpModuleEntry.modBaseAddr;
			}
			__RunModule = Module32Next(hSnapShot, &lpModuleEntry);
		}
		CloseHandle(hSnapShot);
		return NULL;
	}



	template <class T> T Read(INT64 dwAddress)
	{
		T value;
		ReadProcessMemory(_GameHandle, (LPVOID)dwAddress, &value, sizeof(T), NULL);
		return value;
	}
	template <class T> bool Write(INT64 dwAddress, T ValueToWrite)
	{
		return WriteProcessMemory(_GameHandle, (LPVOID)dwAddress, &ValueToWrite, sizeof(T), NULL) == TRUE;
	}


	bool IsValid32(int address)
	{
		return (address >= 0x40000 && address < 0x7fffffff);
	}
	bool IsValid64(INT64 address)
	{
		return (address >= 0x400000 && address < 0x000F000000000000);
	}









 
	DWORD_PTR dwGetModuleBaseAddress(DWORD dwProcID, TCHAR *szModuleName)
	{
		DWORD_PTR dwModuleBaseAddress = 0;
		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, dwProcID);
		if (hSnapshot != INVALID_HANDLE_VALUE)
		{
			MODULEENTRY32 ModuleEntry32;
			ModuleEntry32.dwSize = sizeof(MODULEENTRY32);
			if (Module32First(hSnapshot, &ModuleEntry32))
			{
				do
				{
					if (_tcsicmp(ModuleEntry32.szModule, szModuleName) == 0)
					{
						dwModuleBaseAddress = (DWORD_PTR)ModuleEntry32.modBaseAddr;
						break;
					}
				} while (Module32Next(hSnapshot, &ModuleEntry32));
			}
			CloseHandle(hSnapshot);
		}
		return dwModuleBaseAddress;
	}

    DWORD getThreadByProcess(DWORD __DwordProcess)
    {   
            THREADENTRY32 __ThreadEntry;
            __ThreadEntry.dwSize = sizeof(THREADENTRY32);
            HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
            if (hSnapshot == INVALID_HANDLE_VALUE) return 0;
 
            if (!Thread32First(hSnapshot, &__ThreadEntry)) {CloseHandle(hSnapshot); return 0; }
 
            do {if (__ThreadEntry.th32OwnerProcessID == __DwordProcess)
            {
                CloseHandle(hSnapshot);
                return __ThreadEntry.th32ThreadID;
            }} while (Thread32Next(hSnapshot, &__ThreadEntry));
            CloseHandle(hSnapshot);      
            return 0;
    }
 
    
	
	


	void ReadShare(char *ShareValue, LPSTR Name)
	{
		LPSTR lpMapAddr;
		HANDLE m_hMapFile = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, Name);
		if (m_hMapFile)
		{
			lpMapAddr = (LPSTR)MapViewOfFile(m_hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 0);
			memcpy(ShareValue, lpMapAddr, strlen(lpMapAddr) - 8); //-8 对应超级模块8.0 的指针 .写共享
			UnmapViewOfFile(lpMapAddr);
			CloseHandle(m_hMapFile);
		}
		else
		{
			memset(ShareValue, 0, strlen(ShareValue));
		}
	}


	DWORD GetProcessID(char *FileName)//取进程ID，（进程名）
	{
		HANDLE myhProcess;
		PROCESSENTRY32 mype;
		mype.dwSize = sizeof(PROCESSENTRY32);
		BOOL mybRet;
		myhProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		mybRet = Process32First(myhProcess, &mype);
		while (mybRet)
		{
			if (strcmp(FileName, mype.szExeFile) == 0)
				return mype.th32ProcessID;
			else
				mybRet = Process32Next(myhProcess, &mype);
		}
		return 0;
	}
	int EnableDebugPriv(char* name)
	{
		HANDLE hToken;
		TOKEN_PRIVILEGES tp;
		LUID luid;
		//打开进程令牌环

		OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
		//获得进程本地唯一ID

		LookupPrivilegeValue(NULL, name, &luid);

		tp.PrivilegeCount = 1;
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		tp.Privileges[0].Luid = luid;
		//调整权限
		AdjustTokenPrivileges(hToken, 0, &tp, sizeof(TOKEN_PRIVILEGES), NULL, NULL);
		return 0;

	}
};














