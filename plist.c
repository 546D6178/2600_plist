// Plist.c : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.

#include <windows.h>
#include <stdio.h>
#include <tlhelp32.h>
#include <psapi.h>
#include <tchar.h>

int char_checker(char *process);
void print_by_name(const char *process_name);
void print_by_process_id(const char *process_name);
void print_all();

HANDLE get_snap_procc()
{
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); //prends une instantanée des processus et 0 stipule TOUS les proc
    if (hSnapshot == INVALID_HANDLE_VALUE) { //Je gère l'erreur 
        printf("PB pour Snaap les processus ()... ERROR : %d\n", GetLastError());
        return NULL;
    }
    return hSnapshot;
}

HANDLE get_snap_thr()
{
    HANDLE hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    if (hThreadSnap == INVALID_HANDLE_VALUE) {
        return NULL;
    }
    return hThreadSnap;

}

int main(int ac, char  **av) //argument compteur && argument value 
{
    if (ac > 1)
    {
        if (strcmp(av[1], "-h") == 0)
        {   printf("\n");
            printf("Made by Maxence Brondelle && Axel Moignard \n");
            printf("Ecole 2600 ---- 2022-2025\n");
            printf("Usage : Plist.exe [-h] [-d] [name | pid]\n");
            printf("-h \t Show this help message\n");
            printf("-d \t Show thread detail\n");
            printf("name | pid \t Show information about specified process\n");
            printf("\n");
            printf("All memory values are displayed in KB.\n");
            printf("Abbreviation key:\n");
            printf(" Pri         Priority\n");
            printf(" Thd         Number of Threads\n");
            printf(" Hnd         Number of Handles\n");
            return 0;
        }
        if (strcmp(av[1], "-d") == 0)
        {
            //if (strcmp(av[2], "0") == 0)
            if (av[2] == NULL)
            {
                printf("Veuillez saisir un PPID\n ");
                return 0;
            }
            else
            {
                DWORD owner_pid = _ttoi(av[2]);
                GetProcessNameByPID(owner_pid);
                return 0;
            }
        }
        if(char_checker(av[1]) == 0)
        {    
            print_by_name(av[1]);
            return 0;
        }
        else
        {   
            print_by_process_id(av[1]);
            return(0);
        }

    }
   
    print_all();
    
    return 0;

}


//cl.exe plist.c char_checker.c print_all.c print_by_name.c print_by_process_id.c Thread_details.c