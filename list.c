// progetto lab2 dei files
#include <sys/stat.h> // necessario per stat
#include <dirent.h> // necessario per struct dirent*
#include <stdlib.h> // necessario per exit
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <pwd.h>   
#include <grp.h> 


void info(struct stat buf) {
struct passwd *pwd;
struct group *grp;

    pwd = getpwuid(buf.st_uid);
    grp = getgrgid(buf.st_gid);

    //int bytes = buf.st_size/8;

    printf("\tInode: %d\n", buf.st_ino);
    if(S_ISREG(buf.st_mode))
        printf("\tType: file\n");
    else if(S_ISDIR(buf.st_mode))
        printf("\tType: directory\n");
    else if(S_ISLNK(buf.st_mode))
        printf("\tType: symbolic link\n");
    else if(S_ISFIFO(buf.st_mode))
        printf("\tType: FIFO\n");
    else 
        printf("\tType: other\n");
    printf("\tSize: %d\n", buf.st_size);
    printf("\tOwner: %d %s\n", buf.st_uid, pwd->pw_name);
    printf("\tGroup: %d %s\n", buf.st_gid, grp->gr_name);        
}

void ric(char *arg, int c) {

    struct dirent* dirp;
    struct stat buf;
    // viene aperta la cartella e restituisce un puntatore di tipo DIR *
    DIR *d = opendir(arg);

    if (c==0 && stat(arg, &buf) == 0) {
        printf("Node: %s\n", arg);
        info(buf);
    } else if(stat(arg, &buf) < 0){
        perror("stat");
        closedir(d);
        return;
    }

    c++;

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

        printf("Node: %s\n", path);
        info(buf);
            
        // se e una directory entro nella funzione path
        if(S_ISDIR(buf.st_mode))
            ric(path,c);
    }

    closedir(d);

}

int main(int argc, char * argv[]) {
    
    int c=0;
    if(argc!=2)
        return 1;

    ric(argv[1],c);
   
return 0;

}
