#include<stdio.h>
#include<string.h>	
#include<stdlib.h>	
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<pthread.h> 

int start =0;

//Struktur akun
typedef struct akun{
    char username[2000];
    char password[2000];

}akun;
//Banyaknya client yang terhubung
int connected_client = 0;

void *connection_handler(void *);

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
	int sock = *(int*)socket_desc;
	int read_size;
	char *message , client_message[2000];
	if(start ==0){
	message = "Welcome, please select a command :\n1.Register\n2.Login";
    write(sock, message, strlen(message));	
	}
	start=1;
	while( (read_size = recv(sock , client_message , 2000 , 0)) > 0 )
	{
		if(client_message[0] == 'R'){
			int read_username;
            int read_password;
            
                FILE* fp = fopen ("akun.txt","a");
                fprintf(fp, "%s\n", client_message);
				fclose(fp);
            
			//write(sock, "Berhasil daftar\n", 30);
		}
		if(client_message[0] == 'P'){
			int read_username;
            int read_password;
                FILE* fp = fopen ("akun.txt","a");
                fprintf(fp, "%s\n", client_message);
				fclose(fp);
			write(sock, "Berhasil daftar\n", 30);
		}
        if(strcmp(client_message,"1") == 0){
            message = "Please type username & password :\n";
			write(sock, message, strlen(message));
            /*int read_username;
            int read_password;
            if(read_username = recv(sock , client_message , 2000 , 0) > 0){
                FILE* fp = fopen ("akun.txt","a");
                fprintf(fp, "%s\n", client_message);
				fclose(fp);
			}
            if(read_password = recv(sock , client_message , 2000 , 0) > 0){
                FILE* fp = fopen ("akun.txt","a");
                fprintf(fp, "%s\n", client_message);
				fclose(fp);
            }
			write(sock, "Berhasil daftar\n", 30);
			*/
        }
        else if(strcmp(client_message,"2") == 0){
            message = "Please login using username & password:\n";
            write(sock, message, strlen(message));
            int read_username;
            int read_password;
            if(read_username = recv(sock , client_message , 2000 , 0) > 0){
                FILE* fp = fopen ("akun.txt","a");
                fprintf(fp, "%s\n", client_message);
				fclose(fp);
			}
            if(read_password = recv(sock , client_message , 2000 , 0) > 0){
                FILE* fp = fopen ("akun.txt","a");
                fprintf(fp, "%s\n", client_message);
				fclose(fp);
            }
			write(sock, "Berhasil Login\n", 30);
        } 
		else {
            message = "maaf salah\n";
            write(sock , message , strlen(message));
        }
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
