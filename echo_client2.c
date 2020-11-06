#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#define BUF_SIZE 1024
void error_handling(char *message);

int main(int argc, char *argv[]) {
	WSADATA wsaData;
	SOCKET hSocket;
	char message[BUF_SIZE];
	int strLen, recvLen, recvCnt;
	SOCKADDR_IN servAdr;

	if (argc != 3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET)
		ErrorHandling("socket() error");

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = inet_addr(argv[1]);
	servAdr.sin_port = htons(atoi(argv[2]));

	if (connect(hSocket, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
		ErrorHandling("connect() error!");
	else
		puts("Connected...........");

	while (1) {
		fputs("Input message(Q to quit): ", stdout);
		fgets(message, BUF_SIZE, stdin);

		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			break;

		strLen = send(hSocket, message, strlen(message), 0);
		recvLen = 0;

		/*
			기존 에코 클라이언트 코드에서 변경된 부분이다.
			이전 코드에서는 단순히 recv 함수를 한 번 호출하고 말았던 것을 
			여기서는 전송한 데이터의 크기만큼 데이터를 수신하기 위해서 recv 함수를 반복 호출하고 있다.
			따라서 정확히 전송한 바이트 크기만큼 데이터를 수신할 수 있게 되었다.
		*/
		while (recvLen < strLen) {
			recvCnt = recv(hSocket, message, BUF_SIZE - 1, 0);

			if (recvCnt == -1)
				error_handling("recv() error!");

			recvLen += recvCnt;
		}

		message[recvLen] = 0;
		printf("Message from server: %s", message);
	}

	closesocket(hSocket);
	WSACleanup();
	return 0;
}

void error_handling(char *message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}