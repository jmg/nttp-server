#ifndef SERVERSOCKET_H
#define SERVERSOCKET_H
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <ctype.h>
#include "Thread.h"
#include "../connection/ClientConnection.h"

typedef struct ConcreteServerSocket * AbstractServerSocket;

AbstractServerSocket NewServerSocket(int port);

void ServerSocketListen(AbstractServerSocket abstractSocket, void ( onConnect)(void *));

void ServerSocketClose(AbstractServerSocket abstractSocket);

#endif
