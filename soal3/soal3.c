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
        if(!strcmp(argv[1], "*")){
            getcwd(cwd, sizeof(cwd));
            strcpy(start,cwd);
            strcpy(dest,cwd);
        }
        else if(!strcmp(argv[1],"-d")){
            // printf("BIG D\n");
            strcpy(start, argv[2]);
            getcwd(cwd, sizeof(cwd));
            strcpy(dest,cwd);
        }
        else{
            printf("hadeh\n");
            return 1;
        }
        d = opendir(start);
        if (!d){
            printf("Yah, gagal disimpan :(\n");
            return 1;
        }
        while((dir = readdir(d)) != NULL){
            strcpy(pass,start);
            strcat(pass,"/");
            strcat(pass,dir->d_name);

            if(tipe(pass)||!strcmp(dir->d_name,".")||!strcmp(dir->d_name,"..")){
                continue;
            }

            pair *tes = (pair*)malloc(sizeof(*tes));
            strcpy(tes->awal,start);
            strcpy(tes->akhir,dest);
            strcpy(tes->fname,dir->d_name);
            strcpy(tes->full,pass);
            tes->index = banyak;

            if(pthread_create(&threads[banyak],NULL,jalan,(void *)tes)!=0){
                fprintf(stderr, "error: Cannot create thread # %d\n",banyak);
            }
            banyak++;
        }
        closedir(d);
        for (int i = 0; i < banyak; ++i){
            if (pthread_join(threads[i], NULL)){
            fprintf(stderr, "error: Cannot join thread # %d\n", i);
            }
        }
        printf("Direktori sukses disimpan!\n");
    }
}