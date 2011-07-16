#ifndef CLIENTCONNECTION_H
#define CLIENTCONNECTION_H
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <ctype.h>
#include "../db/LdapWrapper/LdapWrapper.h"
#include "../utils/Logs.h"
#include "../utils/Utils.h"

typedef struct ConcreteClientConnection * AbstractClientConnection;

AbstractClientConnection NewClientConnection(int socketDescriptor);

char* readConnection(AbstractClientConnection clientConnection);

int writeConnection(AbstractClientConnection clientConnection, char * message);

void ClientConnectionFree(AbstractClientConnection clientConnection);

#endif
