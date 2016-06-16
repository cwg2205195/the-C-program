#include<windows.h>
#include<stdio.h>
#include"tchar.h"
#include<ctype.h>
BOOL InjectDll(DWORD dwPID,LPCTSTR szDllPath)
{
	HANDLE hProcess=NULL,hThread=NULL;
	HMODULE hMod=NULL;
	LPVOID pRemoteBuf=NULL;
	DWORD dwBufSize=(DWORD)(_tcslen(szDllPath)+1)*sizeof(TCHAR);
	LPTHREAD_START_ROUTINE pThreadProc;
	//printf("Path=%s  and buff size =%ld\n",szDllPath,dwBufSize);
	//#1.use the dwPID to get target process handle
	if(!(hProcess=OpenProcess(PROCESS_ALL_ACCESS,FALSE,dwPID)))
	{
		_tprintf("OpenProcess(%d) failed!!! [%d]\n",dwPID,GetLastError());
		return FALSE;
	}
	
	//#2. allocate some space in target process for dllname
	if(NULL==(pRemoteBuf=VirtualAllocEx(hProcess,NULL,dwBufSize,MEM_COMMIT,PAGE_READWRITE)))
		return FALSE;//Allocate failed !
		
	//#3. write the path of the dll into the space 
	if(WriteProcessMemory(hProcess,pRemoteBuf,(LPVOID)szDllPath,dwBufSize,NULL)==0)
		return FALSE;//write failed 
	
	//#4. get the address of LoadLibraryW() API
	hMod=GetModuleHandle("kernel32.dll");
	pThreadProc=(LPTHREAD_START_ROUTINE)GetProcAddress(hMod,"LoadLibraryA");
	
	//#5. create remote thread
	if(NULL==(hThread=CreateRemoteThread(hProcess,NULL,0,pThreadProc,pRemoteBuf,0,NULL)))
		return FALSE;//create remote thread failed !
	WaitForSingleObject(hThread,INFINITE);
	CloseHandle(hThread);
	CloseHandle(hProcess);
	return TRUE;
}

int _tmain(int argc,char *argv[])
{
	if(argc != 3)
	{
		_tprintf("Usage: %s pid dll_path\n",argv[0]);
		return 1;
	}
	//inject dll
	if (InjectDll((DWORD)_tstol(argv[1]),argv[2]))
		_tprintf("InjectDll(%s) success!!!\n",argv[2]);
	else
		_tprintf("InjectDll(%s) failed!!!\n",argv[2]);
	return 0;
}
