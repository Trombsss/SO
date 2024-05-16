// progetto lab2 dei files
#include <sys/stat.h> // necessario per stat
#include <dirent.h> // necessario per struct dirent*
#include <stdlib.h> // necessario per exit
#include <stdio.h>
#include <sys/types.h>
#include <string.h>


// prova
void ric(const char *arg) {

    struct dirent* dirp;
    struct stat buf;
    // viene aperta la cartella e restituisce un puntatore di tipo DIR *
    DIR *d = opendir(arg);

    // apro la dir e inizio il ciclo while 
    while ((dirp = readdir(d)) != NULL) {
        // creo una var dove salvare il path
        char path[1000];
        
        if(strcmp(dirp->d_name, ".") == 0 || strcmp(dirp->d_name, "..") == 0) 
            continue;

        // ora path sara il percorso della libreria
        snprintf(path, sizeof(path), "%s/%s", arg, dirp->d_name);

        // stat prende il percorso passato e le colloca nella struct stat buf ()
        if (stat(path, &buf) < 0) {
            perror("stat");
            continue;
        }

        printf("%s\n", path);
            
        // se e una directory entro nella funzione path
        if(S_ISDIR(buf.st_mode))
            ric(path);
    }

    closedir(d);

}

int main(int argc, char * argv[]) {
    
    if(argc!=2)
        return 1;

    ric(argv[1]);
   
return 0;

}