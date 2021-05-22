#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>


#define PORT 1222
#define MAX_CLIENTS 1000

pthread_t tid[MAX_CLIENTS];

int login = 0;
pthread_t login_client_id;
char id_login[100] = {0};
char pass_login[100] = {0};

bool checkClose(int valread, int *new_socket){
    if(valread == 0){
        if(pthread_equal(pthread_self(), login_client_id)){
            login = 0;
            bzero(id_login,100);
            bzero(pass_login,100);
        }
        close(*new_socket);
        return 1;
    }
    return 0;
}

int ctr = 1;

void *play(void *arg){
    int *new_socket = (int *) arg; 
    int state = 0;
    while(1){
        char buffer[1024] = {0};
        char message[1024] = {0};
        char parameter[10][1024] = {0};
        int ind =0 ;
        int valread;
       

        if(state == 0){
            strcpy(message,"1.Register\n2.Login\nInput>>");
            send(*new_socket , message , strlen(message) , 0);
            valread = recv( *new_socket , buffer, 1024, 0);
            if(checkClose(valread,new_socket)){
                break;
            };
            if(strcmp(buffer,"1") == 0){
                strcpy(message,"Registrasi\nID >>");
                send(*new_socket , message , strlen(message) , 0);
                char id[100] = {0};
                char password[100] = {0};
                valread = recv( *new_socket , id, 100, 0);
                if(checkClose(valread,new_socket)){
                    break;
                };


                strcpy(message,"Password >>");
                send(*new_socket , message , strlen(message) , 0);
                valread = recv( *new_socket , password, 100, 0);
                if(checkClose(valread,new_socket)){
                    break;
                };

                FILE *fileout;
                fileout = fopen("akun.txt", "a");
                char concat[200] = {0};
                strcpy(concat,id);
                strcat(concat,":");
                strcat(concat,password);
                fputs(concat,fileout);
                fputs("\n",fileout);
                fclose(fileout);

                printf("Data berhasil disimpan\n");
            }else if(strcmp(buffer,"2") == 0){
                strcpy(message,"Login\nID >>");
                send(*new_socket , message , strlen(message) , 0);
                
                char id[100] = {0};
                char password[100] = {0};
                valread = recv( *new_socket , id, 100, 0);
                if(checkClose(valread,new_socket)){
                    break;
                };


                strcpy(message,"Password >>");
                send(*new_socket , message , strlen(message) , 0);
                valread = recv( *new_socket , password, 100, 0);
                if(checkClose(valread,new_socket)){
                    break;
                };

                FILE *filein;
                filein = fopen("akun.txt","r");
                char temp[100] = {0};
                char tempId[100] = {0};
                char tempPassword[100] = {0};
                bool found = false;
                while((fscanf(filein,"%[^\n]%*c",temp)) != EOF){
                    char *token = strtok(temp,":");
                    if(token != NULL){
                        strcpy(tempId,token);
                        token = strtok(NULL,":");
                    }

                    if(token != NULL){
                        strcpy(tempPassword,token);
                    }
                    if(strcmp(id,tempId) == 0 && strcmp(password,tempPassword)==0){
                        found = true;
                        break;
                    } 
                }

                if(login == 0){
                    if(found){
                        strcpy(message,"Login berhasil\n");
                        send(*new_socket , message , strlen(message) , 0);
                        login_client_id = pthread_self();
                        login = 1;
                        state = 1;
                        strcpy(id_login, id);
                        strcpy(pass_login, password);
                    }else{
                        strcpy(message,"Login gagal\n");
                        send(*new_socket , message , strlen(message) , 0);
                    }
                }else{
                    strcpy(message,"Sudah ada client lain yang login\n");
                    send(*new_socket , message , strlen(message) , 0);
                }
            }
        }else if(state == 1){
            strcpy(message,"Command >> ");
            send(*new_socket , message , strlen(message) , 0);
            
            valread = recv( *new_socket , buffer, 100, 0);

            if(checkClose(valread,new_socket)){
                break;
            };

            if(strcmp("add",buffer) == 0){
                state = 2;
            }else if(strcmp("see", buffer) == 0){
                state = 5;    
            }else{
                char *ret;
                ret = strstr(buffer, "download");
                if(ret){
                    char *token = strtok(buffer," ");
                    while(token != NULL){
                        strcpy(parameter[ind],token);
                        ind++;
                        token = strtok(NULL," ");
                    }
                    state = 3;
                }

                ret = strstr(buffer, "delete");
                if(ret){
                    char *token = strtok(buffer," ");
                    while(token != NULL){
                        strcpy(parameter[ind],token);
                        ind++;
                        token = strtok(NULL," ");
                    }
                    state = 4;
                }

                ret = strstr(buffer, "find");
                if(ret){
                    char *token = strtok(buffer," ");
                    while(token != NULL){
                        strcpy(parameter[ind],token);
                        ind++;
                        token = strtok(NULL," ");
                    }
                    state = 6;
                }
                


            }

        }else if(state == 2){
            strcpy(message, "Publisher: ");
            send(*new_socket , message , strlen(message) , 0);
            char publisher[100]={0};
            valread = recv( *new_socket , publisher, 100, 0);
            if(checkClose(valread,new_socket)){
                break;
            };

            strcpy(message, "Tahun Publikasi: ");
            send(*new_socket , message , strlen(message) , 0);
            char tahun[100]={0};
            valread = recv( *new_socket , tahun, 100, 0);
            if(checkClose(valread,new_socket)){
                break;
            };

            strcpy(message, "Filepath: ");
            send(*new_socket , message , strlen(message) , 0);
            char temp[1000]={0};
            
            char fileName[1024] = {0},filePath[1024] = {0};
            valread = recv( *new_socket , fileName, 1024, 0);
            strcpy(message, "OK");
            send(*new_socket , message , strlen(message) , 0);

            strcpy(filePath,"FILES/");
            strcat(filePath,fileName);

            do{   
                bzero(temp,1000);
                valread = recv( *new_socket , temp, 1000, 0);
                if(strcmp(temp,"DONE!!!") != 0){
                    FILE *fileout;
                    fileout = fopen(filePath,"a");
                    fputs(temp,fileout);
                    fclose(fileout);
                    strcpy(message, "OK");
                    send(*new_socket , message , strlen(message) , 0);
                }
            }while(strcmp(temp,"DONE!!!") != 0);
            printf("Saving done\n");
            state = 1;

            FILE *fileout;
            fileout = fopen("files.tsv","a");
            fprintf(fileout,"%s\t%s\t%s\n",filePath,publisher,tahun);
            fclose(fileout);

            fileout = fopen("running.log","a");
            fprintf(fileout,"Tambah : %s (%s:%s)\n",fileName,id_login,pass_login);
            fclose(fileout);
        }
        
        if(state == 3){
            //cek db
            FILE *filein;
            filein = fopen("files.tsv","r");
            char search[1000] = {0};
            strcpy(search,"FILES/");
            strcat(search,parameter[1]);

            char temp[1000] = {0};
            char filepath[100] = {0};
            bool found = false;
            while((fscanf(filein,"%[^\n]%*c",temp)) != EOF){
                    char *token = strtok(temp,"\t");
                    if(token != NULL){
                        strcpy(filepath,token);
                    }
                    if(strcmp(search, filepath) == 0){
                        found = true;
                        break;
                    }
            }
            fclose(filein);
            //kirim file
            if(found){
                bzero(message,1024);
                bzero(buffer,1024);
                strcpy(message,"Preparing Download");
                send(*new_socket , message , strlen(message) , 0);
                valread = recv( *new_socket , buffer, 1024, 0);

                bzero(message,1024);
                bzero(buffer,1024);

                //kirim namafilenya
                strcpy(message,parameter[1]);
                send(*new_socket , message , strlen(message) , 0);
                valread = recv( *new_socket , buffer, 1024, 0);

                FILE *filein;
                filein = fopen(filepath,"r");

                while (fgets(temp,1000,filein))
                {
                    send(*new_socket , temp , strlen(temp) , 0 );
                    valread = recv( *new_socket , buffer, 1024, 0);
                }
                printf("Sending Done\n");
                strcpy(message, "DONE!!!");
                send(*new_socket , message , strlen(message) , 0 );
                valread = recv( *new_socket , buffer, 1024, 0);
            }else{
                strcpy(message, "Error file not found\n");
                send(*new_socket , message , strlen(message) , 0 );
            }
            state = 1;
        }
        
        if(state == 4){
            //cek db
            FILE *filein;
            filein = fopen("files.tsv","r");
            char search[1000] = {0};
            strcpy(search,"FILES/");
            strcat(search,parameter[1]);

            char temp[1000] = {0};
            char filepath[100]= {0};
            bool found = false;
            while((fscanf(filein,"%[^\n]%*c",temp)) != EOF){
                    char *token = strtok(temp,"\t");
                    if(token != NULL){
                        strcpy(filepath,token);
                    }
                    if(strcmp(search, filepath) == 0){
                        found = true;
                        break;
                    }
            }
            fclose(filein);

            if(found){
                char fileDelete[100] = {0}, newName[100]= {0};
                strcpy(fileDelete,"FILES/");
                strcat(fileDelete,parameter[1]);
                strcpy(newName,"FILES/old-");
                strcat(newName,parameter[1]);
                rename(fileDelete,newName);

                FILE *fileout;
                filein = fopen("files.tsv","r");

                //buat file kosongan
                fileout = fopen("tempfiles.tsv","w");
                fclose(fileout);

                while((fscanf(filein,"%[^\n]%*c",temp)) != EOF){
                    char temp2[100] = {0};
                    strcpy(temp2,temp);
                    char *token = strtok(temp,"\t");
                    if(token != NULL){
                        strcpy(filepath,token);
                    }
                    if(strcmp(search, filepath) != 0){
                        fileout = fopen("tempfiles.tsv","a");
                        fputs(temp2, fileout);
                        fputs("\n",fileout);
                        fclose(fileout);
                    }
                }
                remove("files.tsv");
                rename("tempfiles.tsv","files.tsv");

                fileout = fopen("running.log","a");
                fprintf(fileout,"Hapus : %s (%s:%s)\n",parameter[1],id_login,pass_login);
                fclose(fileout);

                printf("Delete berhasil \n");
            }else{
                printf("Error file not found\n");
            }
            state = 1;
        }

        if(state == 5){
            FILE *filein;
            filein = fopen("files.tsv","r");

            char temp[1000] = {0};
            char filepath[100] = {0},publisher[100] = {0},tahun[100] = {0},nama[100] = {0},ekstensi[100] = {0};

            while((fscanf(filein,"%[^\n]%*c",temp)) != EOF){
                    char *token = strtok(temp,"\t");
                    if(token != NULL){
                        strcpy(filepath,token);
                        token = strtok(NULL,"\t");
                    }
                    if(token !=NULL){
                        strcpy(publisher,token);
                        token = strtok(NULL,"\t");
                    }

                    if(token !=NULL){
                        strcpy(tahun,token);
                    }

                    token = strtok(temp,"/");
                    token = strtok(NULL,".");
                    strcpy(nama, token);
                    token = strtok(NULL,".");
                    strcpy(ekstensi,token);

                    strcpy(message,"Nama: ");
                    strcat(message,nama);
                    strcat(message,"\n");
                    strcat(message,"Publisher: ");
                    strcat(message,publisher);
                    strcat(message,"\n");
                    strcat(message,"Tahun publishing: ");
                    strcat(message,tahun);
                    strcat(message,"\n");
                    strcat(message,"Ekstensi: ");
                    strcat(message,ekstensi);
                    strcat(message,"\n");
                    strcat(message,"Filepath: ");
                    strcat(message,filepath);
                    strcat(message,"\n");
                    send(*new_socket , message , strlen(message) , 0 );

                    valread = recv( *new_socket , buffer, 1000, 0);

            }
            strcpy(message,"DONE!!!");
            send(*new_socket , message , strlen(message) , 0 );
            valread = recv( *new_socket , buffer, 1000, 0);
            
            fclose(filein);
            state = 1;
        }

        if(state == 6){
            FILE *filein;
            filein = fopen("files.tsv","r");

            char temp[1000] = {0};
            char filepath[100] = {0},publisher[100] = {0},tahun[100] = {0},nama[100] = {0},ekstensi[100] = {0};
            
            while((fscanf(filein,"%[^\n]%*c",temp)) != EOF){
                    char *token = strtok(temp,"\t");
                    if(token != NULL){
                        strcpy(filepath,token);
                        token = strtok(NULL,"\t");
                    }
                    if(token !=NULL){
                        strcpy(publisher,token);
                        token = strtok(NULL,"\t");
                    }

                    if(token !=NULL){
                        strcpy(tahun,token);
                    }

                    token = strtok(temp,"/");
                    token = strtok(NULL,".");
                    strcpy(nama, token);
                    token = strtok(NULL,".");
                    strcpy(ekstensi,token);

                    char *ret =strstr(filepath,parameter[1]);

                    if(ret){
                        strcpy(message,"Nama: ");
                        strcat(message,nama);
                        strcat(message,"\n");
                        strcat(message,"Publisher: ");
                        strcat(message,publisher);
                        strcat(message,"\n");
                        strcat(message,"Tahun publishing: ");
                        strcat(message,tahun);
                        strcat(message,"\n");
                        strcat(message,"Ekstensi: ");
                        strcat(message,ekstensi);
                        strcat(message,"\n");
                        strcat(message,"Filepath: ");
                        strcat(message,filepath);
                        strcat(message,"\n");
                        send(*new_socket , message , strlen(message) , 0 );

                        valread = recv( *new_socket , buffer, 1000, 0);
                    }

            }
            strcpy(message,"DONE!!!");
            send(*new_socket , message , strlen(message) , 0 );
            valread = recv( *new_socket , buffer, 1000, 0);
            
            fclose(filein);
            state = 1;
        }
        
    }

}

int main(int argc, char const *argv[]) {
    
    mkdir("FILES",0777);

    int server_fd, new_socket[MAX_CLIENTS];
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
      
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
      
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    int ctr = 0;
    while(1){
        if ((new_socket[ctr] = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        pthread_create(&(tid[ctr]),NULL,play,&new_socket[ctr]);
        ctr++;
        printf("Client %d terhubung\n",ctr);
    }

    for(int i = 0; i<ctr; i++){
        pthread_join(tid[i],NULL);
    }
    return 0;
}