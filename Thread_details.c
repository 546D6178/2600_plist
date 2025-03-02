#include <windows.h>
#include <stdio.h>
#include <tlhelp32.h>
#include <psapi.h>
#include <tchar.h>
#include <time.h>
#include <tchar.h>

HANDLE get_snap_thr();
HANDLE get_snap_procc();



void print_thread_details( DWORD owner_pid)
{
    HANDLE hThreadSnap = get_snap_thr();
    THREADENTRY32 te;
    CONTEXT threadContext;
    te.dwSize = sizeof(THREADENTRY32);

    // formating the output to aligned columns
    const int width_Name = 10;
    const int other_column = 15;
    printf("%-*s %*s %*s %*s %*s %*s %*s\n", other_column, "Tid", other_column, "Pid", other_column,"Cswitch", other_column, "State", other_column, "User Time", other_column, "Kernel Time", other_column, "Elapsed Time");
    int count = 0; 

    BOOL bThread = Thread32First(hThreadSnap, &te);
    while(bThread)
    {
        if( te.th32OwnerProcessID == owner_pid )
         {  
            
            SYSTEMTIME kernelSystemTime, userSystemTime;
            FILETIME threadCreationTime, threadExitTime, threadKernelTime, threadUserTime;
            HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, te.th32ThreadID);
            GetThreadTimes(hThread,&threadCreationTime, &threadExitTime, &threadKernelTime, &threadUserTime);
            FileTimeToSystemTime(&threadKernelTime, &kernelSystemTime);
            FileTimeToSystemTime(&threadUserTime, &userSystemTime);
            SuspendThread(hThread); // Cswtch
            GetThreadContext(hThread, &threadContext);
            //if(GetThreadContext(hThread, &threadContext))
            ResumeThread(hThread); // Cswtch
            TCHAR buffer[100];
            sprintf(buffer, TEXT("%02d:%02d:%02d:%03d "), kernelSystemTime.wHour, kernelSystemTime.wMinute, kernelSystemTime.wSecond, kernelSystemTime.wMilliseconds);

            TCHAR buffer2[100];
            sprintf(buffer2, TEXT("%02d:%02d:%02d:%03d"), userSystemTime.wHour, userSystemTime.wMinute, userSystemTime.wSecond, userSystemTime.wMilliseconds);
            
            //elapsed time 
            FILETIME createTime, exitTime, kernelTime, userTime;
            SYSTEMTIME processTime, currentTime;
            GetThreadTimes(hThread,&createTime, &exitTime, &kernelTime, &userTime);
            FileTimeToSystemTime(&createTime, &processTime);
            GetSystemTime(&currentTime);

            int elapsedTime = (currentTime.wHour - processTime.wHour) * 3600 + (currentTime.wMinute - processTime.wMinute) * 60 + 
                (currentTime.wSecond - processTime.wSecond);
             //end of block
                _tprintf( TEXT("%-*u %*u %*u %*s %*s %*s"), other_column, te.th32ThreadID, other_column, te.tpBasePri, other_column, threadContext.ContextFlags, other_column, "STATE", other_column, buffer2, other_column, buffer);
                
                int nHours = elapsedTime / 3600;
                int nMinutes = (elapsedTime % 3600) / 60;
                int nSeconds = elapsedTime % 60;
                _tprintf(_T("\t\t%02d:%02d:%02d"), nHours, nMinutes, nSeconds);
                //_tprintf(_T("\t\t%d sec\n"), elapsedTime);
                printf("\n");
            }
            bThread = Thread32Next(hThreadSnap, &te );
         }
        CloseHandle(hThreadSnap);
    } 


void GetProcessNameByPID(DWORD owner_pid)
{   int exit;
    exit = 0;
    HANDLE hSnapshot = get_snap_procc();
    PROCESSENTRY32 pe32;// = {0};
    pe32.dwSize = sizeof(PROCESSENTRY32);
    BOOL bProcess = Process32First(hSnapshot, &pe32);
    while(bProcess && exit == 0 )
    {   
        if (pe32.th32ProcessID == owner_pid)
        {
            _tprintf(_T("%s %d :"), pe32.szExeFile, owner_pid);
            exit = 1;
        }
        bProcess = Process32Next(hSnapshot, &pe32);    
    } 
    CloseHandle(hSnapshot);
    printf("\n");
        if(exit == 0)
    {
            _tprintf(_T("Process not found\n"));
    }
    else
    {
            print_thread_details(owner_pid);
    }
}
