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
a). Pada saat client tersambung dengan server, terdapat pilihan register dan login. Jika memilih register, client akan diminta input id dan passwordnya untuk dikirimkan ke server. User juga dapat melakukan login. Login berhasil jika id dan password yang dikirim dari aplikasi client sesuai dengan list akun yang ada didalam aplikasi server. Sistem juga dapat menerima multi-connections. Koneksi terhitung ketika aplikasi client tersambung dengan server. Jika terdapat 2 koneksi atau lebih maka harus menunggu sampai client pertama keluar untuk bisa melakukan login dan mengakses aplikasinya. Lokasi penyimpanan id dan password pada file bernama **akun.txt** dengan format : 
```
id:password
id2:password2
```
</br>
b). Sistem memiliki sebuah database yang bernama **files.tsv**, berisi path file saat berada di server, publisher, dan tahun publikasi. Setiap penambahan dan penghapusan file pada folder file yang bernama FILES pada server akan memengaruhi isi dari files.tsv. Folder FILES otomatis dibuat saat server dijalankan. </br>
c). Client dapat menambah file baru ke dalam server. Direktori FILES memiliki struktur direktori seperti ini : 
```
File1.ekstensi
File2.ekstensi
```
Input client ke server `add` </br>
Output client console :
```
Publisher:
Tahun Publikasi:
Filepath:
```
Kemudian, dari aplikasi client akan dimasukan data buku tersebut dengan Filepath merupakan path file yang akan dikirim ke server. Lalu client akan melakukan pengiriman file ke aplikasi server dengan menggunakan socket. Ketika file diterima di server, maka row dari files.tsv akan bertambah sesuai dengan data terbaru yang ditambahkan. </br>
d). Client dapat mendownload file yang telah ada dalam folder FILES di server, sehingga sistem dapat mengirim file ke client. Server akan melihat dari files.tsv untuk melakukan pengecekan apakah file tersebut valid. Jika tidak valid, maka mengirimkan pesan error balik ke client. Jika berhasil, file akan dikirim dan akan diterima ke client di folder client tersebut. Contoh client command `download TEMPfile.pdf`. </br>
e). Client dapat menghapus file yang tersimpan di server. Namun, file hanya akan diganti namanya menjadi ‘old-NamaFile.ekstensi’. Ketika file telah diubah namanya, maka row dari file tersebut di file.tsv akan terhapus. Contoh client command `delete TEMPfile.pdf`.</br>
f). Client dapat melihat semua isi files.tsv dengan memanggil perintah yang bernama `see`. Output dari perintah tersebut keluar dengan format. </br>
Contoh format output pada client :
```
Nama:
Publisher:
Tahun publishing:
Ekstensi File : 
Filepath : 

Nama:
Publisher:
Tahun publishing:
Ekstensi File : 
Filepath : 
```
</br>
g). Client dapat melakukan pencarian dengan memberikan suatu string. Hasilnya merupakan semua nama file yang mengandung string tersebut. Format output seperti format output f. Contoh client command `find TEMP`. </br>
h). Terdapat suatu log untuk server yang bernama **running.log**. Contoh isi dari log ini berupa
```
Tambah : File1.ektensi (id:pass)
Hapus : File2.ektensi (id:pass)
```
</br>


## Penyelesaian


### Soal 1a


### Soal 1b


### Soal 1c


### soal 1d


### Soal 1e


### Soal 1f


### Soal 1g


### Soal 1h


### Hasil


# Soal 2
## Penjelasan
a). Program berupa perkalian matrix (4x3 dengan 3x6) lalu menampilkan hasilnya. Matriks berisi angka 1-20. </br>
b). Program menggunakan matriks output dari program soal2a.c menggunakan shared memory. Kemudian matriks tersebut akan dilakukan perhitungan dengan matrix baru (input user). Perhitungannya adalah setiap cel yang berasal dari matriks A menjadi angka untuk faktorial, lalu cel dari matriks B menjadi batas maksimal faktorialnya dengan ketentuan :
```
If a >= b  -> a!/(a-b)!
If b > a -> a!
If 0 -> 0
```
</br>
c). Program mengecek 5 proses teratas apa saja yang memakan resource komputer dengan command “ps aux | sort -nrk 3,3 | head -5” dan menggunakan IPC Pipes. </br>


</br>

## Penyelesaian


### Soal 2a


### Soal 2b)


### Soal 2c)


### Hasil

</br>

# Soal 3
## Penjelasan
a). Program menerima opsi -f agar pengguna bisa menambahkan argumen file yang bisa dikategorikan sebanyak yang diinginkan oleh pengguna. 
Output yang dikeluarkan adalah seperti ini :
```
File 1 : Berhasil Dikategorikan (jika berhasil)
File 2 : Sad, gagal :( (jika gagal)
File 3 : Berhasil Dikategorikan
``` 
</br>
b). Program menerima opsi -d untuk melakukan pengkategorian pada suatu directory. Namun user hanya bisa memasukkan input 1 directory saja. Contohnya adalah seperti ini: 
`$ ./soal3 -d /path/to/directory/`
Perintah di atas akan mengkategorikan file di /path/to/directory, lalu menyimpan hasilnya di working directory dimana program C tersebut berjalan.
Output yang dikeluarkan adalah seperti ini :
```
Jika berhasil, print “Direktori sukses disimpan!”
Jika gagal, print “Yah, gagal disimpan :(“
```
</br>
c). Program menerima opsi * : 
`$ ./soal3 \*`
Opsi ini akan mengkategorikan seluruh file yang ada di working directory ketika menjalankan program C tersebut. </br>
d). Semua file berada di dalam folder, jika terdapat file yang tidak memiliki ekstensi, maka file disimpan dalam folder “Unknown”. Jika file hidden, masuk folder “Hidden”. </br>
e). Setiap 1 file yang dikategorikan dioperasikan oleh 1 thread agar bisa berjalan secara paralel sehingga proses kategori bisa berjalan lebih cepat. </br>

## Penyelesaian


### Soal 3a)


### Soal 3b)


### Soal 3c)


### Soal 3d)


### Soal 3e)


### Hasil


# Kendala
- dikarenakan hari selasa ada quiz, kamis ETS, jumat 3 ETS baru bisa mulai ngerjakan jumat malam/malam sabtu :) </br>
