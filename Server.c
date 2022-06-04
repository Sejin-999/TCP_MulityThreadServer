#include <stdio.h>
#include <stdlib.h>
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
	int flag;

	printf("THREAD > running new thread.\n");
	// ----- START : 연결된 client에 대해서 서비스 제공 부분...
	flag = 1;
	int rcvSum, rcvTotal, ret, result;
	char opndCnt, msg[MAX_PACKET_SIZE];

	while (flag) {
		printf("THREAD >  요청대기\n");
		recv(hClntSock, &opndCnt, sizeof(opndCnt), 0);
		printf("THREAD> 피연산자 수 = %d.\n", opndCnt);
		printf("THREAD> close socket with client.\n");
		closesocket(hClntSock);
	}
	// ----- END : 연결된 client에 대해서 서비스 제공 부분...
}