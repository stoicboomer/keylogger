#pragma once

#ifndef _KEYCLIENT_
#define _KEY_CLIENT_
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // WIN32_LEAN_AND_MEAN

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <MSWSock.h>

#pragma comment(lib, "Ws2_32.lib")

/*
resources:
	https://docs.microsoft.com/en-us/windows/win32/winsock/getting-started-with-winsock
*/

typedef struct destInfo{
	const char		*host;
	const char		*port;
	const char		*logPath;

} DESTINFO;

typedef struct client{
	SOCKET				sockfd;
	BOOL				isConnected;
	int					ai_addrlen;
	struct sockaddr		*ai_addr;
	struct sockaddr		*ai_next;

} CLIENT;

CLIENT clientInit(
	const char		*host,
	const char		*port,
	int				af,
	int				type,
	int				protocol){

	WSADATA wsaData;
	ADDRINFOA hints, *result;
	SOCKET sockfd;
	int r;

	WSAStartup(MAKEWORD(2, 2), &wsaData);

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = af;
	hints.ai_socktype = type;
	hints.ai_protocol = protocol;

	r = getaddrinfo(host, port, &hints, &result);
	sockfd = socket(result->ai_family,  result->ai_socktype, 
					result->ai_protocol);

	if (r){
		CLIENT client = {0, FALSE, 0, NULL, NULL};
		return client;
	}

	CLIENT client = {sockfd, FALSE, (int)result->ai_addrlen, result->ai_addr, result->ai_next};
	return client;
}

int clientConnect(CLIENT *client){

	int r = connect(client->sockfd, client->ai_addr, client->ai_addrlen);
	if (r == SOCKET_ERROR){
		client->isConnected = FALSE;
		return 1;
	}

	client->isConnected = TRUE;
	return 0;
}

int clientSend(CLIENT *client, const char *buf, int len){
	
	int r = send(client->sockfd, buf, len, 0);
	if (r == SOCKET_ERROR){
		client->isConnected = FALSE;
		return 1;
	}

	return 0;
}


//simply closes the given socket and replaces it with a new one for reuse
int clientClose(CLIENT *client){

	int r = closesocket(client->sockfd);
	if (r == SOCKET_ERROR){
		return 1;
	}

	client->sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (client->sockfd == INVALID_SOCKET){
		return 1;
	}

	return 0;
}



#endif // !_KEYCLIENT_
