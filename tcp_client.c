#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
void ErrorHandling(char* message);

int main(int argc, char* argv[]) {
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAddr;

	char message[30];
	int strLen = 0;
	int idx = 0, readLen = 0;

	if (argc != 3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	/*
		TCP 소켓을 생성하고 있다.
		첫 번째 인자와 두 번째 인자로 각각 PF_INET, SOCK_STREAM이 전달되면
		세 번째 인자인 IPPROTO_TCP는 생략 가능하다.
	*/
	hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET)
		ErrorHandling("hSocket() error");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons(atoi(argv[2]));

	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("connect() error!");

	/*
		while문 안에서 read 함수를 반복 호출하고 있다.
		중요한 것은 이 함수가 호출될 때마다 1 바이트씩 데이터를 읽어 들인다는 점이다.
		그리고 read 함수가 0을 반환하면 이는 거짓을 의마하기 때문에 while문을 빠져나간다.
		이는 송수신 측의 데이터 경계(Boundary)가 존재하지 않는 TCP의 특징이다.
	*/
	while (readLen = recv(hSocket, &message[idx++], 1, 0)) {
		if (readLen == -1)
			ErrorHandling("read() error!");

		strLen += readLen;
	}

	printf("Message from server: %s \n", message);
	printf("Function read call count: %d \n", strLen);

	closesocket(hSocket);
	WSACleanup();
	return 0;
}

void ErrorHandling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}