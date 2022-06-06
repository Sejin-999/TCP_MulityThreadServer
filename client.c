#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <stdlib.h> // rand()�Լ�
#include <time.h> // time()�Լ�
#include <Windows.h> // sleep


void ErrorHandling(char* message);
double RandomNum(int randSens);  //�����ѹ�����
void timerS(int randSens);       //Ÿ�̸� ����
void randChFun(char* msg, int randSens); // ���� �� Char�� ��ȯ
void sendFun(SOCKET hSocket,int randSens); //������ �۽� Function


#define MAX_PACKET_SIZE  120
#define MAX_SENSER_SIZE 5


struct randRes { // �������� ���� ����ü ����
	int res;
};

int main(int argc, char* argv[])
{
	//�⺻���� Start
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAdr;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET)
		ErrorHandling("socket() error");

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servAdr.sin_port = htons(9000);

	// TCP ���� ��û...
	printf("_________Client 1___________\n");
	int ret;
	ret = connect(hSocket, (SOCKADDR*)&servAdr, sizeof(servAdr));
	if (ret == SOCKET_ERROR) {
		printf("<ERROR> Client. connect() ���� ����.\n");
		closesocket(hSocket);
		printf("Client> close socket...\n");
		WSACleanup();
		return 0;
	}
	else {
		printf("Client> connection established...\n");
	}
	//�⺻���� End

	//Senser Setting Start

	srand(time(NULL)); // �Ź� �ٸ� �õ尪 ����
	int randSens = 0; //������ ������ ����
	randSens = (rand() % 5) + 1; //�������� // 1~5����

	int senserArr[MAX_SENSER_SIZE]; //�����迭 ����

	for (int i = 0; i < randSens; i++) {
		senserArr[i] = 0;
	}

		sendFun(hSocket,randSens);
	

	//debug Code 
	/*
	printf("---%d---\n", randSens);
	for (int i = 0; i < randSens; i++) {
		printf("%d , ", senserArr[i]);
	}
	//debug Code 
	*/
	//Senser Setting End

	return 0;
}

void ErrorHandling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

double RandomNum(int randSens) {

	for (int i = 0; i < randSens; i++) {
		srand(time(NULL)); // �Ź� �ٸ� �õ尪 ����
	}
	for (int i = 0; i < randSens; i++) {
		int randomint = 0; // ������ ���� ����
		randomint = rand() % 10; // ���� ����  // 0~9 ������ ����
		//printf("%d\n", randomint); // ��� //rand()%(T-N+1)+N 

		double randomReal = 0;
		randomReal = (double)(rand() % 45) / 10;   // 0~4.5 ���� ���� �Ǽ��� ����
		//printf("%f\n", randomReal); // ���

		double sumRandom = 0;
		sumRandom = randomint + randomReal;
		printf("%0.1f\n", sumRandom);

		return sumRandom;
	}
}

void timerS() {
	//Timer Setting start
	struct tm* t;
	time_t base = time(NULL);
	t = localtime(&base);
	printf("%d�� %d�� %d�� %d�� %d�� %d��\n", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
	//timer Setting End
}

void randChFun(char *msg ,int randSens) {
	timerS();
	//Random Number start
	double rcvRand = 0;
	rcvRand = RandomNum(randSens);
	//printf("%f", rcvRand);
	sprintf(msg, "%f", rcvRand);
	//Random Number End
}

void sendFun(SOCKET hSocket,int randSens) {

	char msg[MAX_PACKET_SIZE];
	int flag;
	int firstFlag = 1;
	flag = 1;
	struct randRes rR;  //rR = randRes

	while (flag) {

		if (firstFlag == 1) {
			//printf("1ȸ��");
			for (int i = 0; i < randSens; i++) {
				randChFun(msg,randSens);
				send(hSocket, msg, sizeof(MAX_PACKET_SIZE), 0);
				firstFlag = 0; //���ʽ����� �ݺ������� ����ȴ�.
			}
			Sleep(2000);
		}

		else {
			int rcvflag;
			char* fm;
			recv(hSocket, &msg, sizeof(MAX_PACKET_SIZE), 0);
			//printf("%s", msg);
			fm = msg;
			rcvflag = atof(fm);

			//printf("^^^^^%d^^^^^", rcvflag);//����� ��ȯ�Ǿ����� Ȯ��

			if (rcvflag <= 0) {
				printf("ERROR ���� ��ġ\n");
				int selectError;
				printf("������ ������ ���� �����ġ�� �����Ͽ����ϴ�. ���: 1 || ����: 0 || �׿ܰ� �Է� : �����ڿ� ä��\n");
				scanf("%d", &selectError);
				if (selectError == 0) {
					flag = 0;
				}
				else if (selectError == 1) {
					for (int i = 0; i < randSens; i++) {
						randChFun(msg, randSens);
						send(hSocket, msg, sizeof(MAX_PACKET_SIZE), 0);
					}
					Sleep(2000);
				}
				else {
					flag = 0; //ä�� ����
				}
			}
			else {
				for (int i = 0; i < randSens; i++) {
					randChFun(msg, randSens));
					send(hSocket, msg, sizeof(MAX_PACKET_SIZE), 0);
				}
				Sleep(2000);
			}
		}


	}
	closesocket(hSocket);
	printf("Client> close socket...\n");
	WSACleanup();

}