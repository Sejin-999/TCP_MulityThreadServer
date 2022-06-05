#include <stdio.h>
#include <stdlib.h> //atof함수
#include <string.h>
#include <winsock2.h> //Socket 사용
#include <time.h> //Timer 구현


//기본설정 속성 - 링커 ws2_32.lib , 멀티쓰레드 환경, SLD

//함수선언 Start
void ErrorHandling(const char * msg);
DWORD WINAPI DataCheck(LPVOID arg);
//함수선언 End

//Define Start
#define MAX_PACKET_SIZE 120
#define MAX_CLIENT 5
#define MAX_TIME 512
//Define End

//struct Site start

/*
typedef struct timer {
	int year;
	int mount;
	int day;
	int hour;
	int min;
	int sec;

}timer;

timer TL[MAX_TIME]; //timerListen
*/

//struct Site End


//Main Start
int main() {
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAdr, clntAdr;
	int clntAdrSize;
	const char* Error = "Error";

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling(Error);
	//socket Creat
	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	//socket Error check
	if (hServSock == INVALID_SOCKET)
		ErrorHandling("socket() error");
	//Clear
	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(9000);
	//bind Creat&check
	if (bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
		ErrorHandling("bind() error");
	//listen Creat
	listen(hServSock, MAX_CLIENT);  //SOCKET , queueLimit
	DWORD ThreadId; //DWORD -> Double Word -> 32bit
	HANDLE hThread;


	//Client 요청.. 
	int flag = 1;
	
	while (flag) {

		//Timer Setting start
		struct tm* t;
		time_t base = time(NULL);
		t = localtime(&base);
		printf("%d년 %d월 %d일 %d시 %d분 %d초\n", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
		//timer Setting End

		printf("Server > 클라이언트 요청 대기중 \n");
		clntAdrSize = sizeof(clntAdr);
		hClntSock = accept(hServSock, (SOCKADDR*)&clntAdr, &clntAdrSize);
		if (hClntSock == -1) {
			printf("<ERROR> accept 실행 오류.\n");
		}
		else {
			printf("Server> client(IP:%s, Port:%d) 연결됨.\n",
				inet_ntoa(clntAdr.sin_addr), ntohs(clntAdr.sin_port));
		}

		// 쓰레드 생성... (hClntSock)
		hThread = CreateThread(NULL, 0, DataCheck, (LPVOID)hClntSock, 0, &ThreadId);
		if (hThread == NULL) {
			printf("<ERROR> thread 생성 오류.\n");
		}
		else {
			CloseHandle(hThread);
		}
		//
	}
	closesocket(hServSock);
	WSACleanup();
	return 0;

		
	}


//Main end



void ErrorHandling(const char* msg) {
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}


DWORD WINAPI DataCheck(LPVOID arg) {
	SOCKET hClntSock;
	hClntSock = (SOCKET)arg;
	int flag,ctlFlag;

	printf("THREAD > running new thread.\n");
	// ----- START : 연결된 client에 대해서 서비스 제공 부분...
	flag = 1;
	

	double rcvMsgReal;
	char* rcvMsgChar;
	char msg[MAX_PACKET_SIZE];

	while (flag) {
		ctlFlag = 1;
		printf("THREAD >  요청대기\n");
		recv(hClntSock, &msg, sizeof(MAX_PACKET_SIZE), 0);
		rcvMsgChar = msg;
		rcvMsgReal = atof(rcvMsgChar); //문자열 -> 실수 

		if (rcvMsgReal < 0) {
			printf("<ERROR> recv 오류 OR Client 오류 \n");
			flag = 0;
			break;
		}
		else {
			ctlFlag = CheckRcv(rcvMsgReal);
			char sendA[10];
			sprintf(sendA,"%d",ctlFlag);
			printf("THREAD > 받은 값 = %0.1f  :: %d :: %s\n", rcvMsgReal,ctlFlag,sendA);
			send(hClntSock, sendA, sizeof(10), 0);
		}
	}
	printf("THREAD> close socket with client.\n");
	closesocket(hClntSock);
	// ----- END : 연결된 client에 대해서 서비스 제공 부분...
}

int CheckRcv(double rcv) {
	if (rcv < 8) {
		return 1;
	}
	else {
		printf("________DANGER 위험수치_______\n");
		return 0;
	}
}