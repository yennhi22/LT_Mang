#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "struct.h"
#define MAX 1024
void Up(char *s)
{
	for (int i = 0; i < strlen(s); i++)
	{
		s[i] = s[i] - 32;
	}
}
int getRegisterName(node_t *user)
{
	char username[255];
	printf("Username: ");
	scanf("%[^\n]s", username);
	strcpy((*user).username, username);
	return 1;
}
int getRegisterPassword(node_t *user)
{
	char username[255], password[255], confirmPass[255];
	printf("Password: ");
	while (getchar() != '\n')
		;
	scanf("%[^\n]s", password);
	printf("Confirm password: ");
	while (getchar() != '\n')
		;
	scanf("%[^\n]s", confirmPass);
	while (getchar() != '\n')
		;
	if (!strcmp(password, confirmPass))
	{
		// sprintf(mess->payload, "REGISTER\nUSER %s\nPASS %s", username, password);
		// strcpy(user, username);
		strcpy((*user).password, password);
		return 1;
	}
	else
	{
		printf("Confirm password invalid!\n");
		return 0;
	}
}
void menuAuthenticate()
{
	printf("\n---------------QuickTest-------------\n");
	printf("\n1 - Login");
	printf("\n2 - Register");
	printf("\n3 - Exit");
	printf("\nPlease choose: ");
}
int main(int argc, char const *argv[])
{
	// valid number of argument
	char answer[11], choose;
	if (argc != 3)
	{
		printf("Usage: ./client IPAddress PortNumber\n\n");
		return 0;
	}

	int client_sock;
	char username[MAX], password[MAX], buff[8192];
	struct sockaddr_in server_addr;
	int bytes_sent, bytes_received;
	int flag;
	node_t current_user;
	// Construct socket
	client_sock = socket(AF_INET, SOCK_STREAM, 0);

	// Specify server address
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(atoi(argv[2]));
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);

	// Request to connect server
	if (connect(client_sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) < 0)
	{
		printf("\nError!Can not connect to sever! Client exit imediately! ");
		return 0;
	}
	menuAuthenticate();
	scanf("%c", &choose);
	while (getchar() != '\n')
		;
	char status[10];
	strcpy(status, &choose);
	status[strcspn(status, "\n")] = '\0';
	if (0 >= (bytes_sent = send(client_sock, status, strlen(status), 0)))
	{
		printf("\nConnection closed!\n");
		return 0;
	}

	switch (choose)
	{
	case '1':
		while (1)
		{
			char status[10];
			strcpy(status, "1");
			status[strcspn(status, "\n")] = '\0';
			if (0 >= (bytes_sent = send(client_sock, status, strlen(status), 0)))
			{
				printf("\nConnection closed!\n");
				return 0;
			}
			//char *status;

			// get username
			puts("\nHãy nhập mật tên người dùng và mã bảo mật");
			printf("Tên người dùng: ");
			scanf("%[^\n]%*c", username);
			//puts(username);
			username[strcspn(username, "\n")] = '\0';

			// send username to server
			if (0 >= (bytes_sent = send(client_sock, username, strlen(username), 0)))
			{
				printf("\nConnection closed!\n");
				return 0;
			}

			// receive server reply
			if (0 >= (bytes_received = recv(client_sock, buff, 8192, 0)))
			{
				printf("\nError!Cannot receive data from sever!\n");
				return 0;
			}

			// exit if user not found on server
			buff[bytes_received] = '\0';
			if (0 == strcmp(buff, "0"))
			{
				puts("Không tìm thấy tên tài khoản,đang đóng...\n");
				return 0;
			}
			else if (0 == strcmp(buff, "2"))
			{
				puts("Tài khoản đã bị khóa,đang đóng...\n");
				return 0;
			}

			// get password
			printf("Mã bảo mật: ");
			fgets(password, MAX, stdin);
			password[strcspn(password, "\n")] = '\0';

			// send password to server
			if (0 >= (bytes_sent = send(client_sock, password, strlen(password), 0)))
			{
				printf("\nConnection closed!\n");
				return 0;
			}

			// receive server reply
			memset(buff, '\0', MAX);
			if (0 >= (bytes_received = recv(client_sock, buff, 8192, 0)))
			{
				printf("\nError!Cannot receive data from sever!\n");
				return 0;
			}

			// printf("\nhello %d\n",bytes_received);
			buff[bytes_received] = '\0';

			// analyze server reply
			if (0 == strcmp(buff, "0"))
			{ // if pass is wrong < 3 times
				puts("\nMã bảo mật nhập sai,Xin mời nhập lại\n");
				continue;
			}
			else if (0 == strcmp(buff, "2"))
			{ // if pass is wrong 3 times
				puts("\nNhập sai quá 3 lần mã bảo mật,tài khoản của bạn đã bị khóa\n");
				return 0;
			}
			else
			{ // if pass is right
				for (int i = 0; i < 10; i++)
				{
					printf("%s\n câu trả lời là:\t", buff);
					scanf("%[^\n]%*c", answer);
					Up(answer);
					if (0 >= (bytes_sent = send(client_sock, answer, strlen(answer), 0)))
					{
						printf("\nConnection closed!\n");
						return 0;
					}
					memset(buff, 0, 8192);
					if (0 >= (bytes_received = recv(client_sock, buff, 8192, 0)))
					{
						printf("\nError!Cannot receive data from sever!\n");
						return 0;
					}
				}
				memset(buff, '\0', 8192);
				if (0 >= (bytes_received = recv(client_sock, buff, 8192, 0)))
				{
					printf("\nError!Cannot receive data from sever!\n");
					return 0;
				}
				printf("\nBạn trả lời đúng: %s/10", buff);
				break;

				// break;
			}
		}
		break;
	case '2':
		//registerFunc(current_user);
		flag = getRegisterName(&current_user);
		if (flag == 0)
			break;

		else
		{
			strcpy(username, current_user.username);
			// send username to server
			if (0 >= (bytes_sent = send(client_sock, username, strlen(username), 0)))
			{
				printf("\nConnection closed!\n");
				return 0;
			}

			// receive server reply
			if (0 >= (bytes_received = recv(client_sock, buff, 8192, 0)))
			{
				printf("\nError!Cannot receive data from sever!\n");
				return 0;
			}

			// exit if user not found on server
			buff[bytes_received] = '\0';
			// printf("buff : %s",buff);
			if (0 == strcmp(buff, "2"))
			{
				puts("Tai khoan da ton tai\n");
				return 0;
			}
			else if (0 == strcmp(buff, "1"))
			{
				puts("Tai khoan da ton tai\n");
				return 0;
			}
			//get password
			flag = getRegisterPassword(&current_user);
			if (flag == 0)
				getRegisterPassword(&current_user);
			else
			{
				strcpy(password, current_user.password);
				password[strcspn(password, "\n")] = '\0';
				if (0 >= (bytes_sent = send(client_sock, password, strlen(password), 0)))
				{
					printf("\nConnection closed!\n");
					return 0;
				}

				// receive server reply
				memset(buff, '\0', MAX);
				if (0 >= (bytes_received = recv(client_sock, buff, 8192, 0)))
				{
					printf("\nError!Cannot receive data from sever!\n");
					return 0;
				}
				if(strcmp(buff,"4")==0) printf ("\nDang ki thanh cong");
				else printf ("\nDang ki ko thanh cong");
			}
		}
		break;
	case '3':
		// exit(0);
	default:
		printf("Syntax Error! Please choose again!\n");
	}
	//Communicate with server

	//Close socket
	close(client_sock);
	return 0;
}
