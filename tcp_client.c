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
		TCP ������ �����ϰ� �ִ�.
		ù ��° ���ڿ� �� ��° ���ڷ� ���� PF_INET, SOCK_STREAM�� ���޵Ǹ�
		�� ��° ������ IPPROTO_TCP�� ���� �����ϴ�.
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
		while�� �ȿ��� read �Լ��� �ݺ� ȣ���ϰ� �ִ�.
		�߿��� ���� �� �Լ��� ȣ��� ������ 1 ����Ʈ�� �����͸� �о� ���δٴ� ���̴�.
		�׸��� read �Լ��� 0�� ��ȯ�ϸ� �̴� ������ �Ǹ��ϱ� ������ while���� ����������.
		�̴� �ۼ��� ���� ������ ���(Boundary)�� �������� �ʴ� TCP�� Ư¡�̴�.
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