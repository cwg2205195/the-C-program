#include<stdio.h>
#include<windows.h>
#include<unistd.h>
#include<winnt.h>
#include<stdlib.h>
#include"_mingw.h"
#include"apiset.h"
#include"debugapi.h"
#define dwContinuesStatus DBG_CONTINUE
FARPROC g_pfWriteFile=NULL;
CREATE_PROCESS_DEBUG_INFO g_cpdi;
BYTE g_chINT3=0xCC,g_ch0rgByte=0;

BOOL OnCreateProcessDebugEvent(LPDEBUG_EVENT pde)
{
	HMODULE hMod=GetModuleHandle("kernel32.dll");
	SIZE_T a;
	//get the WriteFile() 's address
	g_pfWriteFile=GetProcAddress(hMod,"WriteFile");
	//the API hook ....change the first byte to 0xCC (INT3)  
	//originalbyte is g_ch0rgByte's backup
	memcpy(&g_cpdi,&pde->u.CreateThread,sizeof(CREATE_PROCESS_DEBUG_INFO));
	ReadProcessMemory(g_cpdi.hProcess,(LPVOID)g_pfWriteFile,&g_ch0rgByte,sizeof(BYTE),&a);
	WriteProcessMemory(g_cpdi.hProcess,(LPVOID)g_pfWriteFile,&g_chINT3,sizeof(BYTE),NULL);
	return true;
}

BOOL OnExceptionDebugEvent(LPDEBUG_EVENT pde)
{
	_CONTEXT ctx;
	PBYTE lpBuffer=NULL;
	DWORD dwNumOfBytesToWrite,dwAddrOfBuffer,i;
	PEXCEPTION_RECORD per=&pde->u.Exception.ExceptionRecord;
	//when the exception code is int3
	printf("exception code =%X @ %X \n",per->ExceptionCode,per->ExceptionAddress);
	if(EXCEPTION_BREAKPOINT==per->ExceptionCode)
	{
		printf("Got int 3 \n");
		//if the breakpoint is WriteFile ()
		if(g_pfWriteFile==per->ExceptionAddress)
		{
			printf("At win API writefile()\n");
			//#1.Unhook
			//   recover the 0xCC to original byte
			WriteProcessMemory(g_cpdi.hProcess,(LPVOID)g_pfWriteFile,&g_ch0rgByte,sizeof(BYTE),NULL);
			
			//#2.get the context of the thread
			ctx.ContextFlags=CONTEXT_CONTROL;
			GetThreadContext(g_cpdi.hThread,&ctx);
			
			//#3.get the WriteFile's parameter
			//param 2:ESP+0x8
			//param 3:ESP+0xC
			ReadProcessMemory(g_cpdi.hProcess,(LPVOID)(ctx.Esp+0x8),&dwAddrOfBuffer,sizeof(DWORD),NULL);
			ReadProcessMemory(g_cpdi.hProcess,(LPVOID)(ctx.Esp+0xC),&dwNumOfBytesToWrite,sizeof(DWORD),NULL);
			
			//#4.allocate tempary buffer
			lpBuffer=(PBYTE)malloc(dwNumOfBytesToWrite+1);
			memset(lpBuffer,0,dwNumOfBytesToWrite+1);
			
			//#5.copy writefile() to temporary buffer
			ReadProcessMemory(g_cpdi.hProcess,(LPVOID)dwAddrOfBuffer,lpBuffer,dwNumOfBytesToWrite,NULL);
			printf("\n### orginal string: %s\n",lpBuffer);
			
			//#6.transform from upper case to lowwer case
			for(i=0;i<dwNumOfBytesToWrite;i++)
			{
				if(0x61<=lpBuffer[i]&&lpBuffer[i]<=0x7A)
					lpBuffer[i]-=0x20;
			}
			printf("\n### converted string : %s\n",lpBuffer);
			
			//#7.copy the converted buff string to destination address
			WriteProcessMemory(g_cpdi.hProcess,(LPVOID)dwAddrOfBuffer,lpBuffer,dwNumOfBytesToWrite,NULL);
			
			//#8.free temporary buff
			free(lpBuffer);
			
			//#9.change the EIP of the context to WriteFile()
			ctx.Eip=(DWORD)g_pfWriteFile;
			SetThreadContext(g_cpdi.hThread,&ctx);
			
			//#10.run the debuged process
			ContinueDebugEvent(pde->dwProcessId,pde->dwThreadId,DBG_CONTINUE);
			
			sleep(5);
			//#13.API hook
			WriteProcessMemory(g_cpdi.hProcess,(LPVOID)g_pfWriteFile,&g_chINT3,sizeof(BYTE),NULL);
			return true;
		}
	}
	return false;
}
void DebugLoop()
{
	DEBUG_EVENT de;
	//int dwContinuesStatus;
	
	//wait for event....
	while(WaitForDebugEvent(&de,INFINITE))
	{
		//dwContinuesStatus=0x00010002;
		//被调试进程生成或者附加事件
		if(CREATE_PROCESS_DEBUG_EVENT==de.dwDebugEventCode) 
			{
				printf("created debug !\n");
				OnCreateProcessDebugEvent(&de);
				printf("seccessfully created int 3\n");
			}
		//异常事件
		else if (EXCEPTION_DEBUG_EVENT==de.dwDebugEventCode) 
			{
				printf("Exception debug event !code =%X\n",de.dwDebugEventCode);
				if(OnExceptionDebugEvent(&de))
					{
						printf("on exception debug event\n");
						continue;
					}
			}
			
	
	//被调试进程终止事件
	else if (EXIT_PROCESS_DEBUG_EVENT==de.dwDebugEventCode) //被调试者终止-调试器终止 
		{
			printf("debugee exited \n");
			break;
		}
	//再次运行被调试者
	ContinueDebugEvent(de.dwProcessId,de.dwThreadId,0x00010002) ;
	}
}
int main(int argc,char *argv[]) 
{
	DWORD dwPID;
	
	if(argc!=2)
	{
		printf("Usage:%s pid\n",argv[0]);
		return 1;
	}
	
	//Attach Process
	dwPID=atoi(argv[1]);
	if(!DebugActiveProcess(dwPID))
	{
		printf("DebugActiveProcess(%d) failed!\n","Error code =%d\n",dwPID,GetLastError());
		return 1;
	}
	
	//debug loop
	DebugLoop();
	return 0;
}
