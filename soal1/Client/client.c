#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 1222
  
int main(int argc, char const *argv[]) {
    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char *hello = "Hello from client";
    
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
  
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
      
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
  
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }
    while(1){
        char buffer[1024] = {0};
        char message[1024] = {0};
        sleep(0.0001);
        valread = read( sock , buffer, 1024);
        printf("%s",buffer );
        fflush(stdout);
        
        if(strcmp(buffer,"Filepath: ") == 0){
            

            char filePath[1024] = {0};
            scanf("%[^\n]%*c", filePath);


            //mengirim nama file
            char tempfilePath[1024] = {0};
            strcpy(tempfilePath,filePath);
            char *token = strtok(tempfilePath,"/");
            char namaFile[1024] = {0};
            while(token){
                strcpy(namaFile,token);
                token = strtok(NULL,"/");
            }
            // printf("NAMA FILE = %s\n",namaFile);
            send(sock , namaFile , strlen(namaFile) , 0 );
            valread = read( sock , buffer, 1024);


            FILE *filein;
            filein = fopen(filePath,"r");
            char temp[1000]={0};
            while (fgets(temp,1000,filein))
            {
                send(sock , temp , strlen(temp) , 0 );
                valread = read( sock , buffer, 1024);
            }
            printf("Sending DONE\n");
            strcpy(message, "DONE!!!");
            send(sock , message , strlen(message) , 0 );

        }else if(strcmp(buffer,"Preparing Download") == 0){
            printf("\n");
            bzero(message,1024);
            strcpy(message, "OK1");
            send(sock , message , strlen(message) , 0);

            char temp[1000] = {0}, fileName[1024] = {0};
            valread = recv( sock , fileName, 1024, 0);

            bzero(message,1024);
            strcpy(message, "OK2");
            send(sock , message , strlen(message) , 0);

            do{   
                bzero(temp,1000);
                valread = recv( sock , temp, 1000, 0);
                if(strcmp(temp,"DONE!!!") != 0){
                    FILE *fileout;
                    fileout = fopen(fileName,"a");
                    fputs(temp,fileout);
                    fclose(fileout);
                    strcpy(message, "OK");
                    send(sock , message , strlen(message) , 0);
                }
            }while(strcmp(temp,"DONE!!!") != 0);
            strcpy(message, "OK");
            send(sock , message , strlen(message) , 0);
            printf("Download Done\n");

        }else{
            scanf("%[^\n]%*c", message);
            send(sock , message , strlen(message) , 0 );

            if(strcmp(message,"see") == 0){
                do{
                    
                        bzero(buffer,1024);
                        valread = read( sock , buffer, 1024);
                
                    if(strcmp(buffer,"DONE!!!") != 0){
                        printf("%s\n", buffer);
                        strcpy(message, "OK");
                        send(sock , message , strlen(message) , 0);
                    }
                }while(strcmp(buffer,"DONE!!!") != 0);

                strcpy(message, "OK");
                send(sock , message , strlen(message) , 0);
            }

            char *ret = strstr(message, "find");
            if(ret){
                do{
                    
                        bzero(buffer,1024);
                        valread = read( sock , buffer, 1024);
                
                    if(strcmp(buffer,"DONE!!!") != 0){
                        printf("%s\n", buffer);
                        strcpy(message, "OK");
                        send(sock , message , strlen(message) , 0);
                    }
                }while(strcmp(buffer,"DONE!!!") != 0);
                strcpy(message, "OK");
                send(sock , message , strlen(message) , 0);
            }
        }
        
    }

    close(sock);
    return 0;
}