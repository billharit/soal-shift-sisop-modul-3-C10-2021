#include<stdio.h>
#include<string.h>	
#include<stdlib.h>	
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<pthread.h> 
#include<sys/stat.h>
#include<sys/types.h>
#include<libgen.h>


//Banyaknya client yang terhubung
int connected_client = 0;

void *connection_handler(void *);
void print_file_detail(int indeks, int sock);

//Files yang pernah di-upload beserta keterangan-keterangan
typedef struct F_server{
	int status; // 0 artinya tersedia, 1 telah terhapus
	char nama[200];
	char publisher[200];
	char tahun_terbit[20];
	char ekstensi_file[20];
	char path_file[700];
	char pembuat[200];
	char penghapus[200];
}file_di_server;

file_di_server server_files[200];
char user_sekarang[200];
int index_file_server = 0;
int main(int argc , char *argv[])
{
	int socket_desc , client_sock , c , *new_sock;
	struct sockaddr_in server , client;

	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1)
	{
		printf("Could not create socket");
	}
	puts("Socket created");
	
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
    fclose(running_log);
	printf("berhasil membuat running.log\n");

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons( 8888 );

	//Bind
	if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
	{
		perror("bind failed. Error");
		return 1;
	}
	
	listen(socket_desc , 3);
	
	c = sizeof(struct sockaddr_in);
	
	
	c = sizeof(struct sockaddr_in);
	while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
	{
        connected_client++;
		puts("Connection accepted");
        printf("Number of connected client = %d\n", connected_client);
		pthread_t sniffer_thread;
		new_sock = malloc(1);
		*new_sock = client_sock;
		
		if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0)
		{
			perror("could not create thread");
            connected_client--;
			return 1;
		}
		
		puts("Handler assigned");
	}
	
	if (client_sock < 0)
	{
		perror("accept failed");
		return 1;
	}
	
	return 0;
}

void *connection_handler(void *socket_desc)
{
    int start =0;
	int sock = *(int*)socket_desc;
	int read_size;
	char *message , client_message[2000];
	while(connected_client > 1){
		write(sock, "mohon tunggu", 12);
		sleep(1);
	}
	if(start ==0){
	message = "menu";
    write(sock, message, strlen(message));	
	}
	start=1;
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
		//Menerima pesan dalam menu utama
		else if(client_message[0] == 'M'){
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


		//Jika perintah salah
		else{
			write(sock, "Maaf", 4);
		}
        //Pesan dibuat 0 agar bisa menerima pesan baru
        //Menghindari pesan yang tertumpuk
        memset(client_message,0,sizeof(client_message));
    }
	
	if(read_size == 0)
	{
		puts("Client disconnected");
        connected_client--;
		fflush(stdout);
	}
	else if(read_size == -1)
	{
        connected_client--;
		perror("recv failed");
	}
		
	free(socket_desc);
	
	return 0;
}


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
