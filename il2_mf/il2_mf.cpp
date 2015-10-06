// il2_mf.cpp : Defines the entry point for the console application.
//

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include "mfsimulator.h"

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "10000"

int __cdecl main(int argc, char **argv) 
{
	MFSimulator simulator;
	simulator.initialize();

	WSADATA wsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;
	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;
	char *sendbuf = "R/46/48";
	char recvbuf[DEFAULT_BUFLEN];
	int iResult;
	int recvbuflen = DEFAULT_BUFLEN;

	// Validate the parameters
	//if (argc != 2) {
	//	printf("usage: %s server-name\n", argv[0]);
	//	return 1;
	//}

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	ZeroMemory( &hints, sizeof(hints) );
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;

	// Resolve the server address and port
	iResult = getaddrinfo("192.168.2.147", DEFAULT_PORT, &hints, &result);
	if ( iResult != 0 ) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	// Attempt to connect to an address until one succeeds
	for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, 
			ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return 1;
		}

		// Connect to server.
		iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		return 1;
	}

	int iOptVal = 200;
	int iOptLen = sizeof(int);

	iResult = setsockopt(ConnectSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&iOptVal, iOptLen);
	if(iResult != 0){
		printf("setsockopt failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	bool printedRetrying = false;
	bool printedZeroing = false;
	unsigned int retryCount = 0;

	// Receive until the peer closes the connection
	do {
		// Send an initial buffer
		iResult = send( ConnectSocket, sendbuf, (int)strlen(sendbuf), 0 );
		if (iResult == SOCKET_ERROR) {
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(ConnectSocket);
			WSACleanup();
			return 1;
		}

		//printf("Bytes Sent: %ld\n", iResult);

		iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
		if ( iResult > 0 )
		{
			recvbuf[iResult] = '\0';
			char tempBuffer[512];
			tempBuffer[0] = '\0';

			strcpy_s(tempBuffer, recvbuf);
			tempBuffer[4] = '\0';
			float pitch, roll;

			if(strcmp(tempBuffer, "A/46") == 0)
			{
				sscanf_s(recvbuf, "A/46\\%f/48\\%f", &roll, &pitch);

				//printf("%f, %f\n", pitch, roll);

				simulator.setPitch(-pitch);
				simulator.setRoll(-roll);
				simulator.update();
			}

			if(printedRetrying)
			{
				printedRetrying = false;
				printedZeroing = false;
				printf("Reconnected!\n");
			}

			retryCount = 0;
		}
		else if ( iResult == 0 )
			printf("Connection closed\n");

		else
		{
			int iCode = WSAGetLastError();

			if(iCode == 10060)
			{
				if(!printedRetrying)
				{
					printedRetrying = true;
					printf("Connection Timed Out Retrying...");
				}

				if(retryCount > 5 && !printedZeroing)
				{
					printedZeroing = true;
					printf("Zeroing System...");
					simulator.setPitch(0.0f);
					simulator.setRoll(0.0f);
					simulator.update();
				}

				retryCount++;
				iResult = 1;
			}

			else
			{
				printf("recv failed with error: %d\n", iCode);
			}
		}

		Sleep(31);
	} while( iResult > 0 );

	// shutdown the connection since no more data will be sent
	iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}


	// cleanup
	closesocket(ConnectSocket);
	WSACleanup();

	return 0;
}


