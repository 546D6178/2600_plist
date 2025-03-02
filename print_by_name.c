#include <windows.h>
#include <stdio.h>
#include <tlhelp32.h>
#include <psapi.h>
#include <tchar.h>
#include <time.h>

HANDLE get_snap_thr();
HANDLE get_snap_procc();

void print_by_name(const char *process_name)
{
    
    HANDLE hSnapshot = get_snap_procc();
   // HANDLE hThreadSnap = get_snap_thr();

    PROCESSENTRY32 pe; //obligatoire sinon imp d'appeler Process32First; est utilisé pour stocker les info d'un processus
   // THREADENTRY32 te; // pour les Threads

    pe.dwSize = sizeof(PROCESSENTRY32); //  allocation mémoire 
    //te.dwSize = sizeof(THREADENTRY32);

    BOOL bProcess = Process32First(hSnapshot, &pe); // && Returns TRUE if the first entry of the process list has been copied to the buffer or FALSE otherwise.
    //BOOL bThreads = Process32First(hThreadSnap, &te);
    DWORD dwHandleCount;
    PROCESS_MEMORY_COUNTERS_EX pmc;

    if (!bProcess) {
        printf("Process32First() failed with error %d\n", GetLastError());
        return;
    }
    // formating the output to aligned columns
    const int width_Name = 50;
    const int other_column = 15;
    printf("%-*s %*s %*s %*s %*s %*s %*s %*s\n", width_Name, "Name", other_column, "Pid", other_column,"Pri", other_column, "Thd", other_column, "Hnd", other_column, "Priv" , other_column, "CPU Time", 30, "Elapsed Time");
    int count;
    count = 0;
    while(bProcess)
    {
        int pid = pe.th32ProcessID;
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pe.th32ProcessID);
        FILETIME ftCreate, ftExit, ftKernel, ftUser;
        __int64 nTotalTime = 0;

        GetProcessMemoryInfo(hProcess, (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
        if (hProcess != NULL) 
        {
            (GetProcessHandleCount(hProcess, &dwHandleCount)); //Require Admin right
        }
        if(strcmp(pe.szExeFile, process_name) == 0)
        {  
            //elapsed time block
            FILETIME createTime, exitTime, kernelTime, userTime;
            SYSTEMTIME processTime, currentTime;
            FileTimeToSystemTime(&createTime, &processTime);
            GetSystemTime(&currentTime);

            int elapsedTime = (currentTime.wHour - processTime.wHour) * 3600 + (currentTime.wMinute - processTime.wMinute) * 60 + 
                (currentTime.wSecond - processTime.wSecond);
             //end of block
            //printf("%-*s %*u %*u %*u %*d\n", width_Name, pe.szExeFile, other_column, pe.th32ProcessID, other_column, pe.pcPriClassBase, other_column, pe.cntThreads, other_column, dwHandleCount);
//cpu time
        HANDLE hThread = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
        if (hThread != INVALID_HANDLE_VALUE) 
        {
            THREADENTRY32 te;
            te.dwSize = sizeof(THREADENTRY32);
            if (Thread32First(hThread, &te)) {  
                do{
                    if (te.th32OwnerProcessID == pid) {
                    HANDLE hThread = OpenThread(THREAD_QUERY_INFORMATION, FALSE, te.th32ThreadID);
                        if (hThread != NULL) 
                        {
                            if (GetThreadTimes(hThread, &ftCreate, &ftExit, &ftKernel, &ftUser)) {
                            __int64 nKernel = ftKernel.dwHighDateTime;
                            nKernel = (nKernel << 32) + ftKernel.dwLowDateTime;
                            __int64 nUser = ftUser.dwHighDateTime;
                            nUser = (nUser << 32) + ftUser.dwLowDateTime;
                            __int64 nThreadTime = nKernel + nUser;
                            nTotalTime += nThreadTime;
                            //printf("%u\t\t%s\t%u\t\t%lld\n", pid, pe.szExeFile, te.th32ThreadID, nThreadTime);
                            }
                            CloseHandle(hThread);
                        }
                    }
                }while (Thread32Next(hThread, &te));
            }
            CloseHandle(hThread);
            __int64 nMilliseconds = nTotalTime / 10000;
            int nSeconds = nMilliseconds / 1000 % 60;
            int nMinutes = nMilliseconds / 1000 / 60 % 60;
            int nHours = nMilliseconds / 1000 / 60 / 60 % 24;
            int nDays = nMilliseconds / 1000 / 60 / 60 / 24;
           // printf("\nTotal CPU Time: %d days, %d hours, %d minutes, %d seconds, %lld milliseconds\n", nDays, nHours, nMinutes, nSeconds, nMilliseconds % 1000);
           
            //TCHAR tcpu[100];
            //sprintf(tcpu, TEXT("%02d:%02d:%02d:%03d "), nDays, nHours, nMinutes, nSeconds, nMilliseconds % 1000);
            printf("%-*s %*u %*u %*u %*d %*zuKB", width_Name, pe.szExeFile, other_column, pe.th32ProcessID, other_column, pe.pcPriClassBase, other_column, pe.cntThreads, other_column, dwHandleCount, other_column, pmc.PrivateUsage  / 1024);
            printf("\t\t%02d:%02d:%02d:%02d.%lld", nDays, nHours, nMinutes, nSeconds, nMilliseconds % 1000);
        }
        //cpu time 

        //printf("%-*s %*u %*u %*u %*d %*zuKB %*s", width_Name, pe.szExeFile, other_column, pe.th32ProcessID, other_column, pe.pcPriClassBase, other_column, pe.cntThreads, other_column, dwHandleCount, other_column, pmc.PrivateUsage  / 1024, other_column, tcpu);
        int nHours = elapsedTime / 3600;
        int nMinutes = (elapsedTime % 3600) / 60;
        int nSeconds = elapsedTime % 60;
        _tprintf(_T("\t\t%02d:%02d:%02d"), nHours, nMinutes, nSeconds);
        //_tprintf(_T("\t\t%d sec\n"), elapsedTime);
        printf("\n");
        count++;
        }
        bProcess = Process32Next(hSnapshot, &pe);
        CloseHandle(hProcess);
    }
    if(count == 0)
    {
        printf("No process in the system with name %s\n", process_name);
    }
}