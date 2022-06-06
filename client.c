#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <stdlib.h> // rand()함수
#include <time.h> // time()함수
#include <Windows.h> // sleep


void ErrorHandling(char* message);
double RandomNum(int randSens);  //랜덤넘버생성
void timerS(int randSens);       //타이머 생성
void randChFun(char* msg, int randSens); // 랜덤 값 Char형 변환
void sendFun(SOCKET hSocket,int randSens); //서버로 송신 Function


#define MAX_PACKET_SIZE  120
#define MAX_SENSER_SIZE 5


struct randRes { // 값저장을 위한 구조체 정의
	int res;
};

int main(int argc, char* argv[])
{
	//기본설정 Start
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
	//기본설정 End

	//Senser Setting Start

	srand(time(NULL)); // 매번 다른 시드값 생성
	int randSens = 0; //랜덤한 센서값 생성
	randSens = (rand() % 5) + 1; //난수생성 // 1~5생성

	int senserArr[MAX_SENSER_SIZE]; //센서배열 생성

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
		srand(time(NULL)); // 매번 다른 시드값 생성
	}
	for (int i = 0; i < randSens; i++) {
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
}

void timerS() {
	//Timer Setting start
	struct tm* t;
	time_t base = time(NULL);
	t = localtime(&base);
	printf("%d년 %d월 %d일 %d시 %d분 %d초\n", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
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
			//printf("1회용");
			for (int i = 0; i < randSens; i++) {
				randChFun(msg,randSens);
				send(hSocket, msg, sizeof(MAX_PACKET_SIZE), 0);
				firstFlag = 0; //최초실행후 반복문으로 실행된다.
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

			//printf("^^^^^%d^^^^^", rcvflag);//제대로 변환되었는지 확인

			if (rcvflag <= 0) {
				printf("ERROR 위험 수치\n");
				int selectError;
				printf("감지된 센서의 값이 위험수치를 도달하였습니다. 계속: 1 || 종료: 0 || 그외값 입력 : 관리자와 채팅\n");
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
					flag = 0; //채팅 연결
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