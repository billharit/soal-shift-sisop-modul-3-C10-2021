#include <stdio.h>	
#include <string.h>	
#include <sys/socket.h>	
#include <arpa/inet.h>	
#include <unistd.h>
#include <stdlib.h>

void menu_utama(); //Fungsi menunjukkan menu utama setelah berhasil login
void add_file(); //Fungsi menambah file kedalam Direktori FILES
void download_file(); //Fungsi mengunduh file dari server
void hapus_file();//Fungsi menghapus file dari server
void lihat_file();//Fungsi melihat file yang ada di server
void cari_file();//Fungsi mencari file yang ada di server

int sock;
struct sockaddr_in server;
char message[1000] , server_reply[2000];
int start;
char clReply[1000];//Menerima perintah
char clSend[2000];//Perintah yang dikirim
char currentUser[200]; //Pengguna sekarang

int main(int argc , char *argv[])
{
	
	sock = socket(AF_INET , SOCK_STREAM , 0);
	if (sock == -1)
	{
		printf("Could not create socket");
	}
	puts("Socket created");
	
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons( 8888 );

	if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
	{
		perror("connect failed. Error");
		return 1;
	}
	
	puts("Connected\n");
	while(1)
	{
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

        } else if (strcmp(server_reply, "Maaf") == 0){
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
            memset(clSend, 0,sizeof(clSend));
            memset(clReply, 0, sizeof(clReply));
        } else if (strcmp(server_reply, "mainMenu") == 0){
            menu_utama();
        } else if(strcmp(server_reply, "mohon tunggu") == 0){
            puts(server_reply);
        } else{
            puts(server_reply);
            printf("Maaf ada yang salah\n");
            exit(1);
        }


    memset(clSend, 0,sizeof(clSend));
    memset(clReply, 0, sizeof(clReply));
    }
		
	
	close(sock);
	return 0;
}

void menu_utama(){
    memset(clSend, 0,sizeof(clSend));
    printf("Hello please select a command :\n1.Add\n2.Download\n3.Delete\n4.See\n5.find\n");
    printf("Menu chosen : ");
    scanf("%s", clReply);
    const char* perintah;
    if(clReply[0] == '1'){
        add_file();
    }else if(clReply[0] == '2'){
        download_file();
    }else if(clReply[0] == '3'){
        hapus_file();
    }else if(clReply[0] == '4'){
        lihat_file();
    }else if(clReply[0] == '5'){
        cari_file();
    }
    
}


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
