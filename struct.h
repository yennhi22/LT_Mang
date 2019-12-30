#define BACKLOG 5
#define MAX 8192
#define QUES_FILENAME "question.txt"
#define QUES_IN_LEVER 10
#define QUES_NUMBER 30
typedef struct node
{
	char username[MAX];
	char password[MAX];
	int status;
	int point;
	struct node *next;
} node_t;
typedef struct question
{
	int id;
	int level; //1 : easy, 2 : medium, 3 : hard
	char content[200];
	char choiceA[50];
	char choiceB[50];
	char choiceC[50];
	char choiceD[50];
	char answer;
} Question;
typedef struct message
{
	char content[8192];
	char answer[11];
} Message;