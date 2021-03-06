# soal-shift-sisop-modul-3-E02-2021

## Anggota Kelompok E02
05111940000001 - Christoper Baptista

05111940000101 - Zulfiqar Fauzul Akbar

05111940000152 - Ryan Fernaldy

## Daftar Isi
[Soal 1](https://github.com/Moon3903/soal-shift-sisop-modul-3-E02-2021#soal-1) </br>
[Soal 2](https://github.com/Moon3903/soal-shift-sisop-modul-3-E02-2021#soal-2) </br>
[Soal 3](https://github.com/Moon3903/soal-shift-sisop-modul-3-E02-2021#soal-3) </br>

# Soal 1
## Penjelasan
a) Melakukan register dan login.  Dalam suatu waktu, hanya diperbolehkan maksimal 1 client yang login. Jika ada client yang ingin login, maka harus menunggu client pertama untuk keluar agar bisa login.Id dan password disimpan di file ``akun.txt`` dengan format ``id:password``.</br>
b) Membuat ``files.tsv`` (terletak di folder ``FILES`` yang otomatis dibuat saat server dijalankan) yang berisi path file saat berada di server, publisher, dan tahun publikasi. Setiap ada penambahan atau penghapusan file di server maka akan mempengaruhi isi file tersebut.</br>
c) Client dapat mengirimkan file ke server dengan perintah ``add`` kemudian server akan meminta input publisher, tahun publikasi, dan filepath. Client akan mengirimkan file ke server dan file tersebut akan disimpan pada folder ``FILES``.</br>
d) Client dapat mengunduh file di server dengan perintah ``download namafile.ekstensi``. Jika file yang diminta tidak ada di ``files.tsv`` maka server akan mengirim pesan error. Jika file ditemukan, maka server akan mengirim file tersebut ke client.</br>
e) Client dapat menghapus file diserver dengan perintah ``delete namafile.ekstensi``. File tersebut tidak akan dihapus namun hanya diganti nama menjadi ``old-NamaFile.ekstensi`` dan setelah itu row dari file tersebut pada ``files.tsv`` akan dihapus.</br>
f) Client dapat melihat semua isi files.tsv dengan perintah ``see``</br>
g) Client dapat mencari file yang mengandung suatu string  dengan perintah ``find string``</br>
h) Membuat ``running.log`` untuk mencatat penambahan dan penghapusan file di server dengan format:</br>
```
Tambah : File1.ektensi (id:pass)
Hapus : File2.ektensi (id:pass)
```

## Penyelesaian
### Soal 1a)
``server.c``
```c
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
}
```
``State = 0`` menandakan bahwa sedang berada di menu awal (register dan login). 
- Ketika user memilih 1 (register), maka server akan meminta client untuk memasukkan id dan password. Setelah itu server akan menyimpannya pada ``akun.txt`` dengan ``fopen("akun.txt", "a")`` parameter ``a`` agar bisa menambahkan data di bagian akhir file tanpa menghapus baris yang sudah ada sebelumnya.
- Ketika user memilih 2 (login), maka server akan meminta client untuk memasukkan id dan password. Kemudian server akan mencocokan id dan password yang dimasukkan dengan ``akun.txt``. variabel ``login`` berguna untuk mengetahui apakah sudah ada client yang login. Jika sudah ada maka server akan mengirimkan pesan "Sudah ada client lain yang login". Jika belum ada yang login dan proses verifikasi berhasil maka variable ``login`` diubah menjadi 1, dan variabel login_client_id akan diisi dengan id thread client tersebut dengan menggunakan fungsi ``pthread_self()``.</br>

