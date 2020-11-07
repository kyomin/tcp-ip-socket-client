#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#define BUF_SIZE 30
void ErrorHandling(char *message);

int main(int argc, char *argv[]) {
	WSADATA wsaData;
	SOCKET hSocket;
	FILE *fp;

	char buf[BUF_SIZE];
	int readCnt;
	SOCKADDR_IN servAdr;

	if (argc != 3) {
		printf("Usage: %s <IP> <port>\n", argv[0]);
		exit(1);
	}
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	/*
		서버가 전송하는 파일 데이터를 담기 위해서 파일 하나를 생성하고 있다.
	*/
	fp = fopen("receive.dat", "wb");
	hSocket = socket(PF_INET, SOCK_STREAM, 0);

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = inet_addr(argv[1]);
	servAdr.sin_port = htons(atoi(argv[2]));

	connect(hSocket, (SOCKADDR*)&servAdr, sizeof(servAdr));

	/*
		EOF가 전송될 때까지 데이터를 수신한 다음, 
		위에서 생성한 receive.dat 파일에 담고 있다.
	*/
	while ((readCnt = recv(hSocket, buf, BUF_SIZE, 0)) != 0)
		fwrite((void*)buf, 1, readCnt, fp);

	puts("Received file data");

	/*
		서버로 인사 메시지를 전송하고 있다.
		서버의 입력 스트림이 닫히지 않았다면, 이 메시지를 수신할 수 있다.
	*/
	send(hSocket, "Thank you", 10, 0);
	fclose(fp);
	closesocket(hSocket);
	WSACleanup();
	return 0;
}

void ErrorHandling(char *message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}