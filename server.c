#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include "struct.h"
Question questionList[QUES_NUMBER];
int easyList[QUES_IN_LEVER];
int mediumList[QUES_IN_LEVER];
int hardList[QUES_IN_LEVER];
int easyIndex = 0, mediumIndex = 0, hardIndex = 0;
// load data from text file to linked list
node_t *load_data(char *filename)
{
	int status, point, count = 0; // number of accounts
	FILE *f;
	char username[MAX], password[MAX];
	node_t *head, *current;
	head = current = NULL;

	// open file data
	printf("Loading data from file...\n");

	if ((f = fopen(filename, "r")) == NULL)
	{
		printf("Cannot open file!\n");
		exit(0);
	}

	// load accounts to linked list
	while (fscanf(f, "%s %s %d %d\n", username, password, &status, &point) != EOF)
	{
		// create new node
		node_t *node = malloc(sizeof(node_t));
		strcpy(node->username, username);
		strcpy(node->password, password);
		node->status = status;
		node->point = point;
		// add node to list
		if (head == NULL)
			current = head = node;
		else
			current = current->next = node;
		count++;
	}

	fclose(f);
	printf("LOADED SUCCESSFULY %d ACCOUNT(S)\n", count);
	return head;
}

// find a node exist in linked list given username
node_t *find_node(node_t *head, char *username)
{
	node_t *current = head;
	while (current != NULL)
	{
		if (0 == strcmp(current->username, username))
			return current;
		current = current->next;
	}
	return NULL;
}
node_t *add_note(node_t *head, char username[], char password[])
{
	node_t *new = (node_t*)malloc(sizeof(node_t*));
	strcpy(new->password,password);
	strcpy(new->username,username);
	new->status = 1;
	new->point = 0;
	if(head == NULL){
        head = new; // //Nếu linked list đang trống thì Node temp là head luôn
    }else{
        new->next = head; // Trỏ next của temp = head hiện tại
        head = new; // Đổi head hiện tại = temp(Vì temp bây giờ là head mới mà)
    }
    return head;

}
// save list to text file
void save_list(node_t *head, char *filename)
{
	FILE *f;
	f = fopen(filename, "w");
	node_t *current;
	for (current = head; current; current = current->next)
		fprintf(f, "%s %s %d %d\n", current->username, current->password, current->status, current->point);
	fclose(f);
}
int addLeverList(int id, int lv)
{
	switch (lv)
	{
	case 1:
		easyList[easyIndex] = id;
		easyIndex++;
		break;
	case 2:
		mediumList[mediumIndex] = id;
		mediumIndex++;
		break;
	case 3:
		hardList[hardIndex] = id;
		hardIndex++;
		break;
	default:
		break;
	}
	return 1;
}
void readQues()
{
	FILE *fptr;
	int id, lv;
	char content[1000], a[1000], b[1000], c[1000], d[1000];
	char answ;
	int i = 0;
	fptr = fopen(QUES_FILENAME, "r");
	if (fptr == NULL)
	{
		printf("Can't open file !\n");
		return;
	}

	while (fscanf(fptr, "%d | %d | %[^|] | %[^|] | %[^|] | %[^|] | %[^|] | %c", &id, &lv, content, a, b, c, d, &answ) != EOF)
	{
		questionList[i].id = id;
		questionList[i].level = lv;
		strcpy(questionList[i].content, content);
		strcpy(questionList[i].choiceA, a);
		strcpy(questionList[i].choiceB, b);
		strcpy(questionList[i].choiceC, c);
		strcpy(questionList[i].choiceD, d);
		questionList[i].answer = answ;
		addLeverList(id, lv);
		i++;
	}
	fclose(fptr);
}
void randomId(int *arr, int lv)
{

	if (lv == 1)
	{
		arr[0] = rand() % 9;
		do
		{
			arr[1] = rand() % 9;
		} while (arr[1] == arr[0]);
		do
		{
			arr[2] = rand() % 9;
		} while (arr[2] == arr[0] || arr[2] == arr[1]);
		do
		{
			arr[3] = rand() % 9;
		} while (arr[3] == arr[0] || arr[3] == arr[1] || arr[3] == arr[2]);
		return;
	}
	if (lv == 2)
	{
		arr[0] = rand() % 9;
		do
		{
			arr[1] = rand() % 9;
		} while (arr[1] == arr[0]);
		do
		{
			arr[2] = rand() % 9;
		} while (arr[2] == arr[0] || arr[2] == arr[1]);
		do
		{
			arr[3] = rand() % 9;
		} while (arr[3] == arr[0] || arr[3] == arr[1] || arr[3] == arr[2]);
		return;
	}
	if (lv == 3)
	{
		arr[0] = rand() % 9;
		do
		{
			arr[1] = rand() % 9;
		} while (arr[1] == arr[0]);
		return;
	}
}
int makeQuestion(int arr[])
{
	int easy[4];
	int medium[4];
	int hard[2];
	int i, n = 0;
	srand((int)time(0));
	randomId(easy, 1);
	randomId(medium, 2);
	randomId(hard, 3);
	for (i = 0; i < 4; i++)
	{
		arr[n] = easy[i];
		n++;
	}
	for (i = 0; i < 4; i++)
	{
		arr[n] = medium[i];
		n++;
	}
	for (i = 0; i < 2; i++)
	{
		arr[n] = hard[i];
		n++;
	}
}

