#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> 
#include <dirent.h>
#include <stdbool.h> 
#include <ctype.h> 
#include <sys/stat.h> 
#include <pthread.h>

typedef struct pair{
    char awal[1000];
    char akhir[1000];
    char fname[1000];
    char full[1000];
    int index;
}pair;

bool flag;
char tmp[1000][1000],ex[1000][1000],start[1000],cwd[1000],dest[1000],pass[1000];
int banyak = 0;
bool yay[1000];

int tipe(const char* path) {
    struct stat statbuf; 
    if (stat(path, &statbuf) == -1)
        return -1;
    return S_ISDIR(statbuf.st_mode);
}

void get_ext(const char *filename,int x) {
    char ok[1000];
    const char *name = strrchr(filename, '/');
    if(name)
        strcpy(ok,name);
    else
        strcpy(ok,filename);
    const char *dot = strchr(ok, '.');
    
    if(!dot){
        strcpy(tmp[x], "\0");
        return;
    }

    strcpy(tmp[x], dot+1);
    for(int i=0; tmp[x][i];i++){
        tmp[x][i]=tolower(tmp[x][i]);
    }
    return;
}

bool is_hidden(const char *filename){
    const char *name = strrchr(filename, '/');
    if(name[1]=='.')
        return 1;
    return 0;
}

void *jalan(void *y){
    char tujuan[1000];
    pair *sip = (pair *) y;
    if(is_hidden(sip->full)){
        mkdir("Hidden",0777);
        // printf("HIDDEN\nstart : %s\nend : %s\n",sip->full,sip->akhir);
        strcpy(tujuan,sip->akhir);
        strcat(tujuan,"/Hidden/");
        strcat(tujuan,sip->fname);
        rename(sip->full,tujuan);
        // printf("tujuan : %s\n",tujuan);
    }
    else{
        // printf("start : %s\nend : %s\n",sip->awal,sip->akhir);
        get_ext(sip->full,sip->index);
        if(!strcmp(tmp[sip->index],"")){
            // printf("Unknown\n");
            mkdir("Unknown",0777);
            strcpy(tujuan,sip->akhir);
            strcat(tujuan,"/Unknown/");
            strcat(tujuan,sip->fname);
            rename(sip->full,tujuan);
        }
        else{
            // printf("%s\n",tmp[sip->index]);
            mkdir(tmp[sip->index],0777);
            strcpy(tujuan,sip->akhir);
            strcat(tujuan,"/");
            strcat(tujuan,tmp[sip->index]);
            strcat(tujuan,"/");
            strcat(tujuan,sip->fname);
            rename(sip->full,tujuan);
        }
        // printf("tujuan : %s\n",tujuan);
    }

}

void rekursi(char awl[1000],char tjan[1000]){
    pthread_t threads[1000];
    int total = 0;
    char lewat[1000];

    DIR *d;
    struct dirent *dir;

    d = opendir(awl);
    if (!d){
        return;
    }
    while((dir = readdir(d)) != NULL){
        flag = 1;
        strcpy(lewat,awl);
        strcat(lewat,"/");
        strcat(lewat,dir->d_name);
        // printf("%s \t%s\n",awl,dir->d_name);
        
        if(!strcmp(dir->d_name,".")||!strcmp(dir->d_name,"..")){
            continue;
        }

        if(tipe(lewat)==1){
            rekursi(lewat,tjan);
            // printf("MASUKK %s\n",lewat);
            continue;
        }

        pair *tes = (pair*)malloc(sizeof(*tes));
        strcpy(tes->awal,awl);
        strcpy(tes->akhir,tjan);
        strcpy(tes->fname,dir->d_name);
        strcpy(tes->full,lewat);
        tes->index = total;

        if(pthread_create(&threads[total],NULL,jalan,(void *)tes)!=0){
            fprintf(stderr, "error: Cannot create thread # %d\n",banyak);
        }
        total++;
    }
    closedir(d);
    for (int i = 0; i < total; ++i){
        if (pthread_join(threads[i], NULL)){
        fprintf(stderr, "error: Cannot join thread # %d\n", i);
        }
    }
}

int main(int argc, char **argv){
    pthread_t threads[1000];

    DIR *d;
    struct dirent *dir;
    if(!strcmp(argv[1],"-f")){
        getcwd(cwd, sizeof(cwd));
        strcpy(dest,cwd);
        for (int i= 2; i < argc; i++){
            strcpy(pass, argv[i]);

            if(tipe(pass)){
                yay[i] = 1;
                continue;
            }

            char ini[1000];
            strcpy(ini,strrchr(pass, '/'));

            pair *tes = (pair*)malloc(sizeof(*tes));
            strcpy(tes->akhir,dest);
            strcpy(tes->fname,ini+1);
            strcpy(tes->full,pass);
            tes->index = i;

            if(pthread_create(&threads[banyak],NULL,jalan,(void *)tes)!=0){
                fprintf(stderr, "error: Cannot create thread # %d\n",banyak);
            }
            banyak++;
        }
        for (int i = 0; i < banyak; ++i){
            if (pthread_join(threads[i], NULL)){
                fprintf(stderr, "error: Cannot join thread # %d\n", i);
            }
        }
        for (int i= 2; i < argc; i++){
            if(yay[i])
                printf("File %d : Sad, gagal :(\n",i-1);
            else
                printf("File %d : Berhasil Dikategorikan\n",i-1);
        }
    }
    else {
         getcwd(cwd, sizeof(cwd));
        if(!strcmp(argv[1], "*")){
            strcpy(start,cwd);
            strcpy(dest,cwd);
        }
        else if(!strcmp(argv[1],"-d")){
            // printf("BIG D\n");
            strcpy(start, argv[2]);
            strcpy(dest,cwd);
        }
        rekursi(start,cwd);
        if(flag){
            printf("Direktori sukses disimpan!\n");
        }
        else
            printf("Yah, gagal disimpan :(\n");
    }
}