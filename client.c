#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <stdlib.h> // rand()�Լ�
#include <time.h> // time()�Լ�


void ErrorHandling(char* message);
double RandomNum();
#define MAX_PACKET_SIZE  120

int main(int argc, char* argv[])
{
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

	char msg[MAX_PACKET_SIZE];
	int flag;
	flag = 1;
	while (flag) {
		//Timer Setting start
		struct tm* t;
		time_t base = time(NULL);
		t = localtime(&base);
		printf("%d�� %d�� %d�� %d�� %d�� %d��\n", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
		//timer Setting End

		//Random Number start
		double rcvRand=0;
		rcvRand = RandomNum();
		printf("%f",rcvRand);
		sprintf(msg,"%f",rcvRand);
		//Random Number End
		printf("%s\n",msg);
		send(hSocket, msg, sizeof(MAX_PACKET_SIZE), 0);
		flag = 0;
	}
	


		/*
	{
		// 1. ����� ���� �ڷ� �Է�....(�ǿ����� ��, �ǿ�����, ������)		
		printf("Client: �ǿ����� �� �Է�: ");
		scanf("%d", &opndCnt);
		msg[0] = (char)opndCnt;
		for (int i = 0; i < opndCnt; i++) {
			printf("- %d ��° �ǿ����� �Է�: ", i + 1);
			scanf("%d", &msg[1 + i * sizeof(int)]);
		}
		rewind(stdin);
		printf("> �����ڸ� �Է��ϼ���: ");
		scanf("%c", &msg[1 + opndCnt * sizeof(int)]);
		send(hSocket, msg, 2 + opndCnt * sizeof(int), 0);

		// 2. ��� ���� �� ���		
		rcvSum = 0; // ���� ����ġ.
		rcvTotal = sizeof(result); // ���� ��ǥġ.
		while (rcvSum < rcvTotal) {
			ret = recv(hSocket, &msg[rcvSum], rcvTotal - rcvSum, 0);
			if (ret <= 0) {
				printf("<ERROR> recv ����.\n");
				flag = 0;
				break;
			}
			else {
				rcvSum = rcvSum + ret;
				printf("Client> recv %d bytes. sum=%d, total=%d\n", ret, rcvSum, rcvTotal);
			}
		}
		if (flag == 1) {
			result = *((int*)msg);
			printf("Client> �����κ��� ���ŵ� ��� = %d\n", result);
		}
	}
	*/
	closesocket(hSocket);
	printf("Client> close socket...\n");
	WSACleanup();
	
	return 0;
}

void ErrorHandling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

double RandomNum() {

	srand(time(NULL)); // �Ź� �ٸ� �õ尪 ����
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