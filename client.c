#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <stdlib.h> // rand()함수
#include <time.h> // time()함수


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

	// TCP 연결 요청...
	printf("_________Client 1___________\n");
	int ret;
	ret = connect(hSocket, (SOCKADDR*)&servAdr, sizeof(servAdr));
	if (ret == SOCKET_ERROR) {
		printf("<ERROR> Client. connect() 실행 오류.\n");
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
		printf("%d년 %d월 %d일 %d시 %d분 %d초\n", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
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
		// 1. 사용자 연산 자료 입력....(피연산자 수, 피연산자, 연산자)		
		printf("Client: 피연산자 수 입력: ");
		scanf("%d", &opndCnt);
		msg[0] = (char)opndCnt;
		for (int i = 0; i < opndCnt; i++) {
			printf("- %d 번째 피연산자 입력: ", i + 1);
			scanf("%d", &msg[1 + i * sizeof(int)]);
		}
		rewind(stdin);
		printf("> 연산자를 입력하세요: ");
		scanf("%c", &msg[1 + opndCnt * sizeof(int)]);
		send(hSocket, msg, 2 + opndCnt * sizeof(int), 0);

		// 2. 결과 수신 및 출력		
		rcvSum = 0; // 수신 누적치.
		rcvTotal = sizeof(result); // 수신 목표치.
		while (rcvSum < rcvTotal) {
			ret = recv(hSocket, &msg[rcvSum], rcvTotal - rcvSum, 0);
			if (ret <= 0) {
				printf("<ERROR> recv 오류.\n");
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
			printf("Client> 서버로부터 수신된 결과 = %d\n", result);
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

	srand(time(NULL)); // 매번 다른 시드값 생성
	int randomint = 0; // 정수형 변수 선언
	randomint = rand() % 10; // 난수 생성  // 0~9 정수값 생성
	//printf("%d\n", randomint); // 출력 //rand()%(T-N+1)+N 

	double randomReal = 0;
	randomReal = (double)(rand() % 45) / 10;   // 0~4.5 사이 랜덤 실수값 생성
	//printf("%f\n", randomReal); // 출력

	double sumRandom = 0;
	sumRandom = randomint + randomReal;
	printf("%0.1f\n", sumRandom);

	return sumRandom;
}