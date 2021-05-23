# soal-shift-sisop-modul-3-C10-2021

### KELOMPOK : C10

| **No** | **Nama** | **NRP** | 
| ------------- | ------------- | --------- |
| 1 | Dicksen Alfersius Novian  | 05111940000076 | 
| 2 | Vyra Fania Adelina  | 05111940000109 |
| 3 | Bill Harit Yafi  | 05111940000114 |


### Soal 1
> **[soal 1](https://github.com/billharit/soal-shift-sisop-modul-3-C10-2021/tree/main/soal%201)**

Pada soal 1, diminta membuat server database buku menggunakan client-server, dengan ketentuan sebagai berikut :

    A. Saat client pertama kali masuk ke server, dapat melakukan register/login, dan server mencatat dalam "akun.txt"
Dari sisi client :
```c
        //first time receive message from server
        // parsing pesan server
        memset(server_reply, 0, sizeof(server_reply));
	      read(sock, server_reply, 1024);
        if (strcmp(server_reply, "menu") == 0) {
            printf("Welcome, please select a command :\n1.Register\n2.Login\n");
            printf("Menu chosen:\n");
            scanf("%s", clReply);

            //concate op1 atau op2
            snprintf(clSend, sizeof clSend, "op%s", clReply);
            
            //kirim ke server
            if( send(sock , clSend , strlen(clSend) , 0) < 0){
			    puts("Send failed");
			    return 1;
		    }
            start =1;
            memset(clSend, 0,sizeof(clSend));
            memset(clReply, 0, sizeof(clReply));
        } else if (strcmp(server_reply, "register") == 0) {
            char clUsername[100]; char clPassword[100]; char clAkun[300]; 
            printf("Username:");
            getchar();
            scanf("%[^\n]s", clUsername);
            printf("Password:");
            getchar();
            scanf("%[^\n]s", clPassword);
            
            //combine reg user:paaaaass 
            snprintf(clAkun, sizeof clAkun, "R%s:%s", clUsername, clPassword);

            //kirim ke server
            if( send(sock , clAkun , strlen(clAkun) , 0) < 0){
			    puts("Send failed");
			    return 1;
		    }

        }else if (strcmp(server_reply, "login") == 0) {
            char clUsername[100]; char clPassword[100]; char clAkun[300]; 
            printf("Username:");
            getchar();
            scanf("%[^\n]s", clUsername);
            printf("Password:");
            getchar();
            scanf("%[^\n]s", clPassword);
            
            //combine reg user:paaaaass 
            snprintf(clAkun, sizeof clAkun, "L%s:%s", clUsername, clPassword);
            //clReply=strcat("op"+clReply);
            //printf("%s\n", clAkun);

            //kirim ke server
            if( send(sock , clAkun , strlen(clAkun) , 0) < 0){
			    puts("Send failed");
			    return 1;
		    }

        }

```
Client menerima pesan "menu" dari server ketika pertama kali terhubung. Dari sana, client dapat melakukan register/login. Client mengirimkan pesan ke server
dengan awalan "op1" atau "op2", maupun "L" ataupun "R" agar dapat dimengerti server.

Dari sisi server :
```c

while( (read_size = recv(sock , client_message , 2000 , 0)) > 0 )
	{
        if (strcmp(client_message, "op1") == 0) {    
            message = "register";
            write(sock, message, strlen(message));			
		}
		
        else if (strcmp(client_message, "op2") == 0) {   
			//tambah disini pengecekan . hanya 1 yg boleh login 
            message = "login";
            write(sock, message, strlen(message));
            //printf("mau login\n");	
		}
		else if(client_message[0] == 'R'){
			int read_username;
            int read_password;
            //Menghapus huruf 'R' didepan User + Password
            memmove(client_message, client_message+1, strlen(client_message));
            FILE* fp = fopen ("akun.txt","a");
            fprintf(fp, "%s\n", client_message);
			fclose(fp);
			write(sock, "menu", 4);
		}
        else if(client_message[0] == 'L'){
			int read_username;
            int read_password;
            //Menghapus huruf 'L' didepan User + Password
            memmove(client_message, client_message+1, strlen(client_message));
            //Mencari username & password dalam Database
            FILE* fp = fopen ("akun.txt","r"); //membuka "akun.txt" dalam mode read
            char temp[300];
            int found=0;
            while(fgets(temp, 300, fp)!=NULL){
                if(strstr(temp,client_message) != NULL){
                    found = 1;
					//User yang berhasil login menjadi current user
					strcpy(user_sekarang,client_message);
					//Masuk kedalam menu utama setelah berhasil login
					write(sock, "mainMenu", 8);
                    break;
                }
            }
            if (found == 0){
				write(sock, "menu", 4);
            }
			fclose(fp);
		}

```
Dari sisi server, menerima pesan client dalam variabel "client_message". Dicek awalan pesan client agar server dapat melakukan aksi yang sesuai. Untuk register, server akan menerima username beserta password dan memasukkan kedalam "akun.txt". Untuk login, pertama pesan client akan diolah agar awalan "L" tidak dimasukkan perhitungan. Username dan password akan dicari dalam "akun.txt", jika ada maka server mengirim pesan "mainMenu" kepada client untuk perintah selanjutnya. Server juga mencatat username dan password dari client yang berhasil login kedalam variabel "user_sekarang" untuk keperluan log. Akan dijelaskan lebih lanjut di poin "h".


      B. Membuat folder "FILES" dengan isi file-file yang dikirim client, serta membuat file "files.tsv" untuk mencatat detail file yang dikirim

Untuk membuat folder dan file tersebut, dilakukan perintah berikut saat server dijalankan :
```c


	//Membuat direktori FILES file "files.tsv" dan file "running.log"
	int buat_direktori_FILES = mkdir("FILES",0777);
	if(buat_direktori_FILES == 0){
		printf("Berhasil membuat direktori\n");
	}
	char *path_file_tsv = "files.tsv";
	char *path_running_log = "running.log";
	printf("Test\n");
	FILE* file_tsv = fopen(path_file_tsv,"a");
	if(file_tsv == NULL){
		fprintf(stderr, "maaf gagal\n");
	}
    fclose(file_tsv);
	printf("Berhasil membuat files.tsv\n");
	FILE* running_log = fopen (path_running_log,"a");

```

Perintah tersebut dijalankan saat awal server menyala.


      C. Fitur menambahkan file ke server

Client dapat menambahkan file ke server dengan detail-detail berupa "publisher", "tahun publikasi", dan "filepath". Berikut dari sisi client :

```c

void add_file(){
    char publisher[200];
    char tahun_publikasi[20];
    char filepath[200];
    char kirim_add_file[700];
    printf("Publisher:");
    getchar();
    scanf("%[^\n]s", publisher);
    printf("Tahun Publikasi:");
    getchar();
    scanf("%[^\n]s", tahun_publikasi);
    printf("Filepath:");
    getchar();
    scanf("%[^\n]s", filepath);
    //concate perintah
    snprintf(clSend, sizeof clSend, "M1%s--%s--%s", publisher, tahun_publikasi, filepath);        
    printf("%s\n",clSend);

     if( send(sock , clSend , strlen(clSend) , 0) < 0){
	    puts("Send failed");
		return;
		}
}



```
Client mengirimkan sebuah pesan berisi data-data yang diperlukan. Format pesan yaitu "M1Publisher--tahun publikasi--path file".

Dari sisi server :

```c

//add
			if(client_message[1]== '1'){
				//Memasukkan data kedalam files.tsv
				char data_file_tsv[2000];
				strcpy(data_file_tsv, client_message+2);
				//char *data_file_tsv = strncpy(data_file_tsv, client_message+2, sizeof client_message-2);
				printf("data yang akan dimasukkan = %s\n", data_file_tsv);
				FILE* file_tsv = fopen ("files.tsv","a");
				fprintf(file_tsv, "%s\n", data_file_tsv);
    			fclose(file_tsv);
				//Melakukan tokenisasi untuk mendapat Publisher, tahun terbit,filepath
				char temp[2000];
				strcpy(temp, data_file_tsv);
				int indeks_token=0;
				//Variabel untuk menyimpan data-data pada server_files
				char tahun_terbit_f[20];
				char publisher_f[200];
				char filepath_f[200];
				char *ekstensi_file_f;

				char *token_filepath = strtok(temp, "--");
				strcpy(publisher_f, token_filepath);
				while(indeks_token <  2 && token_filepath!=NULL){
					token_filepath = strtok(NULL,"--");
					if(indeks_token == 0){
						strcpy(tahun_terbit_f,token_filepath);
					}
					else if(index_file_server == 1){
						strcpy(filepath_f,token_filepath);
					}
					indeks_token++;
				}
				strcpy(filepath_f, token_filepath);
				char path_FILES[600] = "./FILES/";
				char *nama_file = basename(token_filepath);
				strcat(path_FILES, nama_file);
				ekstensi_file_f = strrchr(nama_file, '.') + 1;
				//Menyalin isi dari file client kedalam folder FILES
				FILE *file_client = fopen(token_filepath, "r");
				if(fopen(token_filepath, "r") == NULL){
					printf("Maaf file tidak ditemukan\n");
				}
				//Jika file valid, data akan dimasukkan kedalam file_server
				else{
					server_files[index_file_server].status = 0;
					strcpy(server_files[index_file_server].pembuat, user_sekarang);
					strcpy(server_files[index_file_server].publisher, publisher_f);
					strcpy(server_files[index_file_server].tahun_terbit, tahun_terbit_f);
					strcpy(server_files[index_file_server].path_file, filepath_f);
					strcpy(server_files[index_file_server].nama, nama_file);
					strcpy(server_files[index_file_server].ekstensi_file, ekstensi_file_f);
					//Memasukkan kedalam running.log
					FILE* running_log = fopen ("running.log","a");
            		fprintf(running_log, "Tambah : %s(%s)\n", nama_file, user_sekarang);
					fclose(running_log);
					index_file_server++;
				}
				FILE *file_server = fopen(path_FILES, "w");
				char buffer;
				buffer = fgetc(file_client);
				while( buffer != EOF){
					fputc(buffer, file_server);
					buffer = fgetc(file_client);
				}
				fclose(file_server);
				fclose(file_client);
				write(sock, "mainMenu", 8);
				memset(client_message,0,sizeof(client_message));	
			}

```
Server akan menerima data lalu melakukan tokenisasi agar dapat dibaca detail-detail pesan yang diterima. Setelah itu, dilakukan pengecekan apakah file valid. Jika valid, maka server akan mengakses data dan melakukan penyalinan isi file kedalam folder "FILES". Setelah itu, server mengirimkan pesan agar client bisa kembali ke menu utama sehingga bisa melakukan perintah-perintah lain.


      D. Download file dari server ke client

Client dapat mengunduh file dari server, berikut perintah dari sisi client :

```c

void download_file(){
    char nama_file[200];
    printf("download : ");
    getchar();
    scanf("%[^\n]s", nama_file);
    //concate perintah
    snprintf(clSend, sizeof clSend, "M2%s", nama_file);
    
    if( send(sock , clSend , strlen(clSend) , 0) < 0){
		puts("Send failed");
		return ;
	}

    read(sock, server_reply, 1024);
    if(strcmp (server_reply, "noFile") == 0){
        printf("Maaf file tidak ada dalam database\n");
    }
    else{
        printf("Berhasil di download\n");
        menu_utama();
    }
}

```

Client mengirimkan nama file yang ingin diunduh ke server. Jika file tidak ditemukan, maka mencetak "Maaf file tidak ada dalam database".

Dari sisi server :

```c

//download 
			else if(client_message[1] == '2'){
				char nama_file[200];
				char *ps = client_message + 2;
				char *pd = nama_file + strlen(nama_file);
				strcpy(nama_file, client_message + 2);
				int i;
				int found_tsv=0;
				int found=0;
				//Mencari keberadaan file dalam files.tsv
				FILE* fp = fopen ("files.tsv","r"); //membuka "files.tsv" dalam mode read
				char temp[300];
				while(fgets(temp, 300, fp)!=NULL){
					if(strstr(temp,nama_file) != NULL){
						found_tsv = 1;
						break;
					}
				}
				fclose(fp);
				//Mencari indeks file dalam struct
				if(found_tsv==1){
					for(i=0 ; i<index_file_server;i++){
						if(strcmp (nama_file, server_files[i].nama)==0){
							char path_client[200] = {"/home/dicksen/Modul Sisop Coba/Modul 3/Client/"};
							char path_server[200] = {"./FILES/"};
							strcat(path_client, nama_file);
							strcat(path_server, nama_file);
							FILE *file_client = fopen(path_client, "w");
							FILE *file_server = fopen(path_server, "r");
							char buffer;
							buffer = fgetc(file_server);
							while( buffer != EOF){
								fputc(buffer, file_client);
								buffer = fgetc(file_server);
							}
							fclose(file_server);
							fclose(file_client);
							found=1;
							break;
						}
					}
				}
				if(found == 0 || found_tsv == 0){
					write(sock, "noFile", 7);
				}
				else{
					write(sock,"mainMenu", 8);
				}
				memset(client_message,0,sizeof(client_message));	
			}

```

Server akan mencari apakah nama file ada dalam database, jika ada maka akan dilakukan pembuatan file baru di client dan menyalin isi file yang di-inginkan.


        E. Menghapus file pada server

Client dapat meminta server untuk menghapus file tertentu. Server akan menghapus data file tersebut dari "files.tsv", namun file yang dimaksud tidak dihapus dari folder "FILES", hanya diganti nama menjadi "old-namaFile". Berikut perintah dari sisi client :

```c

void hapus_file(){
    char nama_file[200];
    printf("delete : ");
    getchar();
    scanf("%[^\n]s", nama_file);
    //concate perintah
    snprintf(clSend, sizeof clSend, "M3%s", nama_file);
    if( send(sock , clSend , strlen(clSend) , 0) < 0){
		puts("Send failed");
		return ;
	}
    read(sock, server_reply, 1024);
    if(strcmp (server_reply, "noFile") == 0){
        printf("Maaf file tidak ada dalam database\n");
        menu_utama();
    }
    else if (strcmp(server_reply, "mainMenu") == 0){
        printf("Berhasil di hapus\n");
        menu_utama();
    }
}

```

Seperti saat ingin mengunduh, client mengirim file yang ingin dihapus.

Dari sisi server :

```c

//delete 
			else if(client_message[1] == '3'){
				char nama_file[200];
				char *ps = client_message + 2;
				char *pd = nama_file + strlen(nama_file);
				strcpy(nama_file, client_message + 2);
				int i;
				int found_tsv=0;
				int found=0;
				//Mencari keberadaan file dalam files.tsv
				FILE* fp = fopen ("files.tsv","r"); //membuka "files.tsv" dalam mode read
				FILE* file_sementara = fopen("TEMP_files.tsv", "w");//File sementara
				char temp[300];
				while(fgets(temp, 300, fp)!=NULL){
					//Jika file ditemukan, tidak ditulis
					if(strstr(temp,nama_file) != NULL){
						printf("FILE YANG AKAN DIHAPUS ADA DI DATABASE\n");
						found_tsv = 1;
					}
					else{
						fprintf(file_sementara, "%s", temp);
					}
				}
				fclose(fp);
				fclose(file_sementara);
				remove("files.tsv");
				rename("TEMP_files.tsv", "files.tsv");
				//Mencari indeks file dalam struct
				if(found_tsv==1){
					for(i=0 ; i<index_file_server;i++){
						if(strcmp (nama_file, server_files[i].nama)==0){
							server_files[i].status=1;
							found=1;
							break;
						}
					}
				}
				if(found == 0 || found_tsv == 0){
					write(sock, "noFile", 7);
				}
				else{
					write(sock,"mainMenu", 8);
					//Rename file
					char fname_n[400];
					char fname_o[400];
					snprintf(fname_o, sizeof fname_o, "./FILES/%s", nama_file);
					snprintf(fname_n, sizeof fname_n, "./FILES/old-%s", nama_file);
					rename(fname_o, fname_n);
					//Memasukkan kedalam running.log
					FILE* running_log = fopen ("running.log","a");
            		fprintf(running_log, "Hapus : %s(%s)\n", nama_file, user_sekarang);
					fclose(running_log);
				}
				memset(client_message,0,sizeof(client_message));	
			}

```

Jalannya perintah seperti download, namun jika file ditemukan, maka akan diganti namanya menjadi "old-namaFile", data akan dihapus dari "files.tsv" dan status data dalam struct akan diubah menjadi "1".

      F. Melihat file-file yang ada dalam database server
      
Client dapat melihat file-file yang ada dalam server. Perintah dari sisi client :

```c

void lihat_file(){
    if( send(sock , "M4" , 2 , 0) < 0){
		puts("Send failed");
		return ;
	}
    memset(server_reply, 0, sizeof(server_reply));
    read(sock, server_reply, 1024);
    while(strcmp(server_reply, "mainMenu") != 0){
        printf("%s\n", server_reply);
        memset(server_reply, 0 , sizeof (server_reply));
        read(sock, server_reply, 1024);
    }
    menu_utama();
}

```

Client mengirim perintah "M4", lalu akan menerima dan mencetak line dari server hingga server mengirim "mainMenu".

Perintah dari sisi server :

```c
//see
			else if(client_message[1] == '4'){
				int i=0;
				for(i = 0; i < index_file_server; i++){
					if(server_files[i].status == 0){
						print_file_detail(i, sock);
					}
					sleep(1);	
				}
				memset(client_message,0,sizeof(client_message));	
				write(sock, "mainMenu", 8);
			}
```
Server akan melakukan pengecekan struct, jika status data dalam struct adalah 0, artinya file masih ada. Maka akan diprint dengan perintah berikut :

```c
void print_file_detail(int indeks, int sock){
	char f_nama[300];
	char f_publish[400];
	char f_tahun[50];
	char f_ekstensi[300];
	char f_filepath[800];
	int time = 100000;
	
	snprintf(f_nama, sizeof f_nama, "Nama = %s",server_files[indeks].nama);
	snprintf(f_publish, sizeof f_publish, "Publisher = %s",server_files[indeks].publisher);
	snprintf(f_tahun, sizeof f_tahun, "Tahun publishing = %s",server_files[indeks].tahun_terbit);
	snprintf(f_ekstensi, sizeof f_ekstensi, "Ekstensi File = %s",server_files[indeks].ekstensi_file);
	snprintf(f_filepath, sizeof f_filepath, "Filepath = %s\n",server_files[indeks].path_file);
	write(sock, f_nama, sizeof (f_nama));
	usleep(time);
	write(sock, f_publish, sizeof (f_publish));
	usleep(time);
	write(sock, f_tahun, sizeof (f_tahun));
	usleep(time);
	write(sock, f_ekstensi, sizeof (f_ekstensi));
	usleep(time);
	write(sock, f_filepath, sizeof (f_filepath));
	usleep(time);
}
```
Fungsi tersebut mengirim satu-persatu data yang dibutuhkan kepada cient. Dalam tiap pengiriman, terdapat sleep selama 100ms agar menghindari pengiriman yang diterima bersamaan.


        G. Mencari file-file yang ada dalam database server
Client dapat mencari file dengan memasukkan string tertentu. Akan di print data-data file yang mengandung string tersebut. Contoh : Data dalam server = {"abc.txt","ab.txt","c.txt"}, jika client memasukkan "a", maka akan dicetak data file "abc.txt" dan "ab.txt". Berikut perintah dari sisi client :

```c
void cari_file(){
    char nama_file[200];
    printf("find : ");
    getchar();
    scanf("%[^\n]s", nama_file);
    //concate perintah
    snprintf(clSend, sizeof clSend, "M5%s", nama_file);
    if( send(sock , clSend , strlen(clSend) , 0) < 0){
		puts("Send failed");
		return ;
	}
    memset(server_reply, 0, sizeof(server_reply));
    read(sock, server_reply, 1024);
    while(strcmp(server_reply, "mainMenu") != 0){
        printf("%s\n", server_reply);
        memset(server_reply, 0 , sizeof (server_reply));
        read(sock, server_reply, 1024);
    }
    menu_utama();
}
```

Client mengirimkan nama file yang dicari, lalu menerima pesan dari server dan mencetak pesan tersebut hingga server mengirim "mainMenu".

Dari sisi server, perintah sebagai berikut :
```c
//find
			else if(client_message[1] == '5'){
				char nama_file[200];
				char *ps = client_message + 2;
				char *pd = nama_file + strlen(nama_file);
				strcpy(nama_file, client_message + 2);
				int i=0;
				for(i = 0; i < index_file_server; i++){
					if(server_files[i].status == 0 && strstr(server_files[i].nama, nama_file)!=NULL){
						print_file_detail(i, sock);
					}
					usleep(100000);	
				}
				memset(client_message,0,sizeof(client_message));	
				write(sock, "mainMenu", 8);
			}
		memset(client_message,0,sizeof(client_message));	
		}
```

Konsep mirip dengan download maupun hapus, namun perbedaannya adalah file yang memenuhi syarat akan dikirim data-datanya ke client, bukan dihapus ataupun dimanipulasi dalam database.

      H. Membuat "running.log"
Running log dibuat oleh server, berisi data ketika terjadi penambahan atau penghapusan file di server. Berikut potongan kodenya :

```c
          //Memasukkan kedalam running.log untuk add
					FILE* running_log = fopen ("running.log","a");
          fprintf(running_log, "Tambah : %s(%s)\n", nama_file, user_sekarang);
					fclose(running_log);


          //Memasukkan kedalam running.log untuk delete
					FILE* running_log = fopen ("running.log","a");
          fprintf(running_log, "Hapus : %s(%s)\n", nama_file, user_sekarang);
					fclose(running_log);
```

Running log akan berjalan ketika client melakukan perintah "add", maupun "delete". Running log akan berisi keterangan "Tambah" atau "Hapus", dan user yang melakukan aksi tersebut.


**Kendala nomor 1 :**
Pengerjaan pada waktu pekan ETS, sehingga pembagian waktu menjadi tantangan tersendiri. Selain itu, ada kendala berupa mencari tahu logika socket serta pengiriman dan penerimaan pesan. Ada pula kesulitan cara menyalin file.

### Soal 2
> **[soal 2](https://github.com/billharit/soal-shift-sisop-modul-3-C10-2021/tree/main/soal2)**

a. Membuat program perkalian matrix (4x3 dengan 3x6) dan menampilkan hasilnya. Matriks nantinya akan berisi angka 1-20 (tidak perlu dibuat filter angka).

Fungsi perkalian matriks :

```
void *mult(void* arg)
{
    int i = ((struct v*)arg)->i,
        j = ((struct v*)arg)->j;

    for(int k=0; k<Y; k++)
    {
        C[i][j] += A[i][k] * B[k][j];
    }

    int *p = (int *) malloc(sizeof(int));
    *p = C[i][j];

    pthread_exit(p);
}
```



```
    key_t key = 1234;
    int *value;

    int shmid = shmget(key, sizeof(int)*T, IPC_CREAT | 0666);
    value = (int *)shmat(shmid, NULL, 0);

    pthread_t thr[T];
```

```
count = 0;
    for(i = 0; i < X; i++)
    {
        for(j = 0; j < Z; j++)
        {
            struct v *data = (struct v *) malloc(sizeof(struct v));
            data->i = i;
            data->j = j;

            pthread_create(&thr[count], NULL, &mult, (void *)data);
            count++;
        }
    }

    for(i = 0; i < T; i++)
    {
        pthread_join(thr[i], NULL);
    }
```

```
    int a = 0;
    printf("Hasil perkalian matriks:\n");
    for(i = 0; i < X; i++)
    {
        for(j = 0; j < Z; j++)
        {
            value[a] = C[i][j];
            printf("%d\t", value[a]);
            a++;
        }
        printf("\n");
    }

    shmdt(value);
```

b. Membuat program dengan menggunakan matriks output dari program sebelumnya (program soal2a.c). Kemudian matriks tersebut akan dilakukan perhitungan dengan matrix baru. Perhitungannya adalah setiap cel yang berasal dari matriks A menjadi angka untuk faktorial, lalu cel dari matriks B menjadi batas maksimal faktorialnya matri(dari paling besar ke paling kecil)

```
unsigned long long factorial(unsigned long long n)
{
    if(n == 0)
        return 1;
    
    return n*factorial(n-1);
}
```

```
void *fact(void* arg)
{
    int i = ((struct v*)arg)->i,
        j = ((struct v*)arg)->j;
    
    int a = matrix[i][j],
        b = new[i][j];

    if(a == 0 || b == 0)
        hasil[i][j] = 0;
    else if(a >= b)
    {
        hasil[i][j] = factorial(a)/factorial(a-b);
    }
    else if(a < b)
        hasil[i][j] = factorial(a);
    
    pthread_exit(0);
}
```

```
    key_t key = 1234;
    int *value;
    int shmid = shmget(key, sizeof(int)*T, IPC_CREAT | 0666);
    value = (int *)shmat(shmid, NULL, 0);
```

```
    printf("matriks: \n");

    int a = 0;
    for(i=0; i<M; i++)
    {
        for(j=0; j<N; j++)
        {
            printf("%d\t", value[a]);
            matrix[i][j] = value[a];
            a++;
        }
        printf("\n");
    }
```

```
    int count = 0;
    //hasil factorial
    for(i=0; i<M; i++)
    {
        for(j=0; j<N; j++)
        {
            struct v *data = (struct v*)malloc(sizeof(struct v));
            data->i = i;
            data->j = j;

            pthread_create(&tid[count], NULL, &fact, (void *)data);
            count++;
        }
    }

    for(i=0; i<T; i++)
    {
        pthread_join(tid[i], NULL);
    }
```

```
    printf("Hasil matriks: \n");
    for(i=0; i<M; i++)
    {
        for(j=0; j<N; j++)
        {
            printf("%llu\t", hasil[i][j]);
        }
        printf("\n");
    }

    shmdt(value);
    shmctl(shmid, IPC_RMID, NULL);
```

c. Mengecek 5 proses teratas apa saja yang memakan resource komputernya dengan command “ps aux | sort -nrk 3,3 | head -5”

exec1 untuk menjalankan command ps :

```
void exec1()
{
    dup2(fd1[1], 1);
    closepipe1();

    char *argv1[] = {"ps", "aux", NULL};
    
    execv("/bin/ps", argv1);
    
    // execlp("/bin/ps", "ps", "aux", NULL);
    
    perror("ps aux failed");
    _exit(1);
}
```

exec2 untuk menjalankan command sort :

```
void exec2()
{
    dup2(fd1[0], 0);
    dup2(fd2[1], 1);

    closepipe1();
    closepipe2();

    char *argv2[] = {"sort", "-nrk", "3,3", NULL};
    execv("/bin/sort", argv2);

    // execlp("usr/bin/sort", "sort", "-nrk", "3,3", NULL);

    perror("sort failed");
    _exit(1);
}
```

exec3 untuk menjalankan command head :

```
void exec3()
{
    dup2(fd2[0], 0);

    closepipe2();

    char *argv3[] = {"head", "-5", NULL};
    execv("/bin/head", argv3);
    
    // execlp("usr/bin/head", "head", "-5", NULL);
    
    perror("head failed");
    _exit(1);
}
```

Pembuatan pipe :

```
    pipe(fd1);
    pipe(fd2);

    pid_t pid, pid1, pid2;
    

    if(pipe(fd1) == -1)
    {
        perror("Pipe 1 Creation is Failed\n");
        exit(EXIT_FAILURE);
    }

    if(pipe(fd2) == -1)
    {
        perror("Pipe 2 Creation is Failed\n");
        exit(EXIT_FAILURE);
    }
```

Pemanggilan fungsi exec menggunakan fork :

```
    pid = fork();

    if(pid < 0 )
    {
        fprintf(stderr, "fork failed");
        exit(EXIT_FAILURE);
    }
    else if(pid == 0)
    {
        exec1();
    }
```

```
    pid1 = fork();

    if(pid1 < 0 )
    {
        fprintf(stderr, "fork failed");
        return 1;
    }
    else if(pid1 == 0)
    {
        exec2();
    }

    closepipe1();
```

```
    pid2 = fork();
    if(pid2 < 0)
    {
        fprintf(stderr, "fork failed");
        return 1;
    }
    else if(pid2 == 0)
    {
        exec3();
    }
```



### Soal 3
> **[soal 3](https://github.com/billharit/soal-shift-sisop-modul-3-C10-2021/tree/main/soal3)**



Kendala :
Kekurangan waktu karena minggu ETS
