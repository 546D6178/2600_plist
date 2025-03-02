# La génèse du projet

Ce projet de début de cursus, vise à parfaire nos compréhension des domaines suivants : 

- API Windows  
- Intéractions entre l'API Windows et les différentes sphères du système d'exploitation  
- Processus, threads et autres modules d'exécution (DLL)


# My_plsit

PList (Process List) est un utilitaire ligne de commande qui affiche les processus en cours d'exécution sur l'ordinateur local, ainsi que des informations utiles sur chaque processus.  

PList affiche :

- Les processus en cours d'exécution sur l'ordinateur, ainsi que leurs ID de processus (PID). 
- Détails du processus, y compris par exemple son utilisation de la mémoire virtuelle ou la commande dont il émane. 
- Threads en cours d'exécution dans chaque processus, y compris leurs ID de thread, leurs points d'entrée, la dernière erreur signalée et l'état du thread.

## Compilaton
Pour compiler le projet et obtenir l'exécutable plist.exe:

1. Se rendre dans le répertoire contenant tous les fichiers *.c

Puis executer la commande suivante:

2. "cl.exe plist.c print_all.c print_by_name.c print_by_process_id.c char_checker.c Thread_details.c"

## Utilisation

Pour lister les processus par leur nom, utilisez la commande suivante:

"plist.exe NAME_PROCESS.exe"

N'omettez pas l'extension ".exe" à la fin du nom, sinon plist ne le retrouvera pas.

Utilisez la commande "plist.exe -h" pour obtenir quelques informations d'utilisation supplémentaires.
