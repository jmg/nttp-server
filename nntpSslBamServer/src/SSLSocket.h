#ifndef SSLSOCKET_H_
#define SSLSOCKET_H_

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <sys/socket.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "SSLSocket.h"
#include "Utils/StrMng.h"
#include "Utils/Logs.h"


#define BUFFER 1024
#define MAXCONNECTIONS 20

typedef struct SSLSocketClass *SSLSocket;

SSLSocket NewSSLSocket(void);

void FreeSSLSocket(SSLSocket sslSocket);

void SSLSocketOpen(SSLSocket sslSocket, char* port);

void SSLSocketInitCtx(SSLSocket sslSocket);

void SSLSocketLoadCertificates(SSLSocket sslSocket, char* CertFile, char* KeyFile);

void SSLSocketShowCerts(SSLSocket sslSocket);

int SSLSocketConnect(SSLSocket sslSocket);

char* SSLSocketRead(SSLSocket sslSocket);

void SSLSocketWrite(SSLSocket sslSocket, char* response);

void SSLSocketCloseSession(SSLSocket sslSocket);

void SSLSocketClose(SSLSocket sslSocket);

#endif /* SSLSOCKET_H_ */