void makeOneQues(Message message[])
{
	int easy[4];
	int medium[4];
	int hard[2];
	int i, n, count = 0;
	Message *buff;
	srand((int)time(0));
	randomId(easy, 1);
	randomId(medium, 2);
	randomId(hard, 3);
	for (i = 0; i < 4; i++)
	{
		buff = (Message *)malloc(sizeof(Message));
		n = easyList[easy[i]];
		strcpy((*buff).content, questionList[n].content);
		strcat((*buff).content, "\n");
		strcat((*buff).content, questionList[n].choiceA);
		strcat((*buff).content, "\t");
		strcat((*buff).content, questionList[n].choiceB);
		strcat((*buff).content, "\n");
		strcat((*buff).content, questionList[n].choiceC);
		strcat((*buff).content, "\t");
		strcat((*buff).content, questionList[n].choiceD);
		strcat((*buff).content, "\n\n");
		(*buff).answer[0] = questionList[n].answer;
		strcpy(message[count].content, (*buff).content);
		message[count].answer[0] = (*buff).answer[0];
		printf("%d : %s\n", count, message[count].content);
		count++;
		free(buff);
	}
	for (i = 0; i < 4; i++)
	{
		buff = (Message *)malloc(sizeof(Message));
		n = mediumList[medium[i]];
		strcpy((*buff).content, questionList[n].content);
		strcat((*buff).content, "\n");
		strcat((*buff).content, questionList[n].choiceA);
		strcat((*buff).content, "\t");
		strcat((*buff).content, questionList[n].choiceB);
		strcat((*buff).content, "\n");
		strcat((*buff).content, questionList[n].choiceC);
		strcat((*buff).content, "\t");
		strcat((*buff).content, questionList[n].choiceD);
		strcat((*buff).content, "\n\n");
		(*buff).answer[0] = questionList[n].answer;
		strcpy(message[count].content, (*buff).content);
		message[count].answer[0] = (*buff).answer[0];
		printf("%d : %s\n", count, message[count].content);
		count++;
		free(buff);
	}
	for (i = 0; i < 2; i++)
	{
		buff = (Message *)malloc(sizeof(Message));
		n = hardList[hard[i]];
		strcpy((*buff).content, questionList[n].content);
		strcat((*buff).content, "\n");
		strcat((*buff).content, questionList[n].choiceA);
		strcat((*buff).content, "\t");
		strcat((*buff).content, questionList[n].choiceB);
		strcat((*buff).content, "\n");
		strcat((*buff).content, questionList[n].choiceC);
		strcat((*buff).content, "\t");
		strcat((*buff).content, questionList[n].choiceD);
		strcat((*buff).content, "\n\n");
		(*buff).answer[0] = questionList[n].answer;
		strcpy(message[count].content, (*buff).content);
		message[count].answer[0] = (*buff).answer[0];
		printf("%d : %s\n", count, message[count].content);
		count++;
		free(buff);
	}
	//printf("%s",buff.content);
	// return buff;
}
int checkAnswer(char *req, char *local)
{
	int point = 0;
	// for (int i = 0; i < 10; i++)
	// {
	// 	if (req[i] == local[i])
	// 		point++;
	// }
	if (req[0] == local[0])
		point++;
	return point;
}
int main(int argc, char const *argv[])
{
	char filename[] = "account.txt";
	readQues();
	//printf("%s",buff.content);
	// valid number of argument
	if (argc != 2)
	{
		printf("Usage: ./server PortNumber\n\n");
		return 0;
	}

	int listen_sock, conn_sock, n;
	char username[MAX], password[MAX], answer[11], point[3], *reply;
	char status[10];
	int bytes_sent, bytes_received;
	struct sockaddr_in server;
	struct sockaddr_in client;
	socklen_t sin_size;
	node_t *found;
	int pid;

	// load file txt to linked list
	node_t *account_list = load_data(filename);

	// Construct a TCP socket to listen connection request
	if ((listen_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("\nError: ");
		return 0;
	}

	// Bind address to socket
	memset(&server, '\0', sizeof server);
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[1]));
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(listen_sock, (struct sockaddr *)&server, sizeof(server)) == -1)
	{
		perror("\nError: ");
		return 0;
	}

	// Listen request from client
	if (listen(listen_sock, BACKLOG) == -1)
	{
		perror("\nError: ");
		return 0;
	}

	puts("Server up and running...\n");

	// Communicate with client
	while (1)
	{
		// accept request
		sin_size = sizeof(struct sockaddr_in);
		if ((conn_sock = accept(listen_sock, (struct sockaddr *)&client, &sin_size)) == -1)
			perror("\nError: ");

		printf("\nYou got a connection from %s\n", inet_ntoa(client.sin_addr)); /* prints client's IP */

		// start conversation on other fork
		pid = fork();
		if (pid < 0)
		{
			perror("Error");
			return 1;
		}

		// on child process
		if (pid == 0)
		{
			if (0 >= (bytes_received = recv(conn_sock, status, MAX - 1, 0)))
			{
				printf("\nConnection closed 1\n");
				break;
			}
			status[bytes_received] = '\0';
			
			if (strcmp(status, "1") == 0)
			{
				int count = 0; // count password repeatation
				while (1)
				{

					if (0 >= (bytes_received = recv(conn_sock, username, MAX - 1, 0)))
					{
						printf("\nConnection closed 1\n");
						break;
					}
					username[bytes_received] = '\0';

					// check username existence
					if ((found = find_node(account_list, username)))
					{
						if (found->status == 1)
							reply = "1"; // username found
						else
							reply = "2"; // username found but has been locked
					}
					else
						reply = "0"; // username not found

					// echo to client
					if (0 >= (bytes_sent = send(conn_sock, reply, strlen(reply), 0)))
					{
						printf("\nConnection closed 2\n");
						break;
					}

					while (1)
					{
						// receive password
						memset(password, '\0', MAX);
						if (0 >= (bytes_received = recv(conn_sock, password, MAX - 1, 0)))
						{
							printf("\nConnection closed 3\n");
							break;
						}
						password[bytes_received] = '\0';

						// validate password
						if (0 == strcmp(found->password, password))
						{
							int point1 = 0;
							Message buff[20];
							makeOneQues(buff);
							for (int i = 0; i < 10; i++)
							{
								// Message buff = makeQuesList();

								reply = buff[i].content;
								// printf("%d Đáp án:\n%s\n",i, buff[i].content);
								if (0 >= (bytes_sent = send(conn_sock, reply, strlen(reply), 0)))
								{
									printf("\nConnection closed 4\n");
									break;
								}
								if (0 >= (bytes_received = recv(conn_sock, answer, 11, 0)))
								{
									printf("\nConnection closed 5\n");
									break;
								}
								printf("%s", answer);
								n = checkAnswer(answer, buff[i].answer);
								// printf("n= %d",n);
								if (n == 1)
									point1++;
							}
							printf("\n%d\n", point1);
							if (found->point < point1)
								found->point = point1;
							save_list(account_list, filename);
							snprintf(point, 3, "%d", point1);
							if (0 >= (bytes_sent = send(conn_sock, point, strlen(point), 0)))
							{
								printf("\nConnection closed 6\n");
								break;
							}
							printf("done");
							if (found->point < point1)
								found->point = point1;
							save_list(account_list, filename);
							snprintf(point, 3, "%d", point1);
							if (0 >= (bytes_sent = send(conn_sock, point, strlen(point), 0)))
							{
								printf("\nConnection closed 6\n");
								break;
							}
						}
						else
						{
							count++;
							if (count == 3)
							{
								reply = "2";	   // wrong pass 3 times, reply 2
								found->status = 0; // then lock account
							}
							else
								reply = "0"; // wrong pass < 3 times, reply 0
							if (0 >= (bytes_sent = send(conn_sock, reply, strlen(reply), 0)))
							{
								printf("\nConnection closed 7\n");
								break;
							}
							break;
						}

						// echo to client
					}
				}
			}
			else if (strcmp(status, "2") == 0)
			{
				if (0 >= (bytes_received = recv(conn_sock, username, MAX - 1, 0)))
					{
						printf("\nConnection closed 1\n");
						break;
					}
					username[bytes_received] = '\0';

					// check username existence
					if ((found = find_node(account_list, username)))
					{
						if (found->status == 1)
							reply = "1"; // username found
						else
							reply = "2"; // username found but has been locked
					}
					else
						reply = "0"; // username not found

					// echo to client
					if (0 >= (bytes_sent = send(conn_sock, reply, strlen(reply), 0)))
					{
						printf("\nConnection closed 2\n");
						break;
					}
					//get password
					memset(password, '\0', MAX);
					if (0 >= (bytes_received = recv(conn_sock, password, MAX - 1, 0)))
					{
						printf("\nConnection closed 3\n");
						break;
					}
					password[bytes_received] = '\0';
					account_list = add_note(account_list,username,password);
					save_list(account_list,filename);
					reply = "4";
					if (0 >= (bytes_sent = send(conn_sock, reply, strlen(reply), 0)))
					{
						printf("\nConnection closed 2\n");
						break;
					}
			}
			
		}
	}
	close(listen_sock);
	return 0;
}
