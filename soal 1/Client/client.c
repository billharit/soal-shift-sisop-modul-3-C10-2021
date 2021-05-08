#include <stdio.h>	
#include <string.h>	
#include <sys/socket.h>	
#include <arpa/inet.h>	
#include <unistd.h>

int main(int argc , char *argv[])
{
	int sock;
	struct sockaddr_in server;
	char message[1000] , server_reply[2000];
	
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
	int start = 0;
	while(1)
	{
		if(start ==0){
		if( recv(sock , server_reply , 2000 , 0) < 0)
		{
			puts("recv failed");
			break;
		}
		puts(server_reply);

		printf("Enter message : ");
		scanf("%s" , message);
		
		//Send some data
		if( send(sock , message , strlen(message) , 0) < 0)
		{
			puts("Send failed");
			return 1;
		}
		}
		start=1;
		printf("Enter message : ");
		scanf("%s" , message);
		
			/*if( send(sock , username , strlen(username) , 0) < 0)
		    {
			puts("Send failed");
			return 1;
		    }
            if( send(sock , password , strlen(password) , 0) < 0)
		    {
			puts("Send failed");
			return 1;
		    }*/
		char *register_user, *login_user;
        register_user = "Please type username & password :\n";
		login_user = "Please login using username & password:\n";
        //puts("Server reply :");
		//Receive a reply from the server
		if( recv(sock , server_reply , 2000 , 0) < 0)
		{
			puts("recv failed");
			break;
		}
		puts(server_reply);
        if(strcmp(server_reply,register_user) == 0 || strcmp(server_reply,login_user) == 0){
            printf("Username : ");
			char username[2000];
			char password[2000];
            char username_1[200];
            char password_1[200];
			char Er[] = "R";
			char Pe[] = "P";
            scanf("%s", username_1);
            printf("Password : ");
            scanf("%s", password_1);
			strcat(username, Er);
			strcat(username,username_1);
			strcat(password, Pe);
			strcat(password,password_1);
			printf("%s %s\n",username, password);
			//snprintf(username, sizeof username, "%c%s",  Er,username_1);
			//snprintf(password, sizeof password, "%c%s", Pe,password_1);
            if( send(sock , username , strlen(username) , 0) < 0)
		    {
			puts("Send failed");
			return 1;
		    }
            if( send(sock , password , strlen(password) , 0) < 0)
		    {
			puts("Send failed");
			return 1;
		    }

        }
	}
	
	close(sock);
	return 0;
}
