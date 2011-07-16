#ifndef Socket_C
#define Socket_C

#include <winsock.h>
#include <stdio.h>
#include "Socket.h"
#include <signal.h>
#include "Utils/Logs.h"

#define BUFFER 10

struct SocketClass
{	
	WSADATA wsaData;    
	SOCKET sockfd;
	SOCKADDR_IN sockaddr;	

};

Socket NewSocket(void)
{
	Socket Sock = (Socket)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(struct SocketClass));

	if (WSAStartup(MAKEWORD(2,2), &Sock->wsaData) != NO_ERROR) 
	{
		WSACleanup();
		puts("Error at WSAStartup()\n");
		LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEERROR, "Eror al iniciar el socket", NOPARMS);
		exit(1);
	}
	if (Sock->wsaData.wVersion != 0x0202)
	{ 
		WSACleanup();
		puts("error version");
		LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEERROR, "Eror de version del socket", NOPARMS);
		exit(1);
	}
	Sock->sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	return Sock;
}

void FreeSocket(Socket Sock)
{
	WSACleanup();
	HeapFree(GetProcessHeap(), HEAP_ZERO_MEMORY, Sock);
}

int SocketConnect(Socket Sock, char* host, int port)
{	
    memset(&Sock->sockaddr, 0, sizeof(Sock->sockaddr));
 
    Sock->sockaddr.sin_family = AF_INET;
    Sock->sockaddr.sin_addr.s_addr = inet_addr(host); 
    Sock->sockaddr.sin_port = htons(port); 

	if(connect(Sock->sockfd, (LPSOCKADDR)&Sock->sockaddr, sizeof(Sock->sockaddr)) == SOCKET_ERROR)
	{
		LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEERROR, "Error en la conexion a: ", host);
		WSACleanup();        
		return SOCKET_ERROR;
    } 
	LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se conecto el socket a: ", host);
	return Sock;
}

char* SocketRead(Socket Sock)
{
	int i;
    int received;
	char* buffer = (char*) MemReserveForChar(BUFFER * sizeof(char));
	char* rc = (char*) MemReserveForChar(BUFFER * sizeof(char));

    for (i=1;;i++)
    {
		received = recv(Sock->sockfd, buffer, BUFFER, 0);

        if (received > 0)
        {
			rc = (char*) (MemExtendForChar(rc, i * (BUFFER+1) * sizeof(char)));
            strcat(rc, buffer);
        }
        if (received < BUFFER)
            break;
        memset(buffer, '\0', BUFFER);
    }
	LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se leyó en el socket", buffer);
	MemFree(buffer);
	return rc;
}

int SocketWrite(Socket Sock, char* message)
{
	send(Sock->sockfd, message, strlen(message), 0);
	LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se escribió en el socket", message);
}


void close(Socket sock) {
	closesocket(sock->sockfd);
}

#endif