``server.c``
```
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
```
Saat client menghentikan aplikasi, maka harus dicek apakah client tersebut merupakan client yang sedang login dengan membandingnkan id thread client dengan variabel ``login_client_id``. Jika sama maka, maka hapus isi variabel yang berkaitan dengan client yang sedang login.
#### Hasil
- Registrasi
![SS_Registrasi](https://user-images.githubusercontent.com/68326540/118974223-c53bcf00-b99c-11eb-92d4-ad1ac14efad7.png)
- Login
![SS_Login](https://user-images.githubusercontent.com/68326540/118974472-0f24b500-b99d-11eb-9106-82f98542ec63.png)


### Soal 1b) - 1c)
``server.c``
```c
else if(state == 2){
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
    ...
}
```
Server akan meminta input publisher, tahun publikasi, dan filepath. Karena filepath ini merupakan path file di client, maka server tidak perlu mengetahuinya. Server hanya perlu mengetahui nama file.</br>
``client.c``
```c
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

}
```
Pada client akan memecah filepath yang dimasukkan untuk memperoleh nama file yang akan dikirim ke server dengan menggunakan fungsi ``strtok``. Setelah itu, client akan mengirimkan file baris per baris (menggunakan ``fgets``). Setiap baris yang dikirim server akan menunggu respon dari server yang akan menjawab 'OK'. Proses ini akan berakhir, ketika client mengirimkan pesan 'DONE!!!'.

``server.c``
```
else if(state == 2){
    ...
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

    ...
}
```
Pada server akan menulis setiap baris ke suatu file di server (menggunakan ``fputs``) dan merespon dengan pesan 'OK' untuk setiap barisnya hingga server menerima pesan 'DONE!!!'. Setelah itu server akan menambahkan record untuk file tersebut pada ``files.tsv`` dengan ``fopen("files.tsv","a")`` lalu ``fprintf(fileout,"%s\t%s\t%s\n",filePath,publisher,tahun);`` sehingga akan menambahkan baris baru tanpa menghilangkan baris sebelumnya.


#### Hasil
![SS_add](https://user-images.githubusercontent.com/68326540/118979936-2e264580-b9a3-11eb-8ec7-1e437e20170f.png)

### Soal 1d)
``server.c``
```c
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
```
Karena command ``download namafile.ekstensi`` ini terdiri dari 2 kata, maka harus dipecah menggunakan fungsi ``strtok`` sehingga bisa diperoleh nama file yang ingin diunduh dan disimpan ke variabel ``parameter``. </br>
``server.c``
```c
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
```
Server akan melakukan pengecekan apakah file yang diunduh terdapat di ``files.tsv``. Jika ada maka file akan dikirimkan ke client.Untuk pengiriman file dari server ke client caranya sama dengan soal 1c). Jika tidak ada, maka server akan menampilkan pesan error.
#### Hasil
Kondisi direktori client saat ini
![SS_direktori_client](https://user-images.githubusercontent.com/68326540/118975720-7db64280-b99e-11eb-8a39-69e3944fbf43.png)
</br>Setelah melakukan download
![SS_download](https://user-images.githubusercontent.com/68326540/118980467-b3115f00-b9a3-11eb-82be-cb633d323027.png)

### soal 1e)
``server.c``
```c
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
```
Karena command ``delete namafile.ekstensi`` ini terdiri dari 2 kata, maka harus dipecah menggunakan fungsi ``strtok`` sehingga bisa diperoleh nama file yang ingin diunduh dan disimpan ke variabel ``parameter``. </br>
``server.c``
```c
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
```
Server akan mencari file yang dimasukkan pada ``files.tsv`` jika ditemukan maka ubah nama file tersebut dengan awalan ``old-`` menggunakan fungsi ``rename``. Setelah itu melakukan penyalinan ``files.tsv`` ke ``tempfiles.tsv`` untuk seluruh baris kecuali yang barusan dihapus. Setelah itu hapus ``files.tsv`` dengan fungsi ``remove`` dan ubah nama ``tempfiles.tsv`` menjadi ``files.tsv``

#### Hasil
![SS_Delete](https://user-images.githubusercontent.com/68326540/118980918-fd92db80-b9a3-11eb-81ea-c36033d8f6c5.png)


### Soal 1f)
``server.c``
```
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
```
Membaca ``files.tsv`` baris per baris dengan ``fscanf(filein,"%[^\n]%*c",temp)``. Kemudian dipecah dengan menggunakan fungsi ``strtok`` dengan delimiter ``\t`` sehingga bisa diperoleh filepath, tahun publikasi, dan publisher. Setelah itu file path dipecah lagi dengan delimitter ``/`` dan delimitter ``.`` sehingga bisa diperoleh namafile dan ekstensinya. Setelah itu file path dipecah lagi dengan delimitter ``/`` dan delimitter ``.`` sehingga bisa diperoleh namafile dan ekstensinya. Kemudian membuat string sesuai dengan format dan dikirim ke client langsung. Setelah selesai maka server akan mengirimkan pesan 'DONE!!!'.

``client.c``
```c
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
```
Saat menerima input ``see`` maka client akan menerima pesan dari server terus menerus hingga server mengirimkan pesan 'DONE!!!'
#### Hasil
![SS_see](https://user-images.githubusercontent.com/68326540/118981222-519dc000-b9a4-11eb-82ad-858f24230c89.png)


### Soal 1g)
``server.c``
```c
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
```
Karena command ``find string`` ini terdiri dari 2 kata, maka harus dipecah menggunakan fungsi ``strtok`` sehingga bisa diperoleh nama file yang ingin diunduh dan disimpan ke variabel ``parameter``. 


``server.c``
```c
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
```
Server akan membaca ``files.tsv`` baris per baris, kemudian dipecah dengan menggunakan fungsi ``strtok``. Kemudian dipecah dengan menggunakan fungsi ``strtok`` dengan delimiter ``\t`` sehingga bisa diperoleh filepath, tahun publikasi, dan publisher. Setelah itu file path dipecah lagi dengan delimitter ``/`` dan delimitter ``.`` sehingga bisa diperoleh namafile dan ekstensinya. Setelah dicek apakah mengandung string yang diminta client. Jika ya, membuat string sesuai dengan format dan dikirim ke client langsung. Setelah selesai maka server akan mengirimkan pesan 'DONE!!!'.

``client.c``
```c
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
```
Saat menerima input yang mengandung 'find' (dapat dicek dengan fungsi ``strstr``) maka client akan menerima pesan dari server terus menerus hingga server mengirimkan pesan 'DONE!!!'
#### Hasil
![SS_Find](https://user-images.githubusercontent.com/68326540/118981470-8d388a00-b9a4-11eb-95bd-79b4cb6c5ec1.png)


### Soal 1h)
```c
if(login == 0){
    if(found){
        ...
        strcpy(id_login, id);
        strcpy(pass_login, password);
    }
    ...
}
```
Server memiliki variable global ``id_login`` dan ``pass_login`` yang akan menyimpan id dan password dari client yang berhasil melakukan login.</br>
``server.c``
```c
else if(state == 2){
    ...
    FILE *fileout;
    fileout = fopen("files.tsv","a");
    
    ...
    
    fileout = fopen("running.log","a");
    fprintf(fileout,"Tambah : %s (%s:%s)\n",fileName,id_login,pass_login);
    fclose(fileout);
}
```
Saat server sudah selesai menerima file dari client, maka akan membuka file ``running.log`` dengan parameter ``a`` agar bisa menambahkan baris baru tanpa menghapus baris sebelumnya. Kemudian menuliskan log dengan menggunakan ``fprintf`` sesuai dengan format yang diminta. Id dan password client yang melakukan aktivitas tersebut dapat diperoleh dari variabel global ``id_login`` dan ``pass_login``.</br>
``server.c``
```c
if(state == 4){
    ...

    if(found){
        ...

        FILE *fileout;
        filein = fopen("files.tsv","r");
        
        ...

        fileout = fopen("running.log","a");
        fprintf(fileout,"Hapus : %s (%s:%s)\n",parameter[1],id_login,pass_login);
        fclose(fileout);

        ...
    }
    ...
```
Saat server sudah selesai menghapus, maka akan membuka file ``running.log`` dengan parameter ``a`` agar bisa menambahkan baris baru tanpa menghapus baris sebelumnya. Kemudian menuliskan log dengan menggunakan ``fprintf`` sesuai dengan format yang diminta. Id dan password client yang melakukan aktivitas tersebut dapat diperoleh dari variabel global ``id_login`` dan ``pass_login``.

#### Hasil
![SS_log](https://user-images.githubusercontent.com/68326540/118981636-bce79200-b9a4-11eb-8046-9da5be3bf8e0.png)


# Soal 2
## Penjelasan
a) Membuat program perkalian matrix (4x3 dengan 3x6) dan menampilkan hasilnya. Matriks nantinya akan berisi angka 1-20.</br>
b) Membuat program dengan menggunakan matriks output dari program soal2a.c menggunakan shared memory. Kemudian matriks tersebut akan dilakukan perhitungan dengan matrix baru (input user). Perhitungannya adalah setiap cel yang berasal dari matriks A menjadi angka untuk faktorial, lalu cel dari matriks B menjadi batas maksimal faktorialnya matriks.</br>
c) Membuat program untuk mengecek 5 proses teratas apa saja yang memakan resource komputer dengan command ``???ps aux | sort -nrk 3,3 | head -5???`` </br>

## Penyelesaian
### Soal 2a)
``soal2a.c`` </br>
Matrix 4x3 dan 3x6
```
    for(int i=0;i<4;i++)
        for(int j=0;j<3;j++)
            scanf("%d",&a[i][j]);
    for(int i=0;i<3;i++)
        for(int j=0;j<6;j++)
            scanf("%d",&b[i][j]);
```
Ambil input matrix 4x3 dan 3x6 menggunakan ``input()`` </br>
Setup untuk shared memory:</br>
```
key_t key = 1234;
    int *value;
    int count = 24;
```
Membuat shared memory yang berukuran 24 integer </br>
``int shmid = shmget(key, sizeof(int)*count, IPC_CREAT | 0666);``
Cek jikalau gagal membuat shared memory </br>
```
if(shmid  == -1){
        printf("error\n");
        return 0;
    }
```
Alamat dari shared memory di assign ke variable value ``value = shmat(shmid, NULL, 0);`` </br>
Menghitung perkalian matriks dengan hasil berupa matriks 4x6
```
for(int i=0;i<4;i++){
        for(int x=0;x<6;x++){
            int tmp=0;
            for(int j=0;j<3;j++){
                tmp+=a[i][j]*b[j][x];
            }
            //dimasukkan ke shared memory (matriks 2 dimensi ke matriks 1 dimensi)
            value[6*i+x] = tmp;
        }
    }
```
Output untuk hasil perkalian 
```
for(int i=0; i<count; i++){
        if(i%6==0){
            printf("\n");
        }
        printf("%d ", value[i]);
    }
    printf("\n");
```
### Hasil
![1](https://user-images.githubusercontent.com/57831206/119218603-63e63e00-bb0b-11eb-9e33-f7c55a7e3ceb.JPG)


### Soal 2b)
``soal2b.c`` </br>
Struct buat passing ke thread
```
typedef struct pair{
    int a;
    int b;
    int index;
}pair;
```
Mengambil input matriks B
```
void input(){
    for(int i=0;i<24;i++)
    scanf("%d",&b[i]);
}
```
Penjelasan function ``void hitung()``: </br>
```
void *hitung(void *y){
    pair *tmp = (pair *) y;
    
    // kalau value cell sekarang ada yang 0, maka di A atau B hasilnya juga 0
    if(!(tmp->a) || !(tmp->b)){
        
	// kemudian disimpan ke hasil
        hasil[tmp->index] = 0;
    }
    else{
    
        // looping maksimal sebanyak B kali
        int max = tmp->b;
	
        // hitung faktorial dari A
        int i = tmp->a;
        ll total = 1;
        while(i--){
	
            // hitung faktorial
            max--;
            total *= (i+1);
            
	    // jika menyentuh batas B maka break
            if(!max){
                break;
            }
	    
            // digunakan untuk debugging
            if(total < 0){
                printf("overflow\n");
            }
        }
	
        // lalu disimpan ke dalam hasil
        hasil[tmp->index] = total;
    }
}
```
Penjelasan main program: </br>
```
    // ambil shared memory dari soal2a.c yang merupakan matriks A
    key_t key = 1234;

    int shmid = shmget(key, sizeof(int)*24, IPC_CREAT | 0666);
    value = shmat(shmid, NULL, 0);

    // inisialisasi matriks B dan matrix hasil
    b = (int*) malloc(24 * sizeof(int));
    hasil = (ll*) malloc(24 * sizeof(ll));

    // ambil input dari matriks B
    input();

    // buat 24 thread karena ada 24 cell dan masing-masing cell terdiri dari 1 thread
    pthread_t threads[24];

    for(int i=0;i<24;i++){
    
        // struct untuk passing, dimana value merupakan matriks A
        pair *tes = (pair*)malloc(sizeof(*tes));
        tes->a = value[i];
        tes->b = b[i];
        tes->index = i;
	
        // panggil perhitungan, lalu passing tes yang bertipedata struct
        if(pthread_create(&threads[i],NULL,hitung,(void *)tes)!=0){
            fprintf(stderr, "error: Cannot create thread # %d\n",i);
        }
    }

    // tunggu join
    for (int i = 0; i < 24; ++i){
      if (pthread_join(threads[i], NULL))
        {
          fprintf(stderr, "error: Cannot join thread # %d\n", i);
        }
    }

    // menampilkan output hasilnya
    for(int i=0;i<24;i++){
        if(i%6==0)
            printf("\n");
        printf("%lld\t",hasil[i]);
    }
    printf("\n");
    shmdt(value);
    
    // digunakan untu melakukan free shared memory
    shmctl(shmid, IPC_RMID, NULL);
```
### Hasil
![2](https://user-images.githubusercontent.com/57831206/119220904-eb857a00-bb16-11eb-8835-636de276f7ea.JPG)

### Soal 2c)
``soal2c.c`` </br>
Digunakan dua pipe. First pipe untuk mengirim string input dari parent. Second pipe untuk mengirim string gabungan dari child </br>
Cek pipe apakah fail atau tidak
```
	if (pipe(fd1)==-1) 
	{ 
		fprintf(stderr, "Pipe Failed" ); 
		return 1; 
	} 
	if (pipe(fd2)==-1) 
	{ 
		fprintf(stderr, "Pipe Failed" ); 
		return 1; 
	} 

	p = fork(); 

	if (p < 0) 
	{ 
		fprintf(stderr, "fork Failed" ); 
		return 1; 
	} 
```
Simpan output argv di pipe 1 atau fd1 ``dup2(fd1[1], STDOUT_FILENO)`` </br>
Ambil input dari pipe 1 atau fd1 ``dup2(fd1[0], STDIN_FILENO)`` </br>
Output argv disimpan ke pipe 2 atau fd2 ``dup2(fd2[1], STDOUT_FILENO)`` </br>
``wait(NULL)`` tunggu child hingga selesai. </br>
Lalu, ambil input dari child yang telah disort ``dup2(fd2[0], STDIN_FILENO)`` </br>
Menampilkan output ke layar
```
char *argv[] = {"head", "-5", NULL};
    execv("/usr/bin/head", argv);
    printf("hade\n");
}
```
### Hasil
![3 rev](https://user-images.githubusercontent.com/57831206/119247871-75851f80-bbb7-11eb-8816-ffdfed5ac5a3.JPG)
</br>

# Soal 3
## Penjelasan
Membuat program untuk mengkategorikan file berdasarkan ekstensi dan tidak case senstive.</br>
a) jika opsi yang digunakan adalah `-f` maka semua file yang path nya di input user akan dikategorikan kedalam current working directory(cwd) program. Program akan memberi tahu jika berhasil atau gagal dengan
```
File x : Berhasil Dikategorikan (jika berhasil)
File x : Sad, gagal :( (jika gagal)
```
dimana x adalah file keberapa yang di inputkan </br>
b) jika menerima opsi `-d` maka akan mengkategorikan suatu folder kedalam cwd.</br>
c) jika menerima `*` maka yang dikategorikan adalah cwd. </br>
untuk c dan d folder berlaku rekursif yang berarti folder dalam folder tersebut juga harus dikategorikan. program juga akan memberitahukan apabila berhasil atau gagal dengan output</br>
```
Direktori sukses disimpan!
Yah, gagal disimpan :(
```
d) jika terdapat file yang tidak memiliki ekstensi maka dikategorikan dalam folder `Unknown`. jika ada hidden file dikategorikan dalam folder `Hidden`. </br>
e) setiap file dioperasikan oleh satu thread. </br>

## Penyelesaian
### Soal 3d)
Terdapat fungsi `get_ext()` yang digunakan untuk mengambil ekstensi dari file dan mengembalikan string kosong jika tidak memiliki ekstensi. </br>
pertama `get_ext()` akan mengambil nama file dengan mengambil `strrchr()` yang mencari posisi terakhir dari suatu char, char yang dicari sekarang adalah `/` dimana hasilnya dipastikan adalah nama file `const char *name = strrchr(filename, '/')` </br>
lalu mencari `.` yang pertama untuk mendapat ekstensi file dengan memanfaatkan `strchr()` : `const char *dot = strchr(ok, '.');`</br>
setelah itu disimpan kedalam string dan dijadikan lower case karena yang diminta soal tidak case sensitive
```c
strcpy(tmp[x], dot+1);
for(int i=0; tmp[x][i];i++){
    tmp[x][i]=tolower(tmp[x][i]);
}
```
ada juga fungsi `is_hidden()` untuk mengetahui apakah file tersebut hidden, pada linux file hidden memiliki `.` sebagai char pertama dari nama file nya maka tinggal dicek saja nama filenya.
```c
bool is_hidden(const char *filename){
    const char *name = strrchr(filename, '/');
    if(name[1]=='.')
        return 1;
    return 0;
}
```
### Soal 3e)
setiap file akan dikategorikan dengan satu thread dimana akan ada beberapa string yang di passing dengan struct, thread akan membutuhkan full path, nama file, index yang dapat digunakan thread dan cwd.
```c
typedef struct pair{
    char awal[1000];
    char akhir[1000];
    char fname[1000];
    char full[1000];
    int index;
}pair;
```
thread juga akan memanfaatkan `rename()` untuk memindahkan file dan `mkdir()` untuk membuat folder</br>
pertama thread akan mengambil data struct `pair *sip = (pair *) y;`</br>
lalu di cek apakah hidden jika iya maka dikategorikan dalam folder `Hidden`
```c
if(is_hidden(sip->full)){
    mkdir("Hidden",0777);
    strcpy(tujuan,sip->akhir);
    strcat(tujuan,"/Hidden/");
    strcat(tujuan,sip->fname);
    rename(sip->full,tujuan);
}
```
jika tidak maka akan di cek ekstensinya degan `get_ext(sip->full,sip->index);` jika tidak memiliki ekstensi maka akan disimpan kedalam folder `Unknown`.
```c
if(!strcmp(tmp[sip->index],"")){
    mkdir("Unknown",0777);
    strcpy(tujuan,sip->akhir);
    strcat(tujuan,"/Unknown/");
    strcat(tujuan,sip->fname);
    rename(sip->full,tujuan);
}
```
jika memiliki ekstensi maka akan disimpan sesuai ekstensi yang sudah dijadikan lower case(tanpa merubah nama file) agar tidak case sensitive
```c
else{
    mkdir(tmp[sip->index],0777);
    strcpy(tujuan,sip->akhir);
    strcat(tujuan,"/");
    strcat(tujuan,tmp[sip->index]);
    strcat(tujuan,"/");
    strcat(tujuan,sip->fname);
    rename(sip->full,tujuan);
}
```


### Soal 3a)
program juga memiliki fungsi `tipe()` untuk megecek apakah filenya ada, dan apakah folder atau file.
```c
int tipe(const char* path) {
    struct stat statbuf; 
    if (stat(path, &statbuf) == -1)
        return -1;
    return S_ISDIR(statbuf.st_mode);
}
```
ada `pthread_t threads[1000];` untuk threading nya.</br>
pada 3a pertama dicek apakah argumen pertama adalah `-f` dengan `if(!strcmp(argv[1],"-f"))`</br>
setelah itu mengambil cwd dengan
```c
getcwd(cwd, sizeof(cwd));
strcpy(dest,cwd);
```
setelah itu dilooping untuk sisa argumennya `for (int i= 2; i < argc; i++)`, untuk yang berhasil atau tidak di simpan pada array `yay` dimana jika berisi 1 menandakan gagal dan 0 jika berhasil (untuk output)</br>
cara mengecek berhasil atau gagal adalah dengan menggunakan fungsi `tipe()` dimana jika return value nya bukan 0 maka antara path tidak ditemukan atau itu adalah folder(3a hanya menerima file) dan value `yay` dijadikan 1</br>
jika memang merupakan file maka akan di passing ke thread untuk di proses (di jelaskan di atas pada [3e](https://github.com/Moon3903/soal-shift-sisop-modul-3-E02-2021#soal-3e))</br>
```c
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
```
setelah itu menunggu semua thread selesai
```c
for (int i = 0; i < banyak; ++i){
    if (pthread_join(threads[i], NULL)){
        fprintf(stderr, "error: Cannot join thread # %d\n", i);
    }
}
```
jika sudah semua maka akan dioutput, tidak di output
```c
for (int i= 2; i < argc; i++){
    if(yay[i])
        printf("File %d : Sad, gagal :(\n",i-1);
    else
        printf("File %d : Berhasil Dikategorikan\n",i-1);
}
```


### Soal 3b 3c)
b dan c hampir sama, yang membedakan hanya lokasi folder saja, dimana pada c adalah cwd dan pada b di spesifikasi oleh user
```c
getcwd(cwd, sizeof(cwd));
if(!strcmp(argv[1], "*")){
    strcpy(start,cwd);
    strcpy(dest,cwd);
}
else if(!strcmp(argv[1],"-d")){
    strcpy(start, argv[2]);
    strcpy(dest,cwd);
}
```
setelah itu start dan dest akan dipassing ke fungsi rekursi `rekursi(start,cwd);`</br>
jika directory yang sedang dikerjakan sekarang tidak ada maka langsung di return
```c
d = opendir(awl);
if (!d){
    return;
}
```
jika ada maka akan dilooping semua file dan folder didalamnya, jika menemukan folder (di cek dengan `tipe()`) maka fungsi akan dipanggil kembali(rekursive) dengan path awal baru
```c
if(tipe(lewat)==1){
    rekursi(lewat,tjan);
    continue;
}
```
jika path sekarang meruju pada file maka akan memanggil thread (di jelaskan di atas pada [3e](https://github.com/Moon3903/soal-shift-sisop-modul-3-E02-2021#soal-3e))
```c
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
```
setelah selesai pada suatu folder akan dijoin
```c
for (int i = 0; i < total; ++i){
    if (pthread_join(threads[i], NULL)){
    fprintf(stderr, "error: Cannot join thread # %d\n", i);
    }
}
```
### Hasil
#### 3a
![Screenshot from 2021-05-22 13-30-05](https://user-images.githubusercontent.com/62832487/119217444-76a94480-bb04-11eb-9004-4e78f2232b94.png)

#### 3b
![Screenshot from 2021-05-22 13-32-39](https://user-images.githubusercontent.com/62832487/119217491-c0922a80-bb04-11eb-8537-c69f17b677df.png)
![Screenshot from 2021-05-22 13-32-55](https://user-images.githubusercontent.com/62832487/119217494-c5ef7500-bb04-11eb-9b94-6401a6ee5f46.png)

#### 3c
![Screenshot from 2021-05-22 13-45-14](https://user-images.githubusercontent.com/62832487/119217534-09e27a00-bb05-11eb-90b1-a4eebb016603.png)


# Kendala
- dikarenakan hari selasa ada quiz, kamis ETS, jumat 3 ETS baru bisa mulai ngerjakan jumat malam/malam sabtu :) </br>